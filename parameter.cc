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

#include <iostream>
#include <string>
#include <iomanip>

#include "parameter.h"
#include "area.h"

using namespace std;


InputParameter * g_ip;
TechnologyParameter g_tp;



void TechnologyParameter::DeviceType::display(uint32_t indent)
{
  string indent_str(indent, ' ');

  cout << indent_str << "C_g_ideal = " << setw(12) << C_g_ideal << " F/um" << endl;
  cout << indent_str << "C_fringe  = " << setw(12) << C_fringe  << " F/um" << endl;
  cout << indent_str << "C_overlap = " << setw(12) << C_overlap << " F/um" << endl;
  cout << indent_str << "C_junc    = " << setw(12) << C_junc    << " F/um^2" << endl;
  cout << indent_str << "l_phy     = " << setw(12) << l_phy     << " um" << endl;
  cout << indent_str << "l_elec    = " << setw(12) << l_elec    << " um" << endl;
  cout << indent_str << "R_nch_on  = " << setw(12) << R_nch_on  << " ohm-um" << endl;
  cout << indent_str << "R_pch_on  = " << setw(12) << R_pch_on  << " ohm-um" << endl;
  cout << indent_str << "Vdd       = " << setw(12) << Vdd       << " V" << endl;
  cout << indent_str << "Vth       = " << setw(12) << Vth       << " V" << endl;
  cout << indent_str << "I_on_n    = " << setw(12) << I_on_n    << " A/um" << endl;
  cout << indent_str << "I_on_p    = " << setw(12) << I_on_p    << " A/um" << endl;
  cout << indent_str << "I_off_n   = " << setw(12) << I_off_n   << " A/um" << endl;
  cout << indent_str << "I_off_p   = " << setw(12) << I_off_p   << " A/um" << endl;
  cout << indent_str << "C_ox      = " << setw(12) << C_ox      << " F/um^2" << endl;
  cout << indent_str << "t_ox      = " << setw(12) << t_ox      << " um" << endl;
  cout << indent_str << "n_to_p_eff_curr_drv_ratio = " << n_to_p_eff_curr_drv_ratio << endl;
}



void TechnologyParameter::InterconnectType::display(uint32_t indent)
{
  string indent_str(indent, ' ');

  cout << indent_str << "pitch    = " << setw(12) << pitch    << " um" << endl;
  cout << indent_str << "R_per_um = " << setw(12) << R_per_um << " ohm/um" << endl;
  cout << indent_str << "C_per_um = " << setw(12) << C_per_um << " F/um" << endl;
}



void TechnologyParameter::MemoryType::display(uint32_t indent)
{
  string indent_str(indent, ' ');
  
  cout << indent_str << "b_w         = " << setw(12) << b_w << " um" << endl;
  cout << indent_str << "b_h         = " << setw(12) << b_h << " um" << endl;
  cout << indent_str << "cell_a_w    = " << setw(12) << cell_a_w << " um" << endl;
  cout << indent_str << "cell_pmos_w = " << setw(12) << cell_pmos_w << " um" << endl;
  cout << indent_str << "cell_nmos_w = " << setw(12) << cell_nmos_w << " um" << endl;
  cout << indent_str << "Vbitpre     = " << setw(12) << Vbitpre << " V" << endl;
}



void TechnologyParameter::display(uint32_t indent)
{
  string indent_str(indent, ' ');

  cout << indent_str << "ram_wl_stitching_overhead_ = " << setw(12) << ram_wl_stitching_overhead_ << " um" << endl;
  cout << indent_str << "min_w_nmos_                = " << setw(12) << min_w_nmos_                << " um" << endl;
  cout << indent_str << "max_w_nmos_                = " << setw(12) << max_w_nmos_                << " um" << endl;
  cout << indent_str << "unit_len_wire_del          = " << setw(12) << unit_len_wire_del          << " s/um^2" << endl;
  cout << indent_str << "FO4                        = " << setw(12) << FO4                        << " s" << endl;
  cout << indent_str << "kinv                       = " << setw(12) << kinv                       << " s" << endl;
  cout << indent_str << "vpp                        = " << setw(12) << vpp                        << " V" << endl;
  cout << indent_str << "w_sense_en                 = " << setw(12) << w_sense_en                 << " um" << endl;
  cout << indent_str << "w_sense_n                  = " << setw(12) << w_sense_n                  << " um" << endl;
  cout << indent_str << "w_sense_p                  = " << setw(12) << w_sense_p                  << " um" << endl;
  cout << indent_str << "w_iso                      = " << setw(12) << w_iso                      << " um" << endl;
  cout << indent_str << "w_poly_contact             = " << setw(12) << w_poly_contact             << " um" << endl;
  cout << indent_str << "spacing_poly_to_poly       = " << setw(12) << spacing_poly_to_poly       << " um" << endl;
  cout << indent_str << "spacing_poly_to_contact    = " << setw(12) << spacing_poly_to_contact    << " um" << endl;
  cout << endl;
  cout << indent_str << "w_comp_inv_p1              = " << setw(12) << w_comp_inv_p1 << " um" << endl;
  cout << indent_str << "w_comp_inv_p2              = " << setw(12) << w_comp_inv_p2 << " um" << endl;
  cout << indent_str << "w_comp_inv_p3              = " << setw(12) << w_comp_inv_p3 << " um" << endl;
  cout << indent_str << "w_comp_inv_n1              = " << setw(12) << w_comp_inv_n1 << " um" << endl;
  cout << indent_str << "w_comp_inv_n2              = " << setw(12) << w_comp_inv_n2 << " um" << endl;
  cout << indent_str << "w_comp_inv_n3              = " << setw(12) << w_comp_inv_n3 << " um" << endl;
  cout << indent_str << "w_eval_inv_p               = " << setw(12) << w_eval_inv_p  << " um" << endl;
  cout << indent_str << "w_eval_inv_n               = " << setw(12) << w_eval_inv_n  << " um" << endl;
  cout << indent_str << "w_comp_n                   = " << setw(12) << w_comp_n      << " um" << endl;
  cout << indent_str << "w_comp_p                   = " << setw(12) << w_comp_p      << " um" << endl;
  cout << endl;
  cout << indent_str << "dram_cell_I_on             = " << setw(12) << dram_cell_I_on << " A/um" << endl;
  cout << indent_str << "dram_cell_Vdd              = " << setw(12) << dram_cell_Vdd  << " V" << endl;
  cout << indent_str << "dram_cell_I_off_worst_case_len_temp = " << setw(12) << dram_cell_I_off_worst_case_len_temp << " A/um" << endl;
  cout << indent_str << "dram_cell_C                = " << setw(12) << dram_cell_C               << " F" << endl;
  cout << indent_str << "gm_sense_amp_latch         = " << setw(12) << gm_sense_amp_latch        << " F/s" << endl;
  cout << endl;
  cout << indent_str << "w_nmos_b_mux               = " << setw(12) << w_nmos_b_mux              << " um" << endl;
  cout << indent_str << "w_nmos_sa_mux              = " << setw(12) << w_nmos_sa_mux             << " um" << endl;
  cout << indent_str << "w_pmos_bl_precharge        = " << setw(12) << w_pmos_bl_precharge       << " um" << endl;
  cout << indent_str << "w_pmos_bl_eq               = " << setw(12) << w_pmos_bl_eq              << " um" << endl;
  cout << indent_str << "MIN_GAP_BET_P_AND_N_DIFFS  = " << setw(12) << MIN_GAP_BET_P_AND_N_DIFFS << " um" << endl;
  cout << indent_str << "HPOWERRAIL                 = " << setw(12) << HPOWERRAIL                << " um" << endl;
  cout << indent_str << "cell_h_def                 = " << setw(12) << cell_h_def                << " um" << endl;

  cout << endl;
  cout << indent_str << "SRAM cell transistor: " << endl;
  sram_cell.display(indent + 2);

  cout << endl;
  cout << indent_str << "DRAM access transistor: " << endl;
  dram_acc.display(indent + 2);

  cout << endl;
  cout << indent_str << "DRAM wordline transistor: " << endl;
  dram_wl.display(indent + 2);

  cout << endl;
  cout << indent_str << "peripheral global transistor: " << endl;
  peri_global.display(indent + 2);

  cout << endl;
  cout << indent_str << "wire local" << endl;
  wire_local.display(indent + 2);

  cout << endl;
  cout << indent_str << "wire inside mat" << endl;
  wire_inside_mat.display(indent + 2);

  cout << endl;
  cout << indent_str << "wire outside mat" << endl;
  wire_outside_mat.display(indent + 2);

  cout << endl;
  cout << indent_str << "SRAM" << endl;
  sram.display(indent + 2);

  cout << endl;
  cout << indent_str << "DRAM" << endl;
  dram.display(indent + 2);
}


DynamicParameter::DynamicParameter():
  use_inp_params(0), cell(), is_valid(true)
{
}



DynamicParameter::DynamicParameter(
    bool is_tag_,
    int pure_ram_,
    double Nspd_,
    unsigned int Ndwl_, 
    unsigned int Ndbl_,
    unsigned int Ndcm,
    unsigned int Ndsam_lev_1_,
    unsigned int Ndsam_lev_2_,
    bool is_main_mem_):
  is_tag(is_tag_), pure_ram(pure_ram_), tagbits(0), Nspd(Nspd_), Ndwl(Ndwl_), Ndbl(Ndbl_),
  Ndsam_lev_1(Ndsam_lev_1_), Ndsam_lev_2(Ndsam_lev_2_),
  number_way_select_signals_mat(0), V_b_sense(0), use_inp_params(0),
  is_main_mem(is_main_mem_), cell(), is_valid(false)
{
  ram_cell_tech_type = (is_tag) ? g_ip->tag_arr_ram_cell_tech_type : g_ip->data_arr_ram_cell_tech_type;
  is_dram            = ((ram_cell_tech_type == lp_dram) || (ram_cell_tech_type == comm_dram));

  unsigned int capacity_per_die = g_ip->cache_sz / NUMBER_STACKED_DIE_LAYERS;  // capacity per stacked die layer
  const TechnologyParameter::InterconnectType & wire_local = g_tp.wire_local;
  bool fully_assoc = (g_ip->fully_assoc) ? true : false;

  if (fully_assoc)
  { // fully-assocative cache -- ref: CACTi 2.0 report
    if (Ndwl != 1 ||            //Ndwl is fixed to 1 for FA 
        Ndcm != 1 ||            //Ndcm is fixed to 1 for FA
        Nspd < 1 || Nspd > 1 || //Nspd is fixed to 1 for FA
        Ndsam_lev_1 != 1 ||     //Ndsam_lev_1 is fixed to one
        Ndsam_lev_2 != 1 ||     //Ndsam_lev_2 is fixed to one
        Ndbl < 2)
    {
      return;
    }
  }

  if ((is_dram) && (!is_tag) && (Ndcm > 1))
  {
    return;  // For a DRAM array, each bitline has its own sense-amp
  }

  // If it's not an FA tag/data array, Ndwl should be at least two and Ndbl should be
  // at least two because an array is assumed to have at least one mat. And a mat
  // is formed out of two horizontal subarrays and two vertical subarrays
  if (fully_assoc == false && (Ndwl < 1 || Ndbl < 1))
  {
    return;
  }

  // if data array, let tagbits = 0
  if (is_tag)
  {
    if (g_ip->specific_tag)
    {
      tagbits = g_ip->tag_w;
    }
    else
    {
      if (fully_assoc)
      {
        tagbits = ADDRESS_BITS + EXTRA_TAG_BITS - _log2(g_ip->block_sz);
      }
      else
      {
        tagbits = ADDRESS_BITS + EXTRA_TAG_BITS - _log2(capacity_per_die) + 
          _log2(g_ip->tag_assoc*2 - 1) - _log2(g_ip->nbanks);
      }
    }
    tagbits = (((tagbits + 3) >> 2) << 2);

    if (fully_assoc)
    {
      num_r_subarray = (int)(capacity_per_die / (g_ip->block_sz * Ndbl));
      num_c_subarray = (int)((tagbits * Nspd / Ndwl) + EPSILON);
    }
    else
    {
      num_r_subarray = (int)(capacity_per_die / (g_ip->nbanks *
            g_ip->block_sz * g_ip->tag_assoc * Ndbl * Nspd) + EPSILON);
      num_c_subarray = (int)((tagbits * g_ip->tag_assoc * Nspd / Ndwl) + EPSILON);
    }
    //burst_length = 1;
  }
  else
  {
    if (fully_assoc)
    {
      num_r_subarray = (int) (capacity_per_die) / (g_ip->block_sz * Ndbl);
      num_c_subarray = 8 * g_ip->block_sz;
    }
    else
    {
      num_r_subarray = (int)(capacity_per_die / (g_ip->nbanks * 
            g_ip->block_sz * g_ip->data_assoc * Ndbl * Nspd) + EPSILON);
      num_c_subarray = (int)((8 * g_ip->block_sz * g_ip->data_assoc * Nspd / Ndwl) + EPSILON);
    }
    // burst_length = g_ip->block_sz * 8 / g_ip->out_w;
  }    

  if ((!fully_assoc)&&(num_r_subarray < MINSUBARRAYROWS)) return;
  if (num_r_subarray == 0) return;
  if (num_r_subarray > MAXSUBARRAYROWS) return;
  if (num_c_subarray < MINSUBARRAYCOLS) return;
  if (num_c_subarray > MAXSUBARRAYCOLS) return;

  num_subarrays = Ndwl * Ndbl;  

  // calculate wire parameters
  if(is_tag)
  {
    cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 + g_ip->num_rd_ports);
    cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 + 
        (g_ip->num_rd_ports - g_ip->num_se_rd_ports)) + 
      wire_local.pitch * g_ip->num_se_rd_ports;
  }
  else
  {
    if (is_dram)
    {
      cell.h = g_tp.dram.b_h;
      cell.w = g_tp.dram.b_w;
    }
    else
    {
      cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_wr_ports + 
          g_ip->num_rw_ports - 1 + g_ip->num_rd_ports);
      cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 + 
          (g_ip->num_rd_ports - g_ip->num_se_rd_ports) + 
          g_ip->num_wr_ports) + g_tp.wire_local.pitch * g_ip->num_se_rd_ports;
    }
  }

  double c_b_metal = cell.h * wire_local.C_per_um;
  double C_bl;

  if (is_dram)
  {
    deg_bl_muxing = 1;
    if (ram_cell_tech_type == comm_dram)
    {
      C_bl  = num_r_subarray * c_b_metal;
      V_b_sense = (g_tp.dram_cell_Vdd/2) * g_tp.dram_cell_C / (g_tp.dram_cell_C + C_bl);
      if (V_b_sense < VBITSENSEMIN)
      {
        return;
      }
      V_b_sense = VBITSENSEMIN;  // in any case, we fix sense amp input signal to a constant value
      dram_refresh_period = 64e-3;
    }
    else
    {
      double Cbitrow_drain_cap = drain_C_(g_tp.dram.cell_a_w, NCH, 1, 0, cell.w, true, true) / 2.0;
      C_bl  = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
      V_b_sense = (g_tp.dram_cell_Vdd/2) * g_tp.dram_cell_C /(g_tp.dram_cell_C + C_bl);

      if (V_b_sense < VBITSENSEMIN)
      {
        return; //Sense amp input signal is smaller that minimum allowable sense amp input signal
      }
      V_b_sense = VBITSENSEMIN; // in any case, we fix sense amp input signal to a constant value
      //v_storage_worst = g_tp.dram_cell_Vdd / 2 - VBITSENSEMIN * (g_tp.dram_cell_C + C_bl) / g_tp.dram_cell_C;
      //dram_refresh_period = 1.1 * g_tp.dram_cell_C * v_storage_worst / g_tp.dram_cell_I_off_worst_case_len_temp;
      dram_refresh_period = 0.9 * g_tp.dram_cell_C * VDD_STORAGE_LOSS_FRACTION_WORST * g_tp.dram_cell_Vdd / g_tp.dram_cell_I_off_worst_case_len_temp;
    }
  }
  else
  { //SRAM
    V_b_sense = (0.05 * g_tp.sram_cell.Vdd > VBITSENSEMIN) ? 0.05 * g_tp.sram_cell.Vdd : VBITSENSEMIN;
    deg_bl_muxing = Ndcm;
    // "/ 2.0" below is due to the fact that two adjacent access transistors share drain
    // contacts in a physical layout
    double Cbitrow_drain_cap = drain_C_(g_tp.sram.cell_a_w, NCH, 1, 0, cell.w, false, true) / 2.0;
    C_bl = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
    dram_refresh_period = 0;
  }

  if (fully_assoc)
  {
    num_mats_h_dir = 1;
    num_mats_v_dir = Ndbl / 2;
    num_do_b_mat   = 8 * g_ip->block_sz;
    num_mats = num_mats_h_dir * num_mats_v_dir;
  }
  else
  {
    num_mats_h_dir = MAX(Ndwl / 2, 1);
    num_mats_v_dir = MAX(Ndbl / 2, 1);
    num_mats       = num_mats_h_dir * num_mats_v_dir;
    num_do_b_mat   = MAX((num_subarrays/num_mats) * num_c_subarray / (deg_bl_muxing * Ndsam_lev_1 * Ndsam_lev_2), 1);
  }

  if (!(fully_assoc&&is_tag) && (num_do_b_mat < (num_subarrays/num_mats)))
  {
    return;
  }


  int deg_sa_mux_l1_non_assoc;

  if (!is_tag)
  {
    if (is_main_mem == true)
    {
      num_do_b_subbank = g_ip->int_prefetch_w * g_ip->out_w;
      deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
    }
    else
    {
      if (g_ip->fast_access == true)
      {
        num_do_b_subbank = g_ip->out_w * g_ip->data_assoc;
        deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
      }
      else
      {
        if (!fully_assoc)
        {
          num_do_b_subbank = g_ip->out_w;
          deg_sa_mux_l1_non_assoc = Ndsam_lev_1 / g_ip->data_assoc;
          if (deg_sa_mux_l1_non_assoc < 1)
          {
            return;
          }
        }
        else
        {
          num_do_b_subbank = 8 * g_ip->block_sz;
          deg_sa_mux_l1_non_assoc = 1;
        }
      }
    }
  }
  else
  {
    num_do_b_subbank = tagbits * g_ip->tag_assoc;
    if (fully_assoc == false && (num_do_b_mat < tagbits))
    {
      return;
    }
    deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
    //num_do_b_mat = g_ip->tag_assoc / num_mats_h_dir;
  }

  deg_senseamp_muxing_non_associativity = deg_sa_mux_l1_non_assoc;

  if (fully_assoc)
  {
    num_act_mats_hor_dir = 1;
  }
  else
  {
    num_act_mats_hor_dir = num_do_b_subbank / num_do_b_mat;
    if (num_act_mats_hor_dir == 0)
    {
      return;
    }
  }


  if (is_tag)
  {
    if (fully_assoc)
    {
      num_do_b_mat     = 0;
      num_do_b_subbank = 0;
    }
    else
    {
      num_do_b_mat     = g_ip->tag_assoc / num_act_mats_hor_dir;
      num_do_b_subbank = num_act_mats_hor_dir * num_do_b_mat;
    }
  }

  if ((g_ip->is_cache == false && is_main_mem == true) || (PAGE_MODE == 1 && is_dram))
  {
    if (num_act_mats_hor_dir * num_do_b_mat * Ndsam_lev_1 * Ndsam_lev_2 != (int)g_ip->page_sz_bits)
    {
      return;
    }
  }

  if (is_tag == false && g_ip->is_main_mem == true &&
      num_act_mats_hor_dir*num_do_b_mat*Ndsam_lev_1*Ndsam_lev_2 < ((int) g_ip->out_w * (int) g_ip->burst_len * (int) g_ip->data_assoc))
  {
    return;
  }

  if (num_act_mats_hor_dir > num_mats_h_dir) 
  {
    return;
  }

  if(!is_tag)
  {
    if(g_ip->fast_access == true)
    {
      num_di_b_mat = num_do_b_mat / g_ip->data_assoc;
    }
    else
    {
      num_di_b_mat = num_do_b_mat;
    }
  }
  else
  {
    num_di_b_mat = tagbits;
  }

  int num_di_b_subbank       = num_di_b_mat * num_act_mats_hor_dir;
  int num_addr_b_row_dec     = (fully_assoc == true) ? 0 : _log2(num_r_subarray);
  int number_subbanks        = num_mats / num_act_mats_hor_dir;
  number_subbanks_decode = _log2(number_subbanks);

  num_rw_ports = g_ip->num_rw_ports;
  num_rd_ports = g_ip->num_rd_ports;
  num_wr_ports = g_ip->num_wr_ports;
  num_se_rd_ports = g_ip->num_se_rd_ports;

  if (is_dram && is_main_mem)
  {
    number_addr_bits_mat = MAX((unsigned int) num_addr_b_row_dec,
     _log2(deg_bl_muxing) + _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2));
  }
  else
  {
    number_addr_bits_mat = num_addr_b_row_dec + _log2(deg_bl_muxing) +
      _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2);
  }

  if (is_tag)
  {
    num_di_b_bank_per_port = tagbits;
    num_do_b_bank_per_port = g_ip->data_assoc;
  }
  else
  {
    num_di_b_bank_per_port = g_ip->out_w + g_ip->data_assoc;
    num_do_b_bank_per_port = g_ip->out_w;
  }

  if ((!is_tag) && (g_ip->data_assoc > 1) && (!g_ip->fast_access))
  {
    number_way_select_signals_mat = g_ip->data_assoc;
  }    


  // add ECC adjustment to all data signals that traverse on H-trees.
  if (g_ip->add_ecc_b_ == true)
  {
    num_do_b_mat += (int) (ceil(num_do_b_mat / num_bits_per_ecc_b_));
    num_di_b_mat += (int) (ceil(num_di_b_mat / num_bits_per_ecc_b_));
    num_di_b_subbank += (int) (ceil(num_di_b_subbank / num_bits_per_ecc_b_));
    num_do_b_subbank += (int) (ceil(num_do_b_subbank / num_bits_per_ecc_b_));
  }

  is_valid = true;
}

