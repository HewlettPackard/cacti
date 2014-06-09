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

#ifndef __ROUTER_H__
#define __ROUTER_H__

#include <assert.h>
#include <iostream>
#include "basic_circuit.h"
#include "cacti_interface.h"
#include "component.h"
#include "mat.h"
#include "parameter.h"
#include "wire.h"
#include "crossbar.h"
#include "arbiter.h"



class Router : public Component
{
  public:
    Router(
        double flit_size_,
        double vc_buf, /* vc size = vc_buffer_size * flit_size */
        double vc_count,
        TechnologyParameter::DeviceType *dt = &(g_tp.peri_global));
    ~Router();


    void print_router();

    Component arbiter, crossbar, buffer;
    double cycle_time, max_cyc;
    double flit_size;
    double vc_count;
    double vc_buffer_size; /* vc size = vc_buffer_size * flit_size */

  private:
    double FREQUENCY; // move this to config file --TODO
    double Cw3(double len);
    double gate_cap(double w);
    double diff_cap(double w, int type /*0 for n-mos and 1 for p-mos*/, double stack);
    enum Wire_type wtype;
    enum Wire_placement wire_placement;
    //corssbar
    double NTtr, PTtr, wt, ht, I, O, NTi, PTi, NTid, PTid, NTod, PTod, TriS1, TriS2;
    double transmission_buf_inpcap();
    double transmission_buf_outcap();
    double transmission_buf_ctrcap();
    double crossbar_inpline();
    double crossbar_outline();
    double crossbar_ctrline();
    double tr_crossbar_power();
    void  cb_stats ();
    double arb_power();
    void  arb_stats ();
    double buffer_params();
    void buffer_stats();


    //arbiter

    //buffer

    //router params
    double Vdd;

    void calc_router_parameters();
    void get_router_area();
    void get_router_power();
    void get_router_delay();

    double min_w_pmos;
    TechnologyParameter::DeviceType *deviceType;

};

#endif
