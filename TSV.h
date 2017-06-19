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

#ifndef TSV_H_
#define TSV_H_

#include "basic_circuit.h"
#include "component.h"
#include "parameter.h"
//#include "assert.h"
#include "cacti_interface.h"
#include "const.h"
//#include "area.h"
#include <cmath>
#include <iostream>
#include <list>


class TSV : public Component
{
  public:
	TSV(enum TSV_type tsv_type,
        /*TechnologyParameter::*/DeviceType * dt = &(g_tp.peri_global));//Should change peri_global to TSV in technology.cc
	//TSV():len(20),rad(2.5),pitch(50){}
    ~TSV();

    double res;//TSV resistance
    double cap;//TSV capacitance
    double C_load_TSV;//The intrinsic load plus the load TSV is driving, needs changes?
    double min_area;

    //int num_IO;//number of I/O
    int num_gates;
    int num_gates_min;//Necessary?
	double w_TSV_n[MAX_NUMBER_GATES_STAGE];
	double w_TSV_p[MAX_NUMBER_GATES_STAGE];

	//double delay_TSV_path;//Delay of TSV path including the parasitics

	double is_dram;//two external arguments, defaulted to be false in constructor
	double is_wl_tr;

	void compute_buffer_stage();
	void compute_area();
	void compute_delay();
    void print_TSV();

    Area TSV_metal_area;
    Area Buffer_area;

    /*//Herigated from Component
    double delay;
    Area area;
    powerDef power, rt_power;
    double delay;
    double cycle_time;

    int logical_effort();*/

  private:
    double min_w_pmos;
    /*TechnologyParameter::*/DeviceType * deviceType;
    unsigned int tsv_type;

};


#endif /* TSV_H_ */
