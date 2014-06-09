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

#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "area.h"
#include "const.h"
#include "cacti_interface.h"
#include "io.h"


// parameters which are functions of certain device technology
class TechnologyParameter
{
 public:
  class DeviceType
  {
   public:
    double C_g_ideal;
    double C_fringe;
    double C_overlap;
    double C_junc;  // C_junc_area
    double C_junc_sidewall;
    double l_phy;
    double l_elec;
    double R_nch_on;
    double R_pch_on;
    double Vdd;
    double Vth;
    double I_on_n;
    double I_on_p;
    double I_off_n;
    double I_off_p;
    double C_ox;
    double t_ox;
    double n_to_p_eff_curr_drv_ratio;

    DeviceType(): C_g_ideal(0), C_fringe(0), C_overlap(0), C_junc(0),
                  C_junc_sidewall(0), l_phy(0), l_elec(0), R_nch_on(0), R_pch_on(0), 
                  Vdd(0), Vth(0),
                  I_on_n(0), I_on_p(0), I_off_n(0), I_off_p(0),
                  C_ox(0), t_ox(0), n_to_p_eff_curr_drv_ratio(0) { };
    void reset()
    {
      C_g_ideal = 0;
      C_fringe  = 0;
      C_overlap = 0;
      C_junc    = 0;
      l_phy     = 0;
      l_elec    = 0;
      R_nch_on  = 0;
      R_pch_on  = 0;
      Vdd       = 0;
      Vth       = 0;
      I_on_n    = 0;
      I_on_p    = 0;
      I_off_n   = 0;
      I_off_p   = 0;
      C_ox      = 0;
      t_ox      = 0;
      n_to_p_eff_curr_drv_ratio = 0;
    }

    void display(uint32_t indent = 0);
  };
  class InterconnectType
  {
   public:
    double pitch;
    double R_per_um;
    double C_per_um;

    InterconnectType(): pitch(0), R_per_um(0), C_per_um(0) { };

    void reset()
    {
      pitch = 0;
      R_per_um = 0;
      C_per_um = 0;
    }

    void display(uint32_t indent = 0);
  };
  class MemoryType
  {
   public:
    double b_w;
    double b_h;
    double cell_a_w;
    double cell_pmos_w;
    double cell_nmos_w;
    double Vbitpre;

    void reset()
    {
      b_w = 0;
      b_h = 0;
      cell_a_w = 0;
      cell_pmos_w = 0;
      cell_nmos_w = 0;
      Vbitpre = 0;
    }

    void display(uint32_t indent = 0);
  };

  double ram_wl_stitching_overhead_;
  double min_w_nmos_;
  double max_w_nmos_;
  double max_w_nmos_dec;
  double unit_len_wire_del;
  double FO4;
  double kinv;
  double vpp;
  double w_sense_en;
  double w_sense_n;
  double w_sense_p;
  double sense_delay;
  double sense_dy_power;
  double w_iso;
  double w_poly_contact;
  double spacing_poly_to_poly;
  double spacing_poly_to_contact;

  double w_comp_inv_p1;
  double w_comp_inv_p2;
  double w_comp_inv_p3;
  double w_comp_inv_n1;
  double w_comp_inv_n2;
  double w_comp_inv_n3;
  double w_eval_inv_p;
  double w_eval_inv_n;
  double w_comp_n;
  double w_comp_p;

  double dram_cell_I_on;
  double dram_cell_Vdd;
  double dram_cell_I_off_worst_case_len_temp;
  double dram_cell_C;
  double gm_sense_amp_latch;

  double w_nmos_b_mux;
  double w_nmos_sa_mux;
  double w_pmos_bl_precharge;
  double w_pmos_bl_eq;
  double MIN_GAP_BET_P_AND_N_DIFFS;
  double MIN_GAP_BET_SAME_TYPE_DIFFS;
  double HPOWERRAIL;
  double cell_h_def;

  uint64_t h_dec;

  DeviceType sram_cell;   // SRAM cell transistor
  DeviceType dram_acc;    // DRAM access transistor
  DeviceType dram_wl;     // DRAM wordline transistor
  DeviceType peri_global; // peripheral global

  InterconnectType wire_local;
  InterconnectType wire_inside_mat;
  InterconnectType wire_outside_mat;

  double horiz_dielectric_constant;
  double vert_dielectric_constant;
  double aspect_ratio;
  double miller_value;
 
  MemoryType sram;
  MemoryType dram;

  void display(uint32_t indent = 0);

  void reset()
  {
    dram_cell_Vdd  = 0;
    dram_cell_I_on = 0;
    dram_cell_C    = 0;
    vpp            = 0;

    dram_cell_I_off_worst_case_len_temp = 0;

    sram_cell.reset();
    dram_acc.reset();
    dram_wl.reset();
    peri_global.reset();

    wire_local.reset();
    wire_inside_mat.reset();
    wire_outside_mat.reset();

    sram.reset();
    dram.reset();
  }
};



class DynamicParameter
{
  public:
    bool is_tag;
    bool pure_ram;
    int tagbits;
    int num_subarrays;  // only for leakage computation  -- the number of subarrays per bank
    int num_mats;       // only for leakage computation  -- the number of mats per bank
    double Nspd;
    int Ndwl;
    int Ndbl;
    int deg_bl_muxing; 
    int deg_senseamp_muxing_non_associativity; 
    int Ndsam_lev_1;
    int Ndsam_lev_2;
    int number_addr_bits_mat;             // per port
    int number_subbanks_decode;           // per_port
    int num_di_b_bank_per_port;
    int num_do_b_bank_per_port;
    int num_di_b_mat;
    int num_do_b_mat;
    int num_do_b_subbank;
    int number_way_select_signals_mat;
    int num_act_mats_hor_dir;
    bool is_dram;
    double V_b_sense;
    int num_r_subarray;
    int num_c_subarray;
    int num_mats_h_dir;
    int num_mats_v_dir;
    uint32_t ram_cell_tech_type;
    double dram_refresh_period;

    DynamicParameter();
    DynamicParameter(
        bool         is_tag,
        int          pure_ram,
        double       Nspd,
        unsigned int Ndwl,
        unsigned int Ndbl,
        unsigned int Ndcm,
        unsigned int Ndsam_lev_1,
        unsigned int Ndsam_lev_2,
        bool         is_main_mem);

    int use_inp_params;
    unsigned int num_rw_ports;
    unsigned int num_rd_ports;
    unsigned int num_wr_ports;
    unsigned int num_se_rd_ports;  // number of single ended read ports
    unsigned int out_w;// == nr_bits_out
    bool   is_main_mem;
    Area   cell;
    bool   is_valid;
};



extern InputParameter * g_ip;
extern TechnologyParameter g_tp;

#endif

