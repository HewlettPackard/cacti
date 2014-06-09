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

#include "uca.h"


UCA::UCA(const DynamicParameter & dyn_p)
 :dp(dyn_p), bank(dp), nbanks(g_ip->nbanks), refresh_power(0)
{
  int num_banks_ver_dir = 1 << ((bank.area.h > bank.area.w) ? _log2(nbanks)/2 : (_log2(nbanks) - _log2(nbanks)/2));
  int num_banks_hor_dir = nbanks/num_banks_ver_dir;

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

  num_addr_b_bank = (dp.number_addr_bits_mat + dp.number_subbanks_decode)*(RWP+ERP+EWP);
  num_di_b_bank   = dp.num_di_b_bank_per_port * (RWP + ERP);
  num_do_b_bank   = dp.num_do_b_bank_per_port * (RWP + EWP);

  if (g_ip->fast_access && dp.is_tag == false)
  {
    num_do_b_bank *= g_ip->data_assoc;
  }

  htree_in_add   = new Htree2(g_ip->wt, bank.area.w, bank.area.h, 
      num_addr_b_bank, num_di_b_bank, num_do_b_bank, num_banks_ver_dir*2, num_banks_hor_dir*2, Add_htree, true);
  htree_in_data  = new Htree2(g_ip->wt, bank.area.w, bank.area.h,
      num_addr_b_bank, num_di_b_bank, num_do_b_bank, num_banks_ver_dir*2, num_banks_hor_dir*2, Data_in_htree, true);
  htree_out_data = new Htree2(g_ip->wt, bank.area.w, bank.area.h, 
      num_addr_b_bank, num_di_b_bank, num_do_b_bank, num_banks_ver_dir*2, num_banks_hor_dir*2, Data_out_htree, true);

  area.w = htree_in_data->area.w;
  area.h = htree_in_data->area.h;

  area_all_dataramcells = bank.mat.subarray.get_total_cell_area() * dp.num_subarrays * g_ip->nbanks;

  // delay calculation
  double inrisetime = 0.0;
  compute_delays(inrisetime);
  compute_power_energy();
}



UCA::~UCA()
{
  delete htree_in_add;
  delete htree_in_data;
  delete htree_out_data;
}



double UCA::compute_delays(double inrisetime)
{
  double outrisetime = bank.compute_delays(inrisetime);

  double delay_array_to_mat = htree_in_add->delay + bank.htree_in_add->delay;
  double max_delay_before_row_decoder = delay_array_to_mat + bank.mat.r_predec->delay;
  delay_array_to_sa_mux_lev_1_decoder = delay_array_to_mat + 
    bank.mat.sa_mux_lev_1_predec->delay +
    bank.mat.sa_mux_lev_1_dec->delay;
  delay_array_to_sa_mux_lev_2_decoder = delay_array_to_mat + 
    bank.mat.sa_mux_lev_2_predec->delay +
    bank.mat.sa_mux_lev_2_dec->delay;
  double delay_inside_mat = bank.mat.row_dec->delay + bank.mat.delay_bitline + bank.mat.delay_sa;

  delay_before_subarray_output_driver =
    MAX(MAX(max_delay_before_row_decoder + delay_inside_mat,  // row_path
            delay_array_to_mat + bank.mat.b_mux_predec->delay + bank.mat.bit_mux_dec->delay + bank.mat.delay_sa),  // col_path
        MAX(delay_array_to_sa_mux_lev_1_decoder,    // sa_mux_lev_1_path
            delay_array_to_sa_mux_lev_2_decoder));  // sa_mux_lev_2_path
  delay_from_subarray_out_drv_to_out = bank.mat.delay_subarray_out_drv_htree +
                                       bank.htree_out_data->delay + htree_out_data->delay;
  access_time                        = bank.mat.delay_comparator;

  if (g_ip->fully_assoc)
  {
    if (dp.is_tag)
    { //delay of fully-associative tag decoder
      access_time = htree_in_add->delay + bank.htree_in_add->delay + bank.mat.delay_fa_decoder;
    }
    else
    { //delay of fully-associative data array
      access_time = delay_inside_mat + delay_from_subarray_out_drv_to_out;
    }
  }
  else
  {
    access_time = delay_before_subarray_output_driver + delay_from_subarray_out_drv_to_out; //data_acc_path
  }

  if (dp.is_main_mem)
  {
    double t_rcd       = max_delay_before_row_decoder + delay_inside_mat;
    double cas_latency = MAX(delay_array_to_sa_mux_lev_1_decoder, delay_array_to_sa_mux_lev_2_decoder) + 
                         delay_from_subarray_out_drv_to_out;
    access_time = t_rcd + cas_latency;
  }

  double temp;
  if (dp.is_tag && g_ip->fully_assoc)
  {
    temp = bank.mat.delay_fa_decoder;
  }
  else
  {
    temp = delay_inside_mat + bank.mat.delay_wl_reset + bank.mat.delay_bl_restore;
    if (dp.is_dram)
    {
      temp += bank.mat.delay_writeback;  // temp stores random cycle time
    }
  }

  temp = MAX(temp, bank.mat.r_predec->delay);
  temp = MAX(temp, bank.mat.b_mux_predec->delay);
  temp = MAX(temp, bank.mat.sa_mux_lev_1_predec->delay);
  temp = MAX(temp, bank.mat.sa_mux_lev_2_predec->delay);

  // The following is true only if the input parameter "repeaters_in_htree" is set to false --Nav 
  if (g_ip->rpters_in_htree == false)
  {
    temp = MAX(temp, bank.htree_in_add->max_unpipelined_link_delay);
  }
  cycle_time = temp;

  double delay_req_network = max_delay_before_row_decoder;
  double delay_rep_network = delay_from_subarray_out_drv_to_out;
  multisubbank_interleave_cycle_time = MAX(delay_req_network, delay_rep_network);

  if (dp.is_main_mem)
  {
    multisubbank_interleave_cycle_time = htree_in_add->delay;
    precharge_delay = htree_in_add->delay +
                      bank.htree_in_add->delay + bank.mat.delay_writeback +
                      bank.mat.delay_wl_reset + bank.mat.delay_bl_restore;
    cycle_time = access_time + precharge_delay;
  }
  else
  {
    precharge_delay = 0;
  }

  double dram_array_availability = 0;
  if (dp.is_dram)
  {
    dram_array_availability = (1 - dp.num_r_subarray * cycle_time / dp.dram_refresh_period) * 100;
  }

  return outrisetime;
}



// note: currently, power numbers are for a bank of an array
void UCA::compute_power_energy()
{
  bank.compute_power_energy();
  power = bank.power;

  power_routing_to_bank.readOp.dynamic  = htree_in_add->power.readOp.dynamic + htree_out_data->power.readOp.dynamic;
  power_routing_to_bank.writeOp.dynamic = htree_in_add->power.readOp.dynamic + htree_in_data->power.readOp.dynamic;
  power_routing_to_bank.readOp.leakage += htree_in_add->power.readOp.leakage +
                                          htree_in_data->power.readOp.leakage +
                                          htree_out_data->power.readOp.leakage;

  power.readOp.dynamic += power_routing_to_bank.readOp.dynamic;
  power.readOp.leakage += power_routing_to_bank.readOp.leakage;

  // calculate total write energy per access
  power.writeOp.dynamic = power.readOp.dynamic 
                        - bank.mat.power_bitline.readOp.dynamic * dp.num_act_mats_hor_dir
                        + bank.mat.power_bitline.writeOp.dynamic * dp.num_act_mats_hor_dir
                        - power_routing_to_bank.readOp.dynamic
                        + power_routing_to_bank.writeOp.dynamic
                        + bank.htree_in_data->power.readOp.dynamic
                        - bank.htree_out_data->power.readOp.dynamic;

  if (dp.is_dram == false)
  {
    power.writeOp.dynamic -= bank.mat.power_sa.readOp.dynamic * dp.num_act_mats_hor_dir;
  }

  dyn_read_energy_from_closed_page = power.readOp.dynamic;
  dyn_read_energy_from_open_page   = power.readOp.dynamic -
                                     (bank.mat.r_predec->power.readOp.dynamic +
                                      bank.mat.power_row_decoders.readOp.dynamic +
                                      bank.mat.power_bl_precharge_eq_drv.readOp.dynamic +
                                      bank.mat.power_sa.readOp.dynamic +
                                      bank.mat.power_bitline.readOp.dynamic) * dp.num_act_mats_hor_dir;

  dyn_read_energy_remaining_words_in_burst = 
    (MAX((g_ip->burst_len / g_ip->int_prefetch_w), 1) - 1) * 
    ((bank.mat.sa_mux_lev_1_predec->power.readOp.dynamic +
      bank.mat.sa_mux_lev_2_predec->power.readOp.dynamic +
      bank.mat.power_sa_mux_lev_1_decoders.readOp.dynamic +
      bank.mat.power_sa_mux_lev_2_decoders.readOp.dynamic +
      bank.mat.power_subarray_out_drv.readOp.dynamic)     * dp.num_act_mats_hor_dir +
     bank.htree_out_data->power.readOp.dynamic +
     power_routing_to_bank.readOp.dynamic);
  dyn_read_energy_from_closed_page += dyn_read_energy_remaining_words_in_burst;
  dyn_read_energy_from_open_page   += dyn_read_energy_remaining_words_in_burst;

  activate_energy = htree_in_add->power.readOp.dynamic +
                    bank.htree_in_add->power_bit.readOp.dynamic * bank.num_addr_b_routed_to_mat_for_act +
                    (bank.mat.r_predec->power.readOp.dynamic +
                     bank.mat.power_row_decoders.readOp.dynamic +
                     bank.mat.power_sa.readOp.dynamic) * dp.num_act_mats_hor_dir;
  read_energy    = (htree_in_add->power.readOp.dynamic +
                    bank.htree_in_add->power_bit.readOp.dynamic * bank.num_addr_b_routed_to_mat_for_rd_or_wr + 
                    (bank.mat.sa_mux_lev_1_predec->power.readOp.dynamic  +
                     bank.mat.sa_mux_lev_2_predec->power.readOp.dynamic  +
                     bank.mat.power_sa_mux_lev_1_decoders.readOp.dynamic + 
                     bank.mat.power_sa_mux_lev_2_decoders.readOp.dynamic +
                     bank.mat.power_subarray_out_drv.readOp.dynamic) * dp.num_act_mats_hor_dir +
                    bank.htree_out_data->power.readOp.dynamic +
                    htree_in_data->power.readOp.dynamic) * g_ip->burst_len;
  write_energy   = (htree_in_add->power.readOp.dynamic +
                    bank.htree_in_add->power_bit.readOp.dynamic * bank.num_addr_b_routed_to_mat_for_rd_or_wr + 
                    htree_in_data->power.readOp.dynamic +
                    bank.htree_in_data->power.readOp.dynamic +
                    (bank.mat.sa_mux_lev_1_predec->power.readOp.dynamic  +
                     bank.mat.sa_mux_lev_2_predec->power.readOp.dynamic  +
                     bank.mat.power_sa_mux_lev_1_decoders.readOp.dynamic +
                     bank.mat.power_sa_mux_lev_2_decoders.readOp.dynamic) * dp.num_act_mats_hor_dir) * g_ip->burst_len;
  precharge_energy = (bank.mat.power_bitline.readOp.dynamic +
                      bank.mat.power_bl_precharge_eq_drv.readOp.dynamic) * dp.num_act_mats_hor_dir;

  leak_power_subbank_closed_page =
    (bank.mat.r_predec->power.readOp.leakage +
     bank.mat.b_mux_predec->power.readOp.leakage +
     bank.mat.sa_mux_lev_1_predec->power.readOp.leakage +
     bank.mat.sa_mux_lev_2_predec->power.readOp.leakage +
     bank.mat.power_row_decoders.readOp.leakage +
     bank.mat.power_bit_mux_decoders.readOp.leakage +
     bank.mat.power_sa_mux_lev_1_decoders.readOp.leakage +
     bank.mat.power_sa_mux_lev_2_decoders.readOp.leakage +
     bank.mat.leak_power_sense_amps_closed_page_state) * dp.num_act_mats_hor_dir;
  leak_power_subbank_open_page =
    (bank.mat.r_predec->power.readOp.leakage +
     bank.mat.b_mux_predec->power.readOp.leakage +
     bank.mat.sa_mux_lev_1_predec->power.readOp.leakage +
     bank.mat.sa_mux_lev_2_predec->power.readOp.leakage +
     bank.mat.power_row_decoders.readOp.leakage +
     bank.mat.power_bit_mux_decoders.readOp.leakage +
     bank.mat.power_sa_mux_lev_1_decoders.readOp.leakage +
     bank.mat.power_sa_mux_lev_2_decoders.readOp.leakage +
     bank.mat.leak_power_sense_amps_open_page_state) * dp.num_act_mats_hor_dir;
  leak_power_request_and_reply_networks =
    power_routing_to_bank.readOp.leakage +
    bank.htree_in_add->power.readOp.leakage +
    bank.htree_in_data->power.readOp.leakage +
    bank.htree_out_data->power.readOp.leakage;

  if (dp.is_dram)
  { // if DRAM, add contribution of power spent in row predecoder drivers, blocks and decoders to refresh power
    refresh_power  = (bank.mat.r_predec->power.readOp.dynamic * dp.num_act_mats_hor_dir +
                      bank.mat.row_dec->power.readOp.dynamic) * dp.num_r_subarray * dp.num_subarrays;
    refresh_power += bank.mat.per_bitline_read_energy * dp.num_c_subarray * dp.num_r_subarray * dp.num_subarrays;
    refresh_power += bank.mat.power_bl_precharge_eq_drv.readOp.dynamic * dp.num_act_mats_hor_dir;
    refresh_power += bank.mat.power_sa.readOp.dynamic * dp.num_act_mats_hor_dir;
    refresh_power /= dp.dram_refresh_period;
  }


  if (dp.is_tag == false)
  {
    power.readOp.dynamic  = dyn_read_energy_from_closed_page;
    power.writeOp.dynamic = dyn_read_energy_from_closed_page
      - dyn_read_energy_remaining_words_in_burst
      - bank.mat.power_bitline.readOp.dynamic * dp.num_act_mats_hor_dir
      + bank.mat.power_bitline.writeOp.dynamic * dp.num_act_mats_hor_dir
      + (power_routing_to_bank.writeOp.dynamic - 
         power_routing_to_bank.readOp.dynamic -
         bank.htree_out_data->power.readOp.dynamic +
         bank.htree_in_data->power.readOp.dynamic) *
        (MAX((g_ip->burst_len / g_ip->int_prefetch_w), 1) - 1); //FIXME

    if (dp.is_dram == false)
    {
      power.writeOp.dynamic -= bank.mat.power_sa.readOp.dynamic * dp.num_act_mats_hor_dir;
    }
  }

  // if DRAM, add refresh power to total leakage
  if (dp.is_dram)
  {
    power.readOp.leakage += refresh_power;
  }

  assert(power.readOp.dynamic  > 0);
  assert(power.writeOp.dynamic > 0);
  assert(power.readOp.leakage  > 0);
}

