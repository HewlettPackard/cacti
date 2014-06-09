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
#ifndef __DECODER_H__
#define __DECODER_H__

#include "area.h"
#include "component.h"
#include "parameter.h"
#include <vector>

using namespace std;


class Decoder : public Component
{
  public:
    Decoder(
        int _num_dec_signals,
        bool flag_way_select,
        double _C_ld_dec_out,
        double _R_wire_dec_out,
        bool fully_assoc_,
        bool is_dram_,
        bool is_wl_tr_,
        const Area & cell_);
    
    bool   exist;
    int    num_in_signals;
    double C_ld_dec_out;
    double R_wire_dec_out;
    int    num_gates;
    int    num_gates_min;
    double w_dec_n[MAX_NUMBER_GATES_STAGE];
    double w_dec_p[MAX_NUMBER_GATES_STAGE];
    double delay;
    powerDef power;
    bool   fully_assoc;
    bool   is_dram;
    bool   is_wl_tr;
    const  Area & cell;
    
    
    void   compute_widths();
    void   compute_area();
    double compute_delays(double inrisetime);  // return outrisetime
};



class PredecBlk : public Component
{
 public:
  PredecBlk(
      int num_dec_signals,
      Decoder * dec,
      double C_wire_predec_blk_out,
      double R_wire_predec_blk_out,
      int    num_dec_per_predec,
      bool   is_dram_,
      bool   is_blk1);

  Decoder * dec;
  bool exist;
  int number_input_addr_bits;
  double C_ld_predec_blk_out;
  double R_wire_predec_blk_out;
  int branch_effort_nand2_gate_output;
  int branch_effort_nand3_gate_output;
  bool   flag_two_unique_paths;
  int flag_L2_gate;
  int number_inputs_L1_gate;
  int number_gates_L1_nand2_path;
  int number_gates_L1_nand3_path;
  int number_gates_L2;
  int min_number_gates_L1;
  int min_number_gates_L2;
  double w_L1_nand2_n[MAX_NUMBER_GATES_STAGE];
  double w_L1_nand2_p[MAX_NUMBER_GATES_STAGE];
  double w_L1_nand3_n[MAX_NUMBER_GATES_STAGE];
  double w_L1_nand3_p[MAX_NUMBER_GATES_STAGE];
  double w_L2_n[MAX_NUMBER_GATES_STAGE];
  double w_L2_p[MAX_NUMBER_GATES_STAGE];
  double delay_nand2_path;
  double delay_nand3_path;
  powerDef power_nand2_path;
  powerDef power_nand3_path;
  powerDef power_L2;

  bool is_dram_;

  void compute_widths();
  void compute_area();
  pair<double, double> compute_delays(pair<double, double> inrisetime); // <nand2, nand3>
  // return <outrise_nand2, outrise_nand3>
};


class PredecBlkDrv : public Component
{
 public:
  PredecBlkDrv(
      int   way_select,
      PredecBlk * blk_,
      bool  is_dram);

  int flag_driver_exists;
  int number_input_addr_bits;
  int number_gates_nand2_path;
  int number_gates_nand3_path;
  int min_number_gates;
  int num_buffers_driving_1_nand2_load;
  int num_buffers_driving_2_nand2_load;
  int num_buffers_driving_4_nand2_load;
  int num_buffers_driving_2_nand3_load;
  int num_buffers_driving_8_nand3_load;
  int num_buffers_nand3_path;
  double c_load_nand2_path_out;
  double c_load_nand3_path_out;
  double r_load_nand2_path_out;
  double r_load_nand3_path_out;
  double width_nand2_path_n[MAX_NUMBER_GATES_STAGE];
  double width_nand2_path_p[MAX_NUMBER_GATES_STAGE];
  double width_nand3_path_n[MAX_NUMBER_GATES_STAGE];
  double width_nand3_path_p[MAX_NUMBER_GATES_STAGE];
  double delay_nand2_path;
  double delay_nand3_path;
  powerDef power_nand2_path;
  powerDef power_nand3_path;

  PredecBlk * blk;
  Decoder   * dec;
  bool  is_dram_;
  int   way_select;

  void compute_widths();
  void compute_area();
  pair<double, double> compute_delays(
      double inrisetime_nand2_path,
      double inrisetime_nand3_path);  // return <outrise_nand2, outrise_nand3>

  inline int num_addr_bits_nand2_path()
  {
    return num_buffers_driving_1_nand2_load +
           num_buffers_driving_2_nand2_load +
           num_buffers_driving_4_nand2_load;
  }
  inline int num_addr_bits_nand3_path()
  {
    return num_buffers_driving_2_nand3_load +
           num_buffers_driving_8_nand3_load;
  }
  double get_rdOp_dynamic_E(int num_act_mats_hor_dir);
};



class Predec : public Component
{
  public:
    Predec(
        PredecBlkDrv * drv1,
        PredecBlkDrv * drv2);

    double compute_delays(double inrisetime);  // return outrisetime

    PredecBlk    * blk1;
    PredecBlk    * blk2;
    PredecBlkDrv * drv1;
    PredecBlkDrv * drv2;

    powerDef block_power;
    powerDef driver_power;

  private:
    // returns <delay, risetime>
    pair<double, double> get_max_delay_before_decoder(
        pair<double, double> input_pair1,
        pair<double, double> input_pair2);
};



class Driver : public Component
{
 public:
  Driver(double c_gate_load_, double c_wire_load_, double r_wire_load_, bool is_dram);

  int    number_gates;
  int    min_number_gates;
  double width_n[MAX_NUMBER_GATES_STAGE];
  double width_p[MAX_NUMBER_GATES_STAGE];
  double c_gate_load;
  double c_wire_load;
  double r_wire_load;
  double delay;
  powerDef power;
  bool   is_dram_;

  void   compute_widths();
  double compute_delay(double inrisetime);
};


#endif
