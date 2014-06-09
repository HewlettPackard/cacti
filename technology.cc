/*------------------------------------------------------------
 *                              CACTI 6.5
 *         Copyright 2008 Hewlett-Packard Development Corporation
 *                         All Rights Reserved
 *
 * Permission to use, copy, and modify this software and its documentation is
 * hereby granted only under the following terms and conditions.  Both the
 * above copyright notice and this permission notice must appear in all copies
 * of the software, derivative works or modified versions, and any portions
 * thereof, and both notices must appear in supporting documentation.
 *
 * Users of this software agree to the terms and conditions set forth herein, and
 * hereby grant back to Hewlett-Packard Company and its affiliated companies ("HP")
 * a non-exclusive, unrestricted, royalty-free right and license under any changes, 
 * enhancements or extensions  made to the core functions of the software, including 
 * but not limited to those affording compatibility with other hardware or software
 * environments, but excluding applications which incorporate this software.
 * Users further agree to use their best efforts to return to HP any such changes,
 * enhancements or extensions that they make and inform HP of noteworthy uses of
 * this software.  Correspondence should be provided to HP at:
 *
 *                       Director of Intellectual Property Licensing
 *                       Office of Strategy and Technology
 *                       Hewlett-Packard Company
 *                       1501 Page Mill Road
 *                       Palo Alto, California  94304
 *
 * This software may be distributed (but not offered for sale or transferred
 * for compensation) to third parties, provided such third parties agree to
 * abide by the terms and conditions of this notice.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND HP DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL HP 
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *------------------------------------------------------------*/

#include "basic_circuit.h"

#include "parameter.h"

double wire_resistance(double resistivity, double wire_width, double wire_thickness,
    double barrier_thickness, double dishing_thickness, double alpha_scatter)
{
  double resistance;
  resistance = alpha_scatter * resistivity /((wire_thickness - barrier_thickness - dishing_thickness)*(wire_width - 2 * barrier_thickness));
  return(resistance);
}

double wire_capacitance(double wire_width, double wire_thickness, double wire_spacing, 
    double ild_thickness, double miller_value, double horiz_dielectric_constant,
    double vert_dielectric_constant, double fringe_cap)
{
  double vertical_cap, sidewall_cap, total_cap;
  vertical_cap = 2 * PERMITTIVITY_FREE_SPACE * vert_dielectric_constant * wire_width / ild_thickness;
  sidewall_cap = 2 * PERMITTIVITY_FREE_SPACE * miller_value * horiz_dielectric_constant * wire_thickness / wire_spacing;
  total_cap = vertical_cap + sidewall_cap + fringe_cap;
  return(total_cap);
}


void init_tech_params(double technology, bool is_tag)
{
  int    iter, tech, tech_lo, tech_hi;
  double curr_alpha, curr_vpp;
  double aspect_ratio, wire_width, wire_thickness, wire_spacing, barrier_thickness, dishing_thickness,
         alpha_scatter, ild_thickness, miller_value = 1.5, horiz_dielectric_constant, vert_dielectric_constant,
         fringe_cap, pmos_to_nmos_sizing_r;
  double curr_vdd_dram_cell, curr_v_th_dram_access_transistor, curr_I_on_dram_cell, curr_c_dram_cell;

  uint32_t ram_cell_tech_type    = (is_tag) ? g_ip->tag_arr_ram_cell_tech_type : g_ip->data_arr_ram_cell_tech_type;
  uint32_t peri_global_tech_type = (is_tag) ? g_ip->tag_arr_peri_global_tech_type : g_ip->data_arr_peri_global_tech_type;

  technology  = technology * 1000.0;  // in the unit of nm

  // initialize parameters
  g_tp.reset();
  double gmp_to_gmn_multiplier_periph_global = 0; 

  double curr_Wmemcella_dram, curr_Wmemcellpmos_dram, curr_Wmemcellnmos_dram,
         curr_area_cell_dram, curr_asp_ratio_cell_dram, curr_Wmemcella_sram,
         curr_Wmemcellpmos_sram, curr_Wmemcellnmos_sram, curr_area_cell_sram,
         curr_asp_ratio_cell_sram, curr_I_off_dram_cell_worst_case_length_temp;
  double SENSE_AMP_D, SENSE_AMP_P; // J
  double area_cell_dram = 0;
  double asp_ratio_cell_dram = 0;
  double area_cell_sram = 0;
  double asp_ratio_cell_sram = 0;
  double mobility_eff_periph_global = 0;
  double Vdsat_periph_global = 0;
  double nmos_effective_resistance_multiplier;
  double width_dram_access_transistor;



  if (technology < 91 && technology > 89)
  {
    tech_lo = 90;
    tech_hi = 90;
  }
  else if (technology < 66 && technology > 64)
  {
    tech_lo = 65;
    tech_hi = 65;
  }
  else if (technology < 46 && technology > 44)
  {
    tech_lo = 45;
    tech_hi = 45;
  }
  else if (technology < 33 && technology > 31)
  {
    tech_lo = 32;
    tech_hi = 32;
  }
  else if (technology < 90 && technology > 65)
  {
    tech_lo = 90;
    tech_hi = 65;
  }
  else if (technology < 65 && technology > 45)
  {
    tech_lo = 65;
    tech_hi = 45;
  }
  else if (technology < 45 && technology > 32)
  {
    tech_lo = 45;
    tech_hi = 32;
  }

  double vdd[NUMBER_TECH_FLAVORS];
  double Lphy[NUMBER_TECH_FLAVORS];
  double Lelec[NUMBER_TECH_FLAVORS];
  double t_ox[NUMBER_TECH_FLAVORS];
  double v_th[NUMBER_TECH_FLAVORS];
  double c_ox[NUMBER_TECH_FLAVORS];
  double mobility_eff[NUMBER_TECH_FLAVORS];
  double Vdsat[NUMBER_TECH_FLAVORS];
  double c_g_ideal[NUMBER_TECH_FLAVORS];
  double c_fringe[NUMBER_TECH_FLAVORS];
  double c_junc[NUMBER_TECH_FLAVORS];
  double I_on_n[NUMBER_TECH_FLAVORS];
  double I_on_p[NUMBER_TECH_FLAVORS];
  double Rnchannelon[NUMBER_TECH_FLAVORS];
  double Rpchannelon[NUMBER_TECH_FLAVORS];
  double n_to_p_eff_curr_drv_ratio[NUMBER_TECH_FLAVORS];
  double I_off_n[NUMBER_TECH_FLAVORS][101];
  //double I_off_p[NUMBER_TECH_FLAVORS][101];
  double gmp_to_gmn_multiplier[NUMBER_TECH_FLAVORS];

  for (iter = 0; iter <= 1; ++iter)
  {
    // linear interpolation
    if (iter == 0)
    {
      tech = tech_lo;
      if (tech_lo == tech_hi)
      {
        curr_alpha = 1;
      }
      else
      {
        curr_alpha = (technology - tech_hi)/(tech_lo - tech_hi);
      }
    }
    else
    {
      tech = tech_hi;
      if (tech_lo == tech_hi)
      {
        break;
      }
      else
      {
        curr_alpha = (tech_lo - technology)/(tech_lo - tech_hi);
      }
    }

    if (tech == 90)
    {
      SENSE_AMP_D = .28e-9; // s
      SENSE_AMP_P = 14.7e-15; // J
      //90nm technology-node. Corresponds to year 2004 in ITRS
      //ITRS HP device type
      vdd[0]   = 1.2;
      Lphy[0]  = 0.037;//Lphy is the physical gate-length. micron
      Lelec[0] = 0.0266;//Lelec is the electrical gate-length. micron
      t_ox[0]  = 1.2e-3;//micron
      v_th[0]  = 0.23707;//V
      c_ox[0]  = 1.79e-14;//F/micron2
      mobility_eff[0] = 342.16 * (1e-2 * 1e6 * 1e-2 * 1e6); //micron2 / Vs
      Vdsat[0] = 0.128; //V
      c_g_ideal[0] = 6.64e-16;//F/micron
      c_fringe[0]  = 0.08e-15;//F/micron
      c_junc[0] = 1e-15;//F/micron2
      I_on_n[0] = 1076.9e-6;//A/micron
      I_on_p[0] = 712.6e-6;//A/micron
      //Note that nmos_effective_resistance_multiplier, n_to_p_eff_curr_drv_ratio and gmp_to_gmn_multiplier values are calculated offline
      nmos_effective_resistance_multiplier = 1.54;
      n_to_p_eff_curr_drv_ratio[0] = 2.45;
      gmp_to_gmn_multiplier[0] = 1.22;
      Rnchannelon[0] = nmos_effective_resistance_multiplier * vdd[0] / I_on_n[0];//ohm-micron
      Rpchannelon[0] = n_to_p_eff_curr_drv_ratio[0] * Rnchannelon[0];//ohm-micron
      I_off_n[0][0]  = 3.24e-8;//A/micron
      I_off_n[0][10] = 4.01e-8;
      I_off_n[0][20] = 4.90e-8;
      I_off_n[0][30] = 5.92e-8;
      I_off_n[0][40] = 7.08e-8;
      I_off_n[0][50] = 8.38e-8;
      I_off_n[0][60] = 9.82e-8;
      I_off_n[0][70] = 1.14e-7;
      I_off_n[0][80] = 1.29e-7;
      I_off_n[0][90] = 1.43e-7;
      I_off_n[0][100] = 1.54e-7;

      //ITRS LSTP device type
      vdd[1]   = 1.3;
      Lphy[1]  = 0.075;
      Lelec[1] = 0.0486;
      t_ox[1]  = 2.2e-3;
      v_th[1]  = 0.48203;
      c_ox[1]  = 1.22e-14;
      mobility_eff[1] = 356.76 * (1e-2 * 1e6 * 1e-2 * 1e6);
      Vdsat[1] = 0.373; 
      c_g_ideal[1] = 9.15e-16;
      c_fringe[1]  = 0.08e-15;
      c_junc[1] = 1e-15;
      I_on_n[1] = 503.6e-6;
      I_on_p[1] = 235.1e-6;
      nmos_effective_resistance_multiplier = 1.92;
      n_to_p_eff_curr_drv_ratio[1] = 2.44;
      gmp_to_gmn_multiplier[1] =0.88;
      Rnchannelon[1] = nmos_effective_resistance_multiplier * vdd[1] / I_on_n[1];
      Rpchannelon[1] = n_to_p_eff_curr_drv_ratio[1] * Rnchannelon[1];
      I_off_n[1][0]  = 2.81e-12;
      I_off_n[1][10] = 4.76e-12;
      I_off_n[1][20] = 7.82e-12;
      I_off_n[1][30] = 1.25e-11;
      I_off_n[1][40] = 1.94e-11;
      I_off_n[1][50] = 2.94e-11;
      I_off_n[1][60] = 4.36e-11;
      I_off_n[1][70] = 6.32e-11;
      I_off_n[1][80] = 8.95e-11;
      I_off_n[1][90] = 1.25e-10;
      I_off_n[1][100] = 1.7e-10;

      //ITRS LOP device type
      vdd[2] = 0.9;
      Lphy[2] = 0.053;
      Lelec[2] = 0.0354;
      t_ox[2] = 1.5e-3;
      v_th[2] = 0.30764;
      c_ox[2] = 1.59e-14;
      mobility_eff[2] = 460.39 * (1e-2 * 1e6 * 1e-2 * 1e6);
      Vdsat[2] = 0.113; 
      c_g_ideal[2] = 8.45e-16;
      c_fringe[2] = 0.08e-15;
      c_junc[2] = 1e-15;
      I_on_n[2] = 386.6e-6;
      I_on_p[2] = 209.7e-6;
      nmos_effective_resistance_multiplier = 1.77;
      n_to_p_eff_curr_drv_ratio[2] = 2.54;
      gmp_to_gmn_multiplier[2] = 0.98;
      Rnchannelon[2] = nmos_effective_resistance_multiplier * vdd[2] / I_on_n[2];
      Rpchannelon[2] = n_to_p_eff_curr_drv_ratio[2] * Rnchannelon[2];
      I_off_n[2][0] = 2.14e-9;
      I_off_n[2][10] = 2.9e-9;
      I_off_n[2][20] = 3.87e-9;
      I_off_n[2][30] = 5.07e-9;
      I_off_n[2][40] = 6.54e-9;
      I_off_n[2][50] = 8.27e-8;
      I_off_n[2][60] = 1.02e-7;
      I_off_n[2][70] = 1.20e-7;
      I_off_n[2][80] = 1.36e-8;
      I_off_n[2][90] = 1.52e-8;
      I_off_n[2][100] = 1.73e-8;

      if (ram_cell_tech_type == lp_dram)
      {
        //LP-DRAM cell access transistor technology parameters
        curr_vdd_dram_cell = 1.2;
        Lphy[3] = 0.12;
        Lelec[3] = 0.0756;
        curr_v_th_dram_access_transistor = 0.4545;
        width_dram_access_transistor = 0.14;
        curr_I_on_dram_cell = 45e-6;
        curr_I_off_dram_cell_worst_case_length_temp = 21.1e-12;
        curr_Wmemcella_dram = width_dram_access_transistor;
        curr_Wmemcellpmos_dram = 0;
        curr_Wmemcellnmos_dram = 0;
        curr_area_cell_dram = 0.168;
        curr_asp_ratio_cell_dram = 1.46; 
        curr_c_dram_cell = 20e-15;

        //LP-DRAM wordline transistor parameters
        curr_vpp = 1.6;
        t_ox[3] = 2.2e-3;
        v_th[3] = 0.4545;
        c_ox[3] = 1.22e-14;
        mobility_eff[3] =  323.95 * (1e-2 * 1e6 * 1e-2 * 1e6);
        Vdsat[3] = 0.3; 
        c_g_ideal[3] = 1.47e-15;
        c_fringe[3] = 0.08e-15;
        c_junc[3] = 1e-15;
        I_on_n[3] = 321.6e-6;
        I_on_p[3] = 203.3e-6;
        nmos_effective_resistance_multiplier = 1.65;
        n_to_p_eff_curr_drv_ratio[3] = 1.95;
        gmp_to_gmn_multiplier[3] = 0.90;
        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp / I_on_n[3];
        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];
        I_off_n[3][0] = 1.42e-11; 
        I_off_n[3][10] = 2.25e-11;
        I_off_n[3][20] = 3.46e-11;
        I_off_n[3][30] = 5.18e-11;
        I_off_n[3][40] = 7.58e-11;
        I_off_n[3][50] = 1.08e-10;
        I_off_n[3][60] = 1.51e-10;
        I_off_n[3][70] = 2.02e-10;
        I_off_n[3][80] = 2.57e-10;
        I_off_n[3][90] = 3.14e-10;
        I_off_n[3][100] = 3.85e-10;
      }
      else if (ram_cell_tech_type == comm_dram)
      {
        //COMM-DRAM cell access transistor technology parameters
        curr_vdd_dram_cell = 1.6;
        Lphy[3] = 0.09;
        Lelec[3] = 0.0576;
        curr_v_th_dram_access_transistor = 1;
        width_dram_access_transistor = 0.09;
        curr_I_on_dram_cell = 20e-6;
        curr_I_off_dram_cell_worst_case_length_temp = 1e-15;
        curr_Wmemcella_dram = width_dram_access_transistor;
        curr_Wmemcellpmos_dram = 0;
        curr_Wmemcellnmos_dram = 0;
        curr_area_cell_dram = 6*0.09*0.09;
        curr_asp_ratio_cell_dram = 1.5; 
        curr_c_dram_cell = 30e-15;

        //COMM-DRAM wordline transistor parameters
        curr_vpp = 3.7;
        t_ox[3] = 5.5e-3;
        v_th[3] = 1.0;
        c_ox[3] = 5.65e-15;
        mobility_eff[3] =  302.2 * (1e-2 * 1e6 * 1e-2 * 1e6);
        Vdsat[3] = 0.32; 
        c_g_ideal[3] = 5.08e-16;
        c_fringe[3] = 0.08e-15;
        c_junc[3] = 1e-15;
        I_on_n[3] = 1094.3e-6;
        I_on_p[3] = I_on_n[3] / 2;
        nmos_effective_resistance_multiplier = 1.62;
        n_to_p_eff_curr_drv_ratio[3] = 2.05;
        gmp_to_gmn_multiplier[3] = 0.90;
        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp / I_on_n[3];
        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];
        I_off_n[3][0] = 5.80e-15; 
        I_off_n[3][10] = 1.21e-14;
        I_off_n[3][20] = 2.42e-14;
        I_off_n[3][30] = 4.65e-14;
        I_off_n[3][40] = 8.60e-14;
        I_off_n[3][50] = 1.54e-13;
        I_off_n[3][60] = 2.66e-13;
        I_off_n[3][70] = 4.45e-13;
        I_off_n[3][80] = 7.17e-13;
        I_off_n[3][90] = 1.11e-12;
        I_off_n[3][100] = 1.67e-12;
      }

      //SRAM cell properties
      curr_Wmemcella_sram = 1.31 * g_ip->F_sz_um;
      curr_Wmemcellpmos_sram = 1.23 * g_ip->F_sz_um;
      curr_Wmemcellnmos_sram = 2.08 * g_ip->F_sz_um;
      curr_area_cell_sram = 146 * g_ip->F_sz_um * g_ip->F_sz_um;
      curr_asp_ratio_cell_sram = 1.46;
    }

    if (tech == 65)
    { //65nm technology-node. Corresponds to year 2007 in ITRS
      //ITRS HP device type
      SENSE_AMP_D = .2e-9; // s
      SENSE_AMP_P = 5.7e-15; // J
      vdd[0] = 1.1;
      Lphy[0] = 0.025;
      Lelec[0] = 0.019;
      t_ox[0] = 1.1e-3;
      v_th[0] = .19491;
      c_ox[0] = 1.88e-14;
      mobility_eff[0] = 436.24 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[0] = 7.71e-2; 
      c_g_ideal[0] = 4.69e-16;
      c_fringe[0] = 0.077e-15;
      c_junc[0] = 1e-15;
      I_on_n[0] = 1197.2e-6;
      I_on_p[0] = 870.8e-6;
      nmos_effective_resistance_multiplier = 1.50;
      n_to_p_eff_curr_drv_ratio[0] = 2.41;
      gmp_to_gmn_multiplier[0] = 1.38;
      Rnchannelon[0] = nmos_effective_resistance_multiplier * vdd[0] / I_on_n[0];
      Rpchannelon[0] = n_to_p_eff_curr_drv_ratio[0] * Rnchannelon[0];
      I_off_n[0][0] = 1.96e-7;
      I_off_n[0][10] = 2.29e-7;
      I_off_n[0][20] = 2.66e-7;
      I_off_n[0][30] = 3.05e-7;
      I_off_n[0][40] = 3.49e-7;
      I_off_n[0][50] = 3.95e-7;
      I_off_n[0][60] = 4.45e-7;
      I_off_n[0][70] = 4.97e-7;
      I_off_n[0][80] = 5.48e-7;
      I_off_n[0][90] = 5.94e-7;
      I_off_n[0][100] = 6.3e-7;

      //ITRS LSTP device type
      vdd[1] = 1.2;
      Lphy[1] = 0.045;
      Lelec[1] = 0.0298;
      t_ox[1] = 1.9e-3;
      v_th[1] = 0.52354;
      c_ox[1] = 1.36e-14;
      mobility_eff[1] = 341.21 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[1] = 0.128; 
      c_g_ideal[1] = 6.14e-16;
      c_fringe[1] = 0.08e-15;
      c_junc[1] = 1e-15;
      I_on_n[1] = 519.2e-6;
      I_on_p[1] = 266e-6;
      nmos_effective_resistance_multiplier = 1.96;
      n_to_p_eff_curr_drv_ratio[1] = 2.23;
      gmp_to_gmn_multiplier[1] = 0.99;
      Rnchannelon[1] = nmos_effective_resistance_multiplier * vdd[1] / I_on_n[1];
      Rpchannelon[1] = n_to_p_eff_curr_drv_ratio[1] * Rnchannelon[1];
      I_off_n[1][0] = 9.12e-12;
      I_off_n[1][10] = 1.49e-11;
      I_off_n[1][20] = 2.36e-11;
      I_off_n[1][30] = 3.64e-11;
      I_off_n[1][40] = 5.48e-11;
      I_off_n[1][50] = 8.05e-11;
      I_off_n[1][60] = 1.15e-10;
      I_off_n[1][70] = 1.59e-10;
      I_off_n[1][80] = 2.1e-10;
      I_off_n[1][90] = 2.62e-10;
      I_off_n[1][100] = 3.21e-10;

      //ITRS LOP device type
      vdd[2] = 0.8;
      Lphy[2] = 0.032;
      Lelec[2] = 0.0216;
      t_ox[2] = 1.2e-3;
      v_th[2] = 0.28512;
      c_ox[2] = 1.87e-14;
      mobility_eff[2] = 495.19 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[2] = 0.292; 
      c_g_ideal[2] = 6e-16;
      c_fringe[2] = 0.08e-15;
      c_junc[2] = 1e-15;
      I_on_n[2] = 573.1e-6;
      I_on_p[2] = 340.6e-6;
      nmos_effective_resistance_multiplier = 1.82;
      n_to_p_eff_curr_drv_ratio[2] = 2.28;
      gmp_to_gmn_multiplier[2] = 1.11;
      Rnchannelon[2] = nmos_effective_resistance_multiplier * vdd[2] / I_on_n[2];
      Rpchannelon[2] = n_to_p_eff_curr_drv_ratio[2] * Rnchannelon[2];
      I_off_n[2][0] = 4.9e-9;
      I_off_n[2][10] = 6.49e-9;
      I_off_n[2][20] = 8.45e-9;
      I_off_n[2][30] = 1.08e-8;
      I_off_n[2][40] = 1.37e-8;
      I_off_n[2][50] = 1.71e-8;
      I_off_n[2][60] = 2.09e-8;
      I_off_n[2][70] = 2.48e-8;
      I_off_n[2][80] = 2.84e-8;
      I_off_n[2][90] = 3.13e-8;
      I_off_n[2][100] = 3.42e-8;

      if (ram_cell_tech_type == lp_dram)
      {
        //LP-DRAM cell access transistor technology parameters
        curr_vdd_dram_cell = 1.2;
        Lphy[3] = 0.12;
        Lelec[3] = 0.0756;
        curr_v_th_dram_access_transistor = 0.43806;
        width_dram_access_transistor = 0.09;
        curr_I_on_dram_cell = 36e-6;
        curr_I_off_dram_cell_worst_case_length_temp = 19.6e-12;
        curr_Wmemcella_dram = width_dram_access_transistor;
        curr_Wmemcellpmos_dram = 0;
        curr_Wmemcellnmos_dram = 0;
        curr_area_cell_dram = 0.11;
        curr_asp_ratio_cell_dram = 1.46; 
        curr_c_dram_cell = 20e-15;

        //LP-DRAM wordline transistor parameters
        curr_vpp = 1.6;
        t_ox[3] = 2.2e-3;
        v_th[3] = 0.43806;
        c_ox[3] = 1.22e-14;
        mobility_eff[3] =  328.32 * (1e-2 * 1e6 * 1e-2 * 1e6);
        Vdsat[3] = 0.43806; 
        c_g_ideal[3] = 1.46e-15;
        c_fringe[3] = 0.08e-15;
        c_junc[3] = 1e-15 ;
        I_on_n[3] = 399.8e-6;
        I_on_p[3] = 243.4e-6;
        nmos_effective_resistance_multiplier = 1.65;
        n_to_p_eff_curr_drv_ratio[3] = 2.05;
        gmp_to_gmn_multiplier[3] = 0.90;
        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp / I_on_n[3];
        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];
        I_off_n[3][0]  = 2.23e-11; 
        I_off_n[3][10] = 3.46e-11;
        I_off_n[3][20] = 5.24e-11;
        I_off_n[3][30] = 7.75e-11;
        I_off_n[3][40] = 1.12e-10;
        I_off_n[3][50] = 1.58e-10;
        I_off_n[3][60] = 2.18e-10;
        I_off_n[3][70] = 2.88e-10;
        I_off_n[3][80] = 3.63e-10;
        I_off_n[3][90] = 4.41e-10;
        I_off_n[3][100] = 5.36e-10;
      }
      else if (ram_cell_tech_type == comm_dram)
      {
        //COMM-DRAM cell access transistor technology parameters
        curr_vdd_dram_cell = 1.3;
        Lphy[3] = 0.065;
        Lelec[3] = 0.0426;
        curr_v_th_dram_access_transistor = 1;
        width_dram_access_transistor = 0.065;
        curr_I_on_dram_cell = 20e-6;
        curr_I_off_dram_cell_worst_case_length_temp = 1e-15;
        curr_Wmemcella_dram = width_dram_access_transistor;
        curr_Wmemcellpmos_dram = 0;
        curr_Wmemcellnmos_dram = 0;
        curr_area_cell_dram = 6*0.065*0.065;
        curr_asp_ratio_cell_dram = 1.5; 
        curr_c_dram_cell = 30e-15;

        //COMM-DRAM wordline transistor parameters
        curr_vpp = 3.3;
        t_ox[3] = 5e-3;
        v_th[3] = 1.0;
        c_ox[3] = 6.16e-15;
        mobility_eff[3] =  303.44 * (1e-2 * 1e6 * 1e-2 * 1e6);
        Vdsat[3] = 0.385;
        c_g_ideal[3] = 4e-16;
        c_fringe[3] = 0.08e-15;
        c_junc[3] = 1e-15 ;
        I_on_n[3] = 1031e-6;
        I_on_p[3] = I_on_n[3] / 2;
        nmos_effective_resistance_multiplier = 1.69;
        n_to_p_eff_curr_drv_ratio[3] = 2.39;
        gmp_to_gmn_multiplier[3] = 0.90;
        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp / I_on_n[3];
        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];
        I_off_n[3][0]  = 1.80e-14; 
        I_off_n[3][10] = 3.64e-14;
        I_off_n[3][20] = 7.03e-14;
        I_off_n[3][30] = 1.31e-13;
        I_off_n[3][40] = 2.35e-13;
        I_off_n[3][50] = 4.09e-13;
        I_off_n[3][60] = 6.89e-13;
        I_off_n[3][70] = 1.13e-12;
        I_off_n[3][80] = 1.78e-12;
        I_off_n[3][90] = 2.71e-12;
        I_off_n[3][100] = 3.99e-12;
      }

      //SRAM cell properties 
      curr_Wmemcella_sram = 1.31 * g_ip->F_sz_um;
      curr_Wmemcellpmos_sram = 1.23 * g_ip->F_sz_um;
      curr_Wmemcellnmos_sram = 2.08 * g_ip->F_sz_um;
      curr_area_cell_sram = 146 * g_ip->F_sz_um * g_ip->F_sz_um;
      curr_asp_ratio_cell_sram = 1.46;
    }

    if (tech == 45)
    { //45nm technology-node. Corresponds to year 2010 in ITRS
      //ITRS HP device type
      SENSE_AMP_D = .04e-9; // s
      SENSE_AMP_P = 2.7e-15; // J
      vdd[0] = 1.0;
      Lphy[0] = 0.018;
      Lelec[0] = 0.01345;
      t_ox[0] = 0.65e-3;
      v_th[0] = .18035;
      c_ox[0] = 3.77e-14;
      mobility_eff[0] = 266.68 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[0] = 9.38E-2; 
      c_g_ideal[0] = 6.78e-16;
      c_fringe[0] = 0.05e-15;
      c_junc[0] = 1e-15;
      I_on_n[0] = 2046.6e-6;
      //There are certain problems with the ITRS PMOS numbers in MASTAR for 45nm. So we are using 65nm values of
      //n_to_p_eff_curr_drv_ratio and gmp_to_gmn_multiplier for 45nm
      I_on_p[0] = I_on_n[0] / 2;//This value is fixed arbitrarily but I_on_p is not being used in CACTI
      nmos_effective_resistance_multiplier = 1.51;
      n_to_p_eff_curr_drv_ratio[0] = 2.41;
      gmp_to_gmn_multiplier[0] = 1.38;
      Rnchannelon[0] = nmos_effective_resistance_multiplier * vdd[0] / I_on_n[0];
      Rpchannelon[0] = n_to_p_eff_curr_drv_ratio[0] * Rnchannelon[0];
      I_off_n[0][0] = 2.8e-7;
      I_off_n[0][10] = 3.28e-7;
      I_off_n[0][20] = 3.81e-7;
      I_off_n[0][30] = 4.39e-7;
      I_off_n[0][40] = 5.02e-7;
      I_off_n[0][50] = 5.69e-7;
      I_off_n[0][60] = 6.42e-7;
      I_off_n[0][70] = 7.2e-7;
      I_off_n[0][80] = 8.03e-7;
      I_off_n[0][90] = 8.91e-7;
      I_off_n[0][100] = 9.84e-7;

      //ITRS LSTP device type
      vdd[1] = 1.1;
      Lphy[1] =  0.028;
      Lelec[1] = 0.0212;
      t_ox[1] = 1.4e-3;
      v_th[1] = 0.50245;
      c_ox[1] = 2.01e-14;
      mobility_eff[1] =  363.96 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[1] = 9.12e-2; 
      c_g_ideal[1] = 5.18e-16;
      c_fringe[1] = 0.08e-15;
      c_junc[1] = 1e-15;
      I_on_n[1] = 666.2e-6;
      I_on_p[1] = I_on_n[1] / 2;
      nmos_effective_resistance_multiplier = 1.99;
      n_to_p_eff_curr_drv_ratio[1] = 2.23;
      gmp_to_gmn_multiplier[1] = 0.99;
      Rnchannelon[1] = nmos_effective_resistance_multiplier * vdd[1] / I_on_n[1];
      Rpchannelon[1] = n_to_p_eff_curr_drv_ratio[1] * Rnchannelon[1];
      I_off_n[1][0] = 1.01e-11;
      I_off_n[1][10] = 1.65e-11;
      I_off_n[1][20] = 2.62e-11;
      I_off_n[1][30] = 4.06e-11;
      I_off_n[1][40] = 6.12e-11;
      I_off_n[1][50] = 9.02e-11;
      I_off_n[1][60] = 1.3e-10;
      I_off_n[1][70] = 1.83e-10;
      I_off_n[1][80] = 2.51e-10;
      I_off_n[1][90] = 3.29e-10;
      I_off_n[1][100] = 4.1e-10;

      //ITRS LOP device type
      vdd[2] = 0.7;
      Lphy[2] = 0.022;
      Lelec[2] = 0.016;
      t_ox[2] = 0.9e-3;
      v_th[2] = 0.22599;
      c_ox[2] = 2.82e-14;//F/micron2
      mobility_eff[2] = 508.9 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[2] = 5.71e-2; 
      c_g_ideal[2] = 6.2e-16;
      c_fringe[2] = 0.073e-15;
      c_junc[2] = 1e-15;
      I_on_n[2] = 748.9e-6;
      I_on_p[2] = I_on_n[2] / 2;
      nmos_effective_resistance_multiplier = 1.76;
      n_to_p_eff_curr_drv_ratio[2] = 2.28;
      gmp_to_gmn_multiplier[2] = 1.11;
      Rnchannelon[2] = nmos_effective_resistance_multiplier * vdd[2] / I_on_n[2];
      Rpchannelon[2] = n_to_p_eff_curr_drv_ratio[2] * Rnchannelon[2];
      I_off_n[2][0] = 4.03e-9;
      I_off_n[2][10] = 5.02e-9;
      I_off_n[2][20] = 6.18e-9;
      I_off_n[2][30] = 7.51e-9;
      I_off_n[2][40] = 9.04e-9;
      I_off_n[2][50] = 1.08e-8;
      I_off_n[2][60] = 1.27e-8;
      I_off_n[2][70] = 1.47e-8;
      I_off_n[2][80] = 1.66e-8;
      I_off_n[2][90] = 1.84e-8;
      I_off_n[2][100] = 2.03e-8;

      if (ram_cell_tech_type == lp_dram)
      {
        //LP-DRAM cell access transistor technology parameters
        curr_vdd_dram_cell = 1.1;
        Lphy[3] = 0.078;
        Lelec[3] = 0.0504;// Assume Lelec is 30% lesser than Lphy for DRAM access and wordline transistors.
        curr_v_th_dram_access_transistor = 0.44559;
        width_dram_access_transistor = 0.079;
        curr_I_on_dram_cell = 36e-6;//A
        curr_I_off_dram_cell_worst_case_length_temp = 19.5e-12;
        curr_Wmemcella_dram = width_dram_access_transistor;
        curr_Wmemcellpmos_dram = 0;
        curr_Wmemcellnmos_dram  = 0;
        curr_area_cell_dram = width_dram_access_transistor * Lphy[3] * 10.0;
        curr_asp_ratio_cell_dram = 1.46; 
        curr_c_dram_cell = 20e-15;

        //LP-DRAM wordline transistor parameters
        curr_vpp = 1.5;
        t_ox[3] = 2.1e-3;
        v_th[3] = 0.44559;
        c_ox[3] = 1.41e-14;
        mobility_eff[3] =   426.30 * (1e-2 * 1e6 * 1e-2 * 1e6);
        Vdsat[3] = 0.181; 
        c_g_ideal[3] = 1.10e-15;
        c_fringe[3] = 0.08e-15;
        c_junc[3] = 1e-15;
        I_on_n[3] = 456e-6;
        I_on_p[3] = I_on_n[3] / 2;
        nmos_effective_resistance_multiplier = 1.65;
        n_to_p_eff_curr_drv_ratio[3] = 2.05;
        gmp_to_gmn_multiplier[3] = 0.90;
        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp / I_on_n[3];
        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];
        I_off_n[3][0] = 2.54e-11; 
        I_off_n[3][10] = 3.94e-11;
        I_off_n[3][20] = 5.95e-11;
        I_off_n[3][30] = 8.79e-11;
        I_off_n[3][40] = 1.27e-10;
        I_off_n[3][50] = 1.79e-10;
        I_off_n[3][60] = 2.47e-10;
        I_off_n[3][70] = 3.31e-10;
        I_off_n[3][80] = 4.26e-10;
        I_off_n[3][90] = 5.27e-10;
        I_off_n[3][100] = 6.46e-10;
      }
      else if (ram_cell_tech_type == comm_dram)
      {
        //COMM-DRAM cell access transistor technology parameters
        curr_vdd_dram_cell = 1.1;
        Lphy[3] = 0.045;
        Lelec[3] = 0.0298;
        curr_v_th_dram_access_transistor = 1;
        width_dram_access_transistor = 0.045;
        curr_I_on_dram_cell = 20e-6;//A
        curr_I_off_dram_cell_worst_case_length_temp = 1e-15;
        curr_Wmemcella_dram = width_dram_access_transistor;
        curr_Wmemcellpmos_dram = 0;
        curr_Wmemcellnmos_dram  = 0;
        curr_area_cell_dram = 6*0.045*0.045;
        curr_asp_ratio_cell_dram = 1.5; 
        curr_c_dram_cell = 30e-15;

        //COMM-DRAM wordline transistor parameters
        curr_vpp = 2.7;
        t_ox[3] = 4e-3;
        v_th[3] = 1.0;
        c_ox[3] = 7.98e-15;
        mobility_eff[3] = 368.58 * (1e-2 * 1e6 * 1e-2 * 1e6);
        Vdsat[3] = 0.147; 
        c_g_ideal[3] = 3.59e-16;
        c_fringe[3] = 0.08e-15;
        c_junc[3] = 1e-15;
        I_on_n[3] = 999.4e-6;
        I_on_p[3] = I_on_n[3] / 2;
        nmos_effective_resistance_multiplier = 1.69;
        n_to_p_eff_curr_drv_ratio[3] = 1.95;
        gmp_to_gmn_multiplier[3] = 0.90;
        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp / I_on_n[3];
        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];
        I_off_n[3][0] = 1.31e-14; 
        I_off_n[3][10] = 2.68e-14;
        I_off_n[3][20] = 5.25e-14;
        I_off_n[3][30] = 9.88e-14;
        I_off_n[3][40] = 1.79e-13;
        I_off_n[3][50] = 3.15e-13;
        I_off_n[3][60] = 5.36e-13;
        I_off_n[3][70] = 8.86e-13;
        I_off_n[3][80] = 1.42e-12;
        I_off_n[3][90] = 2.20e-12;
        I_off_n[3][100] = 3.29e-12;
      }


      //SRAM cell properties 
      curr_Wmemcella_sram = 1.31 * g_ip->F_sz_um;
      curr_Wmemcellpmos_sram = 1.23 * g_ip->F_sz_um;
      curr_Wmemcellnmos_sram = 2.08 * g_ip->F_sz_um;
      curr_area_cell_sram = 146 * g_ip->F_sz_um * g_ip->F_sz_um;
      curr_asp_ratio_cell_sram = 1.46;
    }

    if (tech == 32)
    {
      SENSE_AMP_D = .03e-9; // s
      SENSE_AMP_P = 2.16e-15; // J
      //For 2013, MPU/ASIC stagger-contacted M1 half-pitch is 32 nm (so this is 32 nm
      //technology i.e. FEATURESIZE = 0.032). Using the SOI process numbers for 
      //HP and LSTP.
      vdd[0] = 0.9;
      Lphy[0] = 0.013;
      Lelec[0] = 0.01013;
      t_ox[0] = 0.5e-3;
      v_th[0] = 0.21835;
      c_ox[0] = 4.11e-14;
      mobility_eff[0] = 361.84 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[0] = 5.09E-2; 
      c_g_ideal[0] = 5.34e-16;
      c_fringe[0] = 0.04e-15;
      c_junc[0] = 1e-15;
      I_on_n[0] =  2211.7e-6;
      I_on_p[0] = I_on_n[0] / 2;
      nmos_effective_resistance_multiplier = 1.49;
      n_to_p_eff_curr_drv_ratio[0] = 2.41;
      gmp_to_gmn_multiplier[0] = 1.38;
      Rnchannelon[0] = nmos_effective_resistance_multiplier * vdd[0] / I_on_n[0];//ohm-micron
      Rpchannelon[0] = n_to_p_eff_curr_drv_ratio[0] * Rnchannelon[0];//ohm-micron
      I_off_n[0][0] = 1.52e-7;
      I_off_n[0][10] = 1.55e-7;
      I_off_n[0][20] = 1.59e-7;
      I_off_n[0][30] = 1.68e-7;
      I_off_n[0][40] = 1.90e-7;
      I_off_n[0][50] = 2.69e-7;
      I_off_n[0][60] = 5.32e-7;
      I_off_n[0][70] = 1.02e-6;
      I_off_n[0][80] = 1.62e-6;
      I_off_n[0][90] = 2.73e-6;
      I_off_n[0][100] = 6.1e-6;

      //LSTP device type
      vdd[1] = 1;
      Lphy[1] = 0.020;
      Lelec[1] = 0.0173;
      t_ox[1] = 1.2e-3;
      v_th[1] = 0.513;
      c_ox[1] = 2.29e-14;
      mobility_eff[1] =  347.46 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[1] = 8.64e-2; 
      c_g_ideal[1] = 4.58e-16;
      c_fringe[1] = 0.053e-15;
      c_junc[1] = 1e-15;
      I_on_n[1] = 683.6e-6;
      I_on_p[1] = I_on_n[1] / 2;
      nmos_effective_resistance_multiplier = 1.99;
      n_to_p_eff_curr_drv_ratio[1] = 2.23;
      gmp_to_gmn_multiplier[1] = 0.99;
      Rnchannelon[1] = nmos_effective_resistance_multiplier * vdd[1] / I_on_n[1];
      Rpchannelon[1] = n_to_p_eff_curr_drv_ratio[1] * Rnchannelon[1];
      I_off_n[1][0] = 2.06e-11;
      I_off_n[1][10] = 3.30e-11;
      I_off_n[1][20] = 5.15e-11;
      I_off_n[1][30] = 7.83e-11;
      I_off_n[1][40] = 1.16e-10;
      I_off_n[1][50] = 1.69e-10;
      I_off_n[1][60] = 2.40e-10;
      I_off_n[1][70] = 3.34e-10;
      I_off_n[1][80] = 4.54e-10;
      I_off_n[1][90] = 5.96e-10;
      I_off_n[1][100] = 7.44e-10;

      //LOP device type
      vdd[2] = 0.6;
      Lphy[2] = 0.016;
      Lelec[2] = 0.01232;
      t_ox[2] = 0.9e-3;
      v_th[2] = 0.24227;
      c_ox[2] = 2.84e-14;
      mobility_eff[2] =  513.52 * (1e-2 * 1e6 * 1e-2 * 1e6); 
      Vdsat[2] = 4.64e-2; 
      c_g_ideal[2] = 4.54e-16;
      c_fringe[2] = 0.057e-15;
      c_junc[2] = 1e-15;
      I_on_n[2] = 827.8e-6;
      I_on_p[2] = I_on_n[2] / 2;
      nmos_effective_resistance_multiplier = 1.73;
      n_to_p_eff_curr_drv_ratio[2] = 2.28;
      gmp_to_gmn_multiplier[2] = 1.11;
      Rnchannelon[2] = nmos_effective_resistance_multiplier * vdd[2] / I_on_n[2];
      Rpchannelon[2] = n_to_p_eff_curr_drv_ratio[2] * Rnchannelon[2];
      I_off_n[2][0] = 5.94e-8;
      I_off_n[2][10] = 7.23e-8;
      I_off_n[2][20] = 8.7e-8;
      I_off_n[2][30] = 1.04e-7;
      I_off_n[2][40] = 1.22e-7;
      I_off_n[2][50] = 1.43e-7;
      I_off_n[2][60] = 1.65e-7;
      I_off_n[2][70] = 1.90e-7;
      I_off_n[2][80] = 2.15e-7;
      I_off_n[2][90] = 2.39e-7;
      I_off_n[2][100] = 2.63e-7;

      if (ram_cell_tech_type == lp_dram)
      {
        //LP-DRAM cell access transistor technology parameters
        curr_vdd_dram_cell = 1.0;
        Lphy[3] = 0.056;
        Lelec[3] = 0.0419;//Assume Lelec is 30% lesser than Lphy for DRAM access and wordline transistors.
        curr_v_th_dram_access_transistor = 0.44129;
        width_dram_access_transistor = 0.056;
        curr_I_on_dram_cell = 36e-6;
        curr_I_off_dram_cell_worst_case_length_temp = 18.9e-12;
        curr_Wmemcella_dram = width_dram_access_transistor;
        curr_Wmemcellpmos_dram = 0;
        curr_Wmemcellnmos_dram = 0;
        curr_area_cell_dram = width_dram_access_transistor * Lphy[3] * 10.0;
        curr_asp_ratio_cell_dram = 1.46; 
        curr_c_dram_cell = 20e-15;

        //LP-DRAM wordline transistor parameters
        curr_vpp = 1.5;
        t_ox[3] = 2e-3;
        v_th[3] = 0.44467;
        c_ox[3] = 1.48e-14;
        mobility_eff[3] =  408.12 * (1e-2 * 1e6 * 1e-2 * 1e6);
        Vdsat[3] = 0.174; 
        c_g_ideal[3] = 7.45e-16;
        c_fringe[3] = 0.053e-15;
        c_junc[3] = 1e-15;
        I_on_n[3] = 1055.4e-6;
        I_on_p[3] = I_on_n[3] / 2;
        nmos_effective_resistance_multiplier = 1.65;
        n_to_p_eff_curr_drv_ratio[3] = 2.05;
        gmp_to_gmn_multiplier[3] = 0.90;
        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp / I_on_n[3];
        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];
        I_off_n[3][0]  = 3.57e-11; 
        I_off_n[3][10] = 5.51e-11;
        I_off_n[3][20] = 8.27e-11;
        I_off_n[3][30] = 1.21e-10;
        I_off_n[3][40] = 1.74e-10;
        I_off_n[3][50] = 2.45e-10;
        I_off_n[3][60] = 3.38e-10;
        I_off_n[3][70] = 4.53e-10;
        I_off_n[3][80] = 5.87e-10;
        I_off_n[3][90] = 7.29e-10;
        I_off_n[3][100] = 8.87e-10;
      }
      else if (ram_cell_tech_type == comm_dram)
      {
        //COMM-DRAM cell access transistor technology parameters
        curr_vdd_dram_cell = 1.0;
        Lphy[3] = 0.032;
        Lelec[3] = 0.0205;//Assume Lelec is 30% lesser than Lphy for DRAM access and wordline transistors.
        curr_v_th_dram_access_transistor = 1;
        width_dram_access_transistor = 0.032;
        curr_I_on_dram_cell = 20e-6;
        curr_I_off_dram_cell_worst_case_length_temp = 1e-15;
        curr_Wmemcella_dram = width_dram_access_transistor;
        curr_Wmemcellpmos_dram = 0;
        curr_Wmemcellnmos_dram = 0;
        curr_area_cell_dram = 6*0.032*0.032;
        curr_asp_ratio_cell_dram = 1.5; 
        curr_c_dram_cell = 30e-15;

        //COMM-DRAM wordline transistor parameters
        curr_vpp = 2.6;
        t_ox[3] = 4e-3;
        v_th[3] = 1.0;
        c_ox[3] = 7.99e-15;
        mobility_eff[3] =  380.76 * (1e-2 * 1e6 * 1e-2 * 1e6);
        Vdsat[3] = 0.129; 
        c_g_ideal[3] = 2.56e-16;
        c_fringe[3] = 0.053e-15;
        c_junc[3] = 1e-15;
        I_on_n[3] = 1024.5e-6;
        I_on_p[3] = I_on_n[3] / 2;
        nmos_effective_resistance_multiplier = 1.69;
        n_to_p_eff_curr_drv_ratio[3] = 1.95;
        gmp_to_gmn_multiplier[3] = 0.90;
        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp / I_on_n[3];
        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];
        I_off_n[3][0]  = 3.63e-14; 
        I_off_n[3][10] = 7.18e-14;
        I_off_n[3][20] = 1.36e-13;
        I_off_n[3][30] = 2.49e-13;
        I_off_n[3][40] = 4.41e-13;
        I_off_n[3][50] = 7.55e-13;
        I_off_n[3][60] = 1.26e-12;
        I_off_n[3][70] = 2.03e-12;
        I_off_n[3][80] = 3.19e-12;
        I_off_n[3][90] = 4.87e-12;
        I_off_n[3][100] = 7.16e-12;
      }

      //SRAM cell properties 
      curr_Wmemcella_sram    = 1.31 * g_ip->F_sz_um;
      curr_Wmemcellpmos_sram = 1.23 * g_ip->F_sz_um;
      curr_Wmemcellnmos_sram = 2.08 * g_ip->F_sz_um;
      curr_area_cell_sram    = 146 * g_ip->F_sz_um * g_ip->F_sz_um;
      curr_asp_ratio_cell_sram = 1.46;
    }



//    if(tech == 22){
//      //For 2016, MPU/ASIC stagger-contacted M1 half-pitch is 22 nm (so this is 32 nm
//      //technology i.e. FEATURESIZE = 0.022). Using the DG process numbers for
//      //HP.
//      //22 nm HP
//      vdd[0] = 0.8;
//      Lphy[0] = 0.009;//Lphy is the physical gate-length.
//      Lelec[0] = 0.00468;//Lelec is the electrical gate-length.
//      t_ox[0] = 0.55e-3;//micron
//      v_th[0] = 0.1395;//V
//      c_ox[0] = 3.63e-14;//F/micron2
//      mobility_eff[0] = 426.07 * (1e-2 * 1e6 * 1e-2 * 1e6); //micron2 / Vs
//      Vdsat[0] = 2.33e-2; //V/micron
//      c_g_ideal[0] = 3.27e-16;//F/micron
//      c_fringe[0] = 0.06e-15;//F/micron
//      c_junc[0] = 0;//F/micron2
//      I_on_n[0] =  2626.4e-6;//A/micron
//      I_on_p[0] = I_on_n[0] / 2;//A/micron //This value for I_on_p is not really used.
//      nmos_effective_resistance_multiplier = 1.45;
//      n_to_p_eff_curr_drv_ratio[0] = 2; //Wpmos/Wnmos = 2 in 2007 MASTAR. Look in
//      //"Dynamic" tab of Device workspace.
//      gmp_to_gmn_multiplier[0] = 1.38; //Just using the 32nm SOI value.
//      Rnchannelon[0] = nmos_effective_resistance_multiplier * vdd[0] / I_on_n[0];//ohm-micron
//      Rpchannelon[0] = n_to_p_eff_curr_drv_ratio[0] * Rnchannelon[0];//ohm-micron
//      I_off_n[0][0] = 1.52e-7/1.5*1.2;
//      I_off_n[0][10] = 1.55e-7/1.5*1.2;
//      I_off_n[0][20] = 1.59e-7/1.5*1.2;
//      I_off_n[0][30] = 1.68e-7/1.5*1.2;
//      I_off_n[0][40] = 1.90e-7/1.5*1.2;
//      I_off_n[0][50] = 2.69e-7/1.5*1.2;
//      I_off_n[0][60] = 5.32e-7/1.5*1.2;
//      I_off_n[0][70] = 1.02e-6/1.5*1.2;
//      I_off_n[0][80] = 1.62e-6/1.5*1.2;
//      I_off_n[0][90] = 2.73e-6/1.5*1.2;
//      I_off_n[0][100] = 6.1e-6/1.5*1.2;
//
//      //22 nm LSTP DG
//      vdd[1] = 0.8;
//      Lphy[1] = 0.014;
//      Lelec[1] = 0.008;//Lelec is the electrical gate-length.
//      t_ox[1] = 1.1e-3;//micron
//      v_th[1] = 0.40126;//V
//      c_ox[1] = 2.30e-14;//F/micron2
//      mobility_eff[1] =  738.09 * (1e-2 * 1e6 * 1e-2 * 1e6); //micron2 / Vs
//      Vdsat[1] = 6.64e-2; //V/micron
//      c_g_ideal[1] = 3.22e-16;//F/micron
//      c_fringe[1] = 0.008e-15;
//      c_junc[1] = 0;//F/micron2
//      I_on_n[1] = 727.6e-6;//A/micron
//      I_on_p[1] = I_on_n[1] / 2;
//      nmos_effective_resistance_multiplier = 1.99;
//      n_to_p_eff_curr_drv_ratio[1] = 2;
//      gmp_to_gmn_multiplier[1] = 0.99;
//      Rnchannelon[1] = nmos_effective_resistance_multiplier * vdd[1] / I_on_n[1];//ohm-micron
//      Rpchannelon[1] = n_to_p_eff_curr_drv_ratio[1] * Rnchannelon[1];//ohm-micron
//      I_off_n[1][0] = 2.43e-11;
//      I_off_n[1][10] = 4.85e-11;
//      I_off_n[1][20] = 9.68e-11;
//      I_off_n[1][30] = 1.94e-10;
//      I_off_n[1][40] = 3.87e-10;
//      I_off_n[1][50] = 7.73e-10;
//      I_off_n[1][60] = 3.55e-10;
//      I_off_n[1][70] = 3.09e-9;
//      I_off_n[1][80] = 6.19e-9;
//      I_off_n[1][90] = 1.24e-8;
//      I_off_n[1][100]= 2.48e-8;
//
//
//      if (ram_cell_tech_type == 3)
//      {}
//      else if (ram_cell_tech_type == 4)
//      {
//        //22 nm commodity DRAM cell access transistor technology parameters.
//        //parameters
//        curr_vdd_dram_cell = 0.9;//0.45;//This value has reduced greatly in 2007 ITRS for all technology nodes. In
//        //2005 ITRS, the value was about twice the value in 2007 ITRS
//        Lphy[3] = 0.022;//micron
//        Lelec[3] = 0.0181;//micron.
//        curr_v_th_dram_access_transistor = 1;//V
//        width_dram_access_transistor = 0.022;//micron
//        curr_I_on_dram_cell = 20e-6; //This is a typical value that I have always
//        //kept constant. In reality this could perhaps be lower
//        curr_I_off_dram_cell_worst_case_length_temp = 1e-15;//A
//        curr_Wmemcella_dram = width_dram_access_transistor;
//        curr_Wmemcellpmos_dram = 0;
//        curr_Wmemcellnmos_dram = 0;
//        curr_area_cell_dram = 6*0.022*0.022;//micron2.
//        curr_asp_ratio_cell_dram = 0.667;
//        curr_c_dram_cell = 30e-15;//This is a typical value that I have alwaus
//        //kept constant.
//
//        //22 nm commodity DRAM wordline transistor parameters obtained using MASTAR.
//        curr_vpp = 2.3;//vpp. V
//        t_ox[3] = 3.5e-3;//micron
//        v_th[3] = 1.0;//V
//        c_ox[3] = 9.06e-15;//F/micron2
//        mobility_eff[3] =  367.29 * (1e-2 * 1e6 * 1e-2 * 1e6);//micron2 / Vs
//        Vdsat[3] = 0.0972; //V/micron
//        c_g_ideal[3] = 1.99e-16;//F/micron
//        c_fringe[3] = 0.053e-15;//F/micron
//        c_junc[3] = 1e-15;//F/micron2
//        I_on_n[3] = 910.5e-6;//A/micron
//        I_on_p[3] = I_on_n[3] / 2;//This value for I_on_p is not really used.
//        nmos_effective_resistance_multiplier = 1.69;//Using the value from 32nm.
//        //
//        n_to_p_eff_curr_drv_ratio[3] = 1.95;//Using the value from 32nm
//        gmp_to_gmn_multiplier[3] = 0.90;
//        Rnchannelon[3] = nmos_effective_resistance_multiplier * curr_vpp  / I_on_n[3];//ohm-micron
//        Rpchannelon[3] = n_to_p_eff_curr_drv_ratio[3] * Rnchannelon[3];//ohm-micron
//        I_off_n[3][0] = 1.1e-13; //A/micron
//        I_off_n[3][10] = 2.11e-13;
//        I_off_n[3][20] = 3.88e-13;
//        I_off_n[3][30] = 6.9e-13;
//        I_off_n[3][40] = 1.19e-12;
//        I_off_n[3][50] = 1.98e-12;
//        I_off_n[3][60] = 3.22e-12;
//        I_off_n[3][70] = 5.09e-12;
//        I_off_n[3][80] = 7.85e-12;
//        I_off_n[3][90] = 1.18e-11;
//        I_off_n[3][100] = 1.72e-11;
//
//      }
//
//
//


      g_tp.peri_global.Vdd       += curr_alpha * vdd[peri_global_tech_type];
      g_tp.peri_global.t_ox      += curr_alpha * t_ox[peri_global_tech_type];
      g_tp.peri_global.Vth       += curr_alpha * v_th[peri_global_tech_type];
      g_tp.peri_global.C_ox      += curr_alpha * c_ox[peri_global_tech_type];
      g_tp.peri_global.C_g_ideal += curr_alpha * c_g_ideal[peri_global_tech_type];
      g_tp.peri_global.C_fringe  += curr_alpha * c_fringe[peri_global_tech_type];
      g_tp.peri_global.C_junc    += curr_alpha * c_junc[peri_global_tech_type];
      g_tp.peri_global.C_junc_sidewall = 0.25e-15;  // F/micron
      g_tp.peri_global.l_phy     += curr_alpha * Lphy[peri_global_tech_type];
      g_tp.peri_global.l_elec    += curr_alpha * Lelec[peri_global_tech_type];
      g_tp.peri_global.I_on_n    += curr_alpha * I_on_n[peri_global_tech_type];
      g_tp.peri_global.R_nch_on  += curr_alpha * Rnchannelon[peri_global_tech_type];
      g_tp.peri_global.R_pch_on  += curr_alpha * Rpchannelon[peri_global_tech_type];
      g_tp.peri_global.n_to_p_eff_curr_drv_ratio 
        += curr_alpha * n_to_p_eff_curr_drv_ratio[peri_global_tech_type];
      g_tp.peri_global.I_off_n   += curr_alpha * I_off_n[peri_global_tech_type][g_ip->temp - 300];
      g_tp.peri_global.I_off_p   += curr_alpha * I_off_n[peri_global_tech_type][g_ip->temp - 300];
      gmp_to_gmn_multiplier_periph_global += curr_alpha * gmp_to_gmn_multiplier[peri_global_tech_type];

      g_tp.sram_cell.Vdd       += curr_alpha * vdd[ram_cell_tech_type];
      g_tp.sram_cell.l_phy     += curr_alpha * Lphy[ram_cell_tech_type];
      g_tp.sram_cell.l_elec    += curr_alpha * Lelec[ram_cell_tech_type];
      g_tp.sram_cell.t_ox      += curr_alpha * t_ox[ram_cell_tech_type];
      g_tp.sram_cell.Vth       += curr_alpha * v_th[ram_cell_tech_type];
      g_tp.sram_cell.C_g_ideal += curr_alpha * c_g_ideal[ram_cell_tech_type];
      g_tp.sram_cell.C_fringe  += curr_alpha * c_fringe[ram_cell_tech_type];
      g_tp.sram_cell.C_junc    += curr_alpha * c_junc[ram_cell_tech_type];
      g_tp.sram_cell.C_junc_sidewall = 0.25e-15;  // F/micron
      g_tp.sram_cell.I_on_n    += curr_alpha * I_on_n[ram_cell_tech_type];
      g_tp.sram_cell.R_nch_on  += curr_alpha * Rnchannelon[ram_cell_tech_type];
      g_tp.sram_cell.R_pch_on  += curr_alpha * Rpchannelon[ram_cell_tech_type];
      g_tp.sram_cell.n_to_p_eff_curr_drv_ratio += curr_alpha * n_to_p_eff_curr_drv_ratio[ram_cell_tech_type];
      g_tp.sram_cell.I_off_n   += curr_alpha * I_off_n[ram_cell_tech_type][g_ip->temp - 300];
      g_tp.sram_cell.I_off_p   += curr_alpha * I_off_n[ram_cell_tech_type][g_ip->temp - 300];

      g_tp.dram_cell_Vdd      += curr_alpha * curr_vdd_dram_cell;
      g_tp.dram_acc.Vth       += curr_alpha * curr_v_th_dram_access_transistor;
      g_tp.dram_acc.l_phy     += curr_alpha * Lphy[dram_cell_tech_flavor];
      g_tp.dram_acc.l_elec    += curr_alpha * Lelec[dram_cell_tech_flavor];
      g_tp.dram_acc.C_g_ideal += curr_alpha * c_g_ideal[dram_cell_tech_flavor];
      g_tp.dram_acc.C_fringe  += curr_alpha * c_fringe[dram_cell_tech_flavor];
      g_tp.dram_acc.C_junc    += curr_alpha * c_junc[dram_cell_tech_flavor];
      g_tp.dram_acc.C_junc_sidewall = 0.25e-15;  // F/micron
      g_tp.dram_cell_I_on     += curr_alpha * curr_I_on_dram_cell;
      g_tp.dram_cell_I_off_worst_case_len_temp += curr_alpha * curr_I_off_dram_cell_worst_case_length_temp;
      g_tp.dram_acc.I_on_n    += curr_alpha * I_on_n[dram_cell_tech_flavor];
      g_tp.dram_cell_C        += curr_alpha * curr_c_dram_cell;
      g_tp.vpp                += curr_alpha * curr_vpp;
      g_tp.dram_wl.l_phy      += curr_alpha * Lphy[dram_cell_tech_flavor];
      g_tp.dram_wl.l_elec     += curr_alpha * Lelec[dram_cell_tech_flavor];
      g_tp.dram_wl.C_g_ideal  += curr_alpha * c_g_ideal[dram_cell_tech_flavor];
      g_tp.dram_wl.C_fringe   += curr_alpha * c_fringe[dram_cell_tech_flavor];
      g_tp.dram_wl.C_junc     += curr_alpha * c_junc[dram_cell_tech_flavor];
      g_tp.dram_wl.C_junc_sidewall = 0.25e-15;  // F/micron
      g_tp.dram_wl.I_on_n     += curr_alpha * I_on_n[dram_cell_tech_flavor];
      g_tp.dram_wl.R_nch_on   += curr_alpha * Rnchannelon[dram_cell_tech_flavor];
      g_tp.dram_wl.R_pch_on   += curr_alpha * Rpchannelon[dram_cell_tech_flavor];
      g_tp.dram_wl.n_to_p_eff_curr_drv_ratio += curr_alpha * n_to_p_eff_curr_drv_ratio[dram_cell_tech_flavor];
      g_tp.dram_wl.I_off_n    += curr_alpha * I_off_n[dram_cell_tech_flavor][g_ip->temp - 300];
      g_tp.dram_wl.I_off_p    += curr_alpha * I_off_n[dram_cell_tech_flavor][g_ip->temp - 300];

      g_tp.dram.cell_a_w    += curr_alpha * curr_Wmemcella_dram;
      g_tp.dram.cell_pmos_w += curr_alpha * curr_Wmemcellpmos_dram;
      g_tp.dram.cell_nmos_w += curr_alpha * curr_Wmemcellnmos_dram;
      area_cell_dram        += curr_alpha * curr_area_cell_dram;
      asp_ratio_cell_dram   += curr_alpha * curr_asp_ratio_cell_dram;
      g_tp.sram.cell_a_w    += curr_alpha * curr_Wmemcella_sram;
      g_tp.sram.cell_pmos_w += curr_alpha * curr_Wmemcellpmos_sram;
      g_tp.sram.cell_nmos_w += curr_alpha * curr_Wmemcellnmos_sram;
      area_cell_sram += curr_alpha * curr_area_cell_sram;
      asp_ratio_cell_sram += curr_alpha * curr_asp_ratio_cell_sram;

      //Sense amplifier latch Gm calculation
      mobility_eff_periph_global += curr_alpha * mobility_eff[peri_global_tech_type]; 
      Vdsat_periph_global += curr_alpha * Vdsat[peri_global_tech_type];
    }


    //Currently we are not modeling the resistance/capacitance of poly anywhere.
    g_tp.w_comp_inv_p1 = 12.5 * g_ip->F_sz_um;//this was 10 micron for the 0.8 micron process
    g_tp.w_comp_inv_n1 =  7.5 * g_ip->F_sz_um;//this was  6 micron for the 0.8 micron process
    g_tp.w_comp_inv_p2 =   25 * g_ip->F_sz_um;//this was 20 micron for the 0.8 micron process
    g_tp.w_comp_inv_n2 =   15 * g_ip->F_sz_um;//this was 12 micron for the 0.8 micron process
    g_tp.w_comp_inv_p3 =   50 * g_ip->F_sz_um;//this was 40 micron for the 0.8 micron process
    g_tp.w_comp_inv_n3 =   30 * g_ip->F_sz_um;//this was 24 micron for the 0.8 micron process
    g_tp.w_eval_inv_p  =  100 * g_ip->F_sz_um;//this was 80 micron for the 0.8 micron process
    g_tp.w_eval_inv_n  =   50 * g_ip->F_sz_um;//this was 40 micron for the 0.8 micron process
    g_tp.w_comp_n     = 12.5 * g_ip->F_sz_um;//this was 10 micron for the 0.8 micron process
    g_tp.w_comp_p     = 37.5 * g_ip->F_sz_um;//this was 30 micron for the 0.8 micron process

    g_tp.MIN_GAP_BET_P_AND_N_DIFFS = 5 * g_ip->F_sz_um;
    g_tp.MIN_GAP_BET_SAME_TYPE_DIFFS = 1.5 * g_ip->F_sz_um;
    g_tp.HPOWERRAIL = 2 * g_ip->F_sz_um;
    g_tp.cell_h_def = 50 * g_ip->F_sz_um; 
    g_tp.w_poly_contact = g_ip->F_sz_um;
    g_tp.spacing_poly_to_contact = g_ip->F_sz_um;
    g_tp.spacing_poly_to_poly = 1.5 * g_ip->F_sz_um;
    g_tp.ram_wl_stitching_overhead_ = 7.5 * g_ip->F_sz_um;

    g_tp.min_w_nmos_ = 3 * g_ip->F_sz_um / 2; 
    g_tp.max_w_nmos_ = 100 * g_ip->F_sz_um; 
//    g_tp.max_w_nmos_ = 100 * g_ip->F_sz_um; 
    g_tp.w_iso       = 12.5*g_ip->F_sz_um;//was 10 micron for the 0.8 micron process
    g_tp.w_sense_n   = 3.75*g_ip->F_sz_um; // sense amplifier N-trans; was 3 micron for the 0.8 micron process
    g_tp.w_sense_p   = 7.5*g_ip->F_sz_um; // sense amplifier P-trans; was 6 micron for the 0.8 micron process
    g_tp.w_sense_en  = 5*g_ip->F_sz_um; // Sense enable transistor of the sense amplifier; was 4 micron for the 0.8 micron process
    g_tp.w_nmos_b_mux  = 6 * g_tp.min_w_nmos_;
    g_tp.w_nmos_sa_mux = 6 * g_tp.min_w_nmos_;

    if (ram_cell_tech_type == comm_dram)
    {
      g_tp.max_w_nmos_dec = 8 * g_ip->F_sz_um;
      g_tp.h_dec          = 8;  // in the unit of memory cell height
    }
    else
    {
      g_tp.max_w_nmos_dec = g_tp.max_w_nmos_;
      g_tp.h_dec          = 4;  // in the unit of memory cell height
    }

    g_tp.peri_global.C_overlap = 0.2 * g_tp.peri_global.C_g_ideal;
    g_tp.sram_cell.C_overlap   = 0.2 * g_tp.sram_cell.C_g_ideal;

    g_tp.dram_acc.C_overlap = 0.2 * g_tp.dram_acc.C_g_ideal;
    g_tp.dram_acc.R_nch_on = g_tp.dram_cell_Vdd / g_tp.dram_acc.I_on_n;
    //g_tp.dram_acc.R_pch_on = g_tp.dram_cell_Vdd / g_tp.dram_acc.I_on_p;

    g_tp.dram_wl.C_overlap = 0.2 * g_tp.dram_wl.C_g_ideal;

    double gmn_sense_amp_latch = (mobility_eff_periph_global / 2) * g_tp.peri_global.C_ox * (g_tp.w_sense_n / g_tp.peri_global.l_elec) * Vdsat_periph_global; 
    double gmp_sense_amp_latch = gmp_to_gmn_multiplier_periph_global * gmn_sense_amp_latch;
    g_tp.gm_sense_amp_latch = gmn_sense_amp_latch + gmp_sense_amp_latch;

    g_tp.dram.b_w = sqrt(area_cell_dram / (asp_ratio_cell_dram));
    g_tp.dram.b_h = asp_ratio_cell_dram * g_tp.dram.b_w;
    g_tp.sram.b_w = sqrt(area_cell_sram / (asp_ratio_cell_sram));
    g_tp.sram.b_h = asp_ratio_cell_sram * g_tp.sram.b_w;

    g_tp.dram.Vbitpre = g_tp.dram_cell_Vdd; 
    g_tp.sram.Vbitpre = vdd[ram_cell_tech_type];
    pmos_to_nmos_sizing_r = pmos_to_nmos_sz_ratio();
    g_tp.w_pmos_bl_precharge = 6 * pmos_to_nmos_sizing_r * g_tp.min_w_nmos_;
    g_tp.w_pmos_bl_eq = pmos_to_nmos_sizing_r * g_tp.min_w_nmos_;


    double wire_pitch       [NUMBER_INTERCONNECT_PROJECTION_TYPES][NUMBER_WIRE_TYPES],
           wire_r_per_micron[NUMBER_INTERCONNECT_PROJECTION_TYPES][NUMBER_WIRE_TYPES],
           wire_c_per_micron[NUMBER_INTERCONNECT_PROJECTION_TYPES][NUMBER_WIRE_TYPES];

    for (iter=0; iter<=1; ++iter)
    {
      // linear interpolation
      if (iter == 0)
      {
        tech = tech_lo;
        if (tech_lo == tech_hi)
        {
          curr_alpha = 1;
        }
        else
        {
          curr_alpha = (technology - tech_hi)/(tech_lo - tech_hi);
        }
      }
      else
      {
        tech = tech_hi;
        if (tech_lo == tech_hi)
        {
          break;  
        }
        else
        {
          curr_alpha = (tech_lo - technology)/(tech_lo - tech_hi);
        }
      }

      if (tech == 90)
      {
        //Aggressive projections
        wire_pitch[0][0] = 2.5 * g_ip->F_sz_um;//micron
        aspect_ratio = 2.4;
        wire_width = wire_pitch[0][0] / 2; //micron
        wire_thickness = aspect_ratio * wire_width;//micron
        wire_spacing = wire_pitch[0][0] - wire_width;//micron
        barrier_thickness = 0.01;//micron
        dishing_thickness = 0;//micron
        alpha_scatter = 1;
        wire_r_per_micron[0][0] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);//ohm/micron
        ild_thickness = 0.48;//micron
        miller_value = 1.5;
        horiz_dielectric_constant = 2.709;
        vert_dielectric_constant = 3.9;
        fringe_cap = 0.115e-15; //F/micron
        wire_c_per_micron[0][0] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);//F/micron.

        wire_pitch[0][1] = 4 * g_ip->F_sz_um;
        wire_width = wire_pitch[0][1] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][1] - wire_width;
        wire_r_per_micron[0][1] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        wire_c_per_micron[0][1] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[0][2] = 8 * g_ip->F_sz_um;
        aspect_ratio = 2.7;
        wire_width = wire_pitch[0][2] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][2] - wire_width;
        wire_r_per_micron[0][2] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.96;
        wire_c_per_micron[0][2] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        //Conservative projections
        wire_pitch[1][0] = 2.5 * g_ip->F_sz_um;
        aspect_ratio = 2.0;
        wire_width = wire_pitch[1][0] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][0] - wire_width;
        barrier_thickness = 0.008;
        dishing_thickness = 0;
        alpha_scatter = 1;
        wire_r_per_micron[1][0] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.48;
        miller_value = 1.5;
        horiz_dielectric_constant = 3.038;
        vert_dielectric_constant = 3.9;
        fringe_cap = 0.115e-15; 
        wire_c_per_micron[1][0] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[1][1] = 4 * g_ip->F_sz_um;
        wire_width = wire_pitch[1][1] / 2;
        aspect_ratio = 2.0;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][1] - wire_width;
        wire_r_per_micron[1][1] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        wire_c_per_micron[1][1] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[1][2] = 8 * g_ip->F_sz_um;
        aspect_ratio = 2.2;
        wire_width = wire_pitch[1][2] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][2] - wire_width;
        dishing_thickness = 0.1 *  wire_thickness; 
        wire_r_per_micron[1][2] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 1.1;
        wire_c_per_micron[1][2] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);
        //Nominal projections for commodity DRAM wordline/bitline
        wire_pitch[1][3] = 2 * 0.09;
        wire_c_per_micron[1][3] = 60e-15 / (256 * 2 * 0.09);
        wire_r_per_micron[1][3] = 12 / 0.09;
      }
      else if (tech == 65)
      {
        //Aggressive projections
        wire_pitch[0][0] = 2.5 * g_ip->F_sz_um;
        aspect_ratio = 2.7;
        wire_width = wire_pitch[0][0] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][0] - wire_width;
        barrier_thickness = 0;
        dishing_thickness = 0;
        alpha_scatter = 1;
        wire_r_per_micron[0][0] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.405;
        miller_value = 1.5;
        horiz_dielectric_constant = 2.303;
        vert_dielectric_constant = 3.9;
        fringe_cap = 0.115e-15; 
        wire_c_per_micron[0][0] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[0][1] = 4 * g_ip->F_sz_um;
        wire_width = wire_pitch[0][1] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][1] - wire_width;
        wire_r_per_micron[0][1] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        wire_c_per_micron[0][1] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[0][2] = 8 * g_ip->F_sz_um;
        aspect_ratio = 2.8;
        wire_width = wire_pitch[0][2] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][2] - wire_width;
        wire_r_per_micron[0][2] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.81;
        wire_c_per_micron[0][2] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        //Conservative projections
        wire_pitch[1][0] = 2.5 * g_ip->F_sz_um;
        aspect_ratio = 2.0;
        wire_width = wire_pitch[1][0] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][0] - wire_width;
        barrier_thickness = 0.006;
        dishing_thickness = 0;
        alpha_scatter = 1;
        wire_r_per_micron[1][0] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.405;
        miller_value = 1.5;
        horiz_dielectric_constant = 2.734;
        vert_dielectric_constant = 3.9;
        fringe_cap = 0.115e-15; 
        wire_c_per_micron[1][0] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[1][1] = 4 * g_ip->F_sz_um;
        wire_width = wire_pitch[1][1] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][1] - wire_width;
        wire_r_per_micron[1][1] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        wire_c_per_micron[1][1] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[1][2] = 8 * g_ip->F_sz_um;
        aspect_ratio = 2.2;
        wire_width = wire_pitch[1][2] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][2] - wire_width;
        dishing_thickness = 0.1 *  wire_thickness; 
        wire_r_per_micron[1][2] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.77;
        wire_c_per_micron[1][2] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);
        //Nominal projections for commodity DRAM wordline/bitline
        wire_pitch[1][3] = 2 * 0.065;
        wire_c_per_micron[1][3] = 52.5e-15 / (256 * 2 * 0.065);
        wire_r_per_micron[1][3] = 12 / 0.065;
      }
      else if (tech == 45)
      {
        //Aggressive projections.
        wire_pitch[0][0] = 2.5 * g_ip->F_sz_um;
        aspect_ratio = 3.0;
        wire_width = wire_pitch[0][0] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][0] - wire_width;
        barrier_thickness = 0;
        dishing_thickness = 0;
        alpha_scatter = 1;
        wire_r_per_micron[0][0] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.315;
        miller_value = 1.5;
        horiz_dielectric_constant = 1.958;
        vert_dielectric_constant = 3.9;
        fringe_cap = 0.115e-15; 
        wire_c_per_micron[0][0] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[0][1] = 4 * g_ip->F_sz_um;
        wire_width = wire_pitch[0][1] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][1] - wire_width;
        wire_r_per_micron[0][1] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        wire_c_per_micron[0][1] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[0][2] = 8 * g_ip->F_sz_um;
        aspect_ratio = 3.0;
        wire_width = wire_pitch[0][2] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][2] - wire_width;
        wire_r_per_micron[0][2] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.63;
        wire_c_per_micron[0][2] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        //Conservative projections
        wire_pitch[1][0] = 2.5 * g_ip->F_sz_um;
        aspect_ratio = 2.0;
        wire_width = wire_pitch[1][0] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][0] - wire_width;
        barrier_thickness = 0.004;
        dishing_thickness = 0;
        alpha_scatter = 1;
        wire_r_per_micron[1][0] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.315;
        miller_value = 1.5;
        horiz_dielectric_constant = 2.46;
        vert_dielectric_constant = 3.9;
        fringe_cap = 0.115e-15; 
        wire_c_per_micron[1][0] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[1][1] = 4 * g_ip->F_sz_um;
        wire_width = wire_pitch[1][1] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][1] - wire_width;
        wire_r_per_micron[1][1] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        wire_c_per_micron[1][1] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[1][2] = 8 * g_ip->F_sz_um;
        aspect_ratio = 2.2;
        wire_width = wire_pitch[1][2] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][2] - wire_width;
        dishing_thickness = 0.1 * wire_thickness; 
        wire_r_per_micron[1][2] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.55;
        wire_c_per_micron[1][2] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);
        //Nominal projections for commodity DRAM wordline/bitline
        wire_pitch[1][3] = 2 * 0.045;
        wire_c_per_micron[1][3] = 37.5e-15 / (256 * 2 * 0.045);
        wire_r_per_micron[1][3] = 12 / 0.045;
      }
      else if (tech == 32)
      {
        //Aggressive projections.
        wire_pitch[0][0] = 2.5 * g_ip->F_sz_um;
        aspect_ratio = 3.0;
        wire_width = wire_pitch[0][0] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][0] - wire_width;
        barrier_thickness = 0;
        dishing_thickness = 0;
        alpha_scatter = 1;
        wire_r_per_micron[0][0] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.21;
        miller_value = 1.5;
        horiz_dielectric_constant = 1.664;
        vert_dielectric_constant = 3.9;
        fringe_cap = 0.115e-15; 
        wire_c_per_micron[0][0] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[0][1] = 4 * g_ip->F_sz_um;
        wire_width = wire_pitch[0][1] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][1] - wire_width;
        wire_r_per_micron[0][1] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        wire_c_per_micron[0][1] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[0][2] = 8 * g_ip->F_sz_um;
        aspect_ratio = 3.0;
        wire_width = wire_pitch[0][2] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[0][2] - wire_width;
        wire_r_per_micron[0][2] = wire_resistance(BULK_CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.42;
        wire_c_per_micron[0][2] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        //Conservative projections
        wire_pitch[1][0] = 2.5 * g_ip->F_sz_um;
        aspect_ratio = 2.0;
        wire_width = wire_pitch[1][0] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][0] - wire_width;
        barrier_thickness = 0.003;
        dishing_thickness = 0;
        alpha_scatter = 1;
        wire_r_per_micron[1][0] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.21;
        miller_value = 1.5;
        horiz_dielectric_constant = 2.214;
        vert_dielectric_constant = 3.9;
        fringe_cap = 0.115e-15; 
        wire_c_per_micron[1][0] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[1][1] = 4 * g_ip->F_sz_um;
        wire_width = wire_pitch[1][1] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][1] - wire_width;
        wire_r_per_micron[1][1] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        wire_c_per_micron[1][1] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);

        wire_pitch[1][2] = 8 * g_ip->F_sz_um;
        aspect_ratio = 2.2;
        wire_width = wire_pitch[1][2] / 2;
        wire_thickness = aspect_ratio * wire_width;
        wire_spacing = wire_pitch[1][2] - wire_width;
        dishing_thickness = 0.1 *  wire_thickness; 
        wire_r_per_micron[1][2] = wire_resistance(CU_RESISTIVITY, wire_width,
            wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);
        ild_thickness = 0.385;
        wire_c_per_micron[1][2] = wire_capacitance(wire_width, wire_thickness, wire_spacing, 
            ild_thickness, miller_value, horiz_dielectric_constant, vert_dielectric_constant,
            fringe_cap);
        //Nominal projections for commodity DRAM wordline/bitline
        wire_pitch[1][3] = 2 * 0.032;//micron
        wire_c_per_micron[1][3] = 31e-15 / (256 * 2 * 0.032);//F/micron
        wire_r_per_micron[1][3] = 12 / 0.032;//ohm/micron
      }
      g_tp.wire_local.pitch    += curr_alpha * wire_pitch[g_ip->ic_proj_type][(ram_cell_tech_type == comm_dram)?3:0];
      g_tp.wire_local.R_per_um += curr_alpha * wire_r_per_micron[g_ip->ic_proj_type][(ram_cell_tech_type == comm_dram)?3:0];
      g_tp.wire_local.C_per_um += curr_alpha * wire_c_per_micron[g_ip->ic_proj_type][(ram_cell_tech_type == comm_dram)?3:0];
      g_tp.wire_inside_mat.pitch     += curr_alpha * wire_pitch[g_ip->ic_proj_type][g_ip->wire_is_mat_type];
      g_tp.wire_inside_mat.R_per_um  += curr_alpha* wire_r_per_micron[g_ip->ic_proj_type][g_ip->wire_is_mat_type];
      g_tp.wire_inside_mat.C_per_um  += curr_alpha* wire_c_per_micron[g_ip->ic_proj_type][g_ip->wire_is_mat_type];
      g_tp.wire_outside_mat.pitch    += curr_alpha * wire_pitch[g_ip->ic_proj_type][g_ip->wire_os_mat_type];
      g_tp.wire_outside_mat.R_per_um += curr_alpha*wire_r_per_micron[g_ip->ic_proj_type][g_ip->wire_os_mat_type];
      g_tp.wire_outside_mat.C_per_um += curr_alpha*wire_c_per_micron[g_ip->ic_proj_type][g_ip->wire_os_mat_type];
      g_tp.unit_len_wire_del = g_tp.wire_inside_mat.R_per_um * g_tp.wire_inside_mat.C_per_um / 2;
    }
    g_tp.sense_delay = SENSE_AMP_D;
    g_tp.sense_dy_power = SENSE_AMP_P;
    g_tp.horiz_dielectric_constant = horiz_dielectric_constant;
    g_tp.vert_dielectric_constant = vert_dielectric_constant;
    g_tp.aspect_ratio = aspect_ratio;
    g_tp.miller_value = miller_value;

    double rd = tr_R_on(g_tp.min_w_nmos_, NCH, 1);
    double p_to_n_sizing_r = pmos_to_nmos_sz_ratio();
    double c_load = gate_C(g_tp.min_w_nmos_ * (1 + p_to_n_sizing_r), 0.0);
    double tf = rd * c_load;
    g_tp.kinv = horowitz(0, tf, 0.5, 0.5, RISE);
    double KLOAD = 1;
    c_load = KLOAD * (drain_C_(g_tp.min_w_nmos_, NCH, 1, 1, g_tp.cell_h_def) + 
        drain_C_(g_tp.min_w_nmos_ * p_to_n_sizing_r, PCH, 1, 1, g_tp.cell_h_def) +
        gate_C(g_tp.min_w_nmos_ * 4 * (1 + p_to_n_sizing_r), 0.0));
    tf = rd * c_load;
    g_tp.FO4 = horowitz(0, tf, 0.5, 0.5, RISE);
  }

