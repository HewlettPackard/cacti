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

#ifndef __UCA_H__
#define __UCA_H__

#include "area.h"
#include "bank.h"
#include "component.h"
#include "parameter.h"
#include "htree2.h"


class UCA : public Component
{
  public:
    UCA(const DynamicParameter & dyn_p);
    ~UCA();
    double compute_delays(double inrisetime);  // returns outrisetime
    void   compute_power_energy();

    DynamicParameter dp;
    Bank   bank;

    Htree2   * htree_in_add;
    Htree2   * htree_in_data;
    Htree2   * htree_out_data;

    powerDef power_routing_to_bank;

    uint32_t nbanks;

    int   num_addr_b_bank;
    int   num_di_b_bank;
    int   num_do_b_bank;
    int   RWP, ERP, EWP;
    double area_all_dataramcells;

    double dyn_read_energy_from_closed_page;
    double dyn_read_energy_from_open_page;
    double dyn_read_energy_remaining_words_in_burst;

    double refresh_power;  // only for DRAM
    double activate_energy;
    double read_energy;
    double write_energy;
    double precharge_energy;
    double leak_power_subbank_closed_page;
    double leak_power_subbank_open_page;
    double leak_power_request_and_reply_networks;

    double delay_array_to_sa_mux_lev_1_decoder;
    double delay_array_to_sa_mux_lev_2_decoder;
    double delay_before_subarray_output_driver;
    double delay_from_subarray_out_drv_to_out;
    double access_time;
    double precharge_delay;
    double multisubbank_interleave_cycle_time;
};

#endif

