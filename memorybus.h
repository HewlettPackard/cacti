/*****************************************************************************
 *                                CACTI 7.0
 *                      SOFTWARE LICENSE AGREEMENT
 *            Copyright 2015 Hewlett-Packard Development Company, L.P.
 *                          All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.”
 *
 ***************************************************************************/


#ifndef __MEMORYBUS_H__
#define __MEMORYBUS_H__

#include "basic_circuit.h"
#include "component.h"
#include "parameter.h"
//#include "assert.h"
#include "cacti_interface.h"
//#include "wire.h"
class Wire;
//#include "area.h"
#include "decoder.h"

class Memorybus : public Component
{
  public:
	Memorybus(enum Wire_type wire_model, double mat_w, double mat_h, double subarray_w, double subarray_h,
			int _row_add_bits, int _col_add_bits, int _data_bits, int _ndbl, int _ndwl, /*enum Htree_type htree_type,*/
			enum Memorybus_type membus_type, const DynamicParameter & dp_,
			/*TechnologyParameter::*/DeviceType *dt = &(g_tp.peri_global)
			);
    ~Memorybus();

    //void in_membus();
    //void out_membus();
    void Network();

    // repeaters only at h-tree nodes
    void limited_in_membus();
    void limited_out_membus();
    void input_nand(double s1, double s2, double l);
    //void output_buffer(double s1, double s2, double l);

    const DynamicParameter & dp;

    double in_rise_time, out_rise_time;

    void set_in_rise_time(double rt)
    {
      in_rise_time = rt;
    }

    double max_unpipelined_link_delay;
    powerDef power_bit;
    void memory_bus();

    double height_bank, length_bank; // The actual height and length of a single bank including all wires between subarrays.
    Wire * center_stripe;
    Wire * bank_bus;
    Wire * global_WL; //3 hierarchical connection wires.
    Wire * column_sel;
    Wire * local_data;
    Wire * global_data;
    Wire * out_seg;
    // Driver for LWL connecting GWL, same as in mat.cc
    double     		lwl_driver_c_gate_load,	lwl_driver_c_wire_load,	lwl_driver_r_wire_load;

    powerDef power_bus;
    powerDef power_lwl_drv;
    powerDef power_add_decoders;
    powerDef power_global_WL;
    powerDef power_local_WL;
    powerDef power_add_predecoder;
    powerDef power_burst;
    powerDef power_col_sel;
    powerDef power_local_data;
    powerDef power_global_data;
    double delay_bus, delay_add_predecoder, delay_add_decoder, delay_lwl_drv, delay_global_data, delay_local_data, delay_data_buffer;
    double area_lwl_drv, area_row_predec_dec, area_col_predec_dec, area_subarray, area_bus, area_address_bus, area_data_bus, area_data_drv, area_IOSA, area_local_dataline, area_sense_amp;


    Area    cell;
    bool is_dram;

    Driver * lwl_drv, * local_data_drv, * global_data_drv ;
    Predec * add_predec;
    Decoder * add_dec;

    double compute_delays(double inrisetime);  // return outrisetime
    void compute_power_energy();	//




  private:
    double wire_bw;
    double init_wire_bw;  // bus width at root
    enum Memorybus_type membus_type;
//    double htree_hnodes;
//    double htree_vnodes;
    double mat_width;
    double mat_height;
    double subarray_width, subarray_height;
    //int add_bits, data_in_bits,search_data_in_bits,data_out_bits,  search_data_out_bits;
    int row_add_bits, col_add_bits;
    int add_bits, data_bits, num_dec_signals;
    int semi_repeated_global_line;

    int ndbl, ndwl;
//    bool uca_tree; // should have full bandwidth to access all banks in the array simultaneously
//    bool search_tree;

    enum Wire_type wt;
    double min_w_nmos;
    double min_w_pmos;

    int num_lwl_drv; //Ratio between GWL and LWL, how many local WL drives each GWL drives.
    int chip_IO_width;
    int burst_length;
    int num_subarray_global_IO;

    double C_GWL, C_LWL, R_GWL, R_LWL, C_colsel, R_colsel, C_global_data, R_global_data; // Capacitance of global/local WLs.

    /*TechnologyParameter::*/DeviceType *deviceType;
};

#endif

