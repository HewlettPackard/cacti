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

#ifndef POWERGATING_H_
#define POWERGATING_H_

#include "component.h"

class Sleep_tx : public Component
{
public:
	Sleep_tx(
				double  _perf_with_sleep_tx,
				double  _active_Isat,//of circuit block, not sleep tx
	            bool    _is_footer,
	            double _c_circuit_wakeup,
	            double _V_delta,
	            int     _num_sleep_tx,
	//            double  _vt_circuit,
	//			double  _vt_sleep_tx,
	//			double  _mobility,//of sleep tx
	//			double  _c_ox,//of sleep tx
				const  Area & cell_);

	double  perf_with_sleep_tx;
	double  active_Isat;
	bool    is_footer;

	double  vt_circuit;
	double  vt_sleep_tx;
	double  vdd;// of circuit block not sleep tx
	double  mobility;//of sleep tx
	double  c_ox;
	double  width;
	double  c_circuit_wakeup;
	double  c_intrinsic_sleep;
	double  delay, wakeup_delay;
	powerDef power, wakeup_power;
//	double  c_circuit_sleep;
//	double  sleep_delay;
//	powerDef sleep_power;
	double  V_delta;

	int     num_sleep_tx;

	const  Area & cell;
	bool    is_sleep_tx;



//	void   compute_area();
	double compute_penalty();  // return outrisetime

	void leakage_feedback(double temperature){};
	~Sleep_tx(){};
};

#endif /* POWERGATING_H_ */
