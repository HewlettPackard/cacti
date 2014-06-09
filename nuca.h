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

#ifndef __NUCA_H__
#define __NUCA_H__

#include "basic_circuit.h"
#include "component.h"
#include "parameter.h"
#include "assert.h"
#include "cacti_interface.h"
#include "wire.h"
#include "mat.h"
#include "io.h"
#include "router.h"
#include <iostream>



class nuca_org_t {
  public:
  ~nuca_org_t();
//    int size;
    /* area, power, access time, and cycle time stats */
    Component nuca_pda;
    Component bank_pda;
    Component wire_pda;
    Wire *h_wire;
    Wire *v_wire;
    Router *router;
    /* for particular network configuration
     * calculated based on a cycle accurate
     * simulation Ref: CACTI 6 - Tech report
     */
    double contention;

    /* grid network stats */
    double avg_hops;
    int rows;
    int columns;
    int bank_count;
};



class Nuca : public Component
{
  public:
    Nuca(
        TechnologyParameter::DeviceType *dt);
    void print_router();
    ~Nuca();
    void sim_nuca();
    void init_cont();
    int calc_cycles(double lat, double oper_freq);
    void calculate_nuca_area (nuca_org_t *nuca);
    int check_nuca_org (nuca_org_t *n, min_values_t *minval);
    nuca_org_t * find_optimal_nuca (list<nuca_org_t *> *n, min_values_t *minval);
    void print_nuca(nuca_org_t *n);
    void print_cont_stats();

  private:

    TechnologyParameter::DeviceType *deviceType;
    int wt_min, wt_max;
    Wire *wire_vertical[WIRE_TYPES],
         *wire_horizontal[WIRE_TYPES];

};


#endif
