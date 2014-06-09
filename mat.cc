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

#include "mat.h"
#include <assert.h>


Mat::Mat(const DynamicParameter & dyn_p)
 :dp(dyn_p),
  power_subarray_out_drv(),
  delay_fa_decoder(0), delay_before_decoder(0), delay_bitline(0),
  delay_wl_reset(0), delay_bl_restore(0),
  subarray(dp, g_ip->fully_assoc),
  power_bitline(), per_bitline_read_energy(0),
  deg_bl_muxing(dp.deg_bl_muxing), 
  num_act_mats_hor_dir(dyn_p.num_act_mats_hor_dir),
  delay_writeback(0), cell(subarray.cell), is_dram(dyn_p.is_dram),
  num_mats(dp.num_mats),
  power_sa(), delay_sa(0),
  leak_power_sense_amps_closed_page_state(0),
  leak_power_sense_amps_open_page_state(0),
  delay_subarray_out_drv(0), 
  delay_comparator(0), power_comparator(), num_do_b_mat(dyn_p.num_do_b_mat),
  num_subarrays_per_mat(dp.num_subarrays/dp.num_mats),
  num_subarrays_per_row(dp.Ndwl/dp.num_mats_h_dir)
{
  assert(num_subarrays_per_mat <= 4);
  assert(num_subarrays_per_row <= 2);
  bool is_fa = (g_ip->fully_assoc) ? true : false;

  if (dp.use_inp_params == 1) {
    RWP = g_ip->num_rw_ports;
    ERP = g_ip->num_rd_ports;
    EWP = g_ip->num_wr_ports;
  }
  else {
    RWP = g_ip->num_rw_ports;
    ERP = g_ip->num_rd_ports;
    EWP = g_ip->num_wr_ports;
  }

  double number_sa_subarray        = subarray.num_cols / deg_bl_muxing;
  int    num_dec_signals           = (is_fa) ? 1 : subarray.num_rows;
  double C_ld_bit_mux_dec_out      = 0;
  double C_ld_sa_mux_lev_1_dec_out = 0;
  double C_ld_sa_mux_lev_2_dec_out = 0;
  double R_wire_wl_drv_out      = subarray.num_cols * cell.w * g_tp.wire_local.R_per_um;
  double R_wire_bit_mux_dec_out = num_subarrays_per_row * subarray.num_cols * g_tp.wire_inside_mat.R_per_um * cell.w;
  double R_wire_sa_mux_dec_out  = num_subarrays_per_row * subarray.num_cols * g_tp.wire_inside_mat.R_per_um * cell.w;

  if (deg_bl_muxing > 1)
  {
    C_ld_bit_mux_dec_out =
      (2 * num_subarrays_per_mat * subarray.num_cols / deg_bl_muxing)*gate_C(g_tp.w_nmos_b_mux, 0, is_dram) +  // 2 transistor per cell
      num_subarrays_per_row * subarray.num_cols*g_tp.wire_inside_mat.C_per_um*cell.get_w();
  }

  if (dp.Ndsam_lev_1 > 1)
  {
    C_ld_sa_mux_lev_1_dec_out =
      (num_subarrays_per_mat * number_sa_subarray / dp.Ndsam_lev_1)*gate_C(g_tp.w_nmos_sa_mux, 0, is_dram) +
      num_subarrays_per_row * subarray.num_cols*g_tp.wire_inside_mat.C_per_um*cell.get_w(); 
  }
  if (dp.Ndsam_lev_2 > 1)
  {
    C_ld_sa_mux_lev_2_dec_out =
      (num_subarrays_per_mat * number_sa_subarray / (dp.Ndsam_lev_1*dp.Ndsam_lev_2))*gate_C(g_tp.w_nmos_sa_mux, 0, is_dram) +
      num_subarrays_per_row * subarray.num_cols*g_tp.wire_inside_mat.C_per_um*cell.get_w();
  }

  if (num_subarrays_per_row >= 2)
  {
    // wire heads for both right and left side of a mat, so half the resistance
    R_wire_bit_mux_dec_out /= 2.0;
    R_wire_sa_mux_dec_out  /= 2.0;
  }

  row_dec = new Decoder(
      num_dec_signals,
      false,
      subarray.C_wl,
      R_wire_wl_drv_out,
      is_fa,
      is_dram,
      true,
      cell);
  if (is_fa && (!dp.is_tag))
  {
    row_dec->exist = true;
  }
  bit_mux_dec = new Decoder(
      deg_bl_muxing,
      false,
      C_ld_bit_mux_dec_out,
      R_wire_bit_mux_dec_out,
      is_fa,
      is_dram,
      false,
      cell);
  sa_mux_lev_1_dec = new Decoder(
      dp.deg_senseamp_muxing_non_associativity,
      dp.number_way_select_signals_mat ? true : false,
      C_ld_sa_mux_lev_1_dec_out,
      R_wire_sa_mux_dec_out,
      is_fa,
      is_dram,
      false,
      cell);
  sa_mux_lev_2_dec = new Decoder(
      dp.Ndsam_lev_2,
      false,
      C_ld_sa_mux_lev_2_dec_out,
      R_wire_sa_mux_dec_out,
      is_fa,
      is_dram,
      false,
      cell);

  PredecBlk * r_predec_blk1 = new PredecBlk(
      num_dec_signals,
      row_dec,
      num_subarrays_per_row * subarray.num_rows * g_tp.wire_inside_mat.C_per_um * cell.h,
      subarray.num_rows * g_tp.wire_inside_mat.R_per_um * cell.h,
      num_subarrays_per_mat,
      is_dram,
      true);
  PredecBlk * r_predec_blk2 = new PredecBlk(
      num_dec_signals,
      row_dec,
      num_subarrays_per_row * subarray.num_rows * g_tp.wire_inside_mat.C_per_um * cell.h,
      subarray.num_rows * g_tp.wire_inside_mat.R_per_um * cell.h,
      num_subarrays_per_mat,
      is_dram,
      false);
  PredecBlk * b_mux_predec_blk1 = new PredecBlk(deg_bl_muxing, bit_mux_dec, 0, 0, 1, is_dram, true);
  PredecBlk * b_mux_predec_blk2 = new PredecBlk(deg_bl_muxing, bit_mux_dec, 0, 0, 1, is_dram, false);
  PredecBlk * sa_mux_lev_1_predec_blk1 = new PredecBlk(dyn_p.deg_senseamp_muxing_non_associativity, sa_mux_lev_1_dec, 0, 0, 1, is_dram, true);
  PredecBlk * sa_mux_lev_1_predec_blk2 = new PredecBlk(dyn_p.deg_senseamp_muxing_non_associativity, sa_mux_lev_1_dec, 0, 0, 1, is_dram, false);
  PredecBlk * sa_mux_lev_2_predec_blk1 = new PredecBlk(dp.Ndsam_lev_2, sa_mux_lev_2_dec, 0, 0, 1, is_dram, true);
  PredecBlk * sa_mux_lev_2_predec_blk2 = new PredecBlk(dp.Ndsam_lev_2, sa_mux_lev_2_dec, 0, 0, 1, is_dram, false);
  dummy_way_sel_predec_blk1 = new PredecBlk(1, sa_mux_lev_1_dec, 0, 0, 0, is_dram, true);
  dummy_way_sel_predec_blk2 = new PredecBlk(1, sa_mux_lev_1_dec, 0, 0, 0, is_dram, false);

  PredecBlkDrv * r_predec_blk_drv1 = new PredecBlkDrv(0, r_predec_blk1, is_dram);
  PredecBlkDrv * r_predec_blk_drv2 = new PredecBlkDrv(0, r_predec_blk2, is_dram);
  PredecBlkDrv * b_mux_predec_blk_drv1 = new PredecBlkDrv(0, b_mux_predec_blk1, is_dram);
  PredecBlkDrv * b_mux_predec_blk_drv2 = new PredecBlkDrv(0, b_mux_predec_blk2, is_dram);
  PredecBlkDrv * sa_mux_lev_1_predec_blk_drv1 = new PredecBlkDrv(0, sa_mux_lev_1_predec_blk1, is_dram);
  PredecBlkDrv * sa_mux_lev_1_predec_blk_drv2 = new PredecBlkDrv(0, sa_mux_lev_1_predec_blk2, is_dram);
  PredecBlkDrv * sa_mux_lev_2_predec_blk_drv1 = new PredecBlkDrv(0, sa_mux_lev_2_predec_blk1, is_dram);
  PredecBlkDrv * sa_mux_lev_2_predec_blk_drv2 = new PredecBlkDrv(0, sa_mux_lev_2_predec_blk2, is_dram);
  way_sel_drv1 = new PredecBlkDrv(dyn_p.number_way_select_signals_mat, dummy_way_sel_predec_blk1, is_dram);
  dummy_way_sel_predec_blk_drv2 = new PredecBlkDrv(1, dummy_way_sel_predec_blk2, is_dram);

  r_predec            = new Predec(r_predec_blk_drv1, r_predec_blk_drv2);
  b_mux_predec        = new Predec(b_mux_predec_blk_drv1, b_mux_predec_blk_drv2);
  sa_mux_lev_1_predec = new Predec(sa_mux_lev_1_predec_blk_drv1, sa_mux_lev_1_predec_blk_drv2);
  sa_mux_lev_2_predec = new Predec(sa_mux_lev_2_predec_blk_drv1, sa_mux_lev_2_predec_blk_drv2);

  subarray_out_wire   = new Wire(g_ip->wt, subarray.area.h);
  bl_precharge_eq_drv = new Driver(
      subarray.num_cols * gate_C(2 * g_tp.w_pmos_bl_precharge + g_tp.w_pmos_bl_eq, 0, is_dram, false, false),
      subarray.num_cols * cell.w * g_tp.wire_outside_mat.C_per_um,
      subarray.num_cols * cell.w * g_tp.wire_outside_mat.R_per_um,
      is_dram);

  double area_row_decoder = row_dec->area.get_area() * subarray.num_rows * (RWP + ERP + EWP);
  double w_row_decoder    = area_row_decoder / subarray.area.get_h();

  double h_bit_mux_sense_amp_precharge_sa_mux_write_driver_write_mux = 
    compute_bit_mux_sa_precharge_sa_mux_wr_drv_wr_mux_h();

  double h_subarray_out_drv = subarray_out_wire->area.get_area() *
    (subarray.num_cols / (deg_bl_muxing * dp.Ndsam_lev_1 * dp.Ndsam_lev_2)) / subarray.area.get_w();

  if (!(is_fa && dp.is_tag))
  {
    h_subarray_out_drv *= (RWP + ERP);
  }
  else
  {
    h_subarray_out_drv  = 0;
  }

  double h_comparators                = 0.0;
  double w_row_predecode_output_wires = 0.0;
  double h_bit_mux_dec_out_wires      = 0.0;
  double h_senseamp_mux_dec_out_wires = 0.0;

  if ((!is_fa)&&(dp.is_tag))
  {
    //tagbits = (4 * num_cols_subarray / (deg_bl_muxing * dp.Ndsam_lev_1 * dp.Ndsam_lev_2)) / num_do_b_mat;
    h_comparators  = compute_comparators_height(dp.tagbits, dyn_p.num_do_b_mat, subarray.area.get_w());
    h_comparators *= (RWP + ERP);
  }

  if (!is_fa)
  {
    int branch_effort_predec_blk1_out = (1 << r_predec_blk2->number_input_addr_bits);
    int branch_effort_predec_blk2_out = (1 << r_predec_blk1->number_input_addr_bits);
    w_row_predecode_output_wires   = (branch_effort_predec_blk1_out + branch_effort_predec_blk2_out) *
      g_tp.wire_inside_mat.pitch * (RWP + ERP + EWP);
  }

  double h_non_cell_area = (num_subarrays_per_mat / num_subarrays_per_row) *
                           (h_bit_mux_sense_amp_precharge_sa_mux_write_driver_write_mux +
                            h_subarray_out_drv + h_comparators);
  double w_non_cell_area = MAX(w_row_predecode_output_wires, num_subarrays_per_row * w_row_decoder);

  if (deg_bl_muxing > 1)
  {
    h_bit_mux_dec_out_wires = deg_bl_muxing * g_tp.wire_inside_mat.pitch * (RWP + ERP);
  }
  if (dp.Ndsam_lev_1 > 1)
  {
    h_senseamp_mux_dec_out_wires =  dp.Ndsam_lev_1 * g_tp.wire_inside_mat.pitch * (RWP + ERP);
  }
  if (dp.Ndsam_lev_2 > 1)
  {
    h_senseamp_mux_dec_out_wires += dp.Ndsam_lev_2 * g_tp.wire_inside_mat.pitch * (RWP + ERP);
  }

  if (!g_ip->ver_htree_wires_over_array)
  {
    double h_addr_datain_wires = (dp.number_addr_bits_mat + dp.number_way_select_signals_mat +
                                  (dp.num_di_b_mat + dp.num_do_b_mat)/num_subarrays_per_row) *
                                 g_tp.wire_inside_mat.pitch * (RWP + ERP + EWP);
    //h_non_cell_area = 2 * h_bit_mux_sense_amp_precharge_sa_mux +
    //MAX(h_addr_datain_wires, 2 * h_subarray_out_drv);
    h_non_cell_area = (h_bit_mux_sense_amp_precharge_sa_mux_write_driver_write_mux + h_comparators +
                       h_subarray_out_drv) * (num_subarrays_per_mat / num_subarrays_per_row) +
                      h_addr_datain_wires +
                      h_bit_mux_dec_out_wires +
                      h_senseamp_mux_dec_out_wires;
  }

  // double area_rectangle_center_mat = h_non_cell_area * w_non_cell_area;
  double area_mat_center_circuitry = (r_predec_blk_drv1->area.get_area() + 
                                      b_mux_predec_blk_drv1->area.get_area() +
                                      sa_mux_lev_1_predec_blk_drv1->area.get_area() +
                                      sa_mux_lev_2_predec_blk_drv1->area.get_area() +
                                      way_sel_drv1->area.get_area() +
                                      r_predec_blk_drv2->area.get_area() +
                                      b_mux_predec_blk_drv2->area.get_area() +
                                      sa_mux_lev_1_predec_blk_drv2->area.get_area() + 
                                      sa_mux_lev_2_predec_blk_drv2->area.get_area() + 
                                      r_predec_blk1->area.get_area() +
                                      b_mux_predec_blk1->area.get_area() + 
                                      sa_mux_lev_1_predec_blk1->area.get_area() +
                                      sa_mux_lev_2_predec_blk1->area.get_area() +
                                      r_predec_blk2->area.get_area() +
                                      b_mux_predec_blk2->area.get_area() + 
                                      sa_mux_lev_1_predec_blk2->area.get_area() +
                                      sa_mux_lev_2_predec_blk2->area.get_area() +
                                      bit_mux_dec->area.get_area() +
                                      sa_mux_lev_1_dec->area.get_area() +
                                      sa_mux_lev_2_dec->area.get_area()) * (RWP + ERP + EWP);

  double area_efficiency_mat;

  if (!is_fa) 
  {
    area.h = (num_subarrays_per_mat / num_subarrays_per_row) * subarray.area.get_h() + h_non_cell_area;
    area.w = num_subarrays_per_row * subarray.area.get_w() + w_non_cell_area;
    area.w = (area.h*area.w + area_mat_center_circuitry) / area.h;
    area_efficiency_mat = subarray.area.get_area() * num_subarrays_per_mat * 100.0 / area.get_area();
  }
  else
  {
    area.h = (num_subarrays_per_mat / num_subarrays_per_row) * subarray.area.get_h() + h_non_cell_area;
    area.w = subarray.area.get_w() + w_non_cell_area;
    area.w = (area.h*area.w + area_mat_center_circuitry) / area.h;
    area_efficiency_mat = subarray.area.get_area() * num_subarrays_per_row * 100.0 / area.get_area();
  }
}



Mat::~Mat()
{
  delete row_dec;
  delete bit_mux_dec;
  delete sa_mux_lev_1_dec;
  delete sa_mux_lev_2_dec;

  delete r_predec->blk1;
  delete r_predec->blk2;
  delete b_mux_predec->blk1;
  delete b_mux_predec->blk2;
  delete sa_mux_lev_1_predec->blk1;
  delete sa_mux_lev_1_predec->blk2;
  delete sa_mux_lev_2_predec->blk1;
  delete sa_mux_lev_2_predec->blk2;
  delete dummy_way_sel_predec_blk1;
  delete dummy_way_sel_predec_blk2;

  delete r_predec->drv1;
  delete r_predec->drv2;
  delete b_mux_predec->drv1;
  delete b_mux_predec->drv2;
  delete sa_mux_lev_1_predec->drv1;
  delete sa_mux_lev_1_predec->drv2;
  delete sa_mux_lev_2_predec->drv1;
  delete sa_mux_lev_2_predec->drv2;
  delete way_sel_drv1;
  delete dummy_way_sel_predec_blk_drv2;

  delete r_predec;
  delete b_mux_predec;
  delete sa_mux_lev_1_predec;
  delete sa_mux_lev_2_predec;

  delete subarray_out_wire;
  delete bl_precharge_eq_drv;
}



double Mat::compute_delays(double inrisetime)
{
  bool is_fa = (g_ip->fully_assoc) ? true : false;

  // delay calculation for tags of fully associative cache
  if (is_fa && dp.is_tag)
  {
    delay_fa_tag(
        dp.tagbits,
        dp.Ndbl,
        is_dram);
  }
  else
  {
    bl_precharge_eq_drv->compute_delay(0);

    if (row_dec->exist == true)
    {
      int k = row_dec->num_gates - 1;
      double rd = tr_R_on(row_dec->w_dec_n[k], NCH, 1, is_dram, false, true);
      // TODO: this 4*cell.h number must be revisited
      double C_intrinsic = drain_C_(row_dec->w_dec_p[k], PCH, 1, 1, 4*cell.h, is_dram, false, true) +
                           drain_C_(row_dec->w_dec_n[k], NCH, 1, 1, 4*cell.h, is_dram, false, true);
      double C_ld = row_dec->C_ld_dec_out;
      double tf = rd * (C_intrinsic + C_ld) + row_dec->R_wire_dec_out * C_ld / 2;
      delay_wl_reset = horowitz(0, tf, 0.5, 0.5, RISE);
    }

    double R_bl_precharge = tr_R_on(g_tp.w_pmos_bl_precharge, PCH, 1, is_dram, false, false);
    double r_b_metal = cell.h * g_tp.wire_local.R_per_um;
    double R_bl = subarray.num_rows * r_b_metal;
    double C_bl = subarray.C_bl;

    if (is_dram)
    {
      delay_bl_restore = bl_precharge_eq_drv->delay + 2.3 * (R_bl_precharge * C_bl + R_bl * C_bl / 2);
    }
    else
    {
      delay_bl_restore = bl_precharge_eq_drv->delay + 
        log((g_tp.sram.Vbitpre - 0.1 * dp.V_b_sense) / (g_tp.sram.Vbitpre - dp.V_b_sense))*
        (R_bl_precharge * C_bl + R_bl * C_bl / 2);
    }
  }
  
  double outrisetime;
  double row_dec_outrisetime;

  outrisetime = r_predec->compute_delays(inrisetime);
  row_dec_outrisetime = row_dec->compute_delays(outrisetime);

  outrisetime = b_mux_predec->compute_delays(inrisetime);
  bit_mux_dec->compute_delays(outrisetime);

  outrisetime = sa_mux_lev_1_predec->compute_delays(inrisetime);
  sa_mux_lev_1_dec->compute_delays(outrisetime);

  outrisetime = sa_mux_lev_2_predec->compute_delays(inrisetime);
  sa_mux_lev_2_dec->compute_delays(outrisetime);

  outrisetime = compute_bitline_delay(row_dec_outrisetime);
  outrisetime = compute_sa_delay(outrisetime);
  outrisetime = compute_subarray_out_drv(outrisetime);
  subarray_out_wire->set_in_rise_time(outrisetime);
  outrisetime = subarray_out_wire->signal_rise_time();

  delay_subarray_out_drv_htree = delay_subarray_out_drv + subarray_out_wire->delay;

  if (dp.is_tag == true && g_ip->fully_assoc == false)
  {
    compute_comparator_delay(0);
  }

  if (row_dec->exist == false)
  {
    delay_wl_reset = MAX(r_predec->blk1->delay, r_predec->blk2->delay);
  }

  return outrisetime;
}



double Mat::compute_bit_mux_sa_precharge_sa_mux_wr_drv_wr_mux_h()
{

  double height = compute_tr_width_after_folding(g_tp.w_pmos_bl_precharge, cell.w / (2 *(RWP + ERP))) + 
    compute_tr_width_after_folding(g_tp.w_pmos_bl_eq, cell.w / (RWP + ERP));  // precharge circuitry

  if (deg_bl_muxing > 1)
  {
    height += compute_tr_width_after_folding(g_tp.w_nmos_b_mux, cell.w / (2 *(RWP + ERP)));  // col mux tr height
    // height += deg_bl_muxing * g_tp.wire_inside_mat.pitch * (RWP + ERP);  // bit mux dec out wires height
  }

  height += height_sense_amplifier(cell.w * deg_bl_muxing / (RWP + ERP));  // sense_amp_height

  if (dp.Ndsam_lev_1 > 1)
  {
    height += compute_tr_width_after_folding(
        g_tp.w_nmos_sa_mux, cell.w * deg_bl_muxing / (RWP + ERP));  // sense_amp_mux_height
    //height_senseamp_mux_decode_output_wires =  Ndsam * wire_inside_mat_pitch * (RWP + ERP);
  }

  if (dp.Ndsam_lev_2 > 1)
  {
    height += compute_tr_width_after_folding(
        g_tp.w_nmos_sa_mux, cell.w * deg_bl_muxing * dp.Ndsam_lev_1 / (RWP + ERP));  // sense_amp_mux_height
    //height_senseamp_mux_decode_output_wires =  Ndsam * wire_inside_mat_pitch * (RWP + ERP);

    // add height of inverter-buffers between the two levels (pass-transistors) of sense-amp mux
    height += 2 * compute_tr_width_after_folding(
        pmos_to_nmos_sz_ratio(is_dram) * g_tp.min_w_nmos_, cell.w * dp.Ndsam_lev_2 / (RWP + ERP));
    height += 2 * compute_tr_width_after_folding(g_tp.min_w_nmos_, cell.w * dp.Ndsam_lev_2 / (RWP + ERP));
  }

  // TODO: this should be uncommented...
  /*if (deg_bl_muxing * dp.Ndsam_lev_1 * dp.Ndsam_lev_2 > 1)
    {
  //height_write_mux_decode_output_wires = deg_bl_muxing * Ndsam * g_tp.wire_inside_mat.pitch * (RWP + EWP);
  double width_write_driver_write_mux  = width_write_driver_or_write_mux();
  double height_write_driver_write_mux = compute_tr_width_after_folding(2 * width_write_driver_write_mux, 
  cell.w * 
  // deg_bl_muxing *
  dp.Ndsam_lev_1 * dp.Ndsam_lev_2 / (RWP + EWP));
  height += height_write_driver_write_mux;  
  }*/

  return height;
}



void Mat::delay_fa_tag(
    int tagbits,
    int Ntbl,
    bool is_dram)
{
  double Tagdrive1, Tagdrive2, Tag1, Tag2, Tag3, outrisetime;
  double Ceq, Rwire, tf, nextinputtime, c_intrinsic, rd, Cwire, c_gate_load;

  double Wdecdrivep, Wdecdriven, Wfadriven, Wfadrivep, Wfadrive2n, Wfadrive2p, Wfadecdrive1n, Wfadecdrive1p, 
         Wfadecdrive2n, Wfadecdrive2p, Wfadecdriven, Wfadecdrivep, Wfaprechn, Wfaprechp,
         Wdummyn, Wdummyinvn, Wdummyinvp, Wfainvn, Wfainvp, Waddrnandn, Waddrnandp,
         Wfanandn, Wfanandp, Wfanorn, Wfanorp, Wdecnandn, Wdecnandp;

  double FACwordmetal, FACbitmetal, FARbitmetal, FARwordmetal, dynPower;
  int Htagbits;

  FACwordmetal = cell.w * g_tp.wire_local.C_per_um;
  FACbitmetal  = cell.h * g_tp.wire_local.C_per_um;
  FARwordmetal = cell.w * g_tp.wire_local.R_per_um;
  FARbitmetal  = cell.h * g_tp.wire_local.R_per_um;

  dynPower = 0.0;

  Wdecdrivep    =  450 * g_ip->F_sz_um;//this was 360 micron for the 0.8 micron process
  Wdecdriven    =  300 * g_ip->F_sz_um;//this was 240 micron for the 0.8 micron process
  Wfadriven     = 62.5 * g_ip->F_sz_um;//this was  50 micron for the 0.8 micron process
  Wfadrivep     =  125 * g_ip->F_sz_um;//this was 100 micron for the 0.8 micron process
  Wfadrive2n    =  250 * g_ip->F_sz_um;//this was 200 micron for the 0.8 micron process
  Wfadrive2p    =  500 * g_ip->F_sz_um;//this was 400 micron for the 0.8 micron process
  Wfadecdrive1n = 6.25 * g_ip->F_sz_um;//this was   5 micron for the 0.8 micron process
  Wfadecdrive1p = 12.5 * g_ip->F_sz_um;//this was  10 micron for the 0.8 micron process
  Wfadecdrive2n =   25 * g_ip->F_sz_um;//this was  20 micron for the 0.8 micron process
  Wfadecdrive2p =   50 * g_ip->F_sz_um;//this was  40 micron for the 0.8 micron process  
  Wfadecdriven  = 62.5 * g_ip->F_sz_um;//this was  50 micron for the 0.8 micron process
  Wfadecdrivep  =  125 * g_ip->F_sz_um;//this was 100 micron for the 0.8 micron process
  Wfaprechn     =  7.5 * g_ip->F_sz_um;//this was   6 micron for the 0.8 micron process
  Wfaprechp     = 12.5 * g_ip->F_sz_um;//this was  10 micron for the 0.8 micron process
  Wdummyn       = 12.5 * g_ip->F_sz_um;//this was  10 micron for the 0.8 micron process
  Wdummyinvn    =   75 * g_ip->F_sz_um;//this was  60 micron for the 0.8 micron process  
  Wdummyinvp    =  100 * g_ip->F_sz_um;//this was  80 micron for the 0.8 micron process
  Wfainvn       = 12.5 * g_ip->F_sz_um;//this was  10 micron for the 0.8 micron process  
  Wfainvp       =   25 * g_ip->F_sz_um;//this was  20 micron for the 0.8 micron process  
  Waddrnandn    = 62.5 * g_ip->F_sz_um;//this was  50 micron for the 0.8 micron process  
  Waddrnandp    = 62.5 * g_ip->F_sz_um;//this was  50 micron for the 0.8 micron process  
  Wfanandn      =   25 * g_ip->F_sz_um;//this was  20 micron for the 0.8 micron process  
  Wfanandp      = 37.5 * g_ip->F_sz_um;//this was  30 micron for the 0.8 micron process
  Wfanorn       = 6.25 * g_ip->F_sz_um;//this was   5 micron for the 0.8 micron process  
  Wfanorp       = 12.5 * g_ip->F_sz_um;//this was  10 micron for the 0.8 micron process
  Wdecnandn     = 12.5 * g_ip->F_sz_um;//this was  10 micron for the 0.8 micron process  
  Wdecnandp     = 37.5 * g_ip->F_sz_um;//this was  30 micron for the 0.8 micron process

  Htagbits = (int)(ceil ((double) (tagbits) / 2.0));

  //please refer to CACTI TR 2 and 3 for the implementation.
  /* First stage, From the driver(am and an) to the comparators in all the rows including the dummy row, 
     Assuming that comparators in both the normal matching line and the dummy matching line have the same sizing */ 
  nextinputtime = 0;
  Ceq = drain_C_(Wfadecdrive2p, PCH, 1, 1, g_tp.cell_h_def, is_dram) + 
    drain_C_(Wfadecdrive2n, NCH, 1, 1, g_tp.cell_h_def, is_dram) +
    gate_C(Wfadecdrivep + Wfadecdriven, 0, is_dram);
  tf  = Ceq * tr_R_on(Wfadecdrive2n, NCH, 1, is_dram);
  Tagdrive1     = horowitz(nextinputtime, tf, VSINV, VTHFA1, FALL);
  nextinputtime = Tagdrive1 / VTHFA1;
  dynPower     += Ceq * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * tagbits * Ntbl;

  rd = tr_R_on(Wfadecdrivep, PCH, 1, is_dram);
  c_intrinsic = drain_C_(Wfadecdrivep, PCH, 1, 1, g_tp.cell_h_def, is_dram) +
    drain_C_(Wfadecdriven, NCH, 1, 1, g_tp.cell_h_def, is_dram);
  c_gate_load = gate_C(Wdummyn, 0, is_dram) * 2 * (subarray.num_rows + 1);
  Cwire = FACbitmetal * 2 * (subarray.num_rows + 1);
  Rwire = FARbitmetal * (subarray.num_rows + 1);
  tf = rd * (c_intrinsic + Ceq) + Rwire * (Cwire / 2 + c_gate_load);
  Tagdrive2 = horowitz(nextinputtime, tf, VTHFA1, VTHFA2, RISE);
  nextinputtime = Tagdrive2 / (1 - VTHFA2);
  dynPower += (c_intrinsic + Cwire + c_gate_load) * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * tagbits * Ntbl;

  /* second stage, from the trasistors in the comparators(both normal row and dummy row) to the NAND gates that combins both half*/
  rd =  tr_R_on(Wdummyn, NCH, 2, is_dram);
  c_intrinsic = Htagbits*2*drain_C_(Wdummyn, NCH, 2, 1, g_tp.cell_h_def, is_dram) +
    drain_C_(Wfaprechp, PCH, 1, 1, g_tp.cell_h_def, is_dram);
  Cwire = FACwordmetal * Htagbits;
  Rwire = FARwordmetal * Htagbits;
  c_gate_load = gate_C(Waddrnandn + Waddrnandp, 0, is_dram);
  tf = rd * (c_intrinsic + Ceq) + Rwire * (Cwire / 2 + c_gate_load);
  Tag1 = horowitz(nextinputtime, tf, VTHFA2, VTHFA3, FALL);
  nextinputtime = Tag1 / VTHFA3;
  dynPower += Ceq * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * subarray.num_rows * Ntbl;

  /* third stage, from the NAND2 gates to the drivers in the dummy row */
  rd = tr_R_on(Waddrnandn, NCH, 2, is_dram);
  c_intrinsic = drain_C_(Waddrnandn, NCH, 2, 1, g_tp.cell_h_def, is_dram) +
    drain_C_(Waddrnandp, PCH, 1, 1, g_tp.cell_h_def, is_dram)*2;
  c_gate_load = gate_C(Wdummyinvn + Wdummyinvp, 0, is_dram);
  tf = rd * (c_intrinsic + c_gate_load);
  Tag2 = horowitz(nextinputtime, tf, VTHFA3, VTHFA4, RISE);
  nextinputtime = Tag2 / (1 - VTHFA4);
  dynPower += Ceq * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * subarray.num_rows * Ntbl;

  /* fourth stage, from the driver in dummy matchline to the NOR2 gate which drives the wordline of the data portion  */
  rd = tr_R_on(Wdummyinvn, NCH, 1, is_dram);
  c_intrinsic = drain_C_(Wdummyinvn, NCH, 1, 1, g_tp.cell_h_def, is_dram);
  Cwire = FACwordmetal * Htagbits +  FACbitmetal * subarray.num_rows;
  Rwire = FARwordmetal * Htagbits +  FARbitmetal * subarray.num_rows;
  c_gate_load = gate_C(Wfanorn + Wfanorp, 0, is_dram);
  tf = rd * (c_intrinsic + Cwire) + Rwire * (Cwire / 2 + c_gate_load);
  Tag3 = horowitz (nextinputtime, tf, VTHFA4, VTHFA5, FALL);
  outrisetime = Tag3 / VTHFA5;
  dynPower += (c_intrinsic + Cwire + c_gate_load) * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd;

  delay_fa_decoder = Tagdrive1 + Tagdrive2 + Tag1 + Tag2 + Tag3;
  power_fa_decoder.readOp.dynamic = dynPower;
}



double Mat::width_write_driver_or_write_mux()
{
  // calculate resistance of SRAM cell pull-up PMOS transistor
  double R_sram_cell_pull_up_tr  = tr_R_on(g_tp.sram.cell_pmos_w, NCH, 1, is_dram, true);
  double R_access_tr             = tr_R_on(g_tp.sram.cell_a_w,    NCH, 1, is_dram, true);
  double target_R_write_driver_and_mux = (2 * R_sram_cell_pull_up_tr - R_access_tr) / 2;
  double width_write_driver_nmos = R_to_w(target_R_write_driver_and_mux, NCH, is_dram);

  return width_write_driver_nmos;
}



double Mat::compute_comparators_height(
    int tagbits,
    int number_ways_in_mat,
    double subarray_mem_cell_area_width)
{
  double nand2_area = compute_gate_area(NAND, 2, 0, g_tp.w_comp_n, g_tp.cell_h_def); 
  double cumulative_area = nand2_area * number_ways_in_mat * tagbits / 4;
  return cumulative_area / subarray_mem_cell_area_width;
}


double Mat::compute_bitline_delay(double inrisetime)
{
  double V_b_pre, v_th_mem_cell, V_wl;
  double tstep;
  double dynRdEnergy = 0.0, dynWriteEnergy = 0.0;
  double R_cell_pull_down, R_cell_acc, r_dev;
  int deg_senseamp_muxing = dp.Ndsam_lev_1 * dp.Ndsam_lev_2;

  double R_b_metal = cell.h * g_tp.wire_local.R_per_um;
  double R_bl      = subarray.num_rows * R_b_metal;
  double C_bl      = subarray.C_bl;

  // TODO: no leakage for DRAMs? 
  double leak_power_cc_inverters_sram_cell = 0;
  double leak_power_acc_tr_RW_or_WR_port_sram_cell = 0;
  double leak_power_RD_port_sram_cell = 0;

  if (is_dram == true)
  {
    V_b_pre = g_tp.dram.Vbitpre;
    v_th_mem_cell = g_tp.dram_acc.Vth;
    V_wl = g_tp.vpp;
    //The access transistor is not folded. So we just need to specify a threshold value for the
    //folding width that is equal to or greater than Wmemcella. 
    R_cell_acc = tr_R_on(g_tp.dram.cell_a_w, NCH, 1, true, true);
    r_dev = g_tp.dram_cell_Vdd / g_tp.dram_cell_I_on + R_bl / 2;
  }
  else
  { //SRAM
    V_b_pre = g_tp.sram.Vbitpre;
    v_th_mem_cell = g_tp.sram_cell.Vth;
    V_wl = g_tp.sram_cell.Vdd;
    R_cell_pull_down = tr_R_on(g_tp.sram.cell_nmos_w, NCH, 1, false, true);
    R_cell_acc = tr_R_on(g_tp.sram.cell_a_w, NCH, 1, false, true);
    //Leakage current of an SRAM cell
    double Iport = cmos_Ileak(g_tp.sram.cell_a_w, 0,  false, true); 
    double Icell = cmos_Ileak(g_tp.sram.cell_nmos_w, g_tp.sram.cell_pmos_w, false, true);
    leak_power_cc_inverters_sram_cell = Icell * g_tp.sram_cell.Vdd;
    leak_power_acc_tr_RW_or_WR_port_sram_cell = Iport * g_tp.sram_cell.Vdd;
    leak_power_RD_port_sram_cell = 
      leak_power_acc_tr_RW_or_WR_port_sram_cell * NAND2_LEAK_STACK_FACTOR;
  }


  double C_drain_bit_mux = drain_C_(g_tp.w_nmos_b_mux, NCH, 1, 0, cell.w / (2 *(RWP + ERP + RWP)), is_dram);
  double R_bit_mux = tr_R_on(g_tp.w_nmos_b_mux, NCH, 1, is_dram);
  double C_drain_sense_amp_iso = drain_C_(g_tp.w_iso, PCH, 1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram);
  double R_sense_amp_iso = tr_R_on(g_tp.w_iso, PCH, 1, is_dram);
  double C_sense_amp_latch = gate_C(g_tp.w_sense_p + g_tp.w_sense_n, 0, is_dram) +
    drain_C_(g_tp.w_sense_n, NCH, 1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram) + 
    drain_C_(g_tp.w_sense_p, PCH, 1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram);
  double C_drain_sense_amp_mux = drain_C_(g_tp.w_nmos_sa_mux, NCH, 1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram);

  if (is_dram) 
  {
    double fraction = dp.V_b_sense / ((g_tp.dram_cell_Vdd/2) * g_tp.dram_cell_C /(g_tp.dram_cell_C + C_bl));
    tstep = 2.3 * fraction * r_dev * 
      (g_tp.dram_cell_C * (C_bl + 2*C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux)) /
      (g_tp.dram_cell_C + (C_bl + 2*C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux));
    delay_writeback = tstep;
    dynRdEnergy += (C_bl + 2*C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux) * 
      (g_tp.dram_cell_Vdd / 2) * g_tp.dram_cell_Vdd * subarray.num_cols * num_subarrays_per_mat;
    dynWriteEnergy += (C_bl + 2*C_drain_sense_amp_iso + C_sense_amp_latch) * 
      (g_tp.dram_cell_Vdd / 2) * g_tp.dram_cell_Vdd * subarray.num_cols * num_subarrays_per_mat * num_act_mats_hor_dir;
    per_bitline_read_energy = (C_bl + 2*C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux) *
      (g_tp.dram_cell_Vdd / 2) * g_tp.dram_cell_Vdd;
  }
  else
  {
    double tau;

    if (deg_bl_muxing > 1)
    {
      tau = (R_cell_pull_down + R_cell_acc) *
        (C_bl + 2*C_drain_bit_mux + 2*C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux) +
        R_bl * (C_bl/2 + 2*C_drain_bit_mux + 2*C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux) +
        R_bit_mux * (C_drain_bit_mux + 2*C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux) +
        R_sense_amp_iso * (C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux);
      dynRdEnergy += (C_bl + 2 * C_drain_bit_mux) * 2 * dp.V_b_sense * g_tp.sram_cell.Vdd *
        subarray.num_cols * num_subarrays_per_mat;
      dynRdEnergy += (2 * C_drain_sense_amp_iso + C_sense_amp_latch +  C_drain_sense_amp_mux) * 
        2 * dp.V_b_sense * g_tp.sram_cell.Vdd * (subarray.num_cols * num_subarrays_per_mat / deg_bl_muxing);
      dynWriteEnergy += ((subarray.num_cols * num_subarrays_per_mat / deg_bl_muxing) / deg_senseamp_muxing) *
          num_act_mats_hor_dir * (C_bl + 2*C_drain_bit_mux) * g_tp.sram_cell.Vdd * g_tp.sram_cell.Vdd;
    }
    else
    {
      tau = (R_cell_pull_down + R_cell_acc) * 
        (C_bl + C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux) + R_bl * C_bl / 2 +
        R_sense_amp_iso * (C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux);
      dynRdEnergy += (C_bl + 2 * C_drain_sense_amp_iso + C_sense_amp_latch + C_drain_sense_amp_mux) *
        2 * dp.V_b_sense * g_tp.sram_cell.Vdd * subarray.num_cols * num_subarrays_per_mat;
      dynWriteEnergy += (((subarray.num_cols * num_subarrays_per_mat / deg_bl_muxing) / deg_senseamp_muxing) *
          num_act_mats_hor_dir * C_bl) * g_tp.sram_cell.Vdd * g_tp.sram_cell.Vdd;

    }
    tstep = tau * log(V_b_pre / (V_b_pre - dp.V_b_sense));
    power_bitline.readOp.leakage =
      leak_power_cc_inverters_sram_cell + 
      leak_power_acc_tr_RW_or_WR_port_sram_cell + 
      leak_power_acc_tr_RW_or_WR_port_sram_cell * (RWP + EWP - 1) +
      leak_power_RD_port_sram_cell * ERP;
  }

  /* take input rise time into account */
  double m = V_wl / inrisetime;
  if (tstep <= (0.5 * (V_wl - v_th_mem_cell) / m))
  {
    delay_bitline = sqrt(2 * tstep * (V_wl - v_th_mem_cell)/ m);
  }
  else
  {
    delay_bitline = tstep + (V_wl - v_th_mem_cell) / (2 * m);
  }

  bool is_fa = (g_ip->fully_assoc) ? true : false;

  if (dp.is_tag == false || is_fa == false)
  {
    power_bitline.readOp.dynamic  = dynRdEnergy;
    power_bitline.writeOp.dynamic = dynWriteEnergy;
  }

  double outrisetime = 0; 
  return outrisetime;
}



double Mat::compute_sa_delay(double inrisetime)
{
  int num_sa_subarray = subarray.num_cols / deg_bl_muxing; //in a subarray

  //Bitline circuitry leakage. 
  double Iiso     = simplified_pmos_leakage(g_tp.w_iso, is_dram);
  double IsenseEn = simplified_nmos_leakage(g_tp.w_sense_en, is_dram);
  double IsenseN  = simplified_nmos_leakage(g_tp.w_sense_n, is_dram);
  double IsenseP  = simplified_pmos_leakage(g_tp.w_sense_p, is_dram);

  double lkgIdlePh  = IsenseEn;//+ 2*IoBufP;
  //double lkgWritePh = Iiso + IsenseEn;// + 2*IoBufP + 2*Ipch;
  double lkgReadPh  = Iiso + IsenseN + IsenseP;//+ IoBufN + IoBufP + 2*IsPch ;
  //double lkgRead = lkgReadPh * num_sa_subarray * 4 * num_act_mats_hor_dir + 
  //    lkgIdlePh * num_sa_subarray * 4 * (num_mats - num_act_mats_hor_dir);
  double lkgIdle = lkgIdlePh * num_sa_subarray * num_subarrays_per_mat;
  leak_power_sense_amps_closed_page_state = lkgIdlePh * g_tp.peri_global.Vdd * num_sa_subarray * num_subarrays_per_mat;
  leak_power_sense_amps_open_page_state   = lkgReadPh * g_tp.peri_global.Vdd * num_sa_subarray * num_subarrays_per_mat;

  // sense amplifier has to drive logic in "data out driver" and sense precharge load.
  // load seen by sense amp. New delay model for sense amp that is sensitive to both the output time 
  //constant as well as the magnitude of input differential voltage.
  double C_ld = gate_C(g_tp.w_sense_p + g_tp.w_sense_n, 0, is_dram) +
    drain_C_(g_tp.w_sense_n, NCH, 1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram) + 
    drain_C_(g_tp.w_sense_p, PCH, 1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram) +
    drain_C_(g_tp.w_iso,PCH,1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram) + 
    drain_C_(g_tp.w_nmos_sa_mux, NCH, 1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram);
  double tau = C_ld / g_tp.gm_sense_amp_latch;
  delay_sa = tau * log(g_tp.peri_global.Vdd / dp.V_b_sense);
  power_sa.readOp.dynamic = C_ld * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * num_sa_subarray *
                            num_subarrays_per_mat;// * num_act_mats_hor_dir;
  power_sa.readOp.leakage = lkgIdle * g_tp.peri_global.Vdd;

  double outrisetime = 0;  
  return outrisetime;
}



double Mat::compute_subarray_out_drv(double inrisetime)
{
  double C_ld, rd, tf, this_delay;
  double p_to_n_sz_r = pmos_to_nmos_sz_ratio(is_dram);

  // delay of signal through pass-transistor of first level of sense-amp mux to input of inverter-buffer.
  rd = tr_R_on(g_tp.w_nmos_sa_mux, NCH, 1, is_dram);
  C_ld = dp.Ndsam_lev_1 * drain_C_(g_tp.w_nmos_sa_mux, NCH, 1, 0, cell.w * deg_bl_muxing / (RWP + ERP), is_dram) +
    gate_C(g_tp.min_w_nmos_ + p_to_n_sz_r * g_tp.min_w_nmos_, 0.0, is_dram);
  tf = rd * C_ld;
  this_delay = horowitz(inrisetime, tf, 0.5, 0.5, RISE);
  delay_subarray_out_drv += this_delay;
  inrisetime = this_delay/(1.0 - 0.5);
  power_subarray_out_drv.readOp.dynamic += C_ld * 0.5 * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd;
  power_subarray_out_drv.readOp.leakage += 0;  // for now, let leakage of the pass transistor be 0

  // delay of signal through inverter-buffer to second level of sense-amp mux.
  // internal delay of buffer
  rd = tr_R_on(g_tp.min_w_nmos_, NCH, 1, is_dram);
  C_ld = drain_C_(g_tp.min_w_nmos_, NCH, 1, 1, g_tp.cell_h_def, is_dram) +
    drain_C_(p_to_n_sz_r * g_tp.min_w_nmos_, PCH, 1, 1, g_tp.cell_h_def, is_dram) +
    gate_C(g_tp.min_w_nmos_ + p_to_n_sz_r * g_tp.min_w_nmos_, 0.0, is_dram);
  tf = rd * C_ld;
  this_delay = horowitz(inrisetime, tf, 0.5, 0.5, RISE);
  delay_subarray_out_drv += this_delay;
  inrisetime = this_delay/(1.0 - 0.5);
  power_subarray_out_drv.readOp.dynamic += C_ld * 0.5 * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd;
  power_subarray_out_drv.readOp.leakage += cmos_Ileak(g_tp.min_w_nmos_, p_to_n_sz_r * g_tp.min_w_nmos_, is_dram) *
    0.5 * g_tp.peri_global.Vdd;

  // inverter driving drain of pass transistor of second level of sense-amp mux.
  rd = tr_R_on(g_tp.min_w_nmos_, NCH, 1, is_dram);
  C_ld = drain_C_(g_tp.min_w_nmos_, NCH, 1, 1, g_tp.cell_h_def, is_dram) +
    drain_C_(p_to_n_sz_r * g_tp.min_w_nmos_, PCH, 1, 1, g_tp.cell_h_def, is_dram) +
    drain_C_(g_tp.w_nmos_sa_mux, NCH, 1, 0, cell.w * deg_bl_muxing * dp.Ndsam_lev_1 / (RWP + ERP), is_dram);
  tf = rd * C_ld;
  this_delay = horowitz(inrisetime, tf, 0.5, 0.5, RISE);
  delay_subarray_out_drv += this_delay;
  inrisetime = this_delay/(1.0 - 0.5);
  power_subarray_out_drv.readOp.dynamic += C_ld * 0.5 * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd;
  power_subarray_out_drv.readOp.leakage += cmos_Ileak(g_tp.min_w_nmos_, p_to_n_sz_r * g_tp.min_w_nmos_, is_dram) *
    0.5 * g_tp.peri_global.Vdd;

  // delay of signal through pass-transistor to input of subarray output driver.
  rd = tr_R_on(g_tp.w_nmos_sa_mux, NCH, 1, is_dram);
  C_ld = dp.Ndsam_lev_2 * drain_C_(g_tp.w_nmos_sa_mux, NCH, 1, 0, cell.w * deg_bl_muxing * dp.Ndsam_lev_1 / (RWP + ERP), is_dram) +
    gate_C(subarray_out_wire->repeater_size * g_tp.min_w_nmos_ * (1 + p_to_n_sz_r), 0.0, is_dram);
  tf = rd * C_ld;
  this_delay = horowitz(inrisetime, tf, 0.5, 0.5, RISE);
  delay_subarray_out_drv += this_delay;
  inrisetime = this_delay/(1.0 - 0.5);
  power_subarray_out_drv.readOp.dynamic += C_ld * 0.5 * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd;
  power_subarray_out_drv.readOp.leakage += 0;  // for now, let leakage of the pass transistor be 0

  return inrisetime;
}



double Mat::compute_comparator_delay(double inrisetime)
{
  int A = g_ip->tag_assoc;

  int tagbits_ = dp.tagbits / 4; // Assuming there are 4 quarter comparators. input tagbits is already
  // a multiple of 4.

  /* First Inverter */
  double Ceq = gate_C(g_tp.w_comp_inv_n2+g_tp.w_comp_inv_p2, 0, is_dram) +
               drain_C_(g_tp.w_comp_inv_p1, PCH, 1, 1, g_tp.cell_h_def, is_dram) +
               drain_C_(g_tp.w_comp_inv_n1, NCH, 1, 1, g_tp.cell_h_def, is_dram);
  double Req = tr_R_on(g_tp.w_comp_inv_p1, PCH, 1, is_dram);
  double tf  = Req*Ceq;
  double st1del = horowitz(inrisetime,tf,VTHCOMPINV,VTHCOMPINV,FALL);
  double nextinputtime = st1del/VTHCOMPINV;
  power_comparator.readOp.dynamic += 0.5 * Ceq * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * 4 * A; 

  //For each degree of associativity 
  //there are 4 such quarter comparators
  double lkgCurrent = 0.5 * cmos_Ileak(g_tp.w_comp_inv_n1, g_tp.w_comp_inv_p1, is_dram) * 4 * A;

  /* Second Inverter */
  Ceq = gate_C(g_tp.w_comp_inv_n3+g_tp.w_comp_inv_p3, 0, is_dram) +
    drain_C_(g_tp.w_comp_inv_p2, PCH, 1, 1, g_tp.cell_h_def, is_dram) +
    drain_C_(g_tp.w_comp_inv_n2, NCH, 1, 1, g_tp.cell_h_def, is_dram);
  Req = tr_R_on(g_tp.w_comp_inv_n2, NCH, 1, is_dram);
  tf = Req*Ceq;
  double st2del = horowitz(nextinputtime,tf,VTHCOMPINV,VTHCOMPINV,RISE);
  nextinputtime = st2del/(1.0-VTHCOMPINV);
  power_comparator.readOp.dynamic += 0.5 * Ceq * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * 4 * A;
  lkgCurrent += 0.5 * cmos_Ileak(g_tp.w_comp_inv_n2, g_tp.w_comp_inv_p2, is_dram) * 4 * A;

  /* Third Inverter */
  Ceq = gate_C(g_tp.w_eval_inv_n+g_tp.w_eval_inv_p, 0, is_dram) +
    drain_C_(g_tp.w_comp_inv_p3, PCH, 1, 1, g_tp.cell_h_def, is_dram) +
    drain_C_(g_tp.w_comp_inv_n3, NCH, 1, 1, g_tp.cell_h_def, is_dram);
  Req = tr_R_on(g_tp.w_comp_inv_p3, PCH, 1, is_dram);
  tf = Req*Ceq;
  double st3del = horowitz(nextinputtime,tf,VTHCOMPINV,VTHEVALINV,FALL);
  nextinputtime = st3del/(VTHEVALINV);
  power_comparator.readOp.dynamic += 0.5 * Ceq * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * 4 * A;
  lkgCurrent += 0.5 * cmos_Ileak(g_tp.w_comp_inv_n3, g_tp.w_comp_inv_p3, is_dram) * 4 * A;

  /* Final Inverter (virtual ground driver) discharging compare part */
  double r1 = tr_R_on(g_tp.w_comp_n,NCH,2, is_dram);
  double r2 = tr_R_on(g_tp.w_eval_inv_n,NCH,1, is_dram); /* was switch */
  double c2 = (tagbits_)*(drain_C_(g_tp.w_comp_n,NCH,1, 1, g_tp.cell_h_def, is_dram) +
                   drain_C_(g_tp.w_comp_n,NCH,2, 1, g_tp.cell_h_def, is_dram)) +
       drain_C_(g_tp.w_eval_inv_p,PCH,1, 1, g_tp.cell_h_def, is_dram) +
       drain_C_(g_tp.w_eval_inv_n,NCH,1, 1, g_tp.cell_h_def, is_dram);
  double c1 = (tagbits_)*(drain_C_(g_tp.w_comp_n,NCH,1, 1, g_tp.cell_h_def, is_dram) +
                          drain_C_(g_tp.w_comp_n,NCH,2, 1, g_tp.cell_h_def, is_dram)) +
    drain_C_(g_tp.w_comp_p,PCH,1, 1, g_tp.cell_h_def, is_dram) +
    gate_C(WmuxdrvNANDn+WmuxdrvNANDp,0, is_dram);
  power_comparator.readOp.dynamic += 0.5 * c2 * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd * 4 * A;
  power_comparator.readOp.dynamic += c1 * g_tp.peri_global.Vdd * g_tp.peri_global.Vdd *  (A - 1);
  lkgCurrent += 0.5 * cmos_Ileak(g_tp.w_eval_inv_n,g_tp.w_eval_inv_p, is_dram) * 4 * A;
  lkgCurrent += 0.2 * 0.5 * cmos_Ileak(g_tp.w_comp_n, g_tp.w_comp_p, is_dram)  * 4 * A;  // stack factor of 0.2

  /* time to go to threshold of mux driver */
  double tstep = (r2*c2+(r1+r2)*c1)*log(1.0/VTHMUXNAND);
  /* take into account non-zero input rise time */
  double m = g_tp.peri_global.Vdd/nextinputtime;
  double Tcomparatorni;

  if((tstep) <= (0.5*(g_tp.peri_global.Vdd-g_tp.peri_global.Vth)/m)) 
  {
    double a = m;
    double b = 2*((g_tp.peri_global.Vdd*VTHEVALINV)-g_tp.peri_global.Vth);
    double c = -2*(tstep)*(g_tp.peri_global.Vdd-g_tp.peri_global.Vth)+1/m*((g_tp.peri_global.Vdd*VTHEVALINV)-g_tp.peri_global.Vth)*((g_tp.peri_global.Vdd*VTHEVALINV)-g_tp.peri_global.Vth);
    Tcomparatorni = (-b+sqrt(b*b-4*a*c))/(2*a);
  }
  else
  {
    Tcomparatorni = (tstep) + (g_tp.peri_global.Vdd+g_tp.peri_global.Vth)/(2*m) - (g_tp.peri_global.Vdd*VTHEVALINV)/m;
  }
  delay_comparator = Tcomparatorni+st1del+st2del+st3del;
  power_comparator.readOp.leakage = lkgCurrent * g_tp.peri_global.Vdd;

  return Tcomparatorni / (1.0 - VTHMUXNAND);;
}



void Mat::compute_power_energy()
{
  bool is_fa = (g_ip->fully_assoc) ? true : false;

  // add energy consumed in predecoder drivers
  power.readOp.dynamic += r_predec->power.readOp.dynamic +
                          b_mux_predec->power.readOp.dynamic +
                          sa_mux_lev_1_predec->power.readOp.dynamic +
                          sa_mux_lev_2_predec->power.readOp.dynamic;

  // add energy consumed in decoders
  power_bl_precharge_eq_drv.readOp.dynamic = bl_precharge_eq_drv->power.readOp.dynamic;
  power_row_decoders.readOp.dynamic        = row_dec->power.readOp.dynamic;

  if (is_fa == false)
  {
    power_bl_precharge_eq_drv.readOp.dynamic *= num_subarrays_per_mat;
    power_row_decoders.readOp.dynamic        *= num_subarrays_per_mat;
  }
  else if (dp.is_tag == true)
  {
    power_row_decoders.readOp.dynamic         = power_fa_decoder.readOp.dynamic;
  }

  if (is_fa == false || dp.is_tag == false)
  {
    power_subarray_out_drv.readOp.dynamic = 
      (power_subarray_out_drv.readOp.dynamic + subarray_out_wire->power.readOp.dynamic) * num_do_b_mat;
    power.readOp.dynamic += power_bl_precharge_eq_drv.readOp.dynamic +
                            power_sa.readOp.dynamic + 
                            power_bitline.readOp.dynamic +
                            power_subarray_out_drv.readOp.dynamic;
  }

  power.readOp.dynamic += power_row_decoders.readOp.dynamic +
                          bit_mux_dec->power.readOp.dynamic +
                          sa_mux_lev_1_dec->power.readOp.dynamic +
                          sa_mux_lev_2_dec->power.readOp.dynamic +
                          power_comparator.readOp.dynamic;


  // calculate leakage power
  if (is_fa == false || dp.is_tag == false)
  { // if fully associative cache and tag array, don't add the following components of leakage power
    power_bitline.readOp.leakage            *= subarray.num_rows * subarray.num_cols * num_subarrays_per_mat;
    power_bl_precharge_eq_drv.readOp.leakage = bl_precharge_eq_drv->power.readOp.leakage * num_subarrays_per_mat;
    power_sa.readOp.leakage                 *= (RWP + ERP);

    int number_sa_subarray             = subarray.num_cols / deg_bl_muxing;
    int number_output_drivers_subarray = number_sa_subarray / (dp.Ndsam_lev_1 * dp.Ndsam_lev_2);
    power_subarray_out_drv.readOp.leakage = 
      (power_subarray_out_drv.readOp.leakage + subarray_out_wire->power.readOp.leakage) *
      number_output_drivers_subarray * num_subarrays_per_mat * (RWP + ERP);

    power.readOp.leakage += power_bitline.readOp.leakage +
                            power_bl_precharge_eq_drv.readOp.leakage +
                            power_sa.readOp.leakage +
                            power_subarray_out_drv.readOp.leakage;
  }

  power_comparator.readOp.leakage *= num_do_b_mat * (RWP + ERP);
  power.readOp.leakage += power_comparator.readOp.leakage;

  // leakage power
  power_row_decoders.readOp.leakage = row_dec->power.readOp.leakage * subarray.num_rows * num_subarrays_per_mat;
  power_bit_mux_decoders.readOp.leakage      = bit_mux_dec->power.readOp.leakage * deg_bl_muxing;
  power_sa_mux_lev_1_decoders.readOp.leakage = sa_mux_lev_1_dec->power.readOp.leakage * dp.Ndsam_lev_1;
  power_sa_mux_lev_2_decoders.readOp.leakage = sa_mux_lev_2_dec->power.readOp.leakage * dp.Ndsam_lev_2;

  power.readOp.leakage += r_predec->power.readOp.leakage +
                          b_mux_predec->power.readOp.leakage +
                          sa_mux_lev_1_predec->power.readOp.leakage +
                          sa_mux_lev_2_predec->power.readOp.leakage +
                          power_row_decoders.readOp.leakage + 
                          power_bit_mux_decoders.readOp.leakage +
                          power_sa_mux_lev_1_decoders.readOp.leakage +
                          power_sa_mux_lev_2_decoders.readOp.leakage;
}

