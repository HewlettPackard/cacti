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



#include <iostream>
#include <math.h>

#include "uca.h"
#include "TSV.h"
#include "memorybus.h"


UCA::UCA(const DynamicParameter & dyn_p)
 :dp(dyn_p), bank(dp), nbanks(g_ip->nbanks), refresh_power(0)
{
  int num_banks_ver_dir = 1 << ((bank.area.h > bank.area.w) ? _log2(nbanks)/2 : (_log2(nbanks) - _log2(nbanks)/2));
  int num_banks_hor_dir = nbanks/num_banks_ver_dir;

  if (dp.use_inp_params)
  {
	  RWP  = dp.num_rw_ports;
	  ERP  = dp.num_rd_ports;
	  EWP  = dp.num_wr_ports;
	  SCHP = dp.num_search_ports;
  }
  else
  {
	  RWP  = g_ip->num_rw_ports;
	  ERP  = g_ip->num_rd_ports;
	  EWP  = g_ip->num_wr_ports;
	  SCHP = g_ip->num_search_ports;
  }

  num_addr_b_bank = (dp.number_addr_bits_mat + dp.number_subbanks_decode)*(RWP+ERP+EWP);
  num_di_b_bank   = dp.num_di_b_bank_per_port * (RWP + EWP);
  num_do_b_bank   = dp.num_do_b_bank_per_port * (RWP + ERP);
  num_si_b_bank   = dp.num_si_b_bank_per_port * SCHP;
  num_so_b_bank   = dp.num_so_b_bank_per_port * SCHP;

  if (!dp.fully_assoc && !dp.pure_cam)
  {

	  if (g_ip->fast_access && dp.is_tag == false)
	  {
		  num_do_b_bank *= g_ip->data_assoc;
	  }

	  htree_in_add   = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
			  num_addr_b_bank, num_di_b_bank,0, num_do_b_bank,0,num_banks_ver_dir*2, num_banks_hor_dir*2, Add_htree, true);
	  htree_in_data  = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
			  num_addr_b_bank, num_di_b_bank, 0, num_do_b_bank, 0, num_banks_ver_dir*2, num_banks_hor_dir*2, Data_in_htree, true);
	  htree_out_data = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
			  num_addr_b_bank, num_di_b_bank, 0, num_do_b_bank, 0, num_banks_ver_dir*2, num_banks_hor_dir*2, Data_out_htree, true);
  }

  else
  {

	  htree_in_add   = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
			  num_addr_b_bank, num_di_b_bank, num_si_b_bank, num_do_b_bank, num_so_b_bank, num_banks_ver_dir*2, num_banks_hor_dir*2, Add_htree, true);
	  htree_in_data  = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
			  num_addr_b_bank, num_di_b_bank,num_si_b_bank, num_do_b_bank, num_so_b_bank, num_banks_ver_dir*2, num_banks_hor_dir*2, Data_in_htree, true);
	  htree_out_data = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
			  num_addr_b_bank, num_di_b_bank,num_si_b_bank, num_do_b_bank, num_so_b_bank, num_banks_ver_dir*2, num_banks_hor_dir*2, Data_out_htree, true);
	  htree_in_search  = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
			  num_addr_b_bank, num_di_b_bank,num_si_b_bank, num_do_b_bank, num_so_b_bank, num_banks_ver_dir*2, num_banks_hor_dir*2, Data_in_htree, true);
	  htree_out_search = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
			  num_addr_b_bank, num_di_b_bank,num_si_b_bank, num_do_b_bank, num_so_b_bank, num_banks_ver_dir*2, num_banks_hor_dir*2, Data_out_htree, true);
  }

  area.w = htree_in_data->area.w;
  area.h = htree_in_data->area.h;

  area_all_dataramcells = bank.mat.subarray.get_total_cell_area() * dp.num_subarrays * g_ip->nbanks;
//  cout<<"area cell"<<area_all_dataramcells<<endl;
//  cout<<area.get_area()<<endl;

  if (g_ip->print_detail_debug)
	  cout << "uca.cc: g_ip->is_3d_mem = " << g_ip->is_3d_mem << endl;
  if(g_ip->is_3d_mem)
  {
	  membus_RAS = new Memorybus(g_ip->wt, bank.mat.area.w, bank.mat.area.h, bank.mat.subarray.area.w, bank.mat.subarray.area.h,
			  _log2(dp.num_r_subarray * dp.Ndbl), _log2(dp.num_c_subarray * dp.Ndwl), g_ip->burst_depth*g_ip->io_width, dp.Ndbl, dp.Ndwl, Row_add_path, dp);
	  membus_CAS = new Memorybus(g_ip->wt, bank.mat.area.w, bank.mat.area.h, bank.mat.subarray.area.w, bank.mat.subarray.area.h,
			  _log2(dp.num_r_subarray * dp.Ndbl), _log2(dp.num_c_subarray * dp.Ndwl), g_ip->burst_depth*g_ip->io_width, dp.Ndbl, dp.Ndwl, Col_add_path, dp);
	  membus_data = new Memorybus(g_ip->wt, bank.mat.area.w, bank.mat.area.h, bank.mat.subarray.area.w, bank.mat.subarray.area.h,
			  _log2(dp.num_r_subarray * dp.Ndbl), _log2(dp.num_c_subarray * dp.Ndwl), g_ip->burst_depth*g_ip->io_width, dp.Ndbl, dp.Ndwl, Data_path, dp);
	  area.h = membus_RAS->area.h;
	  area.w = membus_RAS->area.w;

	  if (g_ip->print_detail_debug)
	  {
		  cout<<"uca.cc: area.h = "<<area.h/1e3<<" mm"<<endl;
		  cout<<"uca.cc: area.w = "<<area.w/1e3<<" mm"<<endl;
		  cout<<"uca.cc: bank.area.h = "<<bank.area.h/1e3<<" mm"<<endl;
		  cout<<"uca.cc: bank.area.w = "<<bank.area.w/1e3<<" mm"<<endl;
		  cout<<"uca.cc: bank.mat.area.h = "<<bank.mat.area.h/1e3<<" mm"<<endl;
		  cout<<"uca.cc: bank.mat.area.w = "<<bank.mat.area.w/1e3<<" mm"<<endl;

		  cout<<"uca.cc: bank.mat.subarray.area.h = "<<bank.mat.subarray.area.h/1e3<<" mm"<<endl;
		  cout<<"uca.cc: bank.mat.subarray.area.w = "<<bank.mat.subarray.area.w/1e3<<" mm"<<endl;
		  cout<<"uca.cc: dp.num_subarrays = "<<dp.num_subarrays<<endl;

		  //cout<<"uca.cc: area_all_dataramcells: "<<area_all_dataramcells/1e6<<" mm2"<<endl;
		  //****************** Final results for output ****************
		  cout<<"uca.cc: area efficiency = "<<area_all_dataramcells/(area.w*area.h)*100 <<" %"<<endl;
		  //cout<<area.get_area()<<endl;
		  cout<<"uca.cc: area = "<<area.get_area()/1e6<<" mm2"<<endl;

	  }


  }
	  // delay calculation
	  double inrisetime = 0.0;
	  compute_delays(inrisetime);
	  compute_power_energy();

	if(g_ip->is_3d_mem)
	{
		// Add TSV delay to the terms
		// --- Although there are coarse and fine, because is_array and os_bank TSV are the same, so they are the same
		TSV tsv_os_bank(Coarse);
		TSV tsv_is_subarray(Fine);
		if(g_ip->print_detail_debug)
		{
			tsv_os_bank.print_TSV();
			tsv_is_subarray.print_TSV();
		}

	comm_bits = 6;
	row_add_bits = _log2(dp.num_r_subarray * dp.Ndbl);
	col_add_bits = _log2(dp.num_c_subarray * dp.Ndwl);
	data_bits = g_ip->burst_depth * g_ip->io_width;

	//enum Part_grain part_gran = Fine_rank_level;

	double redundancy_perc_TSV = 0.5;
	switch(g_ip->partition_gran)
	{
	case 0:// Coarse_rank_level:
		delay_TSV_tot = (g_ip->num_die_3d-1) * tsv_os_bank.delay;
		num_TSV_tot = (comm_bits + row_add_bits + col_add_bits + data_bits*2) * (1 + redundancy_perc_TSV); //* (g_ip->nbanks/4)
		area_TSV_tot = num_TSV_tot * tsv_os_bank.area.get_area();
		dyn_pow_TSV_tot = num_TSV_tot * (g_ip->num_die_3d-1) * tsv_os_bank.power.readOp.dynamic;
		dyn_pow_TSV_per_access = (comm_bits + row_add_bits + col_add_bits + data_bits) * (g_ip->num_die_3d-1) * tsv_os_bank.power.readOp.dynamic;
		area_address_bus = membus_RAS->area_address_bus * (1.0 + (double)comm_bits/(double)(row_add_bits + col_add_bits));
		area_data_bus = membus_RAS->area_data_bus;
		break;
	case 1://Fine_rank_level:
		delay_TSV_tot = (g_ip->num_die_3d) * tsv_os_bank.delay;
		num_TSV_tot = (comm_bits + row_add_bits + col_add_bits + data_bits/2) * g_ip->nbanks * (1 + redundancy_perc_TSV);
		area_TSV_tot = num_TSV_tot * tsv_os_bank.area.get_area();
		dyn_pow_TSV_tot = num_TSV_tot * (g_ip->num_die_3d) * tsv_os_bank.power.readOp.dynamic;
		dyn_pow_TSV_per_access = (comm_bits + row_add_bits + col_add_bits + data_bits) * (g_ip->num_die_3d) * tsv_os_bank.power.readOp.dynamic;
		//area_address_bus = (comm_bits + row_add_bits + col_add_bits) * 25.0;
		//area_data_bus = membus_RAS->area_data_bus + (double)data_bits/2 * 25.0;
		break;
	case 2://Coarse_bank_level:
		delay_TSV_tot = (g_ip->num_die_3d) * tsv_os_bank.delay;
		num_TSV_tot = (comm_bits + row_add_bits + col_add_bits + data_bits/2) * g_ip->nbanks
		* g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd * (1 + redundancy_perc_TSV);
		area_TSV_tot = num_TSV_tot * tsv_os_bank.area.get_area();
		dyn_pow_TSV_tot = num_TSV_tot * (g_ip->num_die_3d) * tsv_os_bank.power.readOp.dynamic;
		dyn_pow_TSV_per_access = (comm_bits + row_add_bits + col_add_bits + data_bits) * (g_ip->num_die_3d) * tsv_os_bank.power.readOp.dynamic;
		//area_address_bus = (comm_bits + row_add_bits + col_add_bits) * 25.0;
		//area_data_bus = (double)data_bits/2 * 25.0;

		//activate_energy *= g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd;
		//read_energy *= g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd;
		//write_energy *= g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd;
		//precharge_energy *= g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd;
		break;
	case 3://Fine_bank_level:
		delay_TSV_tot = (g_ip->num_die_3d) * tsv_os_bank.delay;
		num_TSV_tot = (comm_bits + row_add_bits + col_add_bits + data_bits) * g_ip->nbanks *g_ip->ndwl *g_ip->ndbl
		/g_ip->num_tier_col_sprd /g_ip->num_tier_row_sprd * (1 + redundancy_perc_TSV);
		area_TSV_tot = num_TSV_tot * tsv_os_bank.area.get_area();
		dyn_pow_TSV_tot = num_TSV_tot * (g_ip->num_die_3d) * tsv_os_bank.power.readOp.dynamic;
		dyn_pow_TSV_per_access = (comm_bits + row_add_bits + col_add_bits + data_bits) * (g_ip->num_die_3d) * tsv_os_bank.power.readOp.dynamic;
		//area_address_bus = pow(2, (comm_bits + row_add_bits + col_add_bits)) * 25.0;
		//area_data_bus = pow(2, data_bits/2) * 25.0;
		//activate_energy *= g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd;
		//read_energy *= g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd;
		//write_energy *= g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd;
		//precharge_energy *= g_ip->num_tier_row_sprd * g_ip->num_tier_col_sprd;
		break;
	default:
		assert(0);
		break;
	}

	if(g_ip->print_detail_debug)
	{
		cout << "uca.cc: num_TSV_tot = " << num_TSV_tot << endl;
	}

	area_lwl_drv = membus_RAS->area_lwl_drv * g_ip->nbanks;
	area_row_predec_dec = membus_RAS->area_row_predec_dec * g_ip->nbanks;
	area_col_predec_dec = membus_CAS->area_col_predec_dec * g_ip->nbanks;

	area_subarray = membus_RAS->area_subarray * g_ip->nbanks;
	area_bus = membus_RAS->area_bus * g_ip->nbanks;


	area_data_drv = membus_data->area_data_drv * g_ip->nbanks;
	area_IOSA = membus_data->area_IOSA * g_ip->nbanks;
	area_sense_amp = membus_data->area_sense_amp * g_ip->nbanks;

	area_address_bus = membus_RAS->area_address_bus * (1.0 + (double)comm_bits/(double)(row_add_bits + col_add_bits))  * g_ip->nbanks;;
	area_data_bus = membus_RAS->area_data_bus + membus_data->area_local_dataline * g_ip->nbanks;

	area_per_bank = (area_lwl_drv + area_row_predec_dec + area_col_predec_dec
			+ area_subarray + area_bus + area_data_drv + area_IOSA
			+ area_address_bus + area_data_bus)/g_ip->nbanks + area_sense_amp;


		t_RCD += delay_TSV_tot;
		t_RAS += delay_TSV_tot;
		t_RC += delay_TSV_tot;
		t_RP += delay_TSV_tot;
		t_CAS += 2 * delay_TSV_tot;
		t_RRD += delay_TSV_tot;

		activate_energy += dyn_pow_TSV_per_access;
		read_energy += dyn_pow_TSV_per_access;
		write_energy += dyn_pow_TSV_per_access;
		precharge_energy += dyn_pow_TSV_per_access;

		//double area_per_die = area.get_area();
		//double area_stack_tot = g_ip->num_die_3d * (area.get_area() + area_TSV_tot);
		//int num_die = g_ip->num_die_3d;
		//area.set_area(area_stack_tot);

		if(g_ip->num_die_3d > 1 || g_ip->partition_gran > 0)
			total_area_per_die = area_all_dataramcells + area_TSV_tot;
		else
			total_area_per_die = area_all_dataramcells;



	if(g_ip->is_3d_mem && g_ip->print_detail_debug)
	{

		cout<<"-------  CACTI 3D DRAM Main Memory -------"<<endl;

		cout << "\nMemory Parameters:\n";
		cout << "	Total memory size (Gb): " <<
				(int) (g_ip->cache_sz) << endl;
		cout << "	Number of banks: " << (int) g_ip->nbanks << endl;
		cout << "	Technology size (nm): " <<
				g_ip->F_sz_nm << endl;
		cout << "	Page size (bits): " << g_ip->page_sz_bits << endl;
		cout << "	Burst depth: " << g_ip->burst_depth << endl;
		cout << "	Chip IO width: " << g_ip->io_width << endl;
		cout << "	Ndwl: " << dp.Ndwl << endl;
		cout << "	Ndbl: " << dp.Ndbl << endl;
		cout << "	# rows in subarray: " << dp.num_r_subarray << endl;
		cout << "	# columns in subarray: " << dp.num_c_subarray << endl;

		cout << "\nResults:\n";
		cout<<"	******************Timing terms******************"<<endl;
		cout<<"	t_RCD (Row to Column command Delay): "<<t_RCD * 1e9 << " ns" <<endl;
		cout<<"	t_RAS (Row Access Strobe latency): "<<t_RAS * 1e9  << " ns" <<endl;
		cout<<"	t_RC (Row Cycle): "<<t_RC * 1e9  << " ns" <<endl;
		cout<<"	t_CAS (Column Access Strobe latency): "<<t_CAS * 1e9  << " ns" <<endl;
		cout<<"	t_RP (Row Precharge latency): "<<t_RP* 1e9  << " ns" <<endl;
		//cout<<"	t_RRD (Row activation to Row activation Delay): "<<t_RRD * 1e9 << " ns"<<endl;
		cout<<"	*******************Power terms******************"<<endl;
		cout<<"	Activation energy: "<<activate_energy * 1e9  << " nJ" <<endl;
		cout<<"	Read energy: "<<read_energy * 1e9  << " nJ" <<endl;
		cout<<"	Write energy: "<<write_energy * 1e9  << " nJ" <<endl;
		//cout<<"	Precharge energy: "<<precharge_energy * 1e9  << " nJ" <<endl;
		cout<<"	Activation power: "<< activate_power * 1e3   << " mW" <<endl;
		cout<<"	Read power: "<< read_power * 1e3 << " mW" <<endl;
		cout<<"	Write power: "<< write_power * 1e3  << " mW" <<endl;
		//cout<<"	Peak read power: "<< read_energy/((g_ip->burst_depth)/(g_ip->sys_freq_MHz*1e6)/2) * 1e3  << " mW" <<endl;
		cout<<"	********************Area terms******************"<<endl;
		//cout<<"	Height: "<<area.h/1e3<<" mm"<<endl;
		//cout<<"	Length: "<<area.w/1e3<<" mm"<<endl;
		cout<<"	DRAM+peri Area: "<<area.get_area()/1e6<<" mm2"<<endl;
		//cout<<"	Area efficiency: "<<area_all_dataramcells/area.get_area()*100 <<" %"<<endl;
		cout<<"	Total Area: "<<total_area_per_die/1e6/0.5<<" mm2"<<endl;

		if(g_ip->print_detail_debug)
		{
			cout<<"	********************Other terms******************"<<endl;
			double act_bus_energy = membus_RAS->center_stripe->power.readOp.dynamic + membus_RAS->bank_bus->power.readOp.dynamic
					+ membus_RAS->add_predec->power.readOp.dynamic + membus_RAS->add_dec->power.readOp.dynamic;
			cout<<"	Act Bus Energy: "<< act_bus_energy * 1e9 <<" nJ"<<endl;
			cout<<"	Act Bank Energy: "<< (activate_energy - act_bus_energy) * 1e9 <<" nJ"<<endl;
			double act_bus_latency = membus_RAS->center_stripe->delay + membus_RAS->bank_bus->delay
					+ membus_RAS->add_predec->delay + membus_RAS->add_dec->delay;
			cout<<"	Act Bus Latency: "<< act_bus_latency * 1e9 <<" ns"<<endl;
			cout<<"	Act Bank Latency: "<< (t_RCD - act_bus_latency) * 1e9 <<" ns"<<endl;
			cout<<"	activate_energy: "<< activate_energy*1e9 <<" nJ"<<endl;
		}

		if (g_ip->num_die_3d>1)
		{
			cout<<"	********************TSV terms******************"<<endl;
			cout<<"	TSV area overhead: "<< area_TSV_tot /1e6 <<" mm2"<<endl;
			cout<<"	TSV latency overhead: "<< delay_TSV_tot * 1e9 <<" ns"<<endl;
			cout<<"	TSV energy overhead per access: "<< dyn_pow_TSV_per_access * 1e9 <<" nJ"<<endl;
		}
		cout<<endl<<endl<<endl;
	}
	}
}



UCA::~UCA()
{
  delete htree_in_add;
  delete htree_in_data;
  delete htree_out_data;

  if(g_ip->is_3d_mem)
  {
	  delete membus_RAS;
	  delete membus_CAS;
	  delete membus_data;
  }
}



double UCA::compute_delays(double inrisetime)
{
  double outrisetime = bank.compute_delays(inrisetime);
  //CACTI3DD
  if (g_ip->is_3d_mem)
  {
	  outrisetime = bank.compute_delays(membus_RAS->out_rise_time);

	  //ram_delay_inside_mat = bank.mat.delay_bitline;// + bank.mat.delay_matchchline;
	  //access_time = membus_RAS->delay + bank.mat.delay_bitline + bank.mat.delay_sa + membus_CAS->delay + membus_data->delay;

	  //double t_rcd = membus_RAS->delay + bank.mat.delay_bitline + bank.mat.delay_sa;
	  //t_RCD= membus_RAS->add_dec->delay + membus_RAS->lwl_drv->delay + bank.mat.delay_bitline + bank.mat.delay_sa;
	  t_RCD = membus_RAS->add_dec->delay + membus_RAS->lwl_drv->delay + bank.mat.delay_bitline + bank.mat.delay_sa;
	  t_RAS = membus_RAS->delay + bank.mat.delay_bitline + bank.mat.delay_sa + bank.mat.delay_bl_restore;
	  precharge_delay = bank.mat.delay_writeback +
			  bank.mat.delay_wl_reset + bank.mat.delay_bl_restore;
	  t_RP = precharge_delay;
	  t_RC = t_RAS + t_RP;
	  t_CAS = membus_CAS->delay + bank.mat.delay_subarray_out_drv + membus_data->delay;
	  t_RRD = membus_RAS->center_stripe->delay + membus_RAS->bank_bus->delay;
	  //t_RRD = membus_RAS->delay;
	  access_time = t_RCD + t_CAS;
	  multisubbank_interleave_cycle_time = membus_RAS->center_stripe->delay + membus_RAS->bank_bus->delay;
	  //cout<<"uca.cc: multisubbank_interleave_cycle_time = "<<multisubbank_interleave_cycle_time<<endl;
	  cycle_time = t_RC + precharge_delay;
	  //cout<<"uca.cc: cycle_time = "<<cycle_time<<endl;
	  outrisetime = t_RCD/(1.0-0.5); // correct?



	  if (g_ip->print_detail_debug)
	  {
		  cout<<endl<<"Network delays: "<<endl;
		  cout<<"uca.cc: membus_RAS->delay = "<<membus_RAS->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_CAS->delay = "<<membus_CAS->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_data->delay = "<<membus_data->delay * 1e9 << " ns" <<endl;
		  cout<<"Row Address Delay components: "<<endl;
		  cout<<"uca.cc: membus_RAS->center_stripe->delay = "<<membus_RAS->center_stripe->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_RAS->bank_bus->delay = "<<membus_RAS->bank_bus->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_RAS->add_predec->delay = "<<membus_RAS->add_predec->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_RAS->add_dec->delay = "<<membus_RAS->add_dec->delay * 1e9 << " ns" <<endl;
		  //cout<<"uca.cc: membus_RAS->global_WL->delay = "<<membus_RAS->global_WL->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_RAS->lwl_drv->delay = "<<membus_RAS->lwl_drv->delay * 1e9 << " ns" <<endl;
		  cout<<"Bank Delay components: "<<endl;
		  cout<<"uca.cc: bank.mat.delay_bitline = "<<bank.mat.delay_bitline * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: bank.mat.delay_sa = "<<bank.mat.delay_sa * 1e9 << " ns" <<endl;
		  cout<<"Column Address Delay components: "<<endl;
		  //cout<<"uca.cc: membus_CAS->center_stripe->delay = "<<membus_CAS->center_stripe->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_CAS->bank_bus->delay = "<<membus_CAS->bank_bus->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_CAS->add_predec->delay = "<<membus_CAS->add_predec->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_CAS->add_dec->delay = "<<membus_CAS->add_dec->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_CAS->column_sel->delay = "<<membus_CAS->column_sel->delay * 1e9 << " ns" <<endl;
		  cout<<"Data IO Path Delay components: "<<endl;
		  cout<<"uca.cc: bank.mat.delay_subarray_out_drv = "<<bank.mat.delay_subarray_out_drv * 1e9 << " ns" <<endl;
		  //cout<<"uca.cc: membus_data->center_stripe->delay = "<<membus_data->center_stripe->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_data->bank_bus->delay = "<<membus_data->bank_bus->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_data->global_data->delay = "<<membus_data->global_data->delay * 1e9 << " ns" <<endl;
		  //cout<<"uca.cc: membus_data->data_drv->delay = "<<membus_data->data_drv->delay * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: membus_data->local_data->delay = "<<membus_data->local_data->delay * 1e9 << " ns" <<endl;
		  cout<<"Bank precharge/restore delay components: "<<endl;
		  cout<<"uca.cc: bank.mat.delay_bl_restore = "<<bank.mat.delay_bl_restore * 1e9 << " ns" <<endl;

		  cout<<"General delay components: "<<endl;
		  cout<<"uca.cc: t_RCD = "<<t_RCD * 1e9 << " ns" <<endl;
		  cout<<"uca.cc: t_RAS = "<<t_RAS * 1e9  << " ns" <<endl;
		  cout<<"uca.cc: t_RC = "<<t_RC * 1e9  << " ns" <<endl;
		  cout<<"uca.cc: t_CAS = "<<t_CAS * 1e9  << " ns" <<endl;
		  cout<<"uca.cc: t_RRD = "<<t_RRD * 1e9 << " ns"<<endl;
		  cout<<"uca.cc: access_time = "<<access_time * 1e9 << " ns" <<endl;
	  }
  }//CACTI3DD
  else
  {
  double delay_array_to_mat = htree_in_add->delay + bank.htree_in_add->delay;
  double max_delay_before_row_decoder = delay_array_to_mat + bank.mat.r_predec->delay;
  delay_array_to_sa_mux_lev_1_decoder = delay_array_to_mat +
    bank.mat.sa_mux_lev_1_predec->delay +
    bank.mat.sa_mux_lev_1_dec->delay;
  delay_array_to_sa_mux_lev_2_decoder = delay_array_to_mat +
    bank.mat.sa_mux_lev_2_predec->delay +
    bank.mat.sa_mux_lev_2_dec->delay;
  double delay_inside_mat = bank.mat.row_dec->delay + bank.mat.delay_bitline + bank.mat.delay_sa;

  delay_before_subarray_output_driver =
    MAX(MAX(max_delay_before_row_decoder + delay_inside_mat,  // row_path
            delay_array_to_mat + bank.mat.b_mux_predec->delay + bank.mat.bit_mux_dec->delay + bank.mat.delay_sa),  // col_path
        MAX(delay_array_to_sa_mux_lev_1_decoder,    // sa_mux_lev_1_path
            delay_array_to_sa_mux_lev_2_decoder));  // sa_mux_lev_2_path
  delay_from_subarray_out_drv_to_out = bank.mat.delay_subarray_out_drv_htree +
                                       bank.htree_out_data->delay + htree_out_data->delay;
  access_time                        = bank.mat.delay_comparator;

  double ram_delay_inside_mat;
  if (dp.fully_assoc)
  {
    //delay of FA contains both CAM tag and RAM data
    { //delay of CAM
      ram_delay_inside_mat = bank.mat.delay_bitline + bank.mat.delay_matchchline;
      access_time = htree_in_add->delay + bank.htree_in_add->delay;
      //delay of fully-associative data array
      access_time += ram_delay_inside_mat + delay_from_subarray_out_drv_to_out;
    }
  }
  else
  {
    access_time = delay_before_subarray_output_driver + delay_from_subarray_out_drv_to_out; //data_acc_path
  }

  if (dp.is_main_mem)
  {
    double t_rcd       = max_delay_before_row_decoder + delay_inside_mat;
    double cas_latency = MAX(delay_array_to_sa_mux_lev_1_decoder, delay_array_to_sa_mux_lev_2_decoder) +
                         delay_from_subarray_out_drv_to_out;
    access_time = t_rcd + cas_latency;
  }

  double temp;

  if (!dp.fully_assoc)
  {
    temp = delay_inside_mat + bank.mat.delay_wl_reset + bank.mat.delay_bl_restore;//TODO: : revisit
   if (dp.is_dram)
    {
      temp += bank.mat.delay_writeback;  // temp stores random cycle time
    }


  temp = MAX(temp, bank.mat.r_predec->delay);
  temp = MAX(temp, bank.mat.b_mux_predec->delay);
  temp = MAX(temp, bank.mat.sa_mux_lev_1_predec->delay);
  temp = MAX(temp, bank.mat.sa_mux_lev_2_predec->delay);
  }
  else
   {
	  ram_delay_inside_mat = bank.mat.delay_bitline + bank.mat.delay_matchchline;
	  temp = ram_delay_inside_mat + bank.mat.delay_cam_sl_restore + bank.mat.delay_cam_ml_reset + bank.mat.delay_bl_restore
	         + bank.mat.delay_hit_miss_reset + bank.mat.delay_wl_reset;

	  temp = MAX(temp, bank.mat.b_mux_predec->delay);//TODO:  revisit whether distinguish cam and ram bitline etc.
	  temp = MAX(temp, bank.mat.sa_mux_lev_1_predec->delay);
	  temp = MAX(temp, bank.mat.sa_mux_lev_2_predec->delay);
   }

  // The following is true only if the input parameter "repeaters_in_htree" is set to false --Nav
  if (g_ip->rpters_in_htree == false)
  {
    temp = MAX(temp, bank.htree_in_add->max_unpipelined_link_delay);
  }
  cycle_time = temp;

  double delay_req_network = max_delay_before_row_decoder;
  double delay_rep_network = delay_from_subarray_out_drv_to_out;
  multisubbank_interleave_cycle_time = MAX(delay_req_network, delay_rep_network);

  if (dp.is_main_mem)
  {
    multisubbank_interleave_cycle_time = htree_in_add->delay;
    precharge_delay = htree_in_add->delay +
                      bank.htree_in_add->delay + bank.mat.delay_writeback +
                      bank.mat.delay_wl_reset + bank.mat.delay_bl_restore;
    cycle_time = access_time + precharge_delay;
  }
  else
  {
    precharge_delay = 0;
  }
/**
  double dram_array_availability = 0;
  if (dp.is_dram)
  {
    dram_array_availability = (1 - dp.num_r_subarray * cycle_time / dp.dram_refresh_period) * 100;
  }
**/
  }//CACTI3DD, else
  return outrisetime;
}



// note: currently, power numbers are for a bank of an array
void UCA::compute_power_energy()
{
  bank.compute_power_energy();
  power = bank.power;
  //CACTI3DD
  if (g_ip->is_3d_mem)
  {
	  double datapath_energy = 0.505e-9 *g_ip->F_sz_nm / 55;
	  //double chip_IO_width = 4;
	  //g_ip->burst_len = 4;
	  activate_energy = membus_RAS->power.readOp.dynamic + (bank.mat.power_bitline.readOp.dynamic +
			  bank.mat.power_sa.readOp.dynamic) * dp.Ndwl; // /4
	  read_energy    = (membus_CAS->power.readOp.dynamic + bank.mat.power_subarray_out_drv.readOp.dynamic +
			  membus_data->power.readOp.dynamic ) + datapath_energy; //* g_ip->burst_len;
	  write_energy   = (membus_CAS->power.readOp.dynamic + bank.mat.power_subarray_out_drv.readOp.dynamic +
			  membus_data->power.readOp.dynamic + bank.mat.power_sa.readOp.dynamic * g_ip->burst_depth*g_ip->io_width/g_ip->page_sz_bits)  + datapath_energy; //* g_ip->burst_len;
	  precharge_energy = (bank.mat.power_bitline.readOp.dynamic +
			  bank.mat.power_bl_precharge_eq_drv.readOp.dynamic)* dp.Ndwl; // /4

	  activate_power = activate_energy / t_RC;
	  double col_cycle_act_row;
	  //col_cycle_act_row =  MAX(MAX(MAX(membus_CAS->center_stripe->delay + membus_CAS->bank_bus->delay, bank.mat.delay_subarray_out_drv),
	  			  //membus_data->delay), membus_data->out_seg->delay *g_ip->burst_depth);
	  //col_cycle_act_row = membus_data->out_seg->delay * g_ip->burst_depth;
	  col_cycle_act_row = (1e-6/(double)g_ip->sys_freq_MHz)/2 * g_ip->burst_depth;
	  //--- Activity factor assumption comes from Micron data spreadsheet.
	  read_power = 0.25 * read_energy / col_cycle_act_row;
	  write_power = 0.15 * write_energy / col_cycle_act_row;

	  if (g_ip->print_detail_debug)
	  {
		  cout<<"Row Address Delay components: "<<endl;
		  cout<<"Row Address Delay components: "<<endl;
		  cout<<"Network power terms: "<<endl;
		  cout<<"uca.cc: membus_RAS->power.readOp.dynamic = "<< membus_RAS->power.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: membus_CAS->power.readOp.dynamic = "<< membus_CAS->power.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: membus_data->power.readOp.dynamic = "<< membus_data->power.readOp.dynamic * 1e9  << " nJ" <<endl;

		  cout<<"Row Address Power components: "<<endl;
		  cout<<"uca.cc: membus_RAS->power_bus.readOp.dynamic = "<<membus_RAS->power_bus.readOp.dynamic * 1e9 << " nJ" <<endl;
		  cout<<"uca.cc: membus_RAS->power_add_predecoder.readOp.dynamic = "<< membus_RAS->power_add_predecoder.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: membus_RAS->power_add_decoders.readOp.dynamic = "<< membus_RAS->power_add_decoders.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: membus_RAS->power_lwl_drv.readOp.dynamic = "<< membus_RAS->power_lwl_drv.readOp.dynamic * 1e9  << " nJ" <<endl;

		  cout<<"Bank Power components: "<<endl;
		  cout<<"uca.cc: bank.mat.power_bitline = "<< bank.mat.power_bitline.readOp.dynamic * dp.Ndwl * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: bank.mat.power_sa = "<< bank.mat.power_sa.readOp.dynamic * dp.Ndwl * 1e9  << " nJ" <<endl;

		  cout<<"Column Address Power components: "<<endl;
		  cout<<"uca.cc: membus_CAS->power_bus.readOp.dynamic = "<< membus_CAS->power_bus.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: membus_CAS->power_add_predecoder.readOp.dynamic = "<< membus_CAS->power_add_predecoder.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: membus_CAS->power_add_decoders.readOp.dynamic = "<< membus_CAS->power_add_decoders.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: membus_CAS->power.readOp.dynamic = "<< membus_CAS->power.readOp.dynamic * 1e9  << " nJ" <<endl;

		  cout<<"Data Path Power components: "<<endl;
		  cout<<"uca.cc: bank.mat.power_subarray_out_drv.readOp.dynamic = "<< bank.mat.power_subarray_out_drv.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: membus_data->power.readOp.dynamic = "<< membus_data->power.readOp.dynamic * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: bank.mat.power_sa = "<< bank.mat.power_sa.readOp.dynamic * g_ip->burst_depth*g_ip->io_width/g_ip->page_sz_bits * 1e9  << " nJ" <<endl;

		  //****************** Final results for output ****************
		  cout<<"General Power components: "<<endl;
		  cout<<"uca.cc: activate_energy = "<<activate_energy * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: read_energy = "<<read_energy * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: write_energy = "<<write_energy * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: precharge_energy = "<<precharge_energy * 1e9  << " nJ" <<endl;
		  cout<<"uca.cc: activate_power = "<<activate_power * 1e3  << " mW" <<endl;
		  cout<<"uca.cc: read_power = "<< read_power * 1e3  << " mW" <<endl;
		  cout<<"uca.cc: write_power = "<< write_power * 1e3  << " mW" <<endl;

	  }
  }//CACTI3DD
  else
  {
  power_routing_to_bank.readOp.dynamic  = htree_in_add->power.readOp.dynamic + htree_out_data->power.readOp.dynamic;
  power_routing_to_bank.writeOp.dynamic = htree_in_add->power.readOp.dynamic + htree_in_data->power.readOp.dynamic;
  if (dp.fully_assoc || dp.pure_cam)
      power_routing_to_bank.searchOp.dynamic= htree_in_search->power.searchOp.dynamic + htree_out_search->power.searchOp.dynamic;

  power_routing_to_bank.readOp.leakage += htree_in_add->power.readOp.leakage +
                                          htree_in_data->power.readOp.leakage +
                                          htree_out_data->power.readOp.leakage;

  power_routing_to_bank.readOp.gate_leakage += htree_in_add->power.readOp.gate_leakage +
                                          htree_in_data->power.readOp.gate_leakage +
                                          htree_out_data->power.readOp.gate_leakage;
  if (dp.fully_assoc || dp.pure_cam)
  {
	power_routing_to_bank.readOp.leakage += htree_in_search->power.readOp.leakage + htree_out_search->power.readOp.leakage;
	power_routing_to_bank.readOp.gate_leakage += htree_in_search->power.readOp.gate_leakage + htree_out_search->power.readOp.gate_leakage;
  }

  power.searchOp.dynamic += power_routing_to_bank.searchOp.dynamic;
  power.readOp.dynamic += power_routing_to_bank.readOp.dynamic;
  power.readOp.leakage += power_routing_to_bank.readOp.leakage;
  power.readOp.gate_leakage += power_routing_to_bank.readOp.gate_leakage;

  // calculate total write energy per access
  power.writeOp.dynamic = power.readOp.dynamic
                        - bank.mat.power_bitline.readOp.dynamic * dp.num_act_mats_hor_dir
                        + bank.mat.power_bitline.writeOp.dynamic * dp.num_act_mats_hor_dir
                        - power_routing_to_bank.readOp.dynamic
                        + power_routing_to_bank.writeOp.dynamic
                        + bank.htree_in_data->power.readOp.dynamic
                        - bank.htree_out_data->power.readOp.dynamic;

  if (dp.is_dram == false)
  {
    power.writeOp.dynamic -= bank.mat.power_sa.readOp.dynamic * dp.num_act_mats_hor_dir;
  }

  dyn_read_energy_from_closed_page = power.readOp.dynamic;
  dyn_read_energy_from_open_page   = power.readOp.dynamic -
                                     (bank.mat.r_predec->power.readOp.dynamic +
                                      bank.mat.power_row_decoders.readOp.dynamic +
                                      bank.mat.power_bl_precharge_eq_drv.readOp.dynamic +
                                      bank.mat.power_sa.readOp.dynamic +
                                      bank.mat.power_bitline.readOp.dynamic) * dp.num_act_mats_hor_dir;

  dyn_read_energy_remaining_words_in_burst =
    (MAX((g_ip->burst_len / g_ip->int_prefetch_w), 1) - 1) *
    ((bank.mat.sa_mux_lev_1_predec->power.readOp.dynamic +
      bank.mat.sa_mux_lev_2_predec->power.readOp.dynamic +
      bank.mat.power_sa_mux_lev_1_decoders.readOp.dynamic +
      bank.mat.power_sa_mux_lev_2_decoders.readOp.dynamic +
      bank.mat.power_subarray_out_drv.readOp.dynamic)     * dp.num_act_mats_hor_dir +
     bank.htree_out_data->power.readOp.dynamic +
     power_routing_to_bank.readOp.dynamic);
  dyn_read_energy_from_closed_page += dyn_read_energy_remaining_words_in_burst;
  dyn_read_energy_from_open_page   += dyn_read_energy_remaining_words_in_burst;

  activate_energy = htree_in_add->power.readOp.dynamic +
                    bank.htree_in_add->power_bit.readOp.dynamic * bank.num_addr_b_routed_to_mat_for_act +
                    (bank.mat.r_predec->power.readOp.dynamic +
                     bank.mat.power_row_decoders.readOp.dynamic +
                     bank.mat.power_sa.readOp.dynamic) * dp.num_act_mats_hor_dir;
  read_energy    = (htree_in_add->power.readOp.dynamic +
                    bank.htree_in_add->power_bit.readOp.dynamic * bank.num_addr_b_routed_to_mat_for_rd_or_wr +
                    (bank.mat.sa_mux_lev_1_predec->power.readOp.dynamic  +
                     bank.mat.sa_mux_lev_2_predec->power.readOp.dynamic  +
                     bank.mat.power_sa_mux_lev_1_decoders.readOp.dynamic +
                     bank.mat.power_sa_mux_lev_2_decoders.readOp.dynamic +
                     bank.mat.power_subarray_out_drv.readOp.dynamic) * dp.num_act_mats_hor_dir +
                    bank.htree_out_data->power.readOp.dynamic +
                    htree_in_data->power.readOp.dynamic) * g_ip->burst_len;
  write_energy   = (htree_in_add->power.readOp.dynamic +
                    bank.htree_in_add->power_bit.readOp.dynamic * bank.num_addr_b_routed_to_mat_for_rd_or_wr +
                    htree_in_data->power.readOp.dynamic +
                    bank.htree_in_data->power.readOp.dynamic +
                    (bank.mat.sa_mux_lev_1_predec->power.readOp.dynamic  +
                     bank.mat.sa_mux_lev_2_predec->power.readOp.dynamic  +
                     bank.mat.power_sa_mux_lev_1_decoders.readOp.dynamic +
                     bank.mat.power_sa_mux_lev_2_decoders.readOp.dynamic) * dp.num_act_mats_hor_dir) * g_ip->burst_len;
  precharge_energy = (bank.mat.power_bitline.readOp.dynamic +
                      bank.mat.power_bl_precharge_eq_drv.readOp.dynamic) * dp.num_act_mats_hor_dir;
  } //CACTI3DD
  leak_power_subbank_closed_page =
    (bank.mat.r_predec->power.readOp.leakage +
     bank.mat.b_mux_predec->power.readOp.leakage +
     bank.mat.sa_mux_lev_1_predec->power.readOp.leakage +
     bank.mat.sa_mux_lev_2_predec->power.readOp.leakage +
     bank.mat.power_row_decoders.readOp.leakage +
     bank.mat.power_bit_mux_decoders.readOp.leakage +
     bank.mat.power_sa_mux_lev_1_decoders.readOp.leakage +
     bank.mat.power_sa_mux_lev_2_decoders.readOp.leakage +
     bank.mat.leak_power_sense_amps_closed_page_state) * dp.num_act_mats_hor_dir;

  leak_power_subbank_closed_page +=
    (bank.mat.r_predec->power.readOp.gate_leakage +
     bank.mat.b_mux_predec->power.readOp.gate_leakage +
     bank.mat.sa_mux_lev_1_predec->power.readOp.gate_leakage +
     bank.mat.sa_mux_lev_2_predec->power.readOp.gate_leakage +
     bank.mat.power_row_decoders.readOp.gate_leakage +
     bank.mat.power_bit_mux_decoders.readOp.gate_leakage +
     bank.mat.power_sa_mux_lev_1_decoders.readOp.gate_leakage +
     bank.mat.power_sa_mux_lev_2_decoders.readOp.gate_leakage) * dp.num_act_mats_hor_dir; //+
     //bank.mat.leak_power_sense_amps_closed_page_state) * dp.num_act_mats_hor_dir;

  leak_power_subbank_open_page =
    (bank.mat.r_predec->power.readOp.leakage +
     bank.mat.b_mux_predec->power.readOp.leakage +
     bank.mat.sa_mux_lev_1_predec->power.readOp.leakage +
     bank.mat.sa_mux_lev_2_predec->power.readOp.leakage +
     bank.mat.power_row_decoders.readOp.leakage +
     bank.mat.power_bit_mux_decoders.readOp.leakage +
     bank.mat.power_sa_mux_lev_1_decoders.readOp.leakage +
     bank.mat.power_sa_mux_lev_2_decoders.readOp.leakage +
     bank.mat.leak_power_sense_amps_open_page_state) * dp.num_act_mats_hor_dir;

  leak_power_subbank_open_page +=
    (bank.mat.r_predec->power.readOp.gate_leakage +
     bank.mat.b_mux_predec->power.readOp.gate_leakage +
     bank.mat.sa_mux_lev_1_predec->power.readOp.gate_leakage +
     bank.mat.sa_mux_lev_2_predec->power.readOp.gate_leakage +
     bank.mat.power_row_decoders.readOp.gate_leakage +
     bank.mat.power_bit_mux_decoders.readOp.gate_leakage +
     bank.mat.power_sa_mux_lev_1_decoders.readOp.gate_leakage +
     bank.mat.power_sa_mux_lev_2_decoders.readOp.gate_leakage ) * dp.num_act_mats_hor_dir;
     //bank.mat.leak_power_sense_amps_open_page_state) * dp.num_act_mats_hor_dir;

  leak_power_request_and_reply_networks =
    power_routing_to_bank.readOp.leakage +
    bank.htree_in_add->power.readOp.leakage +
    bank.htree_in_data->power.readOp.leakage +
    bank.htree_out_data->power.readOp.leakage;

  leak_power_request_and_reply_networks +=
    power_routing_to_bank.readOp.gate_leakage +
    bank.htree_in_add->power.readOp.gate_leakage +
    bank.htree_in_data->power.readOp.gate_leakage +
    bank.htree_out_data->power.readOp.gate_leakage;

  if (dp.fully_assoc || dp.pure_cam)
  {
	leak_power_request_and_reply_networks += htree_in_search->power.readOp.leakage + htree_out_search->power.readOp.leakage;
	leak_power_request_and_reply_networks += htree_in_search->power.readOp.gate_leakage + htree_out_search->power.readOp.gate_leakage;
  }


  if (dp.is_dram)
  { // if DRAM, add contribution of power spent in row predecoder drivers, blocks and decoders to refresh power
    refresh_power  = (bank.mat.r_predec->power.readOp.dynamic * dp.num_act_mats_hor_dir +
                      bank.mat.row_dec->power.readOp.dynamic) * dp.num_r_subarray * dp.num_subarrays;
    refresh_power += bank.mat.per_bitline_read_energy * dp.num_c_subarray * dp.num_r_subarray * dp.num_subarrays;
    refresh_power += bank.mat.power_bl_precharge_eq_drv.readOp.dynamic * dp.num_act_mats_hor_dir;
    refresh_power += bank.mat.power_sa.readOp.dynamic * dp.num_act_mats_hor_dir;
    refresh_power /= dp.dram_refresh_period;
  }


  if (dp.is_tag == false)
  {
    power.readOp.dynamic  = dyn_read_energy_from_closed_page;
    power.writeOp.dynamic = dyn_read_energy_from_closed_page
      - dyn_read_energy_remaining_words_in_burst
      - bank.mat.power_bitline.readOp.dynamic * dp.num_act_mats_hor_dir
      + bank.mat.power_bitline.writeOp.dynamic * dp.num_act_mats_hor_dir
      + (power_routing_to_bank.writeOp.dynamic -
         power_routing_to_bank.readOp.dynamic -
         bank.htree_out_data->power.readOp.dynamic +
         bank.htree_in_data->power.readOp.dynamic) *
        (MAX((g_ip->burst_len / g_ip->int_prefetch_w), 1) - 1); //FIXME

    if (dp.is_dram == false)
    {
      power.writeOp.dynamic -= bank.mat.power_sa.readOp.dynamic * dp.num_act_mats_hor_dir;
    }
  }

  // if DRAM, add refresh power to total leakage
  if (dp.is_dram)
  {
    power.readOp.leakage += refresh_power;
  }

  // TODO: below should be  avoided.
  /*if (dp.is_main_mem)
  {
    power.readOp.leakage += MAIN_MEM_PER_CHIP_STANDBY_CURRENT_mA * 1e-3 * g_tp.peri_global.Vdd / g_ip->nbanks;
  }*/

  if (g_ip->is_3d_mem)
  {// ---This is only to make sure the following assert() functions don't generate errors. The values are not used in 3D DRAM models
	  // power = power + membus_RAS->power + membus_CAS->power + membus_data->power; //for leakage power add up, not used yet for optimization
	  power.readOp.dynamic = read_energy;
	  power.writeOp.dynamic = write_energy;
	  // ---Before the brackets, power = power.bank, and all the specific leakage terms have and only have accounted for bank to mat levels.
	 // power.readOp.leakage = power.readOp.leakage + membus_RAS->power.readOp.leakage + membus_CAS->power.readOp.leakage + membus_data->power.readOp.leakage;
	  power.readOp.leakage =membus_RAS->power.readOp.leakage + membus_CAS->power.readOp.leakage + membus_data->power.readOp.leakage;
	  //cout << "test: " << power.readOp.dynamic << endl;
	  //cout << "test: " << membus_RAS->power.readOp.leakage << endl;
	  //cout << "test: " << membus_CAS->power.readOp.leakage << endl;
	  //cout << "test: " << membus_data->power.readOp.leakage << endl;
	  //cout << "test: power.readOp.leakage" << power.readOp.leakage << endl;
  }

  assert(power.readOp.dynamic  > 0);
  assert(power.writeOp.dynamic > 0);
  assert(power.readOp.leakage  > 0);
}

