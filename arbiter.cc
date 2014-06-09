
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

#include "arbiter.h"

Arbiter::Arbiter(
    double n_req, 
    double flit_size_,
    double output_len,
    TechnologyParameter::DeviceType *dt
    ):R(n_req), flit_size(flit_size_), 
    o_len (output_len), deviceType(dt)
{
  min_w_pmos = deviceType->n_to_p_eff_curr_drv_ratio*g_tp.min_w_nmos_;
  Vdd = dt->Vdd;
  double technology = g_ip->F_sz_um;
  NTn1 = 13.5*technology/2;
  PTn1 = 76*technology/2;
  NTn2 = 13.5*technology/2;
  PTn2 = 76*technology/2;
  NTi = 12.5*technology/2;
  PTi = 25*technology/2;
  NTtr = 10*technology/2; /*Transmission gate's nmos tr. length*/
  PTtr = 20*technology/2; /* pmos tr. length*/
}

Arbiter::~Arbiter(){}

double 
Arbiter::arb_req() {
  double temp = ((R-1)*(2*gate_C(NTn1, 0)+gate_C(PTn1, 0)) + 2*gate_C(NTn2, 0) + 
      gate_C(PTn2, 0) + gate_C(NTi, 0) + gate_C(PTi, 0) + 
      drain_C_(NTi, 0, 1, 1, g_tp.cell_h_def) + drain_C_(PTi, 1, 1, 1, g_tp.cell_h_def));
  return temp;
}

double 
Arbiter::arb_pri() {
  double temp = 2*(2*gate_C(NTn1, 0)+gate_C(PTn1, 0)); /* switching capacitance 
                                                 of flip-flop is ignored */
  return temp;                                                 
}


double 
Arbiter::arb_grant() {
  double temp = drain_C_(NTn1, 0, 1, 1, g_tp.cell_h_def)*2 + drain_C_(PTn1, 1, 1, 1, g_tp.cell_h_def) + crossbar_ctrline();
  return temp;
}

double 
Arbiter::arb_int() {
  double temp  =  (drain_C_(NTn1, 0, 1, 1, g_tp.cell_h_def)*2 + drain_C_(PTn1, 1, 1, 1, g_tp.cell_h_def) + 
      2*gate_C(NTn2, 0) + gate_C(PTn2, 0));
  return temp;
}

void 
Arbiter::compute_power() {
  power.readOp.dynamic =  (R*arb_req()*Vdd*Vdd/2 + R*arb_pri()*Vdd*Vdd/2 + 
      arb_grant()*Vdd*Vdd + arb_int()*0.5*Vdd*Vdd);
  double nor1_leak = deviceType->Vdd*cmos_Ileak(g_tp.min_w_nmos_*NTn1*2, min_w_pmos * PTn1*2, false); 
  double nor2_leak = deviceType->Vdd*cmos_Ileak(g_tp.min_w_nmos_*NTn2*R, min_w_pmos * PTn2*R, false); 
  double not_leak = deviceType->Vdd*cmos_Ileak(g_tp.min_w_nmos_*NTi, min_w_pmos * PTi, false); 
  power.readOp.leakage = nor1_leak + nor2_leak + not_leak; //FIXME include priority table leakage
}

double //wire cap with triple spacing
Arbiter::Cw3(double length) {
  Wire wc(g_ip->wt, length, 1, 3, 3);
  double temp = (wc.wire_cap(length));
  return temp;
}

double 
Arbiter::crossbar_ctrline() {
  double temp = (Cw3(o_len * 1e-6 /* m */) + flit_size*transmission_buf_ctrcap() + 
      drain_C_(NTi, 0, 1, 1, g_tp.cell_h_def) + drain_C_(PTi, 1, 1, 1, g_tp.cell_h_def) + 
      gate_C(NTi, 0) + gate_C(PTi, 0));
  return temp;
}

double 
Arbiter::transmission_buf_ctrcap() {
  double temp = gate_C(NTtr, 0)+gate_C(PTtr, 0);
  return temp;
}


void Arbiter::print_arbiter()
{
  cout << "\nArbiter Stats ("   << R << " input arbiter" << ")\n\n";
  cout << "Flit size        : " << flit_size << " bits" << endl;
  cout << "Dynamic Power    : " << power.readOp.dynamic*1e9 << " (nJ)" << endl;
  cout << "Leakage Power    : " << power.readOp.leakage*1e3 << " (mW)" << endl;
}


