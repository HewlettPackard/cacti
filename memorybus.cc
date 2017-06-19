/*****************************************************************************
 *                                CACTI 7.0
 *                      SOFTWARE LICENSE AGREEMENT
 *            Copyright 2015 Hewlett-Packard Development Company, L.P.
 *                          All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.”
 *
 ***************************************************************************/

#include "memorybus.h"
#include "wire.h"
#include <assert.h>
#include <iostream>
#include <cmath>

Memorybus::Memorybus(
		enum Wire_type wire_model, double mat_w, double mat_h, double subarray_w_, double subarray_h_,
		int _row_add_bits, int _col_add_bits, int _data_bits, int _ndbl, int _ndwl, /*enum Htree_type htree_type,*/
		enum Memorybus_type membus_type_, const DynamicParameter & dp_,
		/*TechnologyParameter::*/DeviceType *dt):
		dp(dp_),
		in_rise_time(0), out_rise_time(0),
		is_dram(dp.is_dram),
		membus_type(membus_type_),
		mat_width(mat_w), mat_height(mat_h), subarray_width(subarray_w_), subarray_height(subarray_h_),
		data_bits(_data_bits), ndbl(_ndbl), ndwl(_ndwl),
		wt(wire_model), deviceType(dt)
{
	if (g_ip->print_detail_debug)
		cout << "memorybus.cc: membus_type = " << membus_type << endl;
    power.readOp.dynamic = 0;
    power.readOp.leakage = 0;
    power.readOp.gate_leakage = 0;
    power.searchOp.dynamic =0;
	delay = 0;

	cell.h = g_tp.dram.b_h;
	cell.w = g_tp.dram.b_w;

	if (!g_ip->is_3d_mem)
		assert(ndbl >= 2 && ndwl >= 2);

    if (g_ip->print_detail_debug)
    {
	cout << "burst length: " << g_ip->burst_depth <<endl;
	cout << "output width: " << g_ip->io_width <<endl;
    }

	//Default value
	chip_IO_width = g_ip->io_width; //g_ip->out_w; //x4, x8, x16 chip
	burst_length = g_ip->burst_depth; //g_ip->burst_len; //DDR2 4, DDR3 8
	data_bits = chip_IO_width * burst_length;

	row_add_bits = _row_add_bits;
	col_add_bits = _col_add_bits;


	max_unpipelined_link_delay = 0; //TODO
	min_w_nmos = g_tp.min_w_nmos_;
	min_w_pmos = deviceType->n_to_p_eff_curr_drv_ratio * min_w_nmos;


	semi_repeated_global_line = 0; // 1: semi-repeated global line, repeaters in decoder stripes; 0: Non-repeated global line, slower
	ndwl = _ndwl/ g_ip->num_tier_row_sprd;
	ndbl = _ndbl/ g_ip->num_tier_col_sprd;
	num_subarray_global_IO = ndbl>16?16:ndbl;

	switch (membus_type)
	{
	case Data_path:
		data_bits = chip_IO_width * burst_length;
		Network();
		break;
	case Row_add_path:
		add_bits = _row_add_bits;
		num_dec_signals = dp.num_r_subarray * ndbl;
		Network();
		break;
	case Col_add_path:
		add_bits = _col_add_bits;
		num_dec_signals = dp.num_c_subarray * ndwl / data_bits;
		Network();
		break;
	default:
		assert(0);
		break;
	}

	assert(power.readOp.dynamic >= 0);
	assert(power.readOp.leakage >= 0);
}

Memorybus::~Memorybus()
{
	delete center_stripe;
	delete bank_bus;
	switch (membus_type)
	{
	case Data_path:
		delete local_data;
		delete global_data;
		delete local_data_drv;
		if(semi_repeated_global_line)
			delete global_data_drv;
		delete out_seg;
		break;
	case Row_add_path:
		delete global_WL;
		delete add_predec;
		delete add_dec;
		delete lwl_drv;
		break;
	case Col_add_path:
		delete column_sel;
		delete add_predec;
		delete add_dec;
		break;
	default:
		assert(0);
		break;
	}
}

// ---For 3D DRAM, the bank height and length is reduced to 1/num_tier_row_sprd and 1/num_tier_col_sprd.
// ---As a result, ndwl and ndbl are also reduced to the same ratio, but he number of banks increase to the product of these two parameters
void Memorybus::Network()
{
	//double POLY_RESISTIVITY = 0.148; //ohm-micron
    double R_wire_dec_out = 0;
    double C_ld_dec_out = 0;
    double bank_bus_length = 0;
	double area_bank_vertical_peripheral_circuitry = 0, area_bank_horizontal_peripheral_circuitry = 0;

	area_sense_amp = (mat_height - subarray_height) * mat_width * ndbl * ndwl;
	area_subarray = subarray_height * subarray_width * ndbl * ndwl;

	// ---Because in 3D DRAM mat only has one subarray, but contains the subarray peripheral circuits such as SA. Detail see mat.cc is_3d_mem part.
	subarray_height = mat_height;
	subarray_width = mat_width;

	if(g_ip->partition_gran == 0)// Coarse_rank_level: add/data bus around
	{
		height_bank = subarray_height * ndbl + (col_add_bits + row_add_bits)*g_tp.wire_outside_mat.pitch/2 + data_bits*g_tp.wire_outside_mat.pitch;
		length_bank = subarray_width * ndwl + (col_add_bits + row_add_bits)*g_tp.wire_outside_mat.pitch/2 + data_bits*g_tp.wire_outside_mat.pitch;
		area_address_bus = (row_add_bits + col_add_bits) *g_tp.wire_outside_mat.pitch * sqrt(length_bank * height_bank);
		area_data_bus = data_bits *g_tp.wire_outside_mat.pitch * sqrt(length_bank * height_bank);
	}
	else if(g_ip->partition_gran == 1)//Fine_rank_level: add bus replaced by TSVs
	{
		height_bank = subarray_height * ndbl;
		length_bank = subarray_width * ndwl;
		area_address_bus = 0;
		area_data_bus = data_bits *g_tp.wire_outside_mat.pitch  * sqrt(length_bank * height_bank);
	}
	else if(g_ip->partition_gran == 2)//Coarse_bank_level: add/data bus replaced by TSVs
	{
		height_bank = subarray_height * ndbl;
		length_bank = subarray_width * ndwl;
		area_address_bus = 0;
		area_data_bus = 0;
	}




	if (g_ip->print_detail_debug)
	{
		cout << "memorybus.cc: N subarrays per mat = " << dp.num_subarrays / dp.num_mats << endl;
		cout << "memorybus.cc: g_tp.wire_local.pitch = " << g_tp.wire_local.pitch /1e3 << " mm" << endl;
		cout << "memorybus.cc: subarray_width = " << subarray_width /1e3 << " mm"  << endl;
		cout << "memorybus.cc: subarray_height = " << subarray_height /1e3 << " mm" << endl;
		cout << "memorybus.cc: mat_height = " << mat_height /1e3 << " mm" << endl;
		cout << "memorybus.cc: mat_width = " << mat_width /1e3 << " mm"  << endl;
		cout << "memorybus.cc: height_bank = " << height_bank /1e3 << " mm"  << endl;
		cout << "memorybus.cc: length_bank = " << length_bank /1e3 << " mm"  << endl;
	}

	int num_banks_hor_dir = 1 << (int)ceil((double)_log2( g_ip->nbanks * g_ip->num_tier_row_sprd )/2 ) ;
	int num_banks_ver_dir = 1 << (int)ceil((double)_log2( g_ip->nbanks * g_ip->num_tier_col_sprd  * g_ip->num_tier_row_sprd  /num_banks_hor_dir ) );

    if (g_ip->print_detail_debug)
    {
		cout<<"horz bank #: "<<num_banks_hor_dir<<endl;
		cout<<"vert bank #: "<<num_banks_ver_dir<<endl;

		cout << "memorybus.cc: g_ip->nbanks = " << g_ip->nbanks << endl;
		cout << "memorybus.cc: num_banks_hor_dir = " << num_banks_hor_dir << endl;
    }

	// ************************************* Wire Interconnections *****************************************
    double center_stripe_length = 0.5 * double(num_banks_hor_dir) * height_bank; 
	if(g_ip->print_detail_debug)
	{
		cout << "memorybus.cc: center_stripe wire length = " << center_stripe_length << " um"<< endl;
	}
	center_stripe = new Wire(wt, center_stripe_length);
	area_bus = 2.0 * center_stripe_length * (row_add_bits + col_add_bits + data_bits) *g_tp.wire_outside_mat.pitch / g_ip->nbanks;

	//if (g_ip->partition_gran == 0)
		//area_bus = (row_add_bits + col_add_bits) *g_tp.wire_outside_mat.pitch * center_stripe_length;
	if (membus_type == Row_add_path)
	{
		int num_lwl_per_gwl = 4;
		global_WL = new Wire(wt, length_bank, 1, 1, 1, inside_mat, CU_RESISTIVITY, &(g_tp.peri_global));
		//local_WL = new Wire(wt, length_bank/num_lwl_drv, local_wires, POLY_RESISTIVITY, &(g_tp.dram_wl));
		num_lwl_drv = ndwl;
		//C_GWL = num_lwl_drv * gate_C(g_tp.min_w_nmos_+min_w_pmos,0) + c_w_metal * dp.num_c_subarray * ndwl;
		if(semi_repeated_global_line)
		{
			C_GWL = (double)num_lwl_per_gwl * gate_C(g_tp.min_w_nmos_+min_w_pmos,0) + g_tp.wire_inside_mat.C_per_um * (subarray_width + g_tp.wire_local.pitch);
			R_GWL = g_tp.wire_inside_mat.R_per_um * (subarray_width + g_tp.wire_local.pitch);
		}
		else
		{
			C_GWL = (double)num_lwl_drv * num_lwl_per_gwl * gate_C(g_tp.min_w_nmos_+min_w_pmos,0) + g_tp.wire_inside_mat.C_per_um * length_bank;
			R_GWL = length_bank * g_tp.wire_inside_mat.R_per_um;
		}

		lwl_driver_c_gate_load = dp.num_c_subarray * gate_C_pass(g_tp.dram.cell_a_w, g_tp.dram.b_w, true, true);
		//lwl_driver_c_wire_load = subarray_width * g_tp.wire_local.C_per_um;
		//lwl_driver_r_wire_load = subarray_width * g_tp.wire_local.R_per_um;

		if (g_ip->print_detail_debug)
		{
			cout<<"C_GWL: "<<C_GWL<<endl;
			cout<<"num_lwl_drv: "<<num_lwl_drv<<endl;
			cout<<"g_tp.wire_inside_mat.C_per_um: "<<g_tp.wire_inside_mat.C_per_um<<endl;
			cout<<"length_bank: "<<length_bank<<endl;

			cout << "memorybus.cc: lwl single gate capacitance = " << gate_C_pass(g_tp.dram.cell_a_w, g_tp.dram.b_w, true, true) << endl;
			cout << "memorybus.cc: lwl wire capacitance per single wire = " << g_tp.wire_local.C_per_um << endl;
			cout << "memorybus.cc: dp.num_c_subarray = " << dp.num_c_subarray << endl;
			cout << "memorybus.cc: dram.b_w = " << g_tp.dram.b_w << endl;
		}

		lwl_driver_c_wire_load = dp.num_c_subarray * g_tp.dram.b_w * g_tp.wire_local.C_per_um;
		lwl_driver_r_wire_load = dp.num_c_subarray * g_tp.dram.b_w * g_tp.wire_local.R_per_um;

		C_LWL = lwl_driver_c_gate_load + lwl_driver_c_wire_load;

		lwl_drv = new Driver(
				lwl_driver_c_gate_load,
				lwl_driver_c_wire_load,
				lwl_driver_r_wire_load,
				is_dram);
		lwl_drv->compute_area();

		if(!g_ip->fine_gran_bank_lvl)
		{
			C_ld_dec_out = C_GWL;
			R_wire_dec_out = R_GWL;
		}
		else
		{
			C_ld_dec_out = gate_C(g_tp.min_w_nmos_+min_w_pmos,0);
			R_wire_dec_out = 0;
		}

		if (g_ip->print_detail_debug)
			cout << "memorybus.cc: ndwl * dp.num_c_subarray * g_tp.dram.b_w = " << ndwl * dp.num_c_subarray * g_tp.dram.b_w << endl;
		//bank_bus_length = double(num_banks_ver_dir) * 0.5 * (height_bank + 0.5*double(row_add_bits+col_add_bits+data_bits)*g_tp.wire_outside_mat.pitch);
		bank_bus_length = double(num_banks_ver_dir) * 0.5 * MAX(length_bank, height_bank);
		bank_bus = new Wire(wt, bank_bus_length);

	}
	else if (membus_type == Col_add_path)
	{
		column_sel = new Wire(wt, sqrt(length_bank * height_bank), 1, 1, 1, outside_mat, CU_RESISTIVITY, &(g_tp.peri_global));
		if(semi_repeated_global_line)
		{
			C_colsel = g_tp.wire_inside_mat.C_per_um * (subarray_height + g_tp.wire_local.pitch) ;
			R_colsel = g_tp.wire_inside_mat.R_per_um * (subarray_height + g_tp.wire_local.pitch);
		}
		else
		{
			C_colsel = column_sel->repeater_size * gate_C(g_tp.min_w_nmos_+min_w_pmos,0)
	    	+ (column_sel->repeater_spacing < height_bank ? column_sel->repeater_spacing : height_bank) * g_tp.wire_outside_mat.C_per_um;
			R_colsel = (column_sel->repeater_spacing < height_bank ? column_sel->repeater_spacing : height_bank) * g_tp.wire_outside_mat.R_per_um;
		}

    if(!g_ip->fine_gran_bank_lvl)
    {
    	C_ld_dec_out = C_colsel;
    	//+ (int)(column_sel->repeater_spacing/height_bank) * ndbl*dp.num_r_subarray* gate_C(g_tp.w_nmos_sa_mux,0);
    	R_wire_dec_out = R_colsel;
    }
    else
    {
		C_ld_dec_out = gate_C(g_tp.min_w_nmos_+min_w_pmos,0);
		R_wire_dec_out = 0;
    }


	if (g_ip->print_detail_debug)
		cout << "memorybus.cc: column_sel->repeater_size = " << column_sel->repeater_size << endl;

	bank_bus_length = double(num_banks_ver_dir) * 0.5 * MAX(length_bank, height_bank);
	bank_bus = new Wire(wt, bank_bus_length);
	}
	else if (membus_type == Data_path)
	{
		local_data = new Wire(wt, subarray_width, 1, 1, 1, inside_mat, CU_RESISTIVITY, &(g_tp.peri_global));
		global_data = new Wire(wt, sqrt(length_bank * height_bank), 1, 1, 1, outside_mat, CU_RESISTIVITY, &(g_tp.peri_global));

		if(semi_repeated_global_line)
		{
			C_global_data =  g_tp.wire_inside_mat.C_per_um * (subarray_height + g_tp.wire_local.pitch);
			R_global_data =  g_tp.wire_inside_mat.R_per_um * (subarray_height + g_tp.wire_local.pitch) ;

		}
		else
		{
			C_global_data = g_tp.wire_inside_mat.C_per_um * height_bank /2;
			R_global_data = g_tp.wire_inside_mat.R_per_um * height_bank /2;
		}

		global_data_drv = new Driver(
				0,
				C_global_data,
				R_global_data,
				is_dram);
		global_data_drv->compute_delay(0);
		global_data_drv->compute_area();
		//---Unrepeated local dataline
		double local_data_c_gate_load  = dp.num_c_subarray * drain_C_(g_tp.w_nmos_sa_mux, NCH, 1, 0, cell.w, is_dram);
		//double local_data_c_gate_load  = 0;
		double local_data_c_wire_load  = dp.num_c_subarray * g_tp.dram.b_w * g_tp.wire_inside_mat.C_per_um;
		double local_data_r_wire_load  = dp.num_c_subarray * g_tp.dram.b_w * g_tp.wire_inside_mat.R_per_um;
		//double local_data_r_gate_load  = tr_R_on(g_tp.w_nmos_sa_mux, NCH, 1, is_dram);
		double local_data_r_gate_load  = 0;

		double tf = (local_data_c_gate_load + local_data_c_wire_load) * (local_data_r_wire_load + local_data_r_gate_load);
		double this_delay = horowitz(0, tf, 0.5, 0.5, RISE);
		//double local_data_outrisetime = this_delay/(1.0-0.5);

		//---Unrepeated and undriven local dataline, not significant growth
		//local_data->delay = this_delay;
		//local_data->power.readOp.dynamic = (local_data_c_gate_load + local_data_c_wire_load) * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd;


		double data_drv_c_gate_load  = local_data_c_gate_load;
		double data_drv_c_wire_load  = local_data_c_wire_load;
		double data_drv_r_wire_load  = local_data_r_gate_load + local_data_r_wire_load;

		//---Assume unrepeated global data path, too high RC
		//double data_drv_c_wire_load  = height_bank * g_tp.wire_outside_mat.C_per_um;
		//double data_drv_r_wire_load  = height_bank * g_tp.wire_inside_mat.R_per_um;


		local_data_drv = new Driver(
				data_drv_c_gate_load,
				data_drv_c_wire_load,
				data_drv_r_wire_load,
				is_dram);
		local_data_drv->compute_delay(0);
		local_data_drv->compute_area();

		if (g_ip->print_detail_debug)
		{
			cout<<"C: "<<local_data_c_gate_load + local_data_c_wire_load <<" F"<<endl;
			cout<<"R: "<<local_data_r_gate_load + local_data_r_wire_load <<" Ohm"<<endl;
			cout<<"this_delay" << this_delay * 1e9 <<" ns"<<endl;
			cout<<" local_data_drv delay: " << local_data_drv->delay * 1e9 <<" ns"<<endl;
		}


		//Not accounted for.
		/*local_data_drv = new Driver(
    		global_data->repeater_size * gate_C(g_tp.min_w_nmos_+min_w_pmos,0),
    		global_data->repeater_spacing * g_tp.wire_inside_mat.C_per_um,
    		global_data->repeater_spacing * g_tp.wire_inside_mat.R_per_um,
    		is_dram);*/

		//bank_bus_length = double(num_banks_ver_dir) * 0.5 * (height_bank + 0.5*double(row_add_bits+col_add_bits+data_bits)*g_tp.wire_outside_mat.pitch) - height_bank + length_bank;
		bank_bus_length = double(num_banks_ver_dir) * 0.5 * MAX(length_bank, height_bank);
		bank_bus = new Wire(wt, bank_bus_length);
		if (g_ip->print_detail_debug)
			cout << "memorybus.cc: bank_bus_length = " << bank_bus_length << endl;

		out_seg = new Wire(wt, 0.25 * num_banks_hor_dir * (length_bank + (row_add_bits+col_add_bits+data_bits)*g_tp.wire_outside_mat.pitch) );
		area_IOSA = (875+500)*g_ip->F_sz_um*g_ip->F_sz_um * data_bits;//Reference:
		area_data_drv = local_data_drv->area.get_area() * data_bits;
		if(ndbl>16)
		{
			area_IOSA *= (double)ndbl/16.0;
			area_data_drv *= (double)ndbl/16.0;
		}
		area_local_dataline = data_bits*subarray_width *g_tp.wire_local.pitch*ndbl;

	}


    // Row decoder
	if (membus_type == Row_add_path || membus_type == Col_add_path )
	{

		if (g_ip->print_detail_debug)
		{
			cout << "memorybus.cc: num_dec_signals = " << num_dec_signals << endl;
			cout << "memorybus.cc: C_ld_dec_out = " << C_ld_dec_out << endl;
			cout << "memorybus.cc: R_wire_dec_out = " << R_wire_dec_out << endl;
			cout << "memorybus.cc: is_dram = " << is_dram << endl;
			cout << "memorybus.cc: cell.h = " << cell.h << endl;
		}

		add_dec = new Decoder(
				(num_dec_signals>16)?num_dec_signals:16,
				false,
				C_ld_dec_out,
				R_wire_dec_out,
				false,
				is_dram,
				membus_type == Row_add_path?true:false,
						cell);



		// Predecoder and decoder for GWL
		double C_wire_predec_blk_out;
		double R_wire_predec_blk_out;
		C_wire_predec_blk_out  = 0; // num_subarrays_per_row * dp.num_r_subarray * g_tp.wire_inside_mat.C_per_um * cell.h;
		R_wire_predec_blk_out  = 0; // num_subarrays_per_row * dp.num_r_subarray * g_tp.wire_inside_mat.R_per_um * cell.h;


		//int num_subarrays_per_mat = dp.num_subarrays/dp.num_mats;
		int num_dec_per_predec = 1;
		PredecBlk * add_predec_blk1 = new PredecBlk(
				num_dec_signals,
				add_dec,
				C_wire_predec_blk_out,
				R_wire_predec_blk_out,
				num_dec_per_predec,
				is_dram,
				true);



		PredecBlk * add_predec_blk2 = new PredecBlk(
				num_dec_signals,
				add_dec,
				C_wire_predec_blk_out,
				R_wire_predec_blk_out,
				num_dec_per_predec,
				is_dram,
				false);



		PredecBlkDrv * add_predec_blk_drv1 = new PredecBlkDrv(0, add_predec_blk1, is_dram);
		PredecBlkDrv * add_predec_blk_drv2 = new PredecBlkDrv(0, add_predec_blk2, is_dram);

		add_predec            = new Predec(add_predec_blk_drv1, add_predec_blk_drv2);



		if (membus_type == Row_add_path)
		{
			area_row_predec_dec = add_predec_blk_drv1->area.get_area() + add_predec_blk_drv2->area.get_area() +
					add_predec_blk1->area.get_area() + add_predec_blk2->area.get_area() + num_dec_signals * add_dec->area.get_area();


			area_lwl_drv = num_lwl_drv/2.0 * dp.num_r_subarray * ndbl * lwl_drv->area.get_area(); //num_lwl_drv is ndwl/the lwl driver count one gwl connects. two adjacent lwls share one driver.

			if (g_ip->print_detail_debug)
			{
				cout<<"memorybus.cc: area_bank_vertical_peripheral_circuitry = " << area_bank_vertical_peripheral_circuitry /1e6<<" mm2"<<endl;
				cout<<"memorybus.cc: lwl drv area = " << lwl_drv->area.get_area() /1e6<<" mm2"<<endl;
				cout<<"memorybus.cc: total lwl drv area = " << num_lwl_drv * dp.num_r_subarray
						* ndbl * lwl_drv->area.get_area() /1e6<<" mm2"<<endl;
			}
		}
		else if (membus_type == Col_add_path)
		{
			area_col_predec_dec = add_predec_blk_drv1->area.get_area() + add_predec_blk_drv2->area.get_area() +
					add_predec_blk1->area.get_area() + add_predec_blk2->area.get_area() + num_dec_signals * add_dec->area.get_area();
			if(ndbl>16)
			{
				area_col_predec_dec *= (double)ndbl/16.0;
			}
		}

		area_bank_vertical_peripheral_circuitry = area_row_predec_dec + area_lwl_drv + area_address_bus + area_data_bus ;
		area_bank_horizontal_peripheral_circuitry = area_col_predec_dec + area_data_drv + (area_bus + area_IOSA)/g_ip->nbanks;

		if (g_ip->print_detail_debug)
		{
			cout<<"memorybus.cc: add_predec_blk_drv1->area = " << add_predec_blk_drv1->area.get_area() /1e6<<" mm2"<<endl;
			cout<<"memorybus.cc: add_predec_blk_drv2->area = " << add_predec_blk_drv2->area.get_area() /1e6<<" mm2"<<endl;
			cout<<"memorybus.cc: add_predec_blk1->area = " << add_predec_blk1->area.get_area() /1e6<<" mm2"<<endl;
			cout<<"memorybus.cc: add_predec_blk2->area = " << add_predec_blk2->area.get_area() /1e6<<" mm2"<<endl;
			cout<<"memorybus.cc: total add_dec->area = " << num_dec_signals * add_dec->area.get_area() /1e6<<" mm2"<<endl;
			cout<<"wire bus width for one bank = " << g_tp.wire_outside_mat.pitch * double(add_bits + add_bits + data_bits);
		}

		area.h   = (height_bank + area_bank_horizontal_peripheral_circuitry /length_bank) * num_banks_ver_dir;
		area.w   = (length_bank + area_bank_vertical_peripheral_circuitry	/height_bank) * num_banks_hor_dir; // bank bus, should add cmd wire and predec/decoder space
		if(g_ip->partition_gran == 0)
		{
			area.h += g_tp.wire_outside_mat.pitch * double(add_bits + add_bits + data_bits); //center_stripe, should add cmd wire and other componets
			area.w += g_tp.wire_outside_mat.pitch * double(add_bits + add_bits + data_bits); // + g_tp.wire_outside_mat.pitch * add_bits * 2.5;
		}
		//---This coefficient comes from the extra overhead of voltage regulator,
		//---control logic, bank fuse, burst logic and I/O, see
		//--- A 5.6ns Random Cycle 144Mb DRAM with 1.4Gb/s/pin and DDR3-SRAM Interface

		//area.w *= 1.0672;
		//area.h *= 1.0672;

		if (g_ip->print_detail_debug)
		{
			cout<<"memorybus.cc: circuit height = "<<area_bank_horizontal_peripheral_circuitry /length_bank /1e3<<" mm"<<endl;
			cout<<"memorybus.cc: circuit length = "<<area_bank_vertical_peripheral_circuitry /height_bank /1e3<<" mm"<<endl;
			cout<<"memorybus.cc: area.h = "<<area.h/1e3<<" mm"<<endl;
			cout<<"memorybus.cc: area.w = "<<area.w/1e3<<" mm"<<endl;
			cout<<"memorybus.cc: area = "<<area.get_area()/1e6<<" mm2"<<endl;
		}

	}


	compute_delays(0);
	compute_power_energy();

}


// This is based on the same function in mat.cc
double Memorybus::compute_delays(double inrisetime)
{

//	double outrisetime = 0;
	double predec_outrisetime = 0, add_dec_outrisetime = 0;
	double lwl_drv_outrisetime = 0;///, tf = 0;
	//double local_data_drv_outrisetime = 0;

	if (membus_type == Data_path)
	{
		delay = 0;
		delay_bus = center_stripe->delay + bank_bus->delay;
		delay += delay_bus;
		//outrisetime = local_data_drv->compute_delay(inrisetime);
		//local_data_drv_outrisetime = local_data_drv->delay;
		delay_global_data = (semi_repeated_global_line >0) ? (global_data_drv->delay*num_subarray_global_IO) : (global_data_drv->delay + global_data->delay);
		if(g_ip->partition_gran==0 || g_ip->partition_gran==1)
			delay += delay_global_data;
		//delay += local_data->delay;
		delay_local_data = local_data_drv->delay;
		delay += delay_local_data;
		delay_data_buffer = 2 * 1e-6/(double)g_ip->sys_freq_MHz;
		//delay += bank.mat.delay_subarray_out_drv_htree;
		delay += delay_data_buffer;
		//cout << 1e3/(double)g_ip->sys_freq_MHz<< endl;
		//delay += out_seg->delay * burst_length;
		if (g_ip->print_detail_debug)
			cout << "memorybus.cc: data path delay = " << delay << endl;
		out_rise_time = 0;
	}
	else
	{
		delay = 0;
		delay_bus = center_stripe->delay + bank_bus->delay;
		delay += delay_bus;
		predec_outrisetime = add_predec->compute_delays(inrisetime);
		add_dec_outrisetime = add_dec->compute_delays(predec_outrisetime);
		delay_add_predecoder = add_predec->delay;
		delay += delay_add_predecoder;

		if (membus_type == Row_add_path)
		{
			if(semi_repeated_global_line)
			{
				delay_add_decoder = add_dec->delay * ndwl;
				if(g_ip->page_sz_bits > 8192)
					delay_add_decoder /= (double)(g_ip->page_sz_bits / 8192);
			}
			else
			{
				delay_add_decoder = add_dec->delay;
			}
			delay += delay_add_decoder;
			// There is no function to compute_delay in wire.cc, need to double check if center_stripe->delay and bank_bus->delay is correct.
			lwl_drv_outrisetime = lwl_drv->compute_delay(add_dec_outrisetime);
			///tf = (lwl_driver_c_gate_load + lwl_driver_c_wire_load) * lwl_driver_r_wire_load;
			// ### no need for global_WL->delay
			// delay_WL = global_WL->delay + lwl_drv->delay + horowitz(lwl_drv_outrisetime, tf, 0.5, 0.5, RISE);
			delay_lwl_drv = lwl_drv->delay;
			if(!g_ip->fine_gran_bank_lvl)
				delay += delay_lwl_drv;
			if (g_ip->print_detail_debug)
				cout << "memorybus.cc: row add path delay = " << delay << endl;

			out_rise_time =  lwl_drv_outrisetime;
		}

		else if (membus_type == Col_add_path)
		{
			if(semi_repeated_global_line)
			{
				delay_add_decoder = add_dec->delay * num_subarray_global_IO;
			}
			else
			{
				delay += column_sel->delay;
				delay_add_decoder = add_dec->delay;
			}
			delay += delay_add_decoder;

			out_rise_time = 0;
			  if (g_ip->print_detail_debug)
			  {
					//cout << "memorybus.cc, compute_delays col: center_stripe->delay = " << center_stripe->delay << endl;
					//cout << "memorybus.cc, compute_delays col: bank_bus->delay = " << bank_bus->delay << endl;
					//cout << "memorybus.cc, compute_delays col: add_predec->delay = " << add_predec->delay << endl;
					//cout << "memorybus.cc, compute_delays col: add_dec->delay = " << add_dec->delay << endl;

				  cout << "memorybus.cc: column add path delay = " << delay << endl;
			  }

		}
		else
		{
			assert(0);
		}
	}


	// Double check!
	out_rise_time = delay / (1.0-0.5);
	// Is delay_wl_reset necessary here? Is the 'false' condition appropriate? See the same code as in mat.cc
	/*if (add_dec->exist == false)
	{
		int delay_wl_reset = MAX(add_predec->blk1->delay, add_predec->blk2->delay);
		//delay += delay_wl_reset;
	}*/

	return out_rise_time;
}




void Memorybus::compute_power_energy()
{
	double coeff1[4] = {(double)add_bits, (double)add_bits, (double)add_bits, (double)add_bits};
	double coeff2[4] = {(double)data_bits, (double)data_bits, (double)data_bits, (double)data_bits};
	double coeff3[4] = {(double)num_lwl_drv, (double)num_lwl_drv, (double)num_lwl_drv, (double)num_lwl_drv};
	double coeff4[4] = {(double)burst_length*chip_IO_width, (double)burst_length*chip_IO_width,
			(double)burst_length*chip_IO_width, (double)burst_length*chip_IO_width};
	double coeff5[4] = {(double)ndwl, (double)ndwl, (double)ndwl, (double)ndwl};
	double coeff6[4] = {(double)num_subarray_global_IO, (double)num_subarray_global_IO, (double)num_subarray_global_IO, (double)num_subarray_global_IO};

	//double coeff4[4] = {(double)num_dec_signals, (double)num_dec_signals, (double)num_dec_signals, (double)num_dec_signals};
	switch (membus_type)
	{
	case Data_path:
		power_bus = (center_stripe->power + bank_bus->power) * coeff2;
		power_local_data = local_data_drv->power * coeff2;
		power_global_data = semi_repeated_global_line >0 ? (global_data_drv->power*coeff2) : (global_data_drv->power+global_data->power);

		power_global_data.readOp.dynamic = power_global_data.readOp.dynamic + 1.8/1e3*deviceType->Vdd*10.0/1e9/64*data_bits;
		power = power_bus + power_local_data;
		if(!g_ip->fine_gran_bank_lvl)
			power = power + power_global_data;
		//power += local_data->power;

		power_burst = out_seg->power * coeff4;//Account for burst read, approxmate the wire length by the center stripe
		//power = power + power_burst;
		if(g_ip->print_detail_debug)
		{
			cout << "memorybus.cc: data path center stripe energy = " << center_stripe->power.readOp.dynamic*1e9 << " nJ" << endl;
			cout << "memorybus.cc: data path bank bus energy = " << bank_bus->power.readOp.dynamic*1e9 << " nJ" << endl;
			cout << "memorybus.cc: data path data driver energy = " << local_data_drv->power.readOp.dynamic*1e9 << " nJ" << endl;
		}
		break;
	case Row_add_path:
		power_bus = (center_stripe->power + bank_bus->power) * coeff1;
		power_add_predecoder = add_predec->power;
		if(semi_repeated_global_line)
		{
			power_add_decoders = add_dec->power * coeff5;
			//power_add_decoders.readOp.dynamic /= (g_ip->page_sz_bits > 8192)?((double)g_ip->page_sz_bits/8192):1;
			if(g_ip->page_sz_bits > 8192)
				power_add_decoders.readOp.dynamic /= (double)(g_ip->page_sz_bits / 8192);
		}
		else
			power_add_decoders = add_dec->power;// * (1<< add_predec->blk1->number_input_addr_bits);
		power_lwl_drv = lwl_drv->power * coeff3;
		//power_local_WL.readOp.dynamic = num_lwl_drv * C_LWL * deviceType->Vdd * deviceType->Vdd;
		power = power_bus + power_add_predecoder + power_add_decoders + power_lwl_drv;
		break;
	case Col_add_path:
		power_bus = (center_stripe->power + bank_bus->power) * coeff1;// + column_sel->power * double(chip_IO_width * burst_length);
		power_add_predecoder = add_predec->power;
		if(semi_repeated_global_line)
		{
			power_add_decoders = add_dec->power * coeff6;
			power_add_decoders.readOp.dynamic = power_add_decoders.readOp.dynamic * g_ip->page_sz_bits / data_bits;
			power_col_sel.readOp.dynamic = 0;
		}
		else
		{
			power_add_decoders = add_dec->power;// * (1<< add_predec->blk1->number_input_addr_bits);
		    power_col_sel.readOp.dynamic = column_sel->power.readOp.dynamic * g_ip->page_sz_bits / data_bits;
		}
		power = power_bus + power_add_predecoder + power_add_decoders;
		if(!g_ip->fine_gran_bank_lvl)
				power = power + power_col_sel;
		break;
	default:
		assert(0);
		break;
	}

	return;

}



