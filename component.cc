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


#include <assert.h>
#include <iostream>
#include <math.h>

#include "bank.h"
#include "component.h"
#include "decoder.h"

using namespace std;



Component::Component()
  :area(), power(), delay(0)
{
}



Component::~Component()
{
}



double Component::compute_diffusion_width(int num_stacked_in, int num_folded_tr)
{
  double w_poly = g_ip->F_sz_um;
  double spacing_poly_to_poly = g_tp.w_poly_contact + 2 * g_tp.spacing_poly_to_contact;
  double total_diff_w = 2 * spacing_poly_to_poly +  // for both source and drain
                        num_stacked_in * w_poly +
                        (num_stacked_in - 1) * g_tp.spacing_poly_to_poly;

  if (num_folded_tr > 1)
  {
    total_diff_w += (num_folded_tr - 2) * 2 * spacing_poly_to_poly +
                    (num_folded_tr - 1) * num_stacked_in * w_poly +
                    (num_folded_tr - 1) * (num_stacked_in - 1) * g_tp.spacing_poly_to_poly;
  }

  return total_diff_w;
}



double Component::compute_gate_area(
    int gate_type,
    int num_inputs,
    double w_pmos,
    double w_nmos,
    double h_gate)
{
  if (w_pmos <= 0.0 || w_nmos <= 0.0)
  {
    return 0.0;
  }

  double w_folded_pmos, w_folded_nmos;
  int    num_folded_pmos, num_folded_nmos;
  double total_ndiff_w, total_pdiff_w;
  Area gate;
  
  double h_tr_region  = h_gate - 2 * g_tp.HPOWERRAIL;
  double ratio_p_to_n = w_pmos / (w_pmos + w_nmos);

  if (ratio_p_to_n >= 1 || ratio_p_to_n <= 0)
  {
    return 0.0;
  }

  w_folded_pmos  = (h_tr_region - g_tp.MIN_GAP_BET_P_AND_N_DIFFS) * ratio_p_to_n;
  w_folded_nmos  = (h_tr_region - g_tp.MIN_GAP_BET_P_AND_N_DIFFS) * (1 - ratio_p_to_n);
  assert(w_folded_pmos > 0);

  num_folded_pmos = (int) (ceil(w_pmos / w_folded_pmos));
  num_folded_nmos = (int) (ceil(w_nmos / w_folded_nmos));

  switch (gate_type)
  {
    case INV:
      total_ndiff_w = compute_diffusion_width(1, num_folded_nmos);
      total_pdiff_w = compute_diffusion_width(1, num_folded_pmos);
      break;
      
    case NOR:
      total_ndiff_w = compute_diffusion_width(1, num_inputs * num_folded_nmos);
      total_pdiff_w = compute_diffusion_width(num_inputs, num_folded_pmos);
      break;
      
    case NAND:
      total_ndiff_w = compute_diffusion_width(num_inputs, num_folded_nmos);
      total_pdiff_w = compute_diffusion_width(1, num_inputs * num_folded_pmos);
      break;
    default:
      cout << "Unknown gate type: " << gate_type << endl;
      exit(1);
  }

  gate.w = MAX(total_ndiff_w, total_pdiff_w);

  if (w_folded_nmos > w_nmos)
  {
    //means that the height of the gate can 
    //be made smaller than the input height specified, so calculate the height of the gate.
    gate.h = w_nmos + w_pmos + g_tp.MIN_GAP_BET_P_AND_N_DIFFS + 2 * g_tp.HPOWERRAIL;
  }
  else
  {
    gate.h = h_gate;
  }
  return gate.get_area();
}



double Component::compute_tr_width_after_folding(
    double input_width,
    double threshold_folding_width) 
{
  if (input_width <= 0)
  {
    return 0;
  }

  int    num_folded_tr        = (int) (ceil(input_width / threshold_folding_width));
  double spacing_poly_to_poly = g_tp.w_poly_contact + 2 * g_tp.spacing_poly_to_contact;
  double width_poly           = g_ip->F_sz_um;
  double total_diff_width     = num_folded_tr * width_poly + (num_folded_tr + 1) * spacing_poly_to_poly;

  return total_diff_width;
}



double Component::height_sense_amplifier(double pitch_sense_amp)
{
  // compute the height occupied by all PMOS transistors
  double h_pmos_tr = compute_tr_width_after_folding(g_tp.w_sense_p, pitch_sense_amp) * 2 + 
                     compute_tr_width_after_folding(g_tp.w_iso, pitch_sense_amp) +
                     2 * g_tp.MIN_GAP_BET_SAME_TYPE_DIFFS;

  // compute the height occupied by all NMOS transistors
  double h_nmos_tr = compute_tr_width_after_folding(g_tp.w_sense_n, pitch_sense_amp) * 2 +
                     compute_tr_width_after_folding(g_tp.w_sense_en, pitch_sense_amp) +
                     2 * g_tp.MIN_GAP_BET_SAME_TYPE_DIFFS;

  // compute total height by considering gap between the p and n diffusion areas
  return h_pmos_tr + h_nmos_tr + g_tp.MIN_GAP_BET_P_AND_N_DIFFS;
}



int Component::logical_effort(
    int num_gates_min,
    double g,
    double F,
    double * w_n,
    double * w_p,
    double C_load,
    double p_to_n_sz_ratio,
    bool   is_dram_,
    bool   is_wl_tr_,
    double max_w_nmos)
{
  int num_gates = (int) (log(F) / log(fopt));

  // check if num_gates is odd. if so, add 1 to make it even
  num_gates+= (num_gates % 2) ? 1 : 0;
  num_gates = MAX(num_gates, num_gates_min);

  // recalculate the effective fanout of each stage
  double f = pow(F, 1.0 / num_gates);
  int    i = num_gates - 1;
  double C_in = C_load / f;
  w_n[i]  = (1.0 / (1.0 + p_to_n_sz_ratio)) * C_in / gate_C(1, 0, is_dram_, false, is_wl_tr_);
  w_n[i]  = MAX(w_n[i], g_tp.min_w_nmos_);
  w_p[i]  = p_to_n_sz_ratio * w_n[i];

  if (w_n[i] > max_w_nmos)
  {
    double C_ld = gate_C((1 + p_to_n_sz_ratio) * max_w_nmos, 0, is_dram_, false, is_wl_tr_);
    F = g * C_ld / gate_C(w_n[0] + w_p[0], 0, is_dram_, false, is_wl_tr_);
    num_gates = (int) (log(F) / log(fopt)) + 1;
    num_gates+= (num_gates % 2) ? 1 : 0;
    num_gates = MAX(num_gates, num_gates_min);
    f = pow(F, 1.0 / (num_gates - 1));
    i = num_gates - 1;
    w_n[i]  = max_w_nmos;
    w_p[i]  = p_to_n_sz_ratio * w_n[i];
  }

  for (i = num_gates - 2; i >= 1; i--)
  {
    w_n[i] = MAX(w_n[i+1] / f, g_tp.min_w_nmos_);
    w_p[i] = p_to_n_sz_ratio * w_n[i];
  }

  assert(num_gates <= MAX_NUMBER_GATES_STAGE);
  return num_gates;
}

