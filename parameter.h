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



#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "area.h"
#include "const.h"
#include "cacti_interface.h"
#include "io.h"

// parameters which are functions of certain device technology
/**
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
    double Vcc_min;//allowed min vcc; for memory cell it is the lowest vcc for data retention. for logic it is the vcc to balance the leakage reduction and wakeup latency
    double I_on_n;
    double I_on_p;
    double I_off_n;
    double I_off_p;
    double I_g_on_n;
    double I_g_on_p;
    double C_ox;
    double t_ox;
    double n_to_p_eff_curr_drv_ratio;
    double long_channel_leakage_reduction;
    double Mobility_n;

    DeviceType(): C_g_ideal(0), C_fringe(0), C_overlap(0), C_junc(0),
                  C_junc_sidewall(0), l_phy(0), l_elec(0), R_nch_on(0), R_pch_on(0),
                  Vdd(0), Vth(0), Vcc_min(0),
                  I_on_n(0), I_on_p(0), I_off_n(0), I_off_p(0),I_g_on_n(0),I_g_on_p(0),
                  C_ox(0), t_ox(0), n_to_p_eff_curr_drv_ratio(0), long_channel_leakage_reduction(0),
                  Mobility_n(0) { };
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
      Vcc_min   = 0;
      I_on_n    = 0;
      I_on_p    = 0;
      I_off_n   = 0;
      I_off_p   = 0;
      I_g_on_n   = 0;
      I_g_on_p   = 0;
      C_ox      = 0;
      t_ox      = 0;
      n_to_p_eff_curr_drv_ratio = 0;
      long_channel_leakage_reduction = 0;
      Mobility_n = 0;
    }

    void display(uint32_t indent = 0);
  };
  class InterconnectType
  {
   public:
    double pitch;
    double R_per_um;
    double C_per_um;
    double horiz_dielectric_constant;
    double vert_dielectric_constant;
    double aspect_ratio;
    double miller_value;
    double ild_thickness;

    InterconnectType(): pitch(0), R_per_um(0), C_per_um(0) { };

    void reset()
    {
      pitch = 0;
      R_per_um = 0;
      C_per_um = 0;
      horiz_dielectric_constant = 0;
      vert_dielectric_constant = 0;
      aspect_ratio = 0;
      miller_value = 0;
      ild_thickness = 0;
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
    double Vbitfloating;//voltage when floating bitline is supported

    void reset()
    {
      b_w = 0; //fs and tech
      b_h = 0; //fs and tech
      cell_a_w = 0; // ram_cell_tech_type
      cell_pmos_w = 0; //fs
      cell_nmos_w = 0;
      Vbitpre = 0;
      Vbitfloating = 0;
    }

    void display(uint32_t indent = 0);
  };

  class ScalingFactor
  {
   public:
    double logic_scaling_co_eff;
    double core_tx_density;
    double long_channel_leakage_reduction;

    ScalingFactor(): logic_scaling_co_eff(0), core_tx_density(0),
    long_channel_leakage_reduction(0) { };

    void reset()
    {
      logic_scaling_co_eff= 0;
      core_tx_density = 0;
      long_channel_leakage_reduction= 0;
    }

    void display(uint32_t indent = 0);
  };

  double ram_wl_stitching_overhead_; //fs
  double min_w_nmos_; //fs
  double max_w_nmos_; //fs
  double max_w_nmos_dec; //fs+ ram_cell_tech_type
  double unit_len_wire_del; //wire_inside_mat
  double FO4; //fs
  double kinv; //fs
  double vpp; //input 
  double w_sense_en;//fs
  double w_sense_n; //fs
  double w_sense_p; //fs
  double sense_delay; // input
  double sense_dy_power; //input
  double w_iso; //fs
  double w_poly_contact; //fs
  double spacing_poly_to_poly; //fs
  double spacing_poly_to_contact;//fs

  //CACTI3DD TSV params
  double tsv_parasitic_capacitance_fine; 
  double tsv_parasitic_resistance_fine;
  double tsv_minimum_area_fine;

  double tsv_parasitic_capacitance_coarse;
  double tsv_parasitic_resistance_coarse;
  double tsv_minimum_area_coarse;

	//fs
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

  double dram_cell_I_on; //ram_cell_tech_type
  double dram_cell_Vdd;
  double dram_cell_I_off_worst_case_len_temp;
  double dram_cell_C;
  double gm_sense_amp_latch; // depends on many things

  double w_nmos_b_mux;//fs
  double w_nmos_sa_mux;//fs
  double w_pmos_bl_precharge;//fs
  double w_pmos_bl_eq;//fs
  double MIN_GAP_BET_P_AND_N_DIFFS;//fs
  double MIN_GAP_BET_SAME_TYPE_DIFFS;//fs
  double HPOWERRAIL;//fs
  double cell_h_def;//fs

  double chip_layout_overhead; //input
  double macro_layout_overhead;
  double sckt_co_eff;

  double fringe_cap;//input

  uint64_t h_dec; //ram_cell_tech_type

  DeviceType sram_cell;   // SRAM cell transistor
  DeviceType dram_acc;    // DRAM access transistor
  DeviceType dram_wl;     // DRAM wordline transistor
  DeviceType peri_global; // peripheral global
  DeviceType cam_cell;   // SRAM cell transistor

  DeviceType sleep_tx;   // Sleep transistor cell transistor

  InterconnectType wire_local;
  InterconnectType wire_inside_mat;
  InterconnectType wire_outside_mat;

  ScalingFactor scaling_factor;

  MemoryType sram;
  MemoryType dram;
  MemoryType cam;

  void display(uint32_t indent = 0);

  void reset()
  {
    dram_cell_Vdd  = 0;
    dram_cell_I_on = 0;
    dram_cell_C    = 0;
    vpp            = 0;

    sense_delay               = 0;
    sense_dy_power            = 0;
    fringe_cap                = 0;
//    horiz_dielectric_constant = 0;
//    vert_dielectric_constant  = 0;
//    aspect_ratio              = 0;
//    miller_value              = 0;
//    ild_thickness             = 0;

    dram_cell_I_off_worst_case_len_temp = 0;

    sram_cell.reset();
    dram_acc.reset();
    dram_wl.reset();
    peri_global.reset();
    cam_cell.reset();
    sleep_tx.reset();

    scaling_factor.reset();

    wire_local.reset();
    wire_inside_mat.reset();
    wire_outside_mat.reset();

    sram.reset();
    dram.reset();
    cam.reset();

    chip_layout_overhead  = 0;
    macro_layout_overhead = 0;
    sckt_co_eff           = 0;
  }
};

**/
//ali
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
	double Vcc_min;//allowed min vcc; for memory cell it is the lowest vcc for data retention. for logic it is the vcc to balance the leakage reduction and wakeup latency
	double I_on_n;
	double I_on_p;
	double I_off_n;
	double I_off_p;
	double I_g_on_n;
	double I_g_on_p;
	double C_ox;
	double t_ox;
	double n_to_p_eff_curr_drv_ratio;
	double long_channel_leakage_reduction;
	double Mobility_n;
	
	// auxilary parameters
	double Vdsat;
	double gmp_to_gmn_multiplier;
	
	
	DeviceType(): C_g_ideal(0), C_fringe(0), C_overlap(0), C_junc(0),
				  C_junc_sidewall(0), l_phy(0), l_elec(0), R_nch_on(0), R_pch_on(0),
				  Vdd(0), Vth(0), Vcc_min(0),
				  I_on_n(0), I_on_p(0), I_off_n(0), I_off_p(0),I_g_on_n(0),I_g_on_p(0),
				  C_ox(0), t_ox(0), n_to_p_eff_curr_drv_ratio(0), long_channel_leakage_reduction(0),
				  Mobility_n(0) { reset();};
				  
	void assign(const string & in_file,  int tech_flavor, unsigned int temp);			
	void interpolate(double alpha, const DeviceType& dev1, const DeviceType& dev2);  
	void reset()
	{
	 C_g_ideal=0;
	 C_fringe=0;
	 C_overlap=0;
	 C_junc=0;  // C_junc_area
	 C_junc_sidewall=0;
	 l_phy=0;
	 l_elec=0;
	 R_nch_on=0;
	 R_pch_on=0;
	 Vdd=0;
	 Vth=0;
	 Vcc_min=0;//allowed min vcc, for memory cell it is the lowest vcc for data retention. for logic it is the vcc to balance the leakage reduction and wakeup latency
	 I_on_n=0;
	 I_on_p=0;
	 I_off_n=0;
	 I_off_p=0;
	 I_g_on_n=0;
	 I_g_on_p=0;
	 C_ox=0;
	 t_ox=0;
	 n_to_p_eff_curr_drv_ratio=0;
	 long_channel_leakage_reduction=0;
	 Mobility_n=0;
	
	// auxilary parameters
	 Vdsat=0;
	 gmp_to_gmn_multiplier=0;
	}

	void display(uint32_t indent = 0) const;
	bool isEqual(const DeviceType & dev);
};

class InterconnectType
{
	public:
	double pitch;
	double R_per_um;
	double C_per_um;
	double horiz_dielectric_constant;
	double vert_dielectric_constant;
	double aspect_ratio;
	double miller_value;
	double ild_thickness;

	//auxilary parameters
	double wire_width;
	double wire_thickness;
	double wire_spacing;
	double barrier_thickness;
	double dishing_thickness;
	double alpha_scatter;
	double fringe_cap;
	

	InterconnectType(): pitch(0), R_per_um(0), C_per_um(0) { reset(); };

	void reset()
	{
	     pitch=0;
	     R_per_um=0;
	     C_per_um=0;
	     horiz_dielectric_constant=0;
	     vert_dielectric_constant=0;
	     aspect_ratio=0;
	     miller_value=0;
	     ild_thickness=0;

	//auxilary parameters
	     wire_width=0;
	     wire_thickness=0;
	     wire_spacing=0;
	     barrier_thickness=0;
	     dishing_thickness=0;
	     alpha_scatter=0;
	     fringe_cap=0;
	
	}
	void assign(const string & in_file, int projection_type, int tech_flavor);
	void interpolate(double alpha, const InterconnectType & inter1, const InterconnectType & inter2); 
	void display(uint32_t indent = 0);
	bool isEqual(const InterconnectType & inter);
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
	double Vbitfloating;//voltage when floating bitline is supported
	
	// needed to calculate b_w b_h
	double area_cell;
	double asp_ratio_cell;
	
	MemoryType(){reset();}
	void reset()
	{
	     b_w=0;
	     b_h=0;
	     cell_a_w=0;
	     cell_pmos_w=0;
	     cell_nmos_w=0;
	     Vbitpre=0;
	     Vbitfloating=0;
	}
	void assign(const string & in_file, int tech_flavor, int cell_type); // sram(0),cam(1),dram(2)			
	void interpolate(double alpha, const MemoryType& dev1, const MemoryType& dev2); 
	void display(uint32_t indent = 0) const;
	bool isEqual(const MemoryType & mem);
};

class ScalingFactor
{
	public:
	double logic_scaling_co_eff;
	double core_tx_density;
	double long_channel_leakage_reduction;

	ScalingFactor(): logic_scaling_co_eff(0), core_tx_density(0),
	long_channel_leakage_reduction(0) { reset(); };

	void reset()
	{
	   logic_scaling_co_eff=0;
	   core_tx_density=0;
	   long_channel_leakage_reduction=0;
	}
	void assign(const string & in_file);			
	void interpolate(double alpha, const ScalingFactor& dev1, const ScalingFactor& dev2); 
	void display(uint32_t indent = 0);
	bool isEqual(const ScalingFactor & scal);
};

// parameters which are functions of certain device technology
class TechnologyParameter
{
 public:
  double ram_wl_stitching_overhead_; //fs
  double min_w_nmos_; //fs
  double max_w_nmos_; //fs
  double max_w_nmos_dec; //fs+ ram_cell_tech_type
  double unit_len_wire_del; //wire_inside_mat
  double FO4; //fs
  double kinv; //fs
  double vpp; //input 
  double w_sense_en;//fs
  double w_sense_n; //fs
  double w_sense_p; //fs
  double sense_delay; // input
  double sense_dy_power; //input
  double w_iso; //fs
  double w_poly_contact; //fs
  double spacing_poly_to_poly; //fs
  double spacing_poly_to_contact;//fs

  //CACTI3D auxilary variables
  double tsv_pitch;
  double tsv_diameter;
  double tsv_length;
  double tsv_dielec_thickness;
  double tsv_contact_resistance;
  double tsv_depletion_width;
  double tsv_liner_dielectric_constant;

  //CACTI3DD TSV params
  
  double tsv_parasitic_capacitance_fine; 
  double tsv_parasitic_resistance_fine;
  double tsv_minimum_area_fine;

  double tsv_parasitic_capacitance_coarse;
  double tsv_parasitic_resistance_coarse;
  double tsv_minimum_area_coarse;

	//fs
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

  double dram_cell_I_on; //ram_cell_tech_type
  double dram_cell_Vdd;
  double dram_cell_I_off_worst_case_len_temp;
  double dram_cell_C;
  double gm_sense_amp_latch; // depends on many things

  double w_nmos_b_mux;//fs
  double w_nmos_sa_mux;//fs
  double w_pmos_bl_precharge;//fs
  double w_pmos_bl_eq;//fs
  double MIN_GAP_BET_P_AND_N_DIFFS;//fs
  double MIN_GAP_BET_SAME_TYPE_DIFFS;//fs
  double HPOWERRAIL;//fs
  double cell_h_def;//fs

  double chip_layout_overhead; //input
  double macro_layout_overhead;
  double sckt_co_eff;

  double fringe_cap;//input

  uint64_t h_dec; //ram_cell_tech_type

  DeviceType sram_cell;   // SRAM cell transistor
  DeviceType dram_acc;    // DRAM access transistor
  DeviceType dram_wl;     // DRAM wordline transistor
  DeviceType peri_global; // peripheral global
  DeviceType cam_cell;   // SRAM cell transistor

  DeviceType sleep_tx;   // Sleep transistor cell transistor

  InterconnectType wire_local;
  InterconnectType wire_inside_mat;
  InterconnectType wire_outside_mat;

  ScalingFactor scaling_factor;

  MemoryType sram;
  MemoryType dram;
  MemoryType cam;

  void display(uint32_t indent = 0);
  bool isEqual(const TechnologyParameter & tech);
  
  
  void find_upper_and_lower_tech(double technology, int &tech_lo, string& in_file_lo, int &tech_hi, string& in_file_hi);
  void assign_tsv(const string & in_file);
  void init(double technology, bool is_tag);
  TechnologyParameter()
  {
	  reset();
  }
  void reset()
  {
       ram_wl_stitching_overhead_ =0; //fs
       min_w_nmos_ =0; //fs
       max_w_nmos_ =0; //fs
       max_w_nmos_dec =0; //fs+ ram_cell_tech_type
       unit_len_wire_del =0; //wire_inside_mat
       FO4 =0; //fs
       kinv =0; //fs
       vpp =0; //input 
       w_sense_en =0;//fs
       w_sense_n =0; //fs
       w_sense_p =0; //fs
       sense_delay =0; // input
       sense_dy_power =0; //input
       w_iso =0; //fs
       w_poly_contact =0; //fs
       spacing_poly_to_poly =0; //fs
       spacing_poly_to_contact =0;//fs

  //CACTI3D auxilary variables
       tsv_pitch =0;
       tsv_diameter =0;
       tsv_length =0;
       tsv_dielec_thickness =0;
       tsv_contact_resistance =0;
       tsv_depletion_width =0;
       tsv_liner_dielectric_constant =0;

  //CACTI3DD TSV params
  
       tsv_parasitic_capacitance_fine =0; 
       tsv_parasitic_resistance_fine =0;
       tsv_minimum_area_fine =0;

       tsv_parasitic_capacitance_coarse =0;
       tsv_parasitic_resistance_coarse =0;
       tsv_minimum_area_coarse =0;

	//fs
       w_comp_inv_p1 =0;
       w_comp_inv_p2 =0;
       w_comp_inv_p3 =0;
       w_comp_inv_n1 =0;
       w_comp_inv_n2 =0;
       w_comp_inv_n3 =0;
       w_eval_inv_p =0;
       w_eval_inv_n =0;
       w_comp_n =0;
       w_comp_p =0;

       dram_cell_I_on =0; //ram_cell_tech_type
       dram_cell_Vdd =0;
       dram_cell_I_off_worst_case_len_temp =0;
       dram_cell_C =0;
       gm_sense_amp_latch =0; // depends on many things

       w_nmos_b_mux =0;//fs
       w_nmos_sa_mux =0;//fs
       w_pmos_bl_precharge =0;//fs
       w_pmos_bl_eq =0;//fs
       MIN_GAP_BET_P_AND_N_DIFFS =0;//fs
       MIN_GAP_BET_SAME_TYPE_DIFFS =0;//fs
       HPOWERRAIL =0;//fs
       cell_h_def =0;//fs

    chip_layout_overhead  = 0;
    macro_layout_overhead = 0;
    sckt_co_eff           = 0;

    fringe_cap=0;//input

    h_dec=0; //ram_cell_tech_type

    sram_cell.reset();
    dram_acc.reset();
    dram_wl.reset();
    peri_global.reset();
    cam_cell.reset();
    sleep_tx.reset();

    scaling_factor.reset();

    wire_local.reset();
    wire_inside_mat.reset();
    wire_outside_mat.reset();

    sram.reset();
    dram.reset();
    cam.reset();

    
  }
};

//end ali

class DynamicParameter
{
  public:
    bool is_tag;
    bool pure_ram;
    bool pure_cam;
    bool fully_assoc;
    int tagbits;
    int num_subarrays;  // only for leakage computation  -- the number of subarrays per bank
    int num_mats;       // only for leakage computation  -- the number of mats per bank
    double Nspd;
    int Ndwl;
    int Ndbl;
    int Ndcm;
    int deg_bl_muxing;
    int deg_senseamp_muxing_non_associativity;
    int Ndsam_lev_1;
    int Ndsam_lev_2;
    Wire_type wtype; // merge from cacti-7 code to cacti3d code.
    
    int number_addr_bits_mat;             // per port
    int number_subbanks_decode;           // per_port
    int num_di_b_bank_per_port;
    int num_do_b_bank_per_port;
    int num_di_b_mat;
    int num_do_b_mat;
    int num_di_b_subbank;
    int num_do_b_subbank;

    int num_si_b_mat;
    int num_so_b_mat;
    int num_si_b_subbank;
    int num_so_b_subbank;
	int num_si_b_bank_per_port;
	int num_so_b_bank_per_port;

    int number_way_select_signals_mat;
    int num_act_mats_hor_dir;

    int num_act_mats_hor_dir_sl;
    bool is_dram;
    double V_b_sense;
    unsigned int num_r_subarray;
    unsigned int num_c_subarray;
    int tag_num_r_subarray;//: fully associative cache tag and data must be computed together, data and tag must be separate
    int tag_num_c_subarray;
    int data_num_r_subarray;
    int data_num_c_subarray;
    int num_mats_h_dir;
    int num_mats_v_dir;
    uint32_t ram_cell_tech_type;
    double dram_refresh_period;

    DynamicParameter();
    DynamicParameter(
        bool         is_tag_,
        int          pure_ram_,
        int          pure_cam_,
        double       Nspd_,
        unsigned int Ndwl_,
        unsigned int Ndbl_,
        unsigned int Ndcm_,
        unsigned int Ndsam_lev_1_,
        unsigned int Ndsam_lev_2_,
        Wire_type    wt, // merged from cacti-7 to cacti3d
        bool         is_main_mem_);

    int use_inp_params;
    unsigned int num_rw_ports;
    unsigned int num_rd_ports;
    unsigned int num_wr_ports;
    unsigned int num_se_rd_ports;  // number of single ended read ports
    unsigned int num_search_ports;
    unsigned int out_w;// == nr_bits_out
    bool   is_main_mem;
    Area   cell, cam_cell;//cell is the sram_cell in both nomal cache/ram and FA.
    bool   is_valid;
  private:
    void ECC_adjustment();
    void init_CAM();
    void init_FA();
    bool calc_subarr_rc(unsigned int cap); //to calculate and check subarray rows and columns
};



extern InputParameter * g_ip;
extern TechnologyParameter g_tp;

#endif

