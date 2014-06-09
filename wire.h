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

#ifndef __WIRE_H__
#define __WIRE_H__

#include "basic_circuit.h"
#include "component.h"
#include "parameter.h"
#include "assert.h"
#include "cacti_interface.h"
#include <iostream>
#include <list>

class Wire : public Component
{
  public:
    Wire(enum Wire_type wire_model, double len /* in u*/,
         int nsense = 1/* no. of sense amps connected to the low-swing wire */,
         double width_scaling = 1,
         double spacing_scaling = 1,
         enum Wire_placement wire_placement = outside_mat,
         double resistivity = CU_RESISTIVITY,
         TechnologyParameter::DeviceType *dt = &(g_tp.peri_global));
    ~Wire();

    Wire( double width_scaling = 1,
         double spacing_scaling = 1,
         enum Wire_placement wire_placement = outside_mat,
         double resistivity = CU_RESISTIVITY,
         TechnologyParameter::DeviceType *dt = &(g_tp.peri_global)
    ); // should be used only once for initializing static members
    void init_wire();

    void calculate_wire_stats();
    void delay_optimal_wire();
    double wire_cap(double len);
    double wire_res(double len);
    void low_swing_model();
    double signal_fall_time();
    double signal_rise_time();
    double sense_amp_input_cap();

    enum Wire_type wt;
    double wire_spacing;
    double wire_width;
    enum Wire_placement wire_placement;
    double repeater_size;
    double repeater_spacing;
    double wire_length;
    double in_rise_time, out_rise_time;

    void set_in_rise_time(double rt) 
    {
      in_rise_time = rt;
    }
    static Component global;
    static Component global_5;
    static Component global_10;
    static Component global_20;
    static Component global_30;
    static Component low_swing;
    static double wire_width_init;
    static double wire_spacing_init;
    void print_wire();

  private:

    int nsense; // no. of sense amps connected to a low-swing wire if it
                // is broadcasting data to multiple destinations
    // width and spacing scaling factor can be used
    // to model low level wires or special
    // fat wires
    double w_scale, s_scale;
    double resistivity;
    powerDef wire_model (double space, double size, double *delay);
    list <Component> repeated_wire;
    void update_fullswing();


    //low-swing
    Component transmitter;
    Component l_wire;
    Component sense_amp;

    double min_w_pmos;

    TechnologyParameter::DeviceType *deviceType;
                
};

#endif
