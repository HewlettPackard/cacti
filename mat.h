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

#ifndef __MAT_H__
#define __MAT_H__

#include "component.h"
#include "decoder.h"
#include "wire.h"
#include "subarray.h"

class Mat : public Component
{
  public:
    Mat(const DynamicParameter & dyn_p);
    ~Mat();
    double compute_delays(double inrisetime);  // return outrisetime
    void compute_power_energy();

    const DynamicParameter & dp;

    // TODO: clean up pointers and powerDefs below
    Decoder * row_dec;
    Decoder * bit_mux_dec;
    Decoder * sa_mux_lev_1_dec;
    Decoder * sa_mux_lev_2_dec;
    PredecBlk * dummy_way_sel_predec_blk1;
    PredecBlk * dummy_way_sel_predec_blk2;
    PredecBlkDrv * way_sel_drv1;
    PredecBlkDrv * dummy_way_sel_predec_blk_drv2;

    Predec * r_predec;
    Predec * b_mux_predec;
    Predec * sa_mux_lev_1_predec;
    Predec * sa_mux_lev_2_predec;

    Wire   * subarray_out_wire;
    Driver * bl_precharge_eq_drv;

    powerDef power_row_decoders;
    powerDef power_bit_mux_decoders;
    powerDef power_sa_mux_lev_1_decoders;
    powerDef power_sa_mux_lev_2_decoders;
    powerDef power_fa_decoder;  // TODO: leakage power is not computed yet
    powerDef power_bl_precharge_eq_drv;
    powerDef power_subarray_out_drv;

    double   delay_fa_decoder;
    double   delay_before_decoder;
    double   delay_bitline;
    double   delay_wl_reset;
    double   delay_bl_restore;

    Subarray subarray;
    powerDef power_bitline;
    double   per_bitline_read_energy;
    int      deg_bl_muxing;
    int      num_act_mats_hor_dir;
    double   delay_writeback;
    Area     cell;
    bool     is_dram;
    int      num_mats;
    powerDef power_sa;
    double   delay_sa;
    double   leak_power_sense_amps_closed_page_state;
    double   leak_power_sense_amps_open_page_state;
    double   delay_subarray_out_drv;
    double   delay_subarray_out_drv_htree;
    double   delay_comparator;
    powerDef power_comparator;
    int      num_do_b_mat;
    double   C_bl;

    uint32_t num_subarrays_per_mat;  // the number of subarrays in a mat
    uint32_t num_subarrays_per_row;  // the number of subarrays in a row of a mat


  private:
    double compute_bit_mux_sa_precharge_sa_mux_wr_drv_wr_mux_h();
    double width_write_driver_or_write_mux();
    double compute_comparators_height(int tagbits, int number_ways_in_mat, double subarray_mem_cell_area_w);
    void   delay_fa_tag(int tagbits, int Ntbl, bool is_dram);
    double compute_bitline_delay(double inrisetime);
    double compute_sa_delay(double inrisetime);
    double compute_subarray_out_drv(double inrisetime);
    double compute_comparator_delay(double inrisetime);

    int RWP;
    int ERP;
    int EWP;
};



#endif
