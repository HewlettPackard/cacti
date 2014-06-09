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
#include <iostream>
#include <assert.h>

uint32_t _log2(uint64_t num)
{
  uint32_t log2 = 0;

  if (num == 0)
  {
    std::cerr << "log0?" << std::endl;
    exit(1);
  }

  while (num > 1)
  {
    num = (num >> 1);
    log2++;
  }

  return log2;
}


bool is_pow2(int64_t val)
{
  if (val <= 0)
  {
    return false;
  }
  else if (val == 1)
  {
    return true;
  }
  else
  {
    return (_log2(val) != _log2(val-1));
  }
}


int powers (int base, int n)
{
  int i, p;

  p = 1;
  for (i = 1; i <= n; ++i)
    p *= base;
  return p;
}

/*----------------------------------------------------------------------*/

double logtwo (double x)
{
  assert(x > 0);
  return ((double) (log (x) / log (2.0)));
}

/*----------------------------------------------------------------------*/


double gate_C(
    double width,
    double wirelength,
    bool   _is_dram,
    bool   _is_cell,
    bool   _is_wl_tr)
{
  const TechnologyParameter::DeviceType * dt;

  if (_is_dram && _is_cell)
  { 
    dt = &g_tp.dram_acc;   //DRAM cell access transistor
  }
  else if (_is_dram && _is_wl_tr)
  {
    dt = &g_tp.dram_wl;    //DRAM wordline transistor
  }
  else if (!_is_dram && _is_cell)
  {
    dt = &g_tp.sram_cell;  // SRAM cell access transistor
  }
  else
  {
    dt = &g_tp.peri_global;
  }

  return (dt->C_g_ideal + dt->C_overlap + 3*dt->C_fringe)*width + dt->l_phy*Cpolywire;
}


// returns gate capacitance in Farads
// actually this function is the same as gate_C() now
double gate_C_pass(
    double width,       // gate width in um (length is Lphy_periph_global)
    double wirelength,  // poly wire length going to gate in lambda
    bool   _is_dram,
    bool   _is_cell,
    bool   _is_wl_tr)
{
  // v5.0
  const TechnologyParameter::DeviceType * dt;

  if ((_is_dram) && (_is_cell))
  { 
    dt = &g_tp.dram_acc;   //DRAM cell access transistor
  }
  else if ((_is_dram) && (_is_wl_tr))
  {
    dt = &g_tp.dram_wl;    //DRAM wordline transistor
  }
  else if ((!_is_dram) && _is_cell)
  {
    dt = &g_tp.sram_cell;  // SRAM cell access transistor
  }
  else
  {
    dt = &g_tp.peri_global;
  }

  return (dt->C_g_ideal + dt->C_overlap + 3*dt->C_fringe)*width + dt->l_phy*Cpolywire;
}



double drain_C_(
    double width,
    int nchannel,
    int stack,
    int next_arg_thresh_folding_width_or_height_cell,
    double fold_dimension,
    bool _is_dram,
    bool _is_cell,
    bool _is_wl_tr)
{
  double w_folded_tr;
  const  TechnologyParameter::DeviceType * dt;

  if ((_is_dram) && (_is_cell))
  { 
    dt = &g_tp.dram_acc;   // DRAM cell access transistor
  }
  else if ((_is_dram) && (_is_wl_tr))
  {
    dt = &g_tp.dram_wl;    // DRAM wordline transistor
  }
  else if ((!_is_dram) && _is_cell)
  {
    dt = &g_tp.sram_cell;  // SRAM cell access transistor
  }
  else
  {
    dt = &g_tp.peri_global;
  }

  double c_junc_area = dt->C_junc;
  double c_junc_sidewall = dt->C_junc_sidewall;
  double c_fringe    = 2*dt->C_fringe;
  double c_overlap   = 2*dt->C_overlap;
  double drain_C_metal_connecting_folded_tr = 0;

  // determine the width of the transistor after folding (if it is getting folded)
  if (next_arg_thresh_folding_width_or_height_cell == 0)
  { // interpret fold_dimension as the the folding width threshold 
    // i.e. the value of transistor width above which the transistor gets folded
    w_folded_tr = fold_dimension;
  } 
  else
  { // interpret fold_dimension as the height of the cell that this transistor is part of. 
    double h_tr_region  = fold_dimension - 2 * g_tp.HPOWERRAIL;
    // TODO : w_folded_tr must come from Component::compute_gate_area()
    double ratio_p_to_n = 2.0 / (2.0 + 1.0);
    if (nchannel)
    {
      w_folded_tr = (1 - ratio_p_to_n) * (h_tr_region - g_tp.MIN_GAP_BET_P_AND_N_DIFFS);
    }
    else
    {
      w_folded_tr = ratio_p_to_n * (h_tr_region - g_tp.MIN_GAP_BET_P_AND_N_DIFFS);
    }
  }
  int num_folded_tr = (int) (ceil(width / w_folded_tr));

  if (num_folded_tr < 2)
  {
    w_folded_tr = width;
  }

  double total_drain_w = (g_tp.w_poly_contact + 2 * g_tp.spacing_poly_to_contact) +  // only for drain
                         (stack - 1) * g_tp.spacing_poly_to_poly;
  double drain_h_for_sidewall = w_folded_tr;
  double total_drain_height_for_cap_wrt_gate = w_folded_tr + 2 * w_folded_tr * (stack - 1);
  if (num_folded_tr > 1)
  {
    total_drain_w += (num_folded_tr - 2) * (g_tp.w_poly_contact + 2 * g_tp.spacing_poly_to_contact) +
                     (num_folded_tr - 1) * ((stack - 1) * g_tp.spacing_poly_to_poly);

    if (num_folded_tr%2 == 0)
    {
      drain_h_for_sidewall = 0;
    }
    total_drain_height_for_cap_wrt_gate *= num_folded_tr;
    drain_C_metal_connecting_folded_tr   = g_tp.wire_local.C_per_um * total_drain_w;
  }

  double drain_C_area     = c_junc_area * total_drain_w * w_folded_tr;
  double drain_C_sidewall = c_junc_sidewall * (drain_h_for_sidewall + 2 * total_drain_w);
  double drain_C_wrt_gate = (c_fringe + c_overlap) * total_drain_height_for_cap_wrt_gate;

  return (drain_C_area + drain_C_sidewall + drain_C_wrt_gate + drain_C_metal_connecting_folded_tr);
}


double tr_R_on(
    double width,
    int nchannel,
    int stack,
    bool _is_dram,
    bool _is_cell,
    bool _is_wl_tr)
{
  const TechnologyParameter::DeviceType * dt;

  if ((_is_dram) && (_is_cell))
  { 
    dt = &g_tp.dram_acc;   //DRAM cell access transistor
  }
  else if ((_is_dram) && (_is_wl_tr))
  {
    dt = &g_tp.dram_wl;    //DRAM wordline transistor
  }
  else if ((!_is_dram) && _is_cell)
  {
    dt = &g_tp.sram_cell;  // SRAM cell access transistor
  }
  else
  {
    dt = &g_tp.peri_global;
  }

  double restrans = (nchannel) ? dt->R_nch_on : dt->R_pch_on;
  return (stack * restrans / width);
}


/* This routine operates in reverse: given a resistance, it finds
 * the transistor width that would have this R.  It is used in the
 * data wordline to estimate the wordline driver size. */

// returns width in um
double R_to_w(
    double res,
    int   nchannel,
    bool _is_dram,
    bool _is_cell,
    bool _is_wl_tr)
{
  const TechnologyParameter::DeviceType * dt;

  if ((_is_dram) && (_is_cell))
  { 
    dt = &g_tp.dram_acc;   //DRAM cell access transistor
  }
  else if ((_is_dram) && (_is_wl_tr))
  {
    dt = &g_tp.dram_wl;    //DRAM wordline transistor
  }
  else if ((!_is_dram) && (_is_cell))
  {
    dt = &g_tp.sram_cell;  // SRAM cell access transistor
  }
  else
  {
    dt = &g_tp.peri_global;
  }

  double restrans = (nchannel) ? dt->R_nch_on : dt->R_pch_on;
  return (restrans / res);
}


double pmos_to_nmos_sz_ratio(
    bool _is_dram,
    bool _is_wl_tr)
{
  double p_to_n_sizing_ratio;
  if ((_is_dram) && (_is_wl_tr))
  { //DRAM wordline transistor
    p_to_n_sizing_ratio = g_tp.dram_wl.n_to_p_eff_curr_drv_ratio;
  }
  else
  { //DRAM or SRAM all other transistors
    p_to_n_sizing_ratio = g_tp.peri_global.n_to_p_eff_curr_drv_ratio;
  }
  return p_to_n_sizing_ratio;
}


// "Timing Models for MOS Circuits" by Mark Horowitz, 1984
double horowitz(
    double inputramptime, // input rise time
    double tf,            // time constant of gate
    double vs1,           // threshold voltage
    double vs2,           // threshold voltage
    int    rise)          // whether input rises or fall
{
  if (inputramptime == 0 && vs1 == vs2)
  {
    return tf * (vs1 < 1 ? -log(vs1) : log(vs1));
  }
  double a, b, td;

  a = inputramptime / tf;
  if (rise == RISE)
  {
    b = 0.5;
    td = tf * sqrt(log(vs1)*log(vs1) + 2*a*b*(1.0 - vs1)) + tf*(log(vs1) - log(vs2));
  }
  else
  {
    b = 0.4;
    td = tf * sqrt(log(1.0 - vs1)*log(1.0 - vs1) + 2*a*b*(vs1)) + tf*(log(1.0 - vs1) - log(1.0 - vs2));
  }
  return (td);
}

double cmos_Ileak(
    double nWidth,
    double pWidth,
    bool _is_dram,
    bool _is_cell,
    bool _is_wl_tr)
{
  TechnologyParameter::DeviceType * dt;

  if ((!_is_dram)&&(_is_cell))
  { //SRAM cell access transistor
    dt = &(g_tp.sram_cell);
  }
  else if ((_is_dram)&&(_is_wl_tr))
  { //DRAM wordline transistor
    dt = &(g_tp.dram_wl);
  }
  else
  { //DRAM or SRAM all other transistors
    dt = &(g_tp.peri_global);
  }
  return nWidth*dt->I_off_n + pWidth*dt->I_off_p;
}


double simplified_nmos_leakage(
    double nwidth,
    bool _is_dram,
    bool _is_cell,
    bool _is_wl_tr)
{
  TechnologyParameter::DeviceType * dt;

  if ((!_is_dram)&&(_is_cell))
  { //SRAM cell access transistor
    dt = &(g_tp.sram_cell);
  }
  else if ((_is_dram)&&(_is_wl_tr))
  { //DRAM wordline transistor
    dt = &(g_tp.dram_wl);
  }
  else
  { //DRAM or SRAM all other transistors
    dt = &(g_tp.peri_global);
  }
  return nwidth * dt->I_off_n;
}


double simplified_pmos_leakage(
    double pwidth,
    bool _is_dram,
    bool _is_cell,
    bool _is_wl_tr)
{
  TechnologyParameter::DeviceType * dt;

  if ((!_is_dram)&&(_is_cell))
  { //SRAM cell access transistor
    dt = &(g_tp.sram_cell);
  }
  else if ((_is_dram)&&(_is_wl_tr))
  { //DRAM wordline transistor
    dt = &(g_tp.dram_wl);
  }
  else
  { //DRAM or SRAM all other transistors
    dt = &(g_tp.peri_global);
  }
  return pwidth * dt->I_off_p;
}



