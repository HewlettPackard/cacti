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

#ifndef __HTREE2_H__
#define __HTREE2_H__

#include "basic_circuit.h"
#include "component.h"
#include "parameter.h"
#include "assert.h"
#include "subarray.h"
#include "cacti_interface.h"
#include "wire.h"
  
// leakge power includes entire htree in a bank (when uca_tree == false)
// leakge power includes only part to one bank when uca_tree == true

class Htree2 : public Component
{
  public:
    Htree2(enum Wire_type wire_model, 
        double mat_w, double mat_h, int add, int data_in, int data_out, int bl, int wl,
        enum Htree_type h_type, bool uca_tree = false,
        TechnologyParameter::DeviceType *dt = &(g_tp.peri_global));
    ~Htree2() {};

    void in_htree();
    void out_htree();

    // repeaters only at h-tree nodes
    void limited_in_htree();
    void limited_out_htree();
    void input_nand(double s1, double s2, double l);
    void output_buffer(double s1, double s2, double l);

    double in_rise_time, out_rise_time;

    void set_in_rise_time(double rt) 
    {
      in_rise_time = rt;
    }

    double max_unpipelined_link_delay;
    powerDef power_bit;


  private:
    double wire_bw;
    double init_wire_bw;  // bus width at root
    enum Htree_type tree_type;
    double htree_hnodes;
    double htree_vnodes;
    double mat_width;
    double mat_height;
    int add_bits, data_in_bits, data_out_bits;
    int ndbl, ndwl;
    bool uca_tree; // should have full bandwidth to access all banks in the array simultaneously 


    enum Wire_type wt;
    double min_w_nmos;
    double min_w_pmos;

    TechnologyParameter::DeviceType *deviceType;

};

#endif
