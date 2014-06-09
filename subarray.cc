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
#include <math.h>

#include "subarray.h"


Subarray::Subarray(const DynamicParameter & dp_, bool is_fa_):
  dp(dp_), num_rows(dp.num_r_subarray), num_cols(dp.num_c_subarray), cell(dp.cell), is_fa(is_fa_)
{
  if (is_fa == false)
  {
    uint32_t ram_num_cells_wl_stitching = 
      (dp.ram_cell_tech_type == lp_dram)   ? dram_num_cells_wl_stitching_ :
      (dp.ram_cell_tech_type == comm_dram) ? comm_dram_num_cells_wl_stitching_ : sram_num_cells_wl_stitching_;

    area.h = cell.h * num_rows;
    area.w = cell.w * num_cols +
      ceil(num_cols / ram_num_cells_wl_stitching) * g_tp.ram_wl_stitching_overhead_ +  // stitching overhead
      (g_ip->add_ecc_b_ ? cell.w * ceil(num_cols / num_bits_per_ecc_b_) : 0);         // ECC overhead
  }
  else  // fully associative cache
  {
    // it is assumed that the transistor size of a CAM cell is linearly scaled from 0.8um process
    double CAM2x2_h_1p = 61 * g_ip->F_sz_um;  // 48.8um in 0.8um process
    double CAM2x2_w_1p = 56 * g_ip->F_sz_um;  // 44.8um in 0.8um process
    double fa_h_incr_per_first_rw_or_wr_port_ = 20 * g_ip->F_sz_um;
    double fa_h_incr_per_later_rw_or_wr_port_ = 20 * g_ip->F_sz_um;
    double fa_h_incr_per_first_rd_port_       = 15 * g_ip->F_sz_um;
    double fa_h_incr_per_later_rd_port_       = 15 * g_ip->F_sz_um;
    double fa_w_incr_per_first_rw_or_wr_port_ = 20 * g_ip->F_sz_um;
    double fa_w_incr_per_later_rw_or_wr_port_ = 12 * g_ip->F_sz_um;
    double fa_w_incr_per_first_rd_port_       = 15 * g_ip->F_sz_um;
    double fa_w_incr_per_later_rd_port_       = 12 * g_ip->F_sz_um;
    double w_contact_                         = 2 * g_ip->F_sz_um;
    double overhead_w;
    double overhead_h;

    int h_tag_bits       = (dp.tagbits + 1)/2;
    uint32_t  RWP;
    uint32_t  ERP;
    uint32_t  EWP;
    if (dp.use_inp_params == 1)
    {
      RWP = dp.num_rw_ports;
      ERP = dp.num_rd_ports;
      EWP = dp.num_wr_ports;
    }
    else
    {
      RWP = g_ip->num_rw_ports;
      ERP = g_ip->num_rd_ports;
      EWP = g_ip->num_wr_ports;
    }

    if (RWP == 1 && ERP == 0 && EWP == 0)
    {
      overhead_w = 0;
      overhead_h = 0;
    }
    else if (RWP == 1 && ERP == 1 && EWP == 0)
    {
      overhead_w = fa_w_incr_per_first_rd_port_;
      overhead_h = fa_h_incr_per_first_rd_port_;
    }
    else if (RWP == 1 && ERP == 0 && EWP == 1)
    {
      overhead_w = fa_w_incr_per_first_rw_or_wr_port_;
      overhead_h = fa_h_incr_per_first_rw_or_wr_port_;
    }
    else if (RWP + EWP >= 2)
    {
      overhead_w = fa_w_incr_per_first_rw_or_wr_port_ +
        (RWP + EWP - 2)*fa_w_incr_per_later_rw_or_wr_port_ +
        ERP*fa_w_incr_per_later_rd_port_;
      overhead_h = fa_h_incr_per_first_rw_or_wr_port_ +
        (RWP + EWP - 2)*fa_h_incr_per_later_rw_or_wr_port_ +
        ERP*fa_h_incr_per_later_rd_port_;
    }
    else if (RWP == 0 && EWP == 0)
    {
      overhead_w = fa_w_incr_per_first_rd_port_ + (ERP - 1)*fa_w_incr_per_later_rd_port_;
      overhead_h = fa_h_incr_per_first_rd_port_ + (ERP - 1)*fa_h_incr_per_later_rd_port_;
    }
    else if (RWP == 0 && EWP == 1)
    {
      overhead_w = ERP * fa_w_incr_per_later_rd_port_;
      overhead_h = ERP * fa_h_incr_per_later_rd_port_;
    }
    else if (RWP == 1 && EWP == 0)
    {
      overhead_w = ERP * fa_w_incr_per_later_rd_port_;
      overhead_h = ERP * fa_h_incr_per_later_rd_port_;
    }
    else
    {
      cout << "unsupported combination of RWP, ERP, and EWP" << endl;
      exit(1);
    }

    area.h = (CAM2x2_h_1p + 2*overhead_h) * ((num_rows + 1)/2);
    area.w = 2*(h_tag_bits * ((CAM2x2_w_1p + 2*overhead_w) - w_contact_)) +
      floor(h_tag_bits / sram_num_cells_wl_stitching_)*g_tp.ram_wl_stitching_overhead_;
    // following line is commented out in the latest version of CACTI 5
    //+ (fa_row_NAND_w_ + fa_row_NOR_inv_w_)*(RWP + ERP + EWP);

    cell.h = (CAM2x2_h_1p + 2*overhead_h) / 2.0;
    cell.w = (CAM2x2_w_1p + 2*overhead_w - w_contact_) / 2.0;
  }

  compute_C();
}



Subarray::~Subarray()
{
}



double Subarray::get_total_cell_area()
{
  return cell.get_area() * num_rows * num_cols;
}



void Subarray::compute_C()
{
  double c_w_metal = cell.w * g_tp.wire_local.C_per_um;
//  double r_w_metal = cell.w * g_tp.wire_local.R_per_um;
  double C_b_metal = cell.h * g_tp.wire_local.C_per_um;
  double C_b_row_drain_C;

  if (dp.is_dram)
  {
    C_wl = (gate_C_pass(g_tp.dram.cell_a_w, g_tp.dram.b_w, true, true) + c_w_metal) * num_cols;

    if (dp.ram_cell_tech_type == comm_dram)
    {
      C_bl = num_rows * C_b_metal;
    }
    else
    {
      C_b_row_drain_C = drain_C_(g_tp.dram.cell_a_w, NCH, 1, 0, cell.w, true, true) / 2.0;  // due to shared contact
      C_bl = num_rows * (C_b_row_drain_C + C_b_metal);
    }
  }
  else
  {
    C_wl = (gate_C_pass(g_tp.sram.cell_a_w, (g_tp.sram.b_w-2*g_tp.sram.cell_a_w)/2.0, false, true)*2 +
        c_w_metal) * num_cols;
    C_b_row_drain_C = drain_C_(g_tp.sram.cell_a_w, NCH, 1, 0, cell.w, false, true) / 2.0;  // due to shared contact
    C_bl = num_rows * (C_b_row_drain_C + C_b_metal);
  }
}

