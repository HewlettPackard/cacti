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

#include "bank.h"
#include <iostream>


Bank::Bank(const DynamicParameter & dyn_p):
  dp(dyn_p), mat(dp),
  num_addr_b_mat(dyn_p.number_addr_bits_mat),
  num_mats_hor_dir(dyn_p.num_mats_h_dir), num_mats_ver_dir(dyn_p.num_mats_v_dir)
{
  int RWP;
  int ERP;
  int EWP;

  if (dp.use_inp_params)
  {
    RWP  = dp.num_rw_ports;
    ERP  = dp.num_rd_ports;
    EWP  = dp.num_wr_ports;
  }
  else
  {
    RWP  = g_ip->num_rw_ports;
    ERP  = g_ip->num_rd_ports;
    EWP  = g_ip->num_wr_ports;
  }

  int total_addrbits = (dp.number_addr_bits_mat + dp.number_subbanks_decode)*(RWP+ERP+EWP);
  int datainbits     = dp.num_di_b_bank_per_port * (RWP + ERP);
  int dataoutbits    = dp.num_do_b_bank_per_port * (RWP + EWP);

  if (g_ip->fast_access && dp.is_tag == false)
  {
    dataoutbits *= g_ip->data_assoc;
  }

  htree_in_add   = new Htree2 (g_ip->wt,(double) mat.area.w, (double)mat.area.h, 
      total_addrbits, datainbits, dataoutbits, num_mats_ver_dir*2, num_mats_hor_dir*2, Add_htree);
  htree_in_data  = new Htree2 (g_ip->wt,(double) mat.area.w, (double)mat.area.h,
      total_addrbits, datainbits, dataoutbits, num_mats_ver_dir*2, num_mats_hor_dir*2, Data_in_htree);
  htree_out_data = new Htree2 (g_ip->wt,(double) mat.area.w, (double)mat.area.h, 
      total_addrbits, datainbits, dataoutbits, num_mats_ver_dir*2, num_mats_hor_dir*2, Data_out_htree);

  area.w = htree_in_data->area.w;
  area.h = htree_in_data->area.h;

  num_addr_b_row_dec = (g_ip->fully_assoc == true) ? 0 : _log2(mat.subarray.num_rows);
  num_addr_b_routed_to_mat_for_act = num_addr_b_row_dec;
  num_addr_b_routed_to_mat_for_rd_or_wr = num_addr_b_mat - num_addr_b_row_dec;
}



Bank::~Bank()
{
  delete htree_in_add;
  delete htree_out_data;
  delete htree_in_data;
}



double Bank::compute_delays(double inrisetime)
{
  return mat.compute_delays(inrisetime);
}



void Bank::compute_power_energy()
{
  mat.compute_power_energy();

  power.readOp.dynamic += mat.power.readOp.dynamic * dp.num_act_mats_hor_dir;
  power.readOp.leakage += mat.power.readOp.leakage * dp.num_mats;

  power.readOp.dynamic += htree_in_add->power.readOp.dynamic;
  power.readOp.dynamic += htree_out_data->power.readOp.dynamic;

  power.readOp.leakage += htree_in_add->power.readOp.leakage;
  power.readOp.leakage += htree_in_data->power.readOp.leakage;
  power.readOp.leakage += htree_out_data->power.readOp.leakage;
}

