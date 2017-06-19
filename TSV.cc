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

#include "TSV.h"

TSV::TSV(enum TSV_type tsv_type,
    /*TechnologyParameter::*/DeviceType *dt)://TSV driver's device type set to be peri_global
    deviceType(dt), tsv_type(tsv_type)
{
    num_gates		= 1;
    num_gates_min	= 1;//Is there a minimum number of stages?
    min_w_pmos     	= deviceType -> n_to_p_eff_curr_drv_ratio * g_tp.min_w_nmos_;

    switch (tsv_type)
    {
    case Fine:
    	cap = g_tp.tsv_parasitic_capacitance_fine;
    	res = g_tp.tsv_parasitic_resistance_fine;
    	min_area = g_tp.tsv_minimum_area_fine;
    	break;
    case Coarse:
    	cap = g_tp.tsv_parasitic_capacitance_coarse;
    	res = g_tp.tsv_parasitic_resistance_coarse;
    	min_area = g_tp.tsv_minimum_area_coarse;
    	break;
    default:
    	break;
    }

    for (int i = 0; i < MAX_NUMBER_GATES_STAGE; i++)
        {
          w_TSV_n[i] = 0;
          w_TSV_p[i] = 0;
        }

    double first_buf_stg_coef = 5; // To tune the total buffer delay.
    w_TSV_n[0] = g_tp.min_w_nmos_*first_buf_stg_coef;
    w_TSV_p[0] = min_w_pmos *first_buf_stg_coef;

    is_dram = 0;
    is_wl_tr = 0;

    //What does the function assert() mean?  Should I put the function here?
    compute_buffer_stage();
    compute_area();
    compute_delay();
}

TSV::~TSV()
{
}

void TSV::compute_buffer_stage()
{
	double p_to_n_sz_ratio = deviceType->n_to_p_eff_curr_drv_ratio;

	//BEOL parasitics in Katti's E modeling and charac. of TSV.  Needs further detailed values.
	//double res_beol	= 0.1;//inaccurate
	//double cap_beol	= 1e-15;

	//C_load_TSV	= cap_beol + cap + cap_beol + gate_C(g_tp.min_w_nmos_ + min_w_pmos, 0);
	C_load_TSV	= cap + gate_C(g_tp.min_w_nmos_ + min_w_pmos, 0); //+ 57.5e-15;
	if(g_ip->print_detail_debug)
	{
		cout << " The input cap of 1st buffer: " << gate_C(w_TSV_n[0] + w_TSV_p[0], 0) * 1e15 << " fF";
	}
	double F = C_load_TSV / gate_C(w_TSV_n[0] + w_TSV_p[0], 0);
	if(g_ip->print_detail_debug)
	{
		cout<<"\nF  is "<<F<<" \n";
	}
	//Obtain buffer chain stages using logic effort function. Does stage number have to be even?
    num_gates = logical_effort(
        num_gates_min,
        1,
        F,
        w_TSV_n,
        w_TSV_p,
        C_load_TSV,
        p_to_n_sz_ratio,
        is_dram,
        is_wl_tr,
        g_tp.max_w_nmos_/*Correct? Decoder uses max_w_nmos_dec*/);
}

void TSV::compute_area()
{
	//Obtain the driver chain area and leakage power for TSV
	double Vdd = deviceType->Vdd;
	double cumulative_area = 0;
	double cumulative_curr = 0;  // cumulative leakage current
	double cumulative_curr_Ig = 0;  // cumulative leakage current
	Buffer_area.h = g_tp.cell_h_def;//cell_h_def is the assigned height for memory cell (5um), is it correct to use it here?

	//logic_effort() didn't give the size of w_n[0] and w_p[0], which is min size inverter
	//w_TSV_n[0] = g_tp.min_w_nmos_;
	//w_TSV_p[0] = min_w_pmos;

	int i;
	for (i = 0; i < num_gates; i++)
	{
	  cumulative_area += compute_gate_area(INV, 1, w_TSV_p[i], w_TSV_n[i], Buffer_area.h);
	  if(g_ip->print_detail_debug)
	  {
		  cout << "\n\tArea up to the " << i+1 << " stages is: " << cumulative_area << " um2";
	  }
	  cumulative_curr += cmos_Isub_leakage(w_TSV_n[i], w_TSV_p[i], 1, inv, is_dram);
	  cumulative_curr_Ig += cmos_Ig_leakage(w_TSV_n[i], w_TSV_p[i], 1, inv, is_dram);// The operator += is mistakenly put as = in decoder.cc
	}
	power.readOp.leakage = cumulative_curr * Vdd;
	power.readOp.gate_leakage = cumulative_curr_Ig * Vdd;

	Buffer_area.set_area(cumulative_area);
	Buffer_area.w = (cumulative_area / Buffer_area.h);

	TSV_metal_area.set_area(min_area * 3.1416/16);

	if( Buffer_area.get_area() < min_area - TSV_metal_area.get_area() )
		area.set_area(min_area);
	else
		area.set_area(Buffer_area.get_area() + TSV_metal_area.get_area());

}

void TSV::compute_delay()
{
	//Buffer chain delay and Dynamic Power
	double rd, tf, this_delay, c_load, c_intrinsic, inrisetime = 0/*The initial time*/;
	//is_dram, is_wl_tr are declared to be false in the constructor
	rd = tr_R_on(w_TSV_n[0], NCH, 1, is_dram, false, is_wl_tr);
	c_load = gate_C(w_TSV_n[1] + w_TSV_p[1], 0.0, is_dram, false, is_wl_tr);
	c_intrinsic = drain_C_(w_TSV_p[0], PCH, 1, 1, area.h, is_dram, false, is_wl_tr) +
				  drain_C_(w_TSV_n[0], NCH, 1, 1, area.h, is_dram, false, is_wl_tr);
	tf = rd * (c_intrinsic + c_load);
	//Refer to horowitz function definition
	this_delay = horowitz(inrisetime, tf, 0.5, 0.5, RISE);
	delay += this_delay;
	inrisetime = this_delay / (1.0 - 0.5);

	double Vdd = deviceType -> Vdd;
	power.readOp.dynamic += (c_load + c_intrinsic) * Vdd * Vdd;

	int i;
	for (i = 1; i < num_gates - 1; ++i)
	{
	  rd = tr_R_on(w_TSV_n[i], NCH, 1, is_dram, false, is_wl_tr);
	  c_load = gate_C(w_TSV_p[i+1] + w_TSV_n[i+1], 0.0, is_dram, false, is_wl_tr);
	  c_intrinsic = drain_C_(w_TSV_p[i], PCH, 1, 1, area.h, is_dram, false, is_wl_tr) +
					drain_C_(w_TSV_n[i], NCH, 1, 1, area.h, is_dram, false, is_wl_tr);
	  tf = rd * (c_intrinsic + c_load);
	  this_delay = horowitz(inrisetime, tf, 0.5, 0.5, RISE);
	  delay += this_delay;
	  inrisetime = this_delay / (1.0 - 0.5);
	  power.readOp.dynamic += (c_load + c_intrinsic) * Vdd * Vdd;
	}

    // add delay of final inverter that drives the TSV
    i = num_gates - 1;
    c_load = C_load_TSV;
    rd = tr_R_on(w_TSV_n[i], NCH, 1, is_dram, false, is_wl_tr);
    c_intrinsic = drain_C_(w_TSV_p[i], PCH, 1, 1, area.h, is_dram, false, is_wl_tr) +
                  drain_C_(w_TSV_n[i], NCH, 1, 1, area.h, is_dram, false, is_wl_tr);
    //The delay method for the last stage of buffer chain in Decoder.cc

    //double res_beol	= 0.1;//inaccurate
    //double R_TSV_out = res_beol + res + res_beol;
    double R_TSV_out = res;
    tf = rd * (c_intrinsic + c_load) + R_TSV_out * c_load / 2;
    this_delay = horowitz(inrisetime, tf, 0.5, 0.5, RISE);
    delay  += this_delay;

    power.readOp.dynamic += (c_load + c_intrinsic) * Vdd * Vdd; //Dynamic power done

    //Is the delay actually delay/(1.0-0.5)??
    //ret_val = this_delay / (1.0 - 0.5);
    //return ret_val;//Originally for decoder.cc to get outrise time


    /* This part is to obtain delay in the TSV path, refer to Katti's paper.
     * It can be used alternatively as the step to get the final-stage delay
    double C_ext = c_intrinsic;
    R_dr = rd;
    double C_int = gate_C(g_tp.min_w_nmos_ + min_w_pmos, 0.0, is_dram, false, is_wl_tr);
	delay_TSV_path	= 0.693 * (R_dr * C_ext + (R_dr + res_beol) * cap_beol + (R_dr + res_beol + 0.5 * res) * cap
			+ (R_dr + res_beol + res + res_beol) * (cap_beol + C_int);
	delay += delay_TSV_path;
	*/
}

void TSV::print_TSV()
{

  cout << "\nTSV Properties:\n\n";
  cout << "  Delay Optimal - "<<
  	" \n\tTSV Cap: " << cap * 1e15 << " fF" <<
  	" \n\tTSV Res: " << res * 1e3 << " mOhm"<<
    " \n\tNumber of Buffer Chain stages - " << num_gates <<
    " \n\tDelay - " << delay * 1e9 <<  " (ns) "
    " \n\tPowerD - " << power.readOp.dynamic * 1e9<< " (nJ)"
    " \n\tPowerL - " << power.readOp.leakage * 1e3<< " (mW)"
    " \n\tPowerLgate - " << power.readOp.gate_leakage * 1e3<< " (mW)\n" <<
    " \n\tBuffer  Area: " << Buffer_area.get_area() << " um2" <<
    " \n\tBuffer Height: " << Buffer_area.h << " um" <<
    " \n\tBuffer Width: " << Buffer_area.w << " um" <<
    " \n\tTSV metal area: " << TSV_metal_area.get_area() << " um2" <<
    " \n\tTSV minimum occupied area: " <<min_area << " um2"<<
    " \n\tTotal area: " << area.get_area() << " um2";
  cout <<endl;
  cout <<endl;

}



