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



#include <iostream>
#include <string>
#include <iomanip>

#include "parameter.h"
#include "area.h"

#include "basic_circuit.h"
#include <cassert>

using namespace std;


InputParameter * g_ip;
TechnologyParameter g_tp;

// ali
bool is_equal(double first, double second)
{
	
	if((first == 0) && (second ==0))
	{
			return true;
	}
	
	if((second==0) || (second!=second))
		return true;
	
	if((first!=first) || (second!=second)) // both are NaNs
	{
		return true;
	}
	if(first==0)
	{
		if(fabs(first-second)<(second*0.000001))
			return true;
	}
	else
	{
		if(fabs(first-second)<(first*0.000001))
			return true;
	}
	
	return false;		
}

/**
void DeviceType::display(uint32_t indent) const
{
  string indent_str(indent, ' ');

  cout << indent_str << "C_g_ideal = " << setw(12) << C_g_ideal << " F/um" << endl;
  cout << indent_str << "C_fringe  = " << setw(12) << C_fringe  << " F/um" << endl;
  cout << indent_str << "C_overlap = " << setw(12) << C_overlap << " F/um" << endl;
  cout << indent_str << "C_junc    = " << setw(12) << C_junc    << " F/um^2" << endl;
  cout << indent_str << "C_junc_sw    = " << setw(12) << C_junc_sidewall    << " F/um^2" << endl;
  cout << indent_str << "l_phy     = " << setw(12) << l_phy     << " um" << endl;
  cout << indent_str << "l_elec    = " << setw(12) << l_elec    << " um" << endl;
  cout << indent_str << "R_nch_on  = " << setw(12) << R_nch_on  << " ohm-um" << endl;
  cout << indent_str << "R_pch_on  = " << setw(12) << R_pch_on  << " ohm-um" << endl;
  cout << indent_str << "Vdd       = " << setw(12) << Vdd       << " V" << endl;
  cout << indent_str << "Vth       = " << setw(12) << Vth       << " V" << endl;
  cout << indent_str << "I_on_n    = " << setw(12) << I_on_n    << " A/um" << endl;
  cout << indent_str << "I_on_p    = " << setw(12) << I_on_p    << " A/um" << endl;
  cout << indent_str << "I_off_n   = " << setw(12) << I_off_n   << " A/um" << endl;
  cout << indent_str << "I_off_p   = " << setw(12) << I_off_p   << " A/um" << endl;
  cout << indent_str << "C_ox      = " << setw(12) << C_ox      << " F/um^2" << endl;
  cout << indent_str << "t_ox      = " << setw(12) << t_ox      << " um" << endl;
  cout << indent_str << "n_to_p_eff_curr_drv_ratio = " << n_to_p_eff_curr_drv_ratio << endl;
}
**/
bool DeviceType::isEqual(const DeviceType & dev)
{
	if( !is_equal(C_g_ideal,dev.C_g_ideal)) {display(0); cout << "\n\n\n"; dev.display(0); assert(false);}
	if( !is_equal(C_fringe,dev.C_fringe)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	
	if( !is_equal(C_overlap , dev.C_overlap)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(C_junc , dev.C_junc)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(C_junc_sidewall , dev.C_junc_sidewall)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(l_phy , dev.l_phy)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(l_elec , dev.l_elec)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(R_nch_on , dev.R_nch_on)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(R_pch_on , dev.R_pch_on)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(Vdd , dev.Vdd)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(Vth , dev.Vth)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
////	if( !is_equal(Vcc_min , dev.Vcc_min)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(I_on_n , dev.I_on_n)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(I_on_p , dev.I_on_p)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(I_off_n , dev.I_off_n)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(I_off_p , dev.I_off_p)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(I_g_on_n , dev.I_g_on_n)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(I_g_on_p , dev.I_g_on_p)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(C_ox , dev.C_ox)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(t_ox , dev.t_ox)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(n_to_p_eff_curr_drv_ratio , dev.n_to_p_eff_curr_drv_ratio)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	if( !is_equal(long_channel_leakage_reduction , dev.long_channel_leakage_reduction)) {display(0);cout << "\n\n\n"; dev.display(0); assert(false);}
	if( !is_equal(Mobility_n , dev.Mobility_n)) {display(0); cout << "\n\n\n"; dev.display(0);assert(false);}
	
	// auxilary parameters
	///if( !is_equal(Vdsat , dev.Vdsat)) {display(0); cout << "\n\n\n"; dev.display(0); assert(false);}
	///if( !is_equal(gmp_to_gmn_multiplier , dev.gmp_to_gmn_multiplier)) {display(0); cout << "\n\n\n"; dev.display(0); assert(false);}
	
	return true;
}

double scan_single_input_double(char* line, const char* name, const char* unit_name, bool print)
{
	double temp;
	char unit[300];
	memset(unit,0,300);
	sscanf(&line[strlen(name)], "%*[ \t]%s%*[ \t]%lf",unit,&temp);
	if(print)
		cout << name << ": " << temp << " " << unit << endl;
	return temp;
}

double scan_five_input_double(char* line, const char* name, const char* unit_name, int flavor, bool print)
{
  double temp[5];	
  char unit[300];
  memset(unit,0,300);
  sscanf(&line[strlen(name)], "%*[ \t]%s%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf"
		,unit,&(temp[0]),&(temp[1]),&(temp[2]),&(temp[3]), &(temp[4]) );
		
  if (print)
		cout << name << "[" << flavor <<"]: " << temp[flavor] << " " << unit<< endl;
  return temp[flavor];
		
}

void scan_five_input_double_temperature(char* line, const char* name, const char* unit_name, int flavor, unsigned int temperature, bool print, double & result)
{
  double temp[5];	
  unsigned int thermal_temp;
  char unit[300];
  memset(unit,0,300);
  sscanf(&line[strlen(name)], "%*[ \t]%s%*[ \t]%u%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf"
		,unit,&thermal_temp,&(temp[0]),&(temp[1]),&(temp[2]),&(temp[3]), &(temp[4]) );
		
 
  if(thermal_temp==(temperature-300))
  {
	   if (print)
					cout << name << ": " << temp[flavor] << " "<< unit << endl;
			
	result = temp[flavor];
  }
		
}

void DeviceType::assign(const string & in_file, int tech_flavor,  unsigned int temperature)
{
	FILE *fp = fopen(in_file.c_str(), "r");
	char line[5000];
	//char temp_var[5000];

	//double temp[5];
	//unsigned int thermal_temp;

	double nmos_effective_resistance_multiplier;

	if(!fp) {
	cout << in_file << " is missing!\n";
	exit(-1);
	}

	while(fscanf(fp, "%[^\n]\n", line) != EOF) 
	{
		if (!strncmp("-C_g_ideal", line, strlen("-C_g_ideal"))) 
		{
		  C_g_ideal=scan_five_input_double(line,"-C_g_ideal","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-C_fringe", line, strlen("-C_fringe"))) 
		{
		  C_fringe=scan_five_input_double(line,"-C_fringe","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-C_junc_sw", line, strlen("-C_junc_sw"))) 
		{
		  C_junc_sidewall =scan_five_input_double(line,"-C_junc_sw","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-C_junc", line, strlen("-C_junc"))) 
		{
		  C_junc=scan_five_input_double(line,"-C_junc","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		
		if (!strncmp("-l_phy", line, strlen("-l_phy"))) 
		{
		  l_phy=scan_five_input_double(line,"-l_phy","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-l_elec", line, strlen("-l_elec"))) 
		{
		  l_elec=scan_five_input_double(line,"-l_elec","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-nmos_effective_resistance_multiplier", line, strlen("-nmos_effective_resistance_multiplier"))) 
		{
			nmos_effective_resistance_multiplier=scan_five_input_double(line,"-nmos_effective_resistance_multiplier","F/um",tech_flavor,g_ip->print_detail_debug);
			continue;
		}
		if (!strncmp("-Vdd", line, strlen("-Vdd"))) 
		{
		  Vdd=scan_five_input_double(line,"-Vdd","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-Vth", line, strlen("-Vth"))) 
		{
		  Vth=scan_five_input_double(line,"-Vth","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-Vdsat", line, strlen("-Vdsat"))) 
		{
		  Vdsat=scan_five_input_double(line,"-Vdsat","V",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-I_on_n", line, strlen("-I_on_n"))) 
		{
		  I_on_n=scan_five_input_double(line,"-I_on_n","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-I_on_p", line, strlen("-I_on_p"))) 
		{
		  I_on_p = scan_five_input_double(line,"-I_on_p","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-I_off_n", line, strlen("-I_off_n"))) 
		{
		  scan_five_input_double_temperature(line,"-I_off_n","F/um",tech_flavor,temperature,g_ip->print_detail_debug,I_off_n);
		  continue;
		}
		if (!strncmp("-I_g_on_n", line, strlen("-I_g_on_n"))) 
		{
		 scan_five_input_double_temperature(line,"-I_g_on_n","F/um",tech_flavor,temperature,g_ip->print_detail_debug,I_g_on_n);
		  continue;
		}
		if (!strncmp("-C_ox", line, strlen("-C_ox"))) 
		{
		  C_ox=scan_five_input_double(line,"-C_ox","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-t_ox", line, strlen("-t_ox"))) 
		{
		  t_ox=scan_five_input_double(line,"-t_ox","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-n2p_drv_rt", line, strlen("-n2p_drv_rt"))) 
		{
		  n_to_p_eff_curr_drv_ratio=scan_five_input_double(line,"-n2p_drv_rt","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-lch_lk_rdc", line, strlen("-lch_lk_rdc"))) 
		{
		  long_channel_leakage_reduction=scan_five_input_double(line,"-lch_lk_rdc","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-Mobility_n", line, strlen("-Mobility_n"))) 
		{
		  Mobility_n=scan_five_input_double(line,"-Mobility_n","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-gmp_to_gmn_multiplier", line, strlen("-gmp_to_gmn_multiplier"))) 
		{
		  gmp_to_gmn_multiplier=scan_five_input_double(line,"-gmp_to_gmn_multiplier","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-n_to_p_eff_curr_drv_ratio", line, strlen("-n_to_p_eff_curr_drv_ratio"))) 
		{
		  n_to_p_eff_curr_drv_ratio=scan_five_input_double(line,"-n_to_p_eff_curr_drv_ratio","F/um",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		
	} 

	C_overlap = 0.2*C_g_ideal;
	if(tech_flavor>=3)
		R_nch_on = nmos_effective_resistance_multiplier * g_tp.vpp / I_on_n;//ohm-micron
	else
		R_nch_on = nmos_effective_resistance_multiplier * Vdd / I_on_n;//ohm-micron
    R_pch_on = n_to_p_eff_curr_drv_ratio * R_nch_on;//ohm-micron
    I_off_p = I_off_n;
    I_g_on_p = I_g_on_n;
    if(g_ip->print_detail_debug)
    {
		///cout << nmos_effective_resistance_multiplier << " -- " << Vdd << " -- " << I_on_n << " -- " << n_to_p_eff_curr_drv_ratio << endl;
		cout << "C_overlap: " << C_overlap << " F/um" << endl;
		cout << "R_nch_on: " << R_nch_on << " ohm-micron" << endl;
		cout << "R_pch_on: " << R_pch_on << " ohm-micron" << endl;
	}
 
    fclose(fp);
    
}


void DeviceType::interpolate(double alpha, const DeviceType& dev1, const DeviceType& dev2)
{
	C_g_ideal = alpha*dev1.C_g_ideal+(1-alpha)*dev2.C_g_ideal;
	C_fringe  = alpha*dev1.C_fringe+(1-alpha)*dev2.C_fringe;
	C_overlap = alpha*dev1.C_overlap+(1-alpha)*dev2.C_overlap;
	C_junc    = alpha*dev1.C_junc+(1-alpha)*dev2.C_junc;
	l_phy     = alpha*dev1.l_phy+(1-alpha)*dev2.l_phy;
	l_elec    = alpha*dev1.l_elec+(1-alpha)*dev2.l_elec;
	R_nch_on  = alpha*dev1.R_nch_on+(1-alpha)*dev2.R_nch_on;
	R_pch_on  = alpha*dev1.R_pch_on+(1-alpha)*dev2.R_pch_on;
	Vdd       = alpha*dev1.Vdd+(1-alpha)*dev2.Vdd;
	Vth       = alpha*dev1.Vth+(1-alpha)*dev2.Vth;
	Vcc_min   = alpha*dev1.Vcc_min+(1-alpha)*dev2.Vcc_min;
	I_on_n    = alpha*dev1.I_on_n+(1-alpha)*dev2.I_on_n;
	I_on_p    = alpha*dev1.I_on_p+(1-alpha)*dev2.I_on_p;
	I_off_n   = alpha*dev1.I_off_n+(1-alpha)*dev2.I_off_n;
	I_off_p   = alpha*dev1.I_off_p+(1-alpha)*dev2.I_off_p;
	I_g_on_n   = alpha*dev1.I_g_on_n+(1-alpha)*dev2.I_g_on_n;
	I_g_on_p   = alpha*dev1.I_g_on_p+(1-alpha)*dev2.I_g_on_p;
	C_ox      = alpha*dev1.C_ox+(1-alpha)*dev2.C_ox;
	t_ox      = alpha*dev1.t_ox+(1-alpha)*dev2.t_ox;
	n_to_p_eff_curr_drv_ratio = alpha*dev1.n_to_p_eff_curr_drv_ratio+(1-alpha)*dev2.n_to_p_eff_curr_drv_ratio;
	long_channel_leakage_reduction = alpha*dev1.long_channel_leakage_reduction+(1-alpha)*dev2.long_channel_leakage_reduction;
	Mobility_n = alpha*dev1.Mobility_n+(1-alpha)*dev2.Mobility_n;
	Vdsat = alpha*dev1.Vdsat + (1-alpha)*dev2.Vdsat;
	gmp_to_gmn_multiplier = alpha*dev1.gmp_to_gmn_multiplier + (1-alpha)*dev2.gmp_to_gmn_multiplier;
	n_to_p_eff_curr_drv_ratio = alpha*dev1.n_to_p_eff_curr_drv_ratio + (1-alpha)*dev2.n_to_p_eff_curr_drv_ratio;
	
	C_junc_sidewall = dev1.C_junc_sidewall;
}


double scan_input_double_inter_type(char* line, const char * name, const char * unit_name, int proj_type, int tech_flavor, bool print)
{
	assert(proj_type<NUMBER_INTERCONNECT_PROJECTION_TYPES);
	int index = proj_type*NUMBER_WIRE_TYPES + tech_flavor;
	//cout << name << " index: " << index << endl;
	double temp[8];
	char unit[300];
	memset(unit,0,300);
	sscanf(&line[strlen(name)], "%*[ \t]%s%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf"
		,unit,&(temp[0]),&(temp[1]),&(temp[2]),&(temp[3]), &(temp[4]),&(temp[5]),&(temp[6]),&(temp[7]));
		
	if (print)
		cout << name << " " << temp[index] << " " << unit << endl;
  
  return temp[index];
}

void InterconnectType::assign(const string & in_file, int projection_type, int tech_flavor)
{
	
	FILE *fp = fopen(in_file.c_str(), "r");
	char line[5000];
	//char temp_var[5000];

	//double temp[5];
	//unsigned int thermal_temp;

	//double nmos_effective_resistance_multiplier;

	double resistivity;
 
	if(!fp) {
	cout << in_file << " is missing!\n";
	exit(-1);
	}

	bool print = g_ip->print_detail_debug;

	while(fscanf(fp, "%[^\n]\n", line) != EOF) 
	{
		if (!strncmp("-wire_pitch", line, strlen("-wire_pitch"))) 
		{
		  pitch =scan_input_double_inter_type(line,"-wire_pitch","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-barrier_thickness", line, strlen("-barrier_thickness"))) 
		{
		  barrier_thickness =scan_input_double_inter_type(line,"-barrier_thickness","ohm",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-dishing_thickness", line, strlen("-dishing_thickness"))) 
		{
		  dishing_thickness =scan_input_double_inter_type(line,"-dishing_thickness","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-alpha_scatter", line, strlen("-alpha_scatter"))) 
		{
		  alpha_scatter =scan_input_double_inter_type(line,"-alpha_scatter","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-aspect_ratio", line, strlen("-aspect_ratio"))) 
		{
		  aspect_ratio =scan_input_double_inter_type(line,"-aspect_ratio","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-miller_value", line, strlen("-miller_value"))) 
		{
		  miller_value =scan_input_double_inter_type(line,"-miller_value","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-horiz_dielectric_constant", line, strlen("-horiz_dielectric_constant"))) 
		{
		  horiz_dielectric_constant =scan_input_double_inter_type(line,"-horiz_dielectric_constant","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-vert_dielectric_constant", line, strlen("-vert_dielectric_constant"))) 
		{
		  vert_dielectric_constant =scan_input_double_inter_type(line,"-vert_dielectric_constant","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-ild_thickness", line, strlen("-ild_thickness"))) 
		{
		  ild_thickness =scan_input_double_inter_type(line,"-ild_thickness","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-fringe_cap", line, strlen("-fringe_cap"))) 
		{
		  fringe_cap =scan_input_double_inter_type(line,"-fringe_cap","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-wire_r_per_micron", line, strlen("-wire_r_per_micron"))) 
		{
		  R_per_um =scan_input_double_inter_type(line,"-wire_r_per_micron","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-wire_c_per_micron", line, strlen("-wire_c_per_micron"))) 
		{
		  C_per_um =scan_input_double_inter_type(line,"-wire_c_per_micron","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
		if (!strncmp("-resistivity", line, strlen("-resistivity"))) 
		{
		  resistivity =scan_input_double_inter_type(line,"-resistivity","um",g_ip->ic_proj_type,tech_flavor,print);
		  continue;
		}
	}
	
	pitch *= g_ip->F_sz_um;
	wire_width = pitch/ 2; //micron
    wire_thickness = aspect_ratio * wire_width;//micron
    wire_spacing = pitch - wire_width;//micron
    if((projection_type!=1) || (tech_flavor!=3))
    {
		R_per_um = wire_resistance(resistivity, wire_width,
					wire_thickness, barrier_thickness, dishing_thickness, alpha_scatter);//ohm/micron
		if(print)
			cout << R_per_um << " = wire_resistance(" << resistivity << "," << wire_width << "," << 
				wire_thickness << "," << barrier_thickness << "," << dishing_thickness << "," << alpha_scatter << ")\n";
		
		
		C_per_um = wire_capacitance(wire_width, wire_thickness, wire_spacing,
					ild_thickness, miller_value, horiz_dielectric_constant,
					vert_dielectric_constant, fringe_cap);//F/micron.		
		if(print)
			cout << C_per_um << " = wire_capacitance(" << wire_width << "," << wire_thickness << "," << wire_spacing
				<< "," <<  ild_thickness << "," << miller_value << "," << horiz_dielectric_constant
				<< "," << vert_dielectric_constant << "," << fringe_cap << ")\n";
			
	}			
	fclose(fp);
}

bool InterconnectType::isEqual(const InterconnectType & inter)
{
	if(  !is_equal(pitch , inter.pitch)) {display(0); assert(false);}
	if(  !is_equal(R_per_um , inter.R_per_um)) {display(0); assert(false);}
	if(  !is_equal(C_per_um , inter.C_per_um)) {display(0); assert(false);}
	if(  !is_equal(horiz_dielectric_constant , inter.horiz_dielectric_constant)) {display(0); assert(false);}
	if(  !is_equal(vert_dielectric_constant , inter.vert_dielectric_constant)) {display(0); assert(false);}
	if(  !is_equal(aspect_ratio , inter.aspect_ratio)) {display(0); assert(false);}
	if(  !is_equal(miller_value , inter.miller_value)) {display(0); assert(false);}
	if(  !is_equal(ild_thickness , inter.ild_thickness)) {display(0); assert(false);}

	//auxilary parameters
	///if(  !is_equal(wire_width , inter.wire_width)) {display(0); assert(false);}
	///if(  !is_equal(wire_thickness , inter.wire_thickness)) {display(0); assert(false);}
	///if(  !is_equal(wire_spacing , inter.wire_spacing)) {display(0); assert(false);}
	///if(  !is_equal(barrier_thickness , inter.barrier_thickness)) {display(0); assert(false);}
	///if(  !is_equal(dishing_thickness , inter.dishing_thickness)) {display(0); assert(false);}
	///if(  !is_equal(alpha_scatter , inter.alpha_scatter)) {display(0); assert(false);}
	///if(  !is_equal(fringe_cap , inter.fringe_cap)) {display(0); assert(false);}
	
	return true;
}

void InterconnectType::interpolate(double alpha, const InterconnectType & inter1, const InterconnectType & inter2)
{
	pitch = alpha*inter1.pitch + (1-alpha)*inter2.pitch;
	R_per_um = alpha*inter1.R_per_um + (1-alpha)*inter2.R_per_um;
	C_per_um = alpha*inter1.C_per_um + (1-alpha)*inter2.C_per_um;
	horiz_dielectric_constant = alpha*inter1.horiz_dielectric_constant + (1-alpha)*inter2.horiz_dielectric_constant;
	vert_dielectric_constant = alpha*inter1.vert_dielectric_constant + (1-alpha)*inter2.vert_dielectric_constant;
	aspect_ratio = alpha*inter1.aspect_ratio + (1-alpha)*inter2.aspect_ratio;
	miller_value = alpha*inter1.miller_value + (1-alpha)*inter2.miller_value;
	ild_thickness = alpha*inter1.ild_thickness + (1-alpha)*inter2.ild_thickness;
	
}

void scan_five_input_double_mem_type(char* line, const char* name, const char* unit_name, int flavor, int cell_type, bool print, double & result)
{
  double temp[5];	
  int cell_type_temp;
  char unit[300];
  memset(unit,0,300);
  
  sscanf(&line[strlen(name)], "%*[ \t]%s%*[ \t]%d%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf"
		,unit,&cell_type_temp,&(temp[0]),&(temp[1]),&(temp[2]),&(temp[3]), &(temp[4]) );
		
 
  if(cell_type_temp==cell_type)
  {
	   if (print)
		cout << name << ": " << temp[flavor] << " "<< unit << endl;
			
	result = temp[flavor];
  }
}

// cell_type --> sram(0),cam(1),dram(2)
void MemoryType::assign(const string & in_file, int tech_flavor, int cell_type) 
{
	FILE *fp = fopen(in_file.c_str(), "r");
	char line[5000];
	//char temp_var[5000];

	//double temp;
	//unsigned int thermal_temp;

	double vdd_cell,vdd;
	
	if(!fp) {
	cout << in_file << " is missing!\n";
	exit(-1);
	}
	while(fscanf(fp, "%[^\n]\n", line) != EOF) 
	{
		if (!strncmp("-Vdd", line, strlen("-Vdd"))) 
		{
		  vdd=scan_five_input_double(line,"-Vdd","V",tech_flavor,g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-vdd_cell", line, strlen("-vdd_cell"))) 
		{
		  scan_five_input_double_mem_type(line,"-vdd_cell","V",tech_flavor,cell_type, g_ip->print_detail_debug,vdd_cell);
		  continue;
		}
		if (!strncmp("-Wmemcella", line, strlen("-Wmemcella"))) 
		{
		  scan_five_input_double_mem_type(line,"-Wmemcella","V",tech_flavor,cell_type, g_ip->print_detail_debug,cell_a_w);
		  continue;
		}
		if (!strncmp("-Wmemcellpmos", line, strlen("-Wmemcellpmos"))) 
		{
		  scan_five_input_double_mem_type(line,"-Wmemcellpmos","V",tech_flavor,cell_type, g_ip->print_detail_debug,cell_pmos_w);
		  continue;
		}
		if (!strncmp("-Wmemcellnmos", line, strlen("-Wmemcellnmos"))) 
		{
		  scan_five_input_double_mem_type(line,"-Wmemcellnmos","V",tech_flavor,cell_type, g_ip->print_detail_debug,cell_nmos_w);
		  continue;
		}
		if (!strncmp("-area_cell", line, strlen("-area_cell"))) 
		{
		  scan_five_input_double_mem_type(line,"-area_cell","V",tech_flavor,cell_type, g_ip->print_detail_debug,area_cell);
		  continue;
		}
		if (!strncmp("-asp_ratio_cell", line, strlen("-asp_ratio_cell"))) 
		{
		  scan_five_input_double_mem_type(line,"-asp_ratio_cell","V",tech_flavor,cell_type, g_ip->print_detail_debug,asp_ratio_cell);
		  continue;
		}
	}
	if(cell_type!=2)
		cell_a_w *= g_ip->F_sz_um;
	cell_pmos_w *= g_ip->F_sz_um;
	cell_nmos_w *= g_ip->F_sz_um;
	if(cell_type!=2)
		area_cell *= (g_ip->F_sz_um* g_ip->F_sz_um);
	///assert(asp_ratio_cell!=0);
	b_w = sqrt(area_cell / (asp_ratio_cell));
	b_h = asp_ratio_cell * b_w;
	if(cell_type==2) //dram
		Vbitpre = vdd_cell;
	else // sram or cam
		Vbitpre = vdd;
		
	
    Vbitfloating = Vbitpre*0.7;

	//display(5);
	
}

void MemoryType::interpolate(double alpha, const MemoryType& mem1, const MemoryType& mem2)
{
	cell_a_w    = alpha * mem1.cell_a_w + (1-alpha) * mem2.cell_a_w;
	cell_pmos_w    = alpha * mem1.cell_pmos_w + (1-alpha) * mem2.cell_pmos_w;
	cell_nmos_w    = alpha * mem1.cell_nmos_w + (1-alpha) * mem2.cell_nmos_w;

    area_cell = alpha * mem1.area_cell + (1-alpha) * mem2.area_cell;
    asp_ratio_cell = alpha * mem1.asp_ratio_cell + (1-alpha) * mem2.asp_ratio_cell;
    
    Vbitpre = mem2.Vbitpre;
    Vbitfloating = Vbitpre*0.7;
    // updating dependant variables after scaling/interpolating
    ///assert(asp_ratio_cell!=0);
	b_w = sqrt(area_cell / (asp_ratio_cell));
	b_h = asp_ratio_cell * b_w;
	//display(10);
}

bool MemoryType::isEqual(const MemoryType & mem)
{
	if( !is_equal(b_w , mem.b_w)) {display(0); cout << "\n\n\n"; mem.display(0); assert(false);}
	if( !is_equal(b_h , mem.b_h)) {display(0); cout << "\n\n\n"; mem.display(0); assert(false);}
	if( !is_equal(cell_a_w , mem.cell_a_w)) {display(0); cout << "\n\n\n"; mem.display(0); assert(false);}
	if( !is_equal(cell_pmos_w , mem.cell_pmos_w)) {display(0); cout << "\n\n\n"; mem.display(0); assert(false);}
	if( !is_equal(cell_nmos_w , mem.cell_nmos_w)) {display(0); cout << "\n\n\n"; mem.display(0); assert(false);}
	if( !is_equal(Vbitpre , mem.Vbitpre)) {display(0); cout << "\n\n\n"; mem.display(0); assert(false);}
	///if( !is_equal(Vbitfloating , mem.Vbitfloating)) {display(0); cout << "\n\n\n"; mem.display(0); assert(false);}
	
	// needed to calculate b_w b_h
	///if( !is_equal(area_cell , mem.area_cell)) {display(0); assert(false);}
	///if( !is_equal(asp_ratio_cell , mem.asp_ratio_cell)) {display(0); assert(false);}
	
	return true;
}

void ScalingFactor::assign(const string & in_file)
{
	FILE *fp = fopen(in_file.c_str(), "r");
	char line[5000];
	//char temp_var[5000];
	if(!fp) 
	{
		cout << in_file << " is missing!\n";
		exit(-1);
	}

	while(fscanf(fp, "%[^\n]\n", line) != EOF) 
	{
		if (!strncmp("-logic_scaling_co_eff", line, strlen("-logic_scaling_co_eff"))) 
		{
		  logic_scaling_co_eff = scan_single_input_double(line,"-logic_scaling_co_eff","F/um", g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-core_tx_density", line, strlen("-core_tx_density"))) 
		{
		  core_tx_density = scan_single_input_double(line,"-core_tx_density","F/um", g_ip->print_detail_debug);
		  continue;
		}
		
	}

	fclose(fp);
}

void ScalingFactor::interpolate(double alpha, const ScalingFactor& dev1, const ScalingFactor& dev2)
{
	logic_scaling_co_eff = alpha*dev1.logic_scaling_co_eff + (1-alpha)*dev2.logic_scaling_co_eff;
	core_tx_density = alpha*dev1.core_tx_density + (1-alpha)*dev2.core_tx_density;
}

bool ScalingFactor::isEqual(const ScalingFactor &  scal)
{
	if( !is_equal(logic_scaling_co_eff,scal.logic_scaling_co_eff)) { display(0); assert(false);}
	if( !is_equal(core_tx_density,scal.core_tx_density)) { display(0); assert(false);}
	if( !is_equal(long_channel_leakage_reduction , scal.long_channel_leakage_reduction)) { display(0); assert(false);}
	return true;
}

void TechnologyParameter::find_upper_and_lower_tech(double technology, int &tech_lo, string& in_file_lo, int &tech_hi, string& in_file_hi)
{
	if (technology < 181 && technology > 179)
	{
		tech_lo = 180;
		in_file_lo = "tech_params/180nm.dat";
		tech_hi = 180;
		in_file_hi = "tech_params/180nm.dat"; 
	}
	else if (technology < 91 && technology > 89)
	{
		tech_lo = 90;
		in_file_lo = "tech_params/90nm.dat";
		tech_hi = 90;
		in_file_hi = "tech_params/90nm.dat";
	}
	else if (technology < 66 && technology > 64)
	{
		tech_lo = 65;
		in_file_lo = "tech_params/65nm.dat";
		tech_hi = 65;
		in_file_hi = "tech_params/65nm.dat";
	}
	else if (technology < 46 && technology > 44)
	{
		tech_lo = 45;
		in_file_lo = "tech_params/45nm.dat";
		tech_hi = 45;
		in_file_hi = "tech_params/45nm.dat";
	}
	else if (technology < 33 && technology > 31)
	{
		tech_lo = 32;
		in_file_lo = "tech_params/32nm.dat";
		tech_hi = 32;
		in_file_hi = "tech_params/32nm.dat";
	}
	else if (technology < 23 && technology > 21)
	{
		tech_lo = 22;
		in_file_lo = "tech_params/22nm.dat";
		tech_hi = 22;
		in_file_hi = "tech_params/22nm.dat";
	}
	else if (technology < 180 && technology > 90)
    {
		tech_lo = 180;
		in_file_lo = "tech_params/180nm.dat";
		tech_hi = 90;
		in_file_hi = "tech_params/90nm.dat";
    }
	else if (technology < 90 && technology > 65)
	{
		tech_lo = 90;
		in_file_lo = "tech_params/90nm.dat";
		tech_hi = 65;
		in_file_hi = "tech_params/65nm.dat";
	}
	else if (technology < 65 && technology > 45)
	{
		tech_lo = 65;
		in_file_lo = "tech_params/65nm.dat";
		tech_hi = 45;
		in_file_hi = "tech_params/45nm.dat";
	}
	else if (technology < 45 && technology > 32)
	{
		tech_lo = 45;
		in_file_lo = "tech_params/45nm.dat";
		tech_hi = 32;
		in_file_hi = "tech_params/32nm.dat";
	}
	else if (technology < 32 && technology > 22)
	{
		tech_lo = 32;
		in_file_lo = "tech_params/32nm.dat";
		tech_hi = 22;
		in_file_hi = "tech_params/22nm.dat";
	}
	/**
	else if (technology < 22 && technology > 16)
	{
		tech_lo = 22;
		in_file_lo = "tech_params/22nm.dat";
		tech_hi = 16;
		in_file_hi = "tech_params/16nm.dat";
	}
	**/
	else
	{
		cout<<"Invalid technology nodes"<<endl;
		exit(0);
	}
}


double scan_input_double_tsv_type(char* line, const char * name, const char * unit_name, int proj_type, int tsv_type, bool print)
{
	assert(proj_type<NUMBER_INTERCONNECT_PROJECTION_TYPES);
	int index = proj_type*NUMBER_TSV_TYPES + tsv_type;
	double temp[6];
	char unit[300];
	memset(unit,0,300);
	sscanf(&line[strlen(name)], "%*[ \t]%s%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf%*[ \t]%lf"
		,unit,&(temp[0]),&(temp[1]),&(temp[2]),&(temp[3]), &(temp[4]),&(temp[5]));
		
    if (print)
		cout << name << ": " << temp[index] << " " << unit << endl;
  return temp[index];
}

void TechnologyParameter::assign_tsv(const string & in_file)
{
	FILE *fp ;
	char line[5000];
	for(int iter=0;iter<2;iter++) // 0:fine 1:coarse
	{
		int tsv_type;
		if(iter==0)
		{
			tsv_type = g_ip->tsv_is_subarray_type;
		}
		else
		{
			tsv_type = g_ip->tsv_os_bank_type;
		}
		fp = fopen(in_file.c_str(), "r");
		while(fscanf(fp, "%[^\n]\n", line) != EOF) 
		{
			if (!strncmp("-tsv_pitch", line, strlen("-tsv_pitch"))) 
			{
			  tsv_pitch = scan_input_double_tsv_type(line,"-tsv_pitch","F/um", g_ip->ic_proj_type, tsv_type, g_ip->print_detail_debug);
			  continue;
			}
			if (!strncmp("-tsv_diameter", line, strlen("-tsv_diameter"))) 
			{
			  tsv_diameter = scan_input_double_tsv_type(line,"-tsv_diameter","F/um", g_ip->ic_proj_type, tsv_type, g_ip->print_detail_debug);
			  continue;
			}
			if (!strncmp("-tsv_length", line, strlen("-tsv_length"))) 
			{
			  tsv_length = scan_input_double_tsv_type(line,"-tsv_length","F/um", g_ip->ic_proj_type, tsv_type, g_ip->print_detail_debug);
			  continue;
			}
			if (!strncmp("-tsv_dielec_thickness", line, strlen("-tsv_dielec_thickness"))) 
			{
			  tsv_dielec_thickness = scan_input_double_tsv_type(line,"-tsv_dielec_thickness","F/um", g_ip->ic_proj_type, tsv_type, g_ip->print_detail_debug);
			  continue;
			}
			if (!strncmp("-tsv_contact_resistance", line, strlen("-tsv_contact_resistance"))) 
			{
			  tsv_contact_resistance = scan_input_double_tsv_type(line,"-tsv_contact_resistance","F/um", g_ip->ic_proj_type, tsv_type, g_ip->print_detail_debug);
			  continue;
			}
			if (!strncmp("-tsv_depletion_width", line, strlen("-tsv_depletion_width"))) 
			{
			  tsv_depletion_width = scan_input_double_tsv_type(line,"-tsv_depletion_width","F/um", g_ip->ic_proj_type, tsv_type, g_ip->print_detail_debug);
			  continue;
			}
			if (!strncmp("-tsv_liner_dielectric_cons", line, strlen("-tsv_liner_dielectric_cons"))) 
			{
			  tsv_liner_dielectric_constant = scan_input_double_tsv_type(line,"-tsv_liner_dielectric_cons","F/um", g_ip->ic_proj_type, tsv_type, g_ip->print_detail_debug);
			  continue;
			}
			
			tsv_length *= g_ip->num_die_3d;
			if(iter==0)
			{
				tsv_parasitic_resistance_fine = tsv_resistance(BULK_CU_RESISTIVITY, tsv_length, tsv_diameter, tsv_contact_resistance);
				tsv_parasitic_capacitance_fine = tsv_capacitance(tsv_length, tsv_diameter, tsv_pitch, tsv_dielec_thickness, tsv_liner_dielectric_constant, tsv_depletion_width);
				tsv_minimum_area_fine = tsv_area(tsv_pitch);
			}
			else
			{
				tsv_parasitic_resistance_coarse = tsv_resistance(BULK_CU_RESISTIVITY, tsv_length, tsv_diameter, tsv_contact_resistance);
				tsv_parasitic_capacitance_coarse = tsv_capacitance(tsv_length, tsv_diameter, tsv_pitch, tsv_dielec_thickness, tsv_liner_dielectric_constant, tsv_depletion_width);
				tsv_minimum_area_coarse = tsv_area(tsv_pitch);
			}
		}
		
		fclose(fp);
	}
}

void TechnologyParameter::init(double technology, bool is_tag)
{
	FILE *fp ;
	reset();
	char line[5000];
	//char temp_var[5000];
	
	uint32_t ram_cell_tech_type    = (is_tag) ? g_ip->tag_arr_ram_cell_tech_type : g_ip->data_arr_ram_cell_tech_type;
    uint32_t peri_global_tech_type = (is_tag) ? g_ip->tag_arr_peri_global_tech_type : g_ip->data_arr_peri_global_tech_type;
	
	int tech_lo, tech_hi; 
	string in_file_lo, in_file_hi;
	
	double alpha; // used for technology interpolation
	
	


	technology  = technology * 1000.0;  // in the unit of nm
	
	find_upper_and_lower_tech(technology, tech_lo,in_file_lo,tech_hi,in_file_hi);
	// excluding some cases.
	if((tech_lo==22) && (tech_hi==22))
	{
		if (ram_cell_tech_type == 3 )
		{
		   cout<<"current version does not support eDRAM technologies at 22nm"<<endl;
		   exit(0);
		}
	}
	
	if(tech_lo==tech_hi)
	{
		alpha = 1;
	}
	else
	{
		alpha = (technology - tech_hi)/(tech_lo - tech_hi);
	}
	
	fp = fopen(in_file_lo.c_str(), "r");
	dram_cell_I_on = 0;
	dram_cell_Vdd = 0;
	dram_cell_C = 0;
	dram_cell_I_off_worst_case_len_temp = 0;
	vpp = 0 ;
	macro_layout_overhead = 0;
	chip_layout_overhead =0;
	sckt_co_eff = 0;
	while(fscanf(fp, "%[^\n]\n", line) != EOF) 
	{
		if (!strncmp("-dram_cell_I_on", line, strlen("-dram_cell_I_on"))) 
		{
		  dram_cell_I_on += alpha*scan_five_input_double(line,"-dram_cell_I_on","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-dram_cell_Vdd", line, strlen("-dram_cell_Vdd"))) 
		{
		  dram_cell_Vdd += alpha* scan_five_input_double(line,"-dram_cell_Vdd","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-dram_cell_C", line, strlen("-dram_cell_C"))) 
		{
		  dram_cell_C += alpha* scan_five_input_double(line,"-dram_cell_C","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-dram_cell_I_off_worst_case_len_temp", line, strlen("-dram_cell_I_off_worst_case_len_temp"))) 
		{
		  dram_cell_I_off_worst_case_len_temp += alpha* scan_five_input_double(line,"-dram_cell_I_off_worst_case_len_temp","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-vpp", line, strlen("-vpp"))) 
		{
		  vpp += alpha* scan_five_input_double(line,"-vpp","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-sckt_co_eff", line, strlen("-sckt_co_eff"))) 
		{
		  sckt_co_eff += alpha * scan_single_input_double(line,"-sckt_co_eff","F/um", g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-chip_layout_overhead", line, strlen("-chip_layout_overhead"))) 
		{
		  chip_layout_overhead += alpha * scan_single_input_double(line,"-chip_layout_overhead","F/um", g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-macro_layout_overhead", line, strlen("-macro_layout_overhead"))) 
		{
		  macro_layout_overhead += alpha * scan_single_input_double(line,"-macro_layout_overhead","F/um", g_ip->print_detail_debug);
		  continue;
		}
	}
	fclose(fp);
	
	
	DeviceType peri_global_lo, peri_global_hi;
	peri_global_lo.assign(in_file_lo, peri_global_tech_type, g_ip->temp);
	peri_global_hi.assign(in_file_hi, peri_global_tech_type, g_ip->temp);
	peri_global.interpolate(alpha,peri_global_lo,peri_global_hi);
	// in the original code some field of this devide has not been initialized/
	// I make them 0 for compatibility.
	///peri_global.I_on_p = 0.0;
	
	DeviceType sleep_tx_lo, sleep_tx_hi;
	sleep_tx_lo.assign(in_file_lo, 1, g_ip->temp);
	sleep_tx_hi.assign(in_file_hi, 1, g_ip->temp);
	sleep_tx.interpolate(alpha, sleep_tx_lo, sleep_tx_hi);
	
	
	DeviceType sram_cell_lo, sram_cell_hi;
	sram_cell_lo.assign(in_file_lo, ram_cell_tech_type, g_ip->temp);
	sram_cell_hi.assign(in_file_hi, ram_cell_tech_type, g_ip->temp);
	sram_cell.interpolate(alpha, sram_cell_lo, sram_cell_hi);
	// in the original code some field of this devide has not been initialized/
	// I make them 0 for compatibility.
	//sram_cell.Vdd=0.0;
	///sram_cell.I_on_p=0.0;
	///sram_cell.C_ox=0.0;
	
	
	DeviceType dram_acc_lo, dram_acc_hi;
	dram_acc_lo.assign(in_file_lo, (ram_cell_tech_type==comm_dram? ram_cell_tech_type:dram_cell_tech_flavor), g_ip->temp);
	dram_acc_hi.assign(in_file_hi, (ram_cell_tech_type==comm_dram? ram_cell_tech_type:dram_cell_tech_flavor), g_ip->temp);
	dram_acc.interpolate(alpha, dram_acc_lo, dram_acc_hi);
	// dram_acc exceptions
	//dram_acc.R_nch_on = g_tp.dram_cell_Vdd / g_tp.dram_acc.I_on_n;
	//dram_acc.R_pch_on = 0;
	if(tech_lo<=22)
	{
	}
	else if(tech_lo<=32)
	{
		if(ram_cell_tech_type == lp_dram)
			dram_acc.Vth = 0.44129;
		else
			dram_acc.Vth = 1.0;
	}
	else if(tech_lo<=45)
	{
		if(ram_cell_tech_type == lp_dram)
			dram_acc.Vth = 0.44559;
		else
			dram_acc.Vth = 1.0;
	}
	else if(tech_lo<=65)
	{
		if(ram_cell_tech_type == lp_dram)
			dram_acc.Vth = 0.43806;
		else
			dram_acc.Vth = 1.0;
	}
	else if(tech_lo<=90)
	{
		if(ram_cell_tech_type == lp_dram)
			dram_acc.Vth = 0.4545;
		else
			dram_acc.Vth = 1.0;
	}
	// in the original code some field of this devide has not been initialized/
	// I make them 0 for compatibility.
	dram_acc.Vdd= 0.0;
	dram_acc.I_on_p = 0.0;
	dram_acc.I_off_n = 0.0;
	dram_acc.I_off_p = 0.0;
	dram_acc.C_ox = 0.0;
	dram_acc.t_ox = 0.0;
	dram_acc.n_to_p_eff_curr_drv_ratio = 0.0;
	
	DeviceType dram_wl_lo, dram_wl_hi;
	dram_wl_lo.assign(in_file_lo, (ram_cell_tech_type==comm_dram? ram_cell_tech_type:dram_cell_tech_flavor), g_ip->temp);
	dram_wl_hi.assign(in_file_hi, (ram_cell_tech_type==comm_dram? ram_cell_tech_type:dram_cell_tech_flavor), g_ip->temp);
	dram_wl.interpolate(alpha, dram_wl_lo, dram_wl_hi);
	// in the original code some field of this devide has not been initialized/
	// I make them 0 for compatibility.
	dram_wl.Vdd = 0.0;
	dram_wl.Vth = 0.0;
	dram_wl.I_on_p = 0.0;
	dram_wl.C_ox = 0.0;
	dram_wl.t_ox = 0.0;
	
	// if ram_cell_tech_type is not 3 or 4 ( which means edram and comm-dram)
	// then reset dram_wl dram_acc
	
	if(ram_cell_tech_type <3)
	{
		dram_acc.reset();
		dram_wl.reset();
	}
	

	DeviceType cam_cell_lo, cam_cell_hi;
	cam_cell_lo.assign(in_file_lo, ram_cell_tech_type, g_ip->temp);
	cam_cell_hi.assign(in_file_hi, ram_cell_tech_type, g_ip->temp);
	cam_cell.interpolate(alpha, cam_cell_lo, cam_cell_hi);
	
	MemoryType dram_lo, dram_hi;
	dram_lo.assign(in_file_lo, ram_cell_tech_type, 2); // cell_type = dram(2)
	dram_hi.assign(in_file_hi, ram_cell_tech_type, 2);
	dram.interpolate(alpha,dram_lo,dram_hi);
	
	MemoryType sram_lo, sram_hi;
	sram_lo.assign(in_file_lo, ram_cell_tech_type, 0); // cell_type = sram(0)
	sram_hi.assign(in_file_hi, ram_cell_tech_type, 0);
	sram.interpolate(alpha,sram_lo,sram_hi);
	// sram cell execptions
	/*sram_lo.assign(in_file_lo, 0, g_ip->temp);
	sram.cell_a_w =sram_lo.cell_a_w;
	sram.b_h = sram_lo.b_h;
	sram.b_w = sram_lo.b_w;
*/
	MemoryType cam_lo, cam_hi;
	cam_lo.assign(in_file_lo, ram_cell_tech_type, 1); // cell_type = sram(0)
	cam_hi.assign(in_file_hi, ram_cell_tech_type, 1);
	cam.interpolate(alpha,cam_lo,cam_hi);
	
	
	ScalingFactor scaling_factor_lo, scaling_factor_hi;
	scaling_factor_lo.assign(in_file_lo);
	scaling_factor_hi.assign(in_file_hi);
	scaling_factor.interpolate(alpha, scaling_factor_lo,scaling_factor_hi);
	
	//vcc_min
	peri_global.Vcc_min   += (alpha * peri_global_lo.Vdd + (1-alpha)*peri_global_hi.Vdd) * 0.35;
	sleep_tx.Vcc_min   += (alpha*sleep_tx_lo.Vdd+(1-alpha)*sleep_tx_hi.Vdd);
	sram_cell.Vcc_min  += (alpha*sram_cell_lo.Vdd +(1-alpha)*sram_cell_hi.Vdd)* 0.65;
	
	
	
	fp = fopen(in_file_hi.c_str(), "r");
	
	while(fscanf(fp, "%[^\n]\n", line) != EOF) 
	{
		if (!strncmp("-sense_delay", line, strlen("-sense_delay"))) 
		{
		  sense_delay = scan_single_input_double(line,"-sense_delay","F/um", g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-sense_dy_power", line, strlen("-sense_dy_power"))) 
		{
		  sense_dy_power = scan_single_input_double(line,"-sense_dy_power","F/um", g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-sckt_co_eff", line, strlen("-sckt_co_eff"))) 
		{
		  sckt_co_eff += (1-alpha)* scan_single_input_double(line,"-sckt_co_eff","F/um", g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-chip_layout_overhead", line, strlen("-chip_layout_overhead"))) 
		{
		  chip_layout_overhead += (1-alpha)* scan_single_input_double(line,"-chip_layout_overhead","F/um", g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-macro_layout_overhead", line, strlen("-macro_layout_overhead"))) 
		{
		  macro_layout_overhead += (1-alpha)* scan_single_input_double(line,"-macro_layout_overhead","F/um", g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-dram_cell_I_on", line, strlen("-dram_cell_I_on"))) 
		{
		  dram_cell_I_on += (1-alpha) * scan_five_input_double(line,"-dram_cell_I_on","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-dram_cell_Vdd", line, strlen("-dram_cell_Vdd"))) 
		{
		  dram_cell_Vdd += (1-alpha) * scan_five_input_double(line,"-dram_cell_Vdd","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-dram_cell_C", line, strlen("-dram_cell_C"))) 
		{
		  dram_cell_C += (1-alpha) * scan_five_input_double(line,"-dram_cell_C","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-dram_cell_I_off_worst_case_len_temp", line, strlen("-dram_cell_I_off_worst_case_len_temp"))) 
		{
		  dram_cell_I_off_worst_case_len_temp += (1-alpha) * scan_five_input_double(line,"-dram_cell_I_off_worst_case_len_temp","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
		if (!strncmp("-vpp", line, strlen("-vpp"))) 
		{
		  vpp += (1-alpha)* scan_five_input_double(line,"-vpp","F/um", ram_cell_tech_type, g_ip->print_detail_debug);
		  continue;
		}
	}
	fclose(fp);
	
	//Currently we are not modeling the resistance/capacitance of poly anywhere.
	//Continuous function (or date have been processed) does not need linear interpolation
	w_comp_inv_p1 = 12.5 * g_ip->F_sz_um;//this was 10 micron for the 0.8 micron process
	w_comp_inv_n1 =  7.5 * g_ip->F_sz_um;//this was  6 micron for the 0.8 micron process
	w_comp_inv_p2 =   25 * g_ip->F_sz_um;//this was 20 micron for the 0.8 micron process
	w_comp_inv_n2 =   15 * g_ip->F_sz_um;//this was 12 micron for the 0.8 micron process
	w_comp_inv_p3 =   50 * g_ip->F_sz_um;//this was 40 micron for the 0.8 micron process
	w_comp_inv_n3 =   30 * g_ip->F_sz_um;//this was 24 micron for the 0.8 micron process
	w_eval_inv_p  =  100 * g_ip->F_sz_um;//this was 80 micron for the 0.8 micron process
	w_eval_inv_n  =   50 * g_ip->F_sz_um;//this was 40 micron for the 0.8 micron process
	w_comp_n     = 12.5 * g_ip->F_sz_um;//this was 10 micron for the 0.8 micron process
	w_comp_p     = 37.5 * g_ip->F_sz_um;//this was 30 micron for the 0.8 micron process

	MIN_GAP_BET_P_AND_N_DIFFS = 5 * g_ip->F_sz_um;
	MIN_GAP_BET_SAME_TYPE_DIFFS = 1.5 * g_ip->F_sz_um;
	HPOWERRAIL = 2 * g_ip->F_sz_um;
	cell_h_def = 50 * g_ip->F_sz_um;
	w_poly_contact = g_ip->F_sz_um;
	spacing_poly_to_contact = g_ip->F_sz_um;
	spacing_poly_to_poly = 1.5 * g_ip->F_sz_um;
	ram_wl_stitching_overhead_ = 7.5 * g_ip->F_sz_um;

	min_w_nmos_ = 3 * g_ip->F_sz_um / 2;
	max_w_nmos_ = 100 * g_ip->F_sz_um;
	w_iso       = 12.5*g_ip->F_sz_um;//was 10 micron for the 0.8 micron process
	w_sense_n   = 3.75*g_ip->F_sz_um; // sense amplifier N-trans; was 3 micron for the 0.8 micron process
	w_sense_p   = 7.5*g_ip->F_sz_um; // sense amplifier P-trans; was 6 micron for the 0.8 micron process
	w_sense_en  = 5*g_ip->F_sz_um; // Sense enable transistor of the sense amplifier; was 4 micron for the 0.8 micron process
	w_nmos_b_mux  = 6 * min_w_nmos_;
	w_nmos_sa_mux = 6 * min_w_nmos_;

	
	w_pmos_bl_precharge = 6 * pmos_to_nmos_sz_ratio() * min_w_nmos_;
	w_pmos_bl_eq = pmos_to_nmos_sz_ratio() * min_w_nmos_;
	

	if (ram_cell_tech_type == comm_dram)
	{
		max_w_nmos_dec = 8 * g_ip->F_sz_um;
		h_dec          = 8;  // in the unit of memory cell height
	}
	else
	{
		max_w_nmos_dec = g_tp.max_w_nmos_;
		h_dec          = 4;  // in the unit of memory cell height
	}
  
	
    
	double gmn_sense_amp_latch 
	= (peri_global.Mobility_n / 2) * peri_global.C_ox 
	* (w_sense_n / peri_global.l_elec) * peri_global.Vdsat;
	double gmp_sense_amp_latch = peri_global.gmp_to_gmn_multiplier * gmn_sense_amp_latch;
	gm_sense_amp_latch = gmn_sense_amp_latch + gmp_sense_amp_latch;
	
	
	///cout << "wire_local " << g_ip->ic_proj_type << " " << ((ram_cell_tech_type == comm_dram)?3:0) << endl;
	InterconnectType wire_local_lo, wire_local_hi;
	wire_local_lo.assign(in_file_lo,g_ip->ic_proj_type,(ram_cell_tech_type == comm_dram)?3:0);
	wire_local_hi.assign(in_file_hi,g_ip->ic_proj_type,(ram_cell_tech_type == comm_dram)?3:0);
	wire_local.interpolate(alpha,wire_local_lo,wire_local_hi);
	
	
	///cout << "wire_inside_mat " << g_ip->ic_proj_type << " " << g_ip->wire_is_mat_type << endl;
	InterconnectType wire_inside_mat_lo, wire_inside_mat_hi;
	wire_inside_mat_lo.assign(in_file_lo, g_ip->ic_proj_type, g_ip->wire_is_mat_type);
	wire_inside_mat_hi.assign(in_file_hi, g_ip->ic_proj_type, g_ip->wire_is_mat_type);
	wire_inside_mat.interpolate(alpha, wire_inside_mat_lo, wire_inside_mat_hi);
	
	///cout << "wire_outside_mat " << g_ip->ic_proj_type << " " << g_ip->wire_os_mat_type << endl;
	InterconnectType wire_outside_mat_lo, wire_outside_mat_hi;
	wire_outside_mat_lo.assign(in_file_lo, g_ip->ic_proj_type, g_ip->wire_os_mat_type);
	wire_outside_mat_hi.assign(in_file_hi, g_ip->ic_proj_type, g_ip->wire_os_mat_type);
	wire_outside_mat.interpolate(alpha, wire_outside_mat_lo, wire_outside_mat_hi);
	
	unit_len_wire_del = wire_inside_mat.R_per_um * wire_inside_mat.C_per_um / 2;
	
	// assign value for TSV parameters
	
	assign_tsv(in_file_hi);
		
	fringe_cap = wire_local_hi.fringe_cap; // fringe_cap is similar for all wire types.

	double rd = tr_R_on(min_w_nmos_, NCH, 1);
	double p_to_n_sizing_r = pmos_to_nmos_sz_ratio();
	double c_load = gate_C(min_w_nmos_ * (1 + p_to_n_sizing_r), 0.0);
	double tf = rd * c_load;
	kinv = horowitz(0, tf, 0.5, 0.5, RISE);
	double KLOAD = 1;
	c_load = KLOAD * (drain_C_(min_w_nmos_, NCH, 1, 1, cell_h_def) +
					drain_C_(min_w_nmos_ * p_to_n_sizing_r, PCH, 1, 1, cell_h_def) +
					gate_C(min_w_nmos_ * 4 * (1 + p_to_n_sizing_r), 0.0));
	tf = rd * c_load;
	FO4 = horowitz(0, tf, 0.5, 0.5, RISE);
	
}

#define PRINT(A,X) cout << A << ": " << X << " , " << tech.X << endl

bool TechnologyParameter::isEqual(const TechnologyParameter& tech)
{
  if(!is_equal(ram_wl_stitching_overhead_,tech.ram_wl_stitching_overhead_)) {assert(false);} //fs
  if(!is_equal(min_w_nmos_,tech.min_w_nmos_)) {assert(false);} //fs
  if(!is_equal(max_w_nmos_,tech.max_w_nmos_)) {assert(false);} //fs
  if(!is_equal(max_w_nmos_dec,tech.max_w_nmos_dec)) {assert(false);} //fs+ ram_cell_tech_type
  if(!is_equal(unit_len_wire_del,tech.unit_len_wire_del)) {assert(false);} //wire_inside_mat
  if(!is_equal(FO4,tech.FO4)) {assert(false);} //fs
  if(!is_equal(kinv,tech.kinv)) {assert(false);} //fs
  if(!is_equal(vpp,tech.vpp )) {assert(false);}//input 
  if(!is_equal(w_sense_en,tech.w_sense_en)) {assert(false);}//fs
  if(!is_equal(w_sense_n,tech.w_sense_n)) {assert(false);} //fs
  if(!is_equal(w_sense_p,tech.w_sense_p)) {assert(false);} //fs
  if(!is_equal(sense_delay,tech.sense_delay)) {PRINT("sense_delay",sense_delay); assert(false);} // input
  if(!is_equal(sense_dy_power,tech.sense_dy_power)) {assert(false);} //input
  if(!is_equal(w_iso,tech.w_iso)) {assert(false);} //fs
  if(!is_equal(w_poly_contact,tech.w_poly_contact)) {assert(false);} //fs
  if(!is_equal(spacing_poly_to_poly,tech.spacing_poly_to_poly)) {assert(false);} //fs
  if(!is_equal(spacing_poly_to_contact,tech.spacing_poly_to_contact)) {assert(false);}//fs

  //CACTI3D auxilary variables
  ///if(!is_equal(tsv_pitch,tech.tsv_pitch)) {assert(false);}
  ///if(!is_equal(tsv_diameter,tech.tsv_diameter)) {assert(false);}
  ///if(!is_equal(tsv_length,tech.tsv_length)) {assert(false);}
  ///if(!is_equal(tsv_dielec_thickness,tech.tsv_dielec_thickness)) {assert(false);}
  ///if(!is_equal(tsv_contact_resistance,tech.tsv_contact_resistance)) {assert(false);}
  ///if(!is_equal(tsv_depletion_width,tech.tsv_depletion_width)) {assert(false);}
  ///if(!is_equal(tsv_liner_dielectric_constant,tech.tsv_liner_dielectric_constant)) {assert(false);}

  //CACTI3DD TSV params
  
  if(!is_equal(tsv_parasitic_capacitance_fine,tech.tsv_parasitic_capacitance_fine )) {PRINT("tsv_parasitic_capacitance_fine",tsv_parasitic_capacitance_fine); assert(false);}
  if(!is_equal(tsv_parasitic_resistance_fine,tech.tsv_parasitic_resistance_fine)) {assert(false);}
  if(!is_equal(tsv_minimum_area_fine,tech.tsv_minimum_area_fine)) {assert(false);}

  if(!is_equal(tsv_parasitic_capacitance_coarse,tech.tsv_parasitic_capacitance_coarse)) {assert(false);}
  if(!is_equal(tsv_parasitic_resistance_coarse,tech.tsv_parasitic_resistance_coarse)) {assert(false);}
  if(!is_equal(tsv_minimum_area_coarse,tech.tsv_minimum_area_coarse)) {assert(false);}

	//fs
  if(!is_equal(w_comp_inv_p1,tech.w_comp_inv_p1)) {assert(false);}
  if(!is_equal(w_comp_inv_p2,tech.w_comp_inv_p2)) {assert(false);}
  if(!is_equal(w_comp_inv_p3,tech.w_comp_inv_p3)) {assert(false);}
  if(!is_equal(w_comp_inv_n1,tech.w_comp_inv_n1)) {assert(false);}
  if(!is_equal(w_comp_inv_n2,tech.w_comp_inv_n2)) {assert(false);}
  if(!is_equal(w_comp_inv_n3,tech.w_comp_inv_n3)) {assert(false);}
  if(!is_equal(w_eval_inv_p,tech.w_eval_inv_p)) {assert(false);}
  if(!is_equal(w_eval_inv_n,tech.w_eval_inv_n)) {assert(false);}
  if(!is_equal(w_comp_n,tech.w_comp_n)) {assert(false);}
  if(!is_equal(w_comp_p,tech.w_comp_p)) {assert(false);}

  if(!is_equal(dram_cell_I_on,tech.dram_cell_I_on)) {assert(false);} //ram_cell_tech_type
  if(!is_equal(dram_cell_Vdd,tech.dram_cell_Vdd)) {assert(false);}
  if(!is_equal(dram_cell_I_off_worst_case_len_temp,tech.dram_cell_I_off_worst_case_len_temp)) {assert(false);}
  if(!is_equal(dram_cell_C,tech.dram_cell_C)) {assert(false);}
  if(!is_equal(gm_sense_amp_latch,tech.gm_sense_amp_latch)) {assert(false);} // depends on many things

  if(!is_equal(w_nmos_b_mux,tech.w_nmos_b_mux)) {assert(false);} //fs
  if(!is_equal(w_nmos_sa_mux,tech.w_nmos_sa_mux)) {assert(false);}//fs
  if(!is_equal(w_pmos_bl_precharge,tech.w_pmos_bl_precharge)) {PRINT("w_pmos_bl_precharge",w_pmos_bl_precharge);assert(false);}//fs
  if(!is_equal(w_pmos_bl_eq,tech.w_pmos_bl_eq)) {assert(false);}//fs
  if(!is_equal(MIN_GAP_BET_P_AND_N_DIFFS,tech.MIN_GAP_BET_P_AND_N_DIFFS)) {assert(false);}//fs
  if(!is_equal(MIN_GAP_BET_SAME_TYPE_DIFFS,tech.MIN_GAP_BET_SAME_TYPE_DIFFS)) {assert(false);}//fs
  if(!is_equal(HPOWERRAIL,tech.HPOWERRAIL)) {assert(false);}//fs
  if(!is_equal(cell_h_def,tech.cell_h_def)) {assert(false);}//fs

  if(!is_equal(chip_layout_overhead,tech.chip_layout_overhead )) {assert(false);}//input
  if(!is_equal(macro_layout_overhead,tech.macro_layout_overhead)) {cout <<macro_layout_overhead << " vs. "<< tech.macro_layout_overhead<< endl; assert(false);}
  if(!is_equal(sckt_co_eff,tech.sckt_co_eff)) {assert(false);}

  if(!is_equal(fringe_cap,tech.fringe_cap)) {PRINT("fringe_cap",fringe_cap);assert(false);}//input

  if(h_dec!=tech.h_dec) {assert(false);} //ram_cell_tech_type

  cout << "sram_cell \n";
  sram_cell.isEqual(tech.sram_cell);   // SRAM cell transistor
  cout << "dram_acc \n";
  dram_acc.isEqual(tech.dram_acc);    // DRAM access transistor
  cout << "dram_wl \n";
  dram_wl.isEqual(tech.dram_wl);     // DRAM wordline transistor
  cout << "peri_global \n";
  peri_global.isEqual(tech.peri_global); // peripheral global
  cout << "cam_cell \n";
  cam_cell.isEqual(tech.cam_cell);   // SRAM cell transistor

  cout << "sleep_tx \n";
  sleep_tx.isEqual(tech.sleep_tx);   // Sleep transistor cell transistor

  cout << "wire_local \n";
  wire_local.isEqual(tech.wire_local);
  cout << "wire_inside_mat \n";
  wire_inside_mat.isEqual(tech.wire_inside_mat);
  cout << "wire_outside_mat \n";
  wire_outside_mat.isEqual(tech.wire_outside_mat);

  cout << "scaling_factor \n";
  scaling_factor.isEqual(tech.scaling_factor);
  cout << "sram: \n";
  sram.isEqual(tech.sram);
  cout << "dram: \n";
  dram.isEqual(tech.dram);
  cout << "cam: \n";
  cam.isEqual(tech.cam);
  
  return true;
}
//end ali

void /*TechnologyParameter::*/DeviceType::display(uint32_t indent) const
{
  string indent_str(indent, ' ');

  cout << indent_str << "C_g_ideal = " << setw(12) << C_g_ideal << " F/um" << endl;
  cout << indent_str << "C_fringe  = " << setw(12) << C_fringe  << " F/um" << endl;
  cout << indent_str << "C_overlap = " << setw(12) << C_overlap << " F/um" << endl;
  cout << indent_str << "C_junc    = " << setw(12) << C_junc    << " F/um^2" << endl;
  cout << indent_str << "C_junc_sidewall= " << setw(12) << C_junc_sidewall    << " F/um^2" << endl;
  cout << indent_str << "l_phy     = " << setw(12) << l_phy     << " um" << endl;
  cout << indent_str << "l_elec    = " << setw(12) << l_elec    << " um" << endl;
  cout << indent_str << "R_nch_on  = " << setw(12) << R_nch_on  << " ohm-um" << endl;
  cout << indent_str << "R_pch_on  = " << setw(12) << R_pch_on  << " ohm-um" << endl;
  cout << indent_str << "Vdd       = " << setw(12) << Vdd       << " V" << endl;
  cout << indent_str << "Vth       = " << setw(12) << Vth       << " V" << endl;
  cout << indent_str << "Vcc_min   = " << setw(12) << Vcc_min       << " V" << endl;
  cout << indent_str << "I_on_n    = " << setw(12) << I_on_n    << " A/um" << endl;
  cout << indent_str << "I_on_p    = " << setw(12) << I_on_p    << " A/um" << endl;
  cout << indent_str << "I_off_n   = " << setw(12) << I_off_n   << " A/um" << endl;
  cout << indent_str << "I_off_p   = " << setw(12) << I_off_p   << " A/um" << endl;
  cout << indent_str << "I_g_on_n   = " << setw(12) << I_g_on_n   << " A/um" << endl;
  cout << indent_str << "I_g_on_p   = " << setw(12) << I_g_on_p   << " A/um" << endl;
  cout << indent_str << "C_ox      = " << setw(12) << C_ox      << " F/um^2" << endl;
  cout << indent_str << "t_ox      = " << setw(12) << t_ox      << " um" << endl;
  cout << indent_str << "n_to_p_eff_curr_drv_ratio = " << n_to_p_eff_curr_drv_ratio << endl;
  cout << indent_str << "long_channel_leakage_reduction = " << long_channel_leakage_reduction << endl;
  cout << indent_str << "Mobility_n = " << Mobility_n << endl;
}



void /*TechnologyParameter::*/InterconnectType::display(uint32_t indent)
{
  string indent_str(indent, ' ');

  cout << indent_str << "pitch    = " << setw(12) << pitch    << " um" << endl;
  cout << indent_str << "R_per_um = " << setw(12) << R_per_um << " ohm/um" << endl;
  cout << indent_str << "C_per_um = " << setw(12) << C_per_um << " F/um" << endl;
  
  cout << indent_str << "horiz_dielectric_constant = " << setw(12) << horiz_dielectric_constant << " F/um" << endl;
  cout << indent_str << "vert_dielectric_constant = " << setw(12) << vert_dielectric_constant << " F/um" << endl;
  cout << indent_str << "aspect_ratio = " << setw(12) << aspect_ratio << " F/um" << endl;
  cout << indent_str << "miller_value = " << setw(12) << miller_value << " F/um" << endl;
  cout << indent_str << "ild_thickness = " << setw(12) << ild_thickness << " F/um" << endl;
  
  cout << indent_str << "wire_width       = " << setw(12) << wire_width    << " um" << endl;
  cout << indent_str << "wire_thickness   = " << setw(12) << wire_thickness    << " um" << endl;
  cout << indent_str << "wire_spacing     = " << setw(12) << wire_spacing    << " um" << endl;
  cout << indent_str << "barrier_thickness= " << setw(12) << barrier_thickness    << " um" << endl;
  cout << indent_str << "dishing_thickness= " << setw(12) << dishing_thickness    << " um" << endl;
  cout << indent_str << "alpha_scatter    = " << setw(12) << alpha_scatter    << " um" << endl;
  cout << indent_str << "fringe_cap       = " << setw(12) << fringe_cap    << " um" << endl;
}

void /*TechnologyParameter::*/ScalingFactor::display(uint32_t indent)
{
  string indent_str(indent, ' ');

  cout << indent_str << "logic_scaling_co_eff    = " << setw(12) << logic_scaling_co_eff << endl;
  cout << indent_str << "curr_core_tx_density = " << setw(12) << core_tx_density << " # of tx/um^2" << endl;
  cout << indent_str << "long_channel_leakage_reduction = " << setw(12) << long_channel_leakage_reduction << " # of tx/um^2" << endl;
}

void /*TechnologyParameter::*/MemoryType::display(uint32_t indent) const
{
  string indent_str(indent, ' ');

  cout << indent_str << "b_w         = " << setw(12) << b_w << " um" << endl;
  cout << indent_str << "b_h         = " << setw(12) << b_h << " um" << endl;
  cout << indent_str << "cell_a_w    = " << setw(12) << cell_a_w << " um" << endl;
  cout << indent_str << "cell_pmos_w = " << setw(12) << cell_pmos_w << " um" << endl;
  cout << indent_str << "cell_nmos_w = " << setw(12) << cell_nmos_w << " um" << endl;
  cout << indent_str << "Vbitpre     = " << setw(12) << Vbitpre << " V" << endl;
  cout << indent_str << "Vbitfloating= " << setw(12) << Vbitfloating << " V" << endl;
}



void TechnologyParameter::display(uint32_t indent)
{
  string indent_str(indent, ' ');
  
  cout << indent_str << "ram_wl_stitching_overhead_ = " << setw(12) << ram_wl_stitching_overhead_ << " um" << endl;
  cout << indent_str << "min_w_nmos_                = " << setw(12) << min_w_nmos_                << " um" << endl;
  cout << indent_str << "max_w_nmos_                = " << setw(12) << max_w_nmos_                << " um" << endl;
  cout << indent_str << "max_w_nmos_dec             = " << setw(12) << max_w_nmos_dec                << " um" << endl;
  cout << indent_str << "unit_len_wire_del          = " << setw(12) << unit_len_wire_del          << " s/um^2" << endl;
  cout << indent_str << "FO4                        = " << setw(12) << FO4                        << " s" << endl;
  cout << indent_str << "kinv                       = " << setw(12) << kinv                       << " s" << endl;
  cout << indent_str << "vpp                        = " << setw(12) << vpp                        << " V" << endl;
  cout << indent_str << "w_sense_en                 = " << setw(12) << w_sense_en                 << " um" << endl;
  cout << indent_str << "w_sense_n                  = " << setw(12) << w_sense_n                  << " um" << endl;
  cout << indent_str << "w_sense_p                  = " << setw(12) << w_sense_p                  << " um" << endl;
  cout << indent_str << "sense_delay                = " << setw(12) << sense_delay                  << " um" << endl;
  cout << indent_str << "sense_dy_power             = " << setw(12) << sense_dy_power                  << " um" << endl;
  cout << indent_str << "w_iso                      = " << setw(12) << w_iso                      << " um" << endl;
  cout << indent_str << "w_poly_contact             = " << setw(12) << w_poly_contact             << " um" << endl;
  cout << indent_str << "spacing_poly_to_poly       = " << setw(12) << spacing_poly_to_poly       << " um" << endl;
  cout << indent_str << "spacing_poly_to_contact    = " << setw(12) << spacing_poly_to_contact    << " um" << endl;
  cout << endl;
  cout << indent_str << "tsv_pitch                  = " << setw(12) << tsv_pitch << " um" << endl;
  cout << indent_str << "tsv_diameter               = " << setw(12) << tsv_diameter << " um" << endl;
  cout << indent_str << "tsv_length                 = " << setw(12) << tsv_length << " um" << endl;
  cout << indent_str << "tsv_dielec_thickness       = " << setw(12) << tsv_dielec_thickness << " um" << endl;
  cout << indent_str << "tsv_contact_resistance     = " << setw(12) << tsv_contact_resistance << " um" << endl;
  cout << indent_str << "tsv_depletion_width        = " << setw(12) << tsv_depletion_width << " um" << endl;
  cout << indent_str << "tsv_liner_dielectric_constant= " << setw(12) << tsv_liner_dielectric_constant << " um" << endl;
  cout << endl;
  cout << indent_str << "tsv_parasitic_capacitance_fine= " << setw(12) << tsv_parasitic_capacitance_fine << " um" << endl;
  cout << indent_str << "tsv_parasitic_resistance_fine = " << setw(12) << tsv_parasitic_resistance_fine << " um" << endl;
  cout << indent_str << "tsv_minimum_area_fine         = " << setw(12) << tsv_minimum_area_fine << " um" << endl;
  cout << indent_str << "tsv_parasitic_capacitance_coarse= " << setw(12) << tsv_parasitic_capacitance_coarse << " um" << endl;
  cout << indent_str << "tsv_parasitic_resistance_coarse = " << setw(12) << tsv_parasitic_resistance_coarse << " um" << endl;
  cout << indent_str << "tsv_minimum_area_coarse         = " << setw(12) << tsv_minimum_area_coarse << " um" << endl;
  cout << endl;
  cout << indent_str << "w_comp_inv_p1              = " << setw(12) << w_comp_inv_p1 << " um" << endl;
  cout << indent_str << "w_comp_inv_p2              = " << setw(12) << w_comp_inv_p2 << " um" << endl;
  cout << indent_str << "w_comp_inv_p3              = " << setw(12) << w_comp_inv_p3 << " um" << endl;
  cout << indent_str << "w_comp_inv_n1              = " << setw(12) << w_comp_inv_n1 << " um" << endl;
  cout << indent_str << "w_comp_inv_n2              = " << setw(12) << w_comp_inv_n2 << " um" << endl;
  cout << indent_str << "w_comp_inv_n3              = " << setw(12) << w_comp_inv_n3 << " um" << endl;
  cout << indent_str << "w_eval_inv_p               = " << setw(12) << w_eval_inv_p  << " um" << endl;
  cout << indent_str << "w_eval_inv_n               = " << setw(12) << w_eval_inv_n  << " um" << endl;
  cout << indent_str << "w_comp_n                   = " << setw(12) << w_comp_n      << " um" << endl;
  cout << indent_str << "w_comp_p                   = " << setw(12) << w_comp_p      << " um" << endl;
  cout << endl;
  cout << indent_str << "dram_cell_I_on             = " << setw(12) << dram_cell_I_on << " A/um" << endl;
  cout << indent_str << "dram_cell_Vdd              = " << setw(12) << dram_cell_Vdd  << " V" << endl;
  cout << indent_str << "dram_cell_I_off_worst_case_len_temp = " << setw(12) << dram_cell_I_off_worst_case_len_temp << " A/um" << endl;
  cout << indent_str << "dram_cell_C                = " << setw(12) << dram_cell_C               << " F" << endl;
  cout << indent_str << "gm_sense_amp_latch         = " << setw(12) << gm_sense_amp_latch        << " F/s" << endl;
  cout << endl;
  cout << indent_str << "w_nmos_b_mux               = " << setw(12) << w_nmos_b_mux              << " um" << endl;
  cout << indent_str << "w_nmos_sa_mux              = " << setw(12) << w_nmos_sa_mux             << " um" << endl;
  cout << indent_str << "w_pmos_bl_precharge        = " << setw(12) << w_pmos_bl_precharge       << " um" << endl;
  cout << indent_str << "w_pmos_bl_eq               = " << setw(12) << w_pmos_bl_eq              << " um" << endl;
  cout << indent_str << "MIN_GAP_BET_P_AND_N_DIFFS  = " << setw(12) << MIN_GAP_BET_P_AND_N_DIFFS << " um" << endl;
  cout << indent_str << "MIN_GAP_BET_SAME_TYPE_DIFFS= " << setw(12) << MIN_GAP_BET_SAME_TYPE_DIFFS << " um" << endl;
  cout << indent_str << "HPOWERRAIL                 = " << setw(12) << HPOWERRAIL                << " um" << endl;
  cout << indent_str << "cell_h_def                 = " << setw(12) << cell_h_def                << " um" << endl;
  cout << endl;
  cout << indent_str << "chip_layout_overhead       = " << setw(12) << chip_layout_overhead << " um" << endl;
  cout << indent_str << "macro_layout_overhead      = " << setw(12) << macro_layout_overhead << " um" << endl;
  cout << indent_str << "sckt_co_eff                = " << setw(12) << sckt_co_eff << " um" << endl;
  cout << indent_str << "fringe_cap                 = " << setw(12) << fringe_cap << " um" << endl;
  cout << indent_str << "h_dec                      = " << setw(12) << h_dec << " um" << endl;
  cout << endl;
  cout << indent_str << "SRAM cell transistor: " << endl;
  sram_cell.display(indent + 2);

  cout << endl;
  cout << indent_str << "DRAM access transistor: " << endl;
  dram_acc.display(indent + 2);

  cout << endl;
  cout << indent_str << "DRAM wordline transistor: " << endl;
  dram_wl.display(indent + 2);

  cout << endl;
  cout << indent_str << "peripheral global transistor: " << endl;
  peri_global.display(indent + 2);

  cout << endl;
  cout << indent_str << "wire local" << endl;
  wire_local.display(indent + 2);

  cout << endl;
  cout << indent_str << "wire inside mat" << endl;
  wire_inside_mat.display(indent + 2);

  cout << endl;
  cout << indent_str << "wire outside mat" << endl;
  wire_outside_mat.display(indent + 2);

  cout << endl;
  cout << indent_str << "SRAM" << endl;
  sram.display(indent + 2);

  cout << endl;
  cout << indent_str << "DRAM" << endl;
  dram.display(indent + 2);
}


DynamicParameter::DynamicParameter():
  use_inp_params(0), cell(), is_valid(true)
{
}


void
DynamicParameter::init_CAM()
{
  const InterconnectType &wire_local = g_tp.wire_local;
  unsigned int capacity_per_die = g_ip->cache_sz / NUMBER_STACKED_DIE_LAYERS;  // capacity per stacked die layer

  if (Ndwl != 1 ||            //Ndwl is fixed to 1 for CAM
      Ndcm != 1 ||            //Ndcm is fixed to 1 for CAM
      Nspd < 1 || Nspd > 1 || //Nspd is fixed to 1 for CAM
      Ndsam_lev_1 != 1 ||     //Ndsam_lev_1 is fixed to one
      Ndsam_lev_2 != 1 ||     //Ndsam_lev_2 is fixed to one
      Ndbl < 2) //FIXME: why should Ndbl be >1 for very small CAMs?
  {
    return;
  }



  if (g_ip->specific_tag)
  {
    tagbits = int(ceil(g_ip->tag_w/8.0)*8);
  }
  else
  {
    tagbits = int(ceil((ADDRESS_BITS + EXTRA_TAG_BITS)/8.0)*8);
  }

  //computation of no. of rows and cols of a subarray
  tag_num_r_subarray = (int)ceil(capacity_per_die / (g_ip->nbanks*tagbits/8.0 * Ndbl));//TODO: error check input of tagbits and blocksize //TODO: for pure CAM, g_ip->block should be number of entries.
  tag_num_c_subarray = tagbits;

  if (tag_num_r_subarray == 0) return;
  if (tag_num_r_subarray > MAXSUBARRAYROWS) return;
  if (tag_num_c_subarray < MINSUBARRAYCOLS) return;
  if (tag_num_c_subarray > MAXSUBARRAYCOLS) return;
  num_r_subarray = tag_num_r_subarray; //FIXME: what about num_c_subarray?

  num_subarrays = Ndwl * Ndbl; 

  // calculate cell dimensions
  cam_cell.h = g_tp.cam.b_h + 2 * wire_local.pitch * (g_ip->num_rw_ports-1 + g_ip->num_rd_ports + g_ip->num_wr_ports)
    + 2 * wire_local.pitch*(g_ip->num_search_ports-1) + wire_local.pitch * g_ip->num_se_rd_ports;
  cam_cell.w = g_tp.cam.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports-1 + g_ip->num_rd_ports + g_ip->num_wr_ports)
    + 2 * wire_local.pitch*(g_ip->num_search_ports-1) + wire_local.pitch * g_ip->num_se_rd_ports;

  //FIXME: curious where this is getting used in a CAM
  cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_wr_ports +g_ip->num_rw_ports-1 + g_ip->num_rd_ports)
    + 2 * wire_local.pitch*(g_ip->num_search_ports-1);
  cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports -1 + (g_ip->num_rd_ports - g_ip->num_se_rd_ports)
      + g_ip->num_wr_ports) + g_tp.wire_local.pitch * g_ip->num_se_rd_ports + 2 * wire_local.pitch*(g_ip->num_search_ports-1);

  // calculate wire parameters

  double c_b_metal = cell.h * wire_local.C_per_um;
//  double C_bl;

  c_b_metal = cam_cell.h * wire_local.C_per_um;//IBM and SUN design, SRAM array uses dummy cells to fill the blank space due to mismatch on CAM-RAM
  V_b_sense = (0.05 * g_tp.sram_cell.Vdd > VBITSENSEMIN) ? 0.05 * g_tp.sram_cell.Vdd : VBITSENSEMIN;
  deg_bl_muxing = 1;//FA fix as 1
  // "/ 2.0" below is due to the fact that two adjacent access transistors share drain
  // contacts in a physical layout
  double Cbitrow_drain_cap = drain_C_(g_tp.cam.cell_a_w, NCH, 1, 0, cam_cell.w, false, true) / 2.0;//TODO: comment out these two lines
//  C_bl = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
  dram_refresh_period = 0;


  // do/di: data in/out, for fully associative they are the data width for normal read and write
  // so/si: search data in/out, for fully associative they are the data width for the search ops
  // for CAM, si=di, but so = matching address. do = data out = di (for normal read/write)
  // so/si needs broadcase while do/di do not

  switch (Ndbl) {
    case (0):
      cout <<  "   Invalid Ndbl \n"<<endl;
      exit(0);
      break;
    case (1):
      num_mats_h_dir = 1;//one subarray per mat
      num_mats_v_dir = 1;
      break;
    case (2):
      num_mats_h_dir = 1;//two subarrays per mat
      num_mats_v_dir = 1;
      break;
    default:
      num_mats_h_dir = int(floor(sqrt(Ndbl/4.0)));//4 subbarrys per mat
      num_mats_v_dir = int(Ndbl/4.0 / num_mats_h_dir);
      break;
  }
  num_mats = num_mats_h_dir * num_mats_v_dir;

  num_so_b_mat = int(ceil(log2(num_r_subarray)) + ceil(log2(num_subarrays)));//the address contains the matched data
  num_do_b_mat = tagbits;

  int deg_sa_mux_l1_non_assoc;

  num_so_b_subbank = int(ceil(log2(num_r_subarray)) + ceil(log2(num_subarrays)));//the address contains the matched data
  num_do_b_subbank = tag_num_c_subarray;

  deg_sa_mux_l1_non_assoc = 1;

  deg_senseamp_muxing_non_associativity = deg_sa_mux_l1_non_assoc;

  num_act_mats_hor_dir = 1;
  num_act_mats_hor_dir_sl = num_mats_h_dir;//TODO: this is unnecessary, since search op, num_mats is used

  //compute num_do_mat for tag

  if (num_act_mats_hor_dir > num_mats_h_dir)
  {
    return;
  }


  num_di_b_mat = tagbits;
  num_si_b_mat = tagbits;//*num_subarrays/num_mats;

  num_di_b_subbank       = num_di_b_mat * num_act_mats_hor_dir;//normal cache or normal r/w for FA
  num_si_b_subbank       = num_si_b_mat; //* num_act_mats_hor_dir_sl; inside the data is broadcast

  int num_addr_b_row_dec     = _log2(num_r_subarray);
  num_addr_b_row_dec     +=_log2(num_subarrays/num_mats);
  int number_subbanks        = num_mats / num_act_mats_hor_dir;
  number_subbanks_decode = _log2(number_subbanks);//TODO: add log2(num_subarray_per_bank) to FA/CAM

  num_rw_ports = g_ip->num_rw_ports;
  num_rd_ports = g_ip->num_rd_ports;
  num_wr_ports = g_ip->num_wr_ports;
  num_se_rd_ports = g_ip->num_se_rd_ports;
  num_search_ports = g_ip->num_search_ports;

  number_addr_bits_mat = num_addr_b_row_dec + _log2(deg_bl_muxing) +
    _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2);

  num_di_b_bank_per_port = tagbits;
  num_si_b_bank_per_port = tagbits;
  num_do_b_bank_per_port = tagbits;
  num_so_b_bank_per_port = int(ceil(log2(num_r_subarray)) + ceil(log2(num_subarrays)));

  if ((!is_tag) && (g_ip->data_assoc > 1) && (!g_ip->fast_access))
  {
    number_way_select_signals_mat = g_ip->data_assoc;
  }

  // add ECC adjustment to all data signals that traverse on H-trees.
  if (g_ip->add_ecc_b_ == true)
  {
    ECC_adjustment();
  }

  is_valid = true;
}

void
DynamicParameter::init_FA()
{
  const InterconnectType &wire_local = g_tp.wire_local;
  //Disabling 3D model since a 3D stacked FA is never tested 
  assert(NUMBER_STACKED_DIE_LAYERS == 1);
  unsigned int capacity_per_die = g_ip->cache_sz;

  if (Ndwl != 1 ||            //Ndwl is fixed to 1 for FA
      Ndcm != 1 ||            //Ndcm is fixed to 1 for FA
      Nspd < 1 || Nspd > 1 || //Nspd is fixed to 1 for FA
      Ndsam_lev_1 != 1 ||     //Ndsam_lev_1 is fixed to one
      Ndsam_lev_2 != 1 ||     //Ndsam_lev_2 is fixed to one
      Ndbl < 2)
  {
    return;
  }


  //***********compute row, col of an subarray

  //either fully-asso or cam
  if (g_ip->specific_tag)
  {
    tagbits = g_ip->tag_w;
  }
  else
  {
    tagbits = ADDRESS_BITS + EXTRA_TAG_BITS - _log2(g_ip->block_sz);
  }
  tagbits = (((tagbits + 3) >> 2) << 2);

  tag_num_r_subarray = (int)(capacity_per_die / (g_ip->nbanks*g_ip->block_sz * Ndbl));
  tag_num_c_subarray = (int)ceil((tagbits * Nspd / Ndwl));// + EPSILON);
  if (tag_num_r_subarray == 0) return;
  if (tag_num_r_subarray > MAXSUBARRAYROWS) return;
  if (tag_num_c_subarray < MINSUBARRAYCOLS) return;
  if (tag_num_c_subarray > MAXSUBARRAYCOLS) return;

  data_num_r_subarray = tag_num_r_subarray;
  data_num_c_subarray = 8 * g_ip->block_sz;
  if (data_num_r_subarray == 0) return;
  if (data_num_r_subarray > MAXSUBARRAYROWS) return;
  if (data_num_c_subarray < MINSUBARRAYCOLS) return;
  if (data_num_c_subarray > MAXSUBARRAYCOLS) return;
  num_r_subarray = tag_num_r_subarray;

  num_subarrays = Ndwl * Ndbl;
  //****************end of computation of row, col of an subarray

  // calculate wire parameters
  cam_cell.h = g_tp.cam.b_h + 2 * wire_local.pitch * (g_ip->num_rw_ports-1 + g_ip->num_rd_ports + g_ip->num_wr_ports)
    + 2 * wire_local.pitch*(g_ip->num_search_ports-1) + wire_local.pitch * g_ip->num_se_rd_ports;
  cam_cell.w = g_tp.cam.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports-1 + g_ip->num_rd_ports + g_ip->num_wr_ports)
    + 2 * wire_local.pitch*(g_ip->num_search_ports-1) + wire_local.pitch * g_ip->num_se_rd_ports;

  cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_wr_ports +g_ip->num_rw_ports-1 + g_ip->num_rd_ports)
    + 2 * wire_local.pitch*(g_ip->num_search_ports-1);
  cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports -1 + (g_ip->num_rd_ports - g_ip->num_se_rd_ports)
      + g_ip->num_wr_ports) + g_tp.wire_local.pitch * g_ip->num_se_rd_ports + 2 * wire_local.pitch*(g_ip->num_search_ports-1);

  double c_b_metal = cell.h * wire_local.C_per_um;
  //  double C_bl;

  c_b_metal = cam_cell.h * wire_local.C_per_um;//IBM and SUN design, SRAM array uses dummy cells to fill the blank space due to mismatch on CAM-RAM
  V_b_sense = (0.05 * g_tp.sram_cell.Vdd > VBITSENSEMIN) ? 0.05 * g_tp.sram_cell.Vdd : VBITSENSEMIN;
  deg_bl_muxing = 1;//FA fix as 1
  // "/ 2.0" below is due to the fact that two adjacent access transistors share drain
  // contacts in a physical layout
  double Cbitrow_drain_cap = drain_C_(g_tp.cam.cell_a_w, NCH, 1, 0, cam_cell.w, false, true) / 2.0;//TODO: comment out these two lines
  //	  C_bl = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
  dram_refresh_period = 0;


  // do/di: data in/out, for fully associative they are the data width for normal read and write
  // so/si: search data in/out, for fully associative they are the data width for the search ops
  // for CAM, si=di, but so = matching address. do = data out = di (for normal read/write)
  // so/si needs broadcase while do/di do not

  switch (Ndbl) {
    case (0):
      cout <<  "   Invalid Ndbl \n"<<endl;
      exit(0);
      break;
    case (1):
      num_mats_h_dir = 1;//one subarray per mat
      num_mats_v_dir = 1;
      break;
    case (2):
      num_mats_h_dir = 1;//two subarrays per mat
      num_mats_v_dir = 1;
      break;
    default:
      num_mats_h_dir = int(floor(sqrt(Ndbl/4.0)));//4 subbarrys per mat
      num_mats_v_dir = int(Ndbl/4.0 / num_mats_h_dir);
      break;
  }
  num_mats = num_mats_h_dir * num_mats_v_dir;

  num_so_b_mat   = data_num_c_subarray;
  num_do_b_mat   = data_num_c_subarray + tagbits;



  int deg_sa_mux_l1_non_assoc;
  num_so_b_subbank = 8 * g_ip->block_sz;//TODO:internal perfetch should be considered also for fa
  num_do_b_subbank = num_so_b_subbank + tag_num_c_subarray;

  deg_sa_mux_l1_non_assoc = 1;

  deg_senseamp_muxing_non_associativity = deg_sa_mux_l1_non_assoc;

  num_act_mats_hor_dir = 1;
  num_act_mats_hor_dir_sl = num_mats_h_dir;//TODO: this is unnecessary, since search op, num_mats is used

  //compute num_do_mat for tag
  if (num_act_mats_hor_dir > num_mats_h_dir)
  {
    return;
  }


  //compute di for mat subbank and bank
  if (fully_assoc)
  {
    num_di_b_mat = num_do_b_mat;
    //*num_subarrays/num_mats; bits per mat of CAM/FA is as same as cache,
    //but inside the mat wire tracks need to be reserved for search data bus
    num_si_b_mat = tagbits;
  }
  num_di_b_subbank       = num_di_b_mat * num_act_mats_hor_dir;//normal cache or normal r/w for FA
  num_si_b_subbank       = num_si_b_mat; //* num_act_mats_hor_dir_sl; inside the data is broadcast

  int num_addr_b_row_dec     = _log2(num_r_subarray);
  num_addr_b_row_dec     +=_log2(num_subarrays/num_mats);
  int number_subbanks        = num_mats / num_act_mats_hor_dir;
  number_subbanks_decode = _log2(number_subbanks);//TODO: add log2(num_subarray_per_bank) to FA/CAM

  num_rw_ports = g_ip->num_rw_ports;
  num_rd_ports = g_ip->num_rd_ports;
  num_wr_ports = g_ip->num_wr_ports;
  num_se_rd_ports = g_ip->num_se_rd_ports;
  num_search_ports = g_ip->num_search_ports;

  number_addr_bits_mat = num_addr_b_row_dec + _log2(deg_bl_muxing) +
    _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2);

  num_di_b_bank_per_port = g_ip->out_w + tagbits;//TODO: out_w or block_sz?
  num_si_b_bank_per_port = tagbits;
  num_do_b_bank_per_port = g_ip->out_w + tagbits;
  num_so_b_bank_per_port = g_ip->out_w;

  if ((!is_tag) && (g_ip->data_assoc > 1) && (!g_ip->fast_access))
  {
    number_way_select_signals_mat = g_ip->data_assoc;
  }

  // add ECC adjustment to all data signals that traverse on H-trees.
  if (g_ip->add_ecc_b_ == true)
  {
    ECC_adjustment();
  }

  is_valid = true;
}

//DynamicParameter::init_Mem()
//{
//}
//
//DynamicParameter::init_3DMem()
//{
//}

//*** Calculate number of rows and columns in a subarray
bool
DynamicParameter::calc_subarr_rc(unsigned int capacity_per_die) {
  // If it's not an FA tag/data array, Ndwl should be at least two and Ndbl should be
  // at least two because an array is assumed to have at least one mat. A mat
  // consists of two rows and two columns of subarrays.
  if (Ndwl < 2 || Ndbl < 2)
  {
	  return false;
  }

  if ((is_dram) && (!is_tag) && (Ndcm > 1))
  {
	  return false;  // For a DRAM array, each bitline has its own sense-amp
  }

  // if data array, let tagbits = 0
  if (is_tag)
  {
    if (g_ip->specific_tag)
    {
      tagbits = g_ip->tag_w;
    }
    else
    {
      tagbits = ADDRESS_BITS + EXTRA_TAG_BITS - _log2(capacity_per_die) +
        _log2(g_ip->tag_assoc*2 - 1); 

    }
//    tagbits = (((tagbits + 3) >> 2) << 2); //FIXME: NAV: Why are we doing this?

    num_r_subarray = (int)ceil(capacity_per_die / (g_ip->nbanks *
          g_ip->block_sz * g_ip->tag_assoc * Ndbl * Nspd));
    num_c_subarray = (int)ceil((tagbits * g_ip->tag_assoc * Nspd / Ndwl));
  }
  else
  {
    num_r_subarray = (int)ceil(capacity_per_die / (g_ip->nbanks *
          g_ip->block_sz * g_ip->data_assoc * Ndbl * Nspd));
    num_c_subarray = (int)ceil((8 * g_ip->block_sz * g_ip->data_assoc * Nspd / Ndwl));
    if(g_ip->is_3d_mem)
    {
      double capacity_per_die_double = (double)g_ip->cache_sz / g_ip->num_die_3d;
      //num_c_subarray = 1 << (int)ceil((double)_log2( 8*capacity_per_die / (g_ip->nbanks * Ndbl * Ndwl) )/2 ) ;
      //num_r_subarray = 1 << (int)ceil((double)_log2( 8*capacity_per_die / (g_ip->nbanks * Ndbl * Ndwl * num_c_subarray)	) );
      num_c_subarray = g_ip->page_sz_bits/Ndwl;
      num_r_subarray = 1 << (int)floor(_log2((double) g_ip->cache_sz / g_ip->num_die_3d
            / num_c_subarray / g_ip->nbanks / Ndbl / Ndwl * 1024 * 1024 * 1024) +0.5);
      if (g_ip->print_detail_debug)
      {
        cout  << "parameter.cc: capacity_per_die_double = " << capacity_per_die_double << " Gbit"<< endl;
        cout  << "parameter.cc: g_ip->nbanks * Ndbl * Ndwl = " << (g_ip->nbanks * Ndbl * Ndwl) << endl;
        //cout  << "parameter.cc: subarray capacity = " << 8*capacity_per_die / (g_ip->nbanks * Ndbl * Ndwl) << endl;
        //cout  << "parameter.cc: total bit add per subarray = " << _log2( 8*capacity_per_die / (g_ip->nbanks * Ndbl * Ndwl) ) << endl;
        cout  << "parameter.cc: num_r_subarray = " << num_r_subarray << endl;
        cout  << "parameter.cc: num_c_subarray = " << num_c_subarray << endl;
      }

    }
  }

	  if (num_r_subarray < MINSUBARRAYROWS) return false;
	  if (num_r_subarray == 0) return false;
	  if (num_r_subarray > MAXSUBARRAYROWS) return false;
	  if (num_c_subarray < MINSUBARRAYCOLS) return false;
	  if (num_c_subarray > MAXSUBARRAYCOLS) return false;



  num_subarrays = Ndwl * Ndbl;
  return true;
}





DynamicParameter::DynamicParameter(
    bool is_tag_,
    int pure_ram_,
    int pure_cam_,
    double Nspd_,
    unsigned int Ndwl_,
    unsigned int Ndbl_,
    unsigned int Ndcm_,
    unsigned int Ndsam_lev_1_,
    unsigned int Ndsam_lev_2_,
    Wire_type wt,
    bool is_main_mem_):
  is_tag(is_tag_), pure_ram(pure_ram_), pure_cam(pure_cam_), tagbits(0), Nspd(Nspd_), Ndwl(Ndwl_), Ndbl(Ndbl_),Ndcm(Ndcm_),
  Ndsam_lev_1(Ndsam_lev_1_), Ndsam_lev_2(Ndsam_lev_2_),wtype(wt),
  number_way_select_signals_mat(0), V_b_sense(0), use_inp_params(0),
  is_main_mem(is_main_mem_), cell(), is_valid(false)
{
  ram_cell_tech_type = (is_tag) ? g_ip->tag_arr_ram_cell_tech_type : g_ip->data_arr_ram_cell_tech_type;
  is_dram            = ((ram_cell_tech_type == lp_dram) || (ram_cell_tech_type == comm_dram));

  unsigned int capacity_per_die = g_ip->cache_sz / NUMBER_STACKED_DIE_LAYERS;  // capacity per stacked die layer
  const InterconnectType & wire_local = g_tp.wire_local;
  fully_assoc = (g_ip->fully_assoc) ? true : false;

  if (pure_cam) 
  {
    init_CAM();
    return;
  }

  if (fully_assoc) {
    init_FA();
    return;
  }

  //*** Calculate number of rows and columns in a subarray
  //    Return if their dimensions do not meet the minimum specs
  if (!calc_subarr_rc(capacity_per_die)) return;

  //** Calculate cell dimensions
  if(is_tag)
  {
    cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 + g_ip->num_rd_ports +
        g_ip->num_wr_ports);
    cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 + g_ip->num_wr_ports +
        (g_ip->num_rd_ports - g_ip->num_se_rd_ports)) +
      wire_local.pitch * g_ip->num_se_rd_ports;
  }
  else
  {
    if (is_dram)
    {
      cell.h = g_tp.dram.b_h;
      cell.w = g_tp.dram.b_w;
    }
    else
    {
      cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_wr_ports +
          g_ip->num_rw_ports - 1 + g_ip->num_rd_ports);
      cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 +
          (g_ip->num_rd_ports - g_ip->num_se_rd_ports) +
          g_ip->num_wr_ports) + g_tp.wire_local.pitch * g_ip->num_se_rd_ports;
    }
  }

  double c_b_metal = cell.h * wire_local.C_per_um;
  double C_bl;

  if (is_dram)
  {
    deg_bl_muxing = 1;
    if (ram_cell_tech_type == comm_dram)
    {
      double Cbitrow_drain_cap = drain_C_(g_tp.dram.cell_a_w, NCH, 1, 0, cell.w, true, true) / 2.0;
      C_bl  = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
      //C_bl  = num_r_subarray * c_b_metal;
      V_b_sense = (g_tp.dram_cell_Vdd/2) * g_tp.dram_cell_C / (g_tp.dram_cell_C + C_bl);
      if (V_b_sense < VBITSENSEMIN && !(g_ip->is_3d_mem && g_ip->force_cache_config) )
      {
        return;
      }

      dram_refresh_period = 64e-3;

    }
    else
    {
      double Cbitrow_drain_cap = drain_C_(g_tp.dram.cell_a_w, NCH, 1, 0, cell.w, true, true) / 2.0;
      C_bl  = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
      V_b_sense = (g_tp.dram_cell_Vdd/2) * g_tp.dram_cell_C /(g_tp.dram_cell_C + C_bl);

      if (V_b_sense < VBITSENSEMIN)
      {
        return; //Sense amp input signal is smaller that minimum allowable sense amp input signal
      }
      V_b_sense = VBITSENSEMIN; // in any case, we fix sense amp input signal to a constant value
      //v_storage_worst = g_tp.dram_cell_Vdd / 2 - VBITSENSEMIN * (g_tp.dram_cell_C + C_bl) / g_tp.dram_cell_C;
      //dram_refresh_period = 1.1 * g_tp.dram_cell_C * v_storage_worst / g_tp.dram_cell_I_off_worst_case_len_temp;
      dram_refresh_period = 0.9 * g_tp.dram_cell_C * VDD_STORAGE_LOSS_FRACTION_WORST * g_tp.dram_cell_Vdd / g_tp.dram_cell_I_off_worst_case_len_temp;
    }
  }
  else
  { //SRAM
    V_b_sense = (0.05 * g_tp.sram_cell.Vdd > VBITSENSEMIN) ? 0.05 * g_tp.sram_cell.Vdd : VBITSENSEMIN;
    deg_bl_muxing = Ndcm;
    // "/ 2.0" below is due to the fact that two adjacent access transistors share drain
    // contacts in a physical layout
    double Cbitrow_drain_cap = drain_C_(g_tp.sram.cell_a_w, NCH, 1, 0, cell.w, false, true) / 2.0;
    C_bl = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
    dram_refresh_period = 0;
  }


  // do/di: data in/out, for fully associative they are the data width for normal read and write
  // so/si: search data in/out, for fully associative they are the data width for the search ops
  // for CAM, si=di, but so = matching address. do = data out = di (for normal read/write)
  // so/si needs broadcase while do/di do not

	  num_mats_h_dir = MAX(Ndwl / 2, 1);
	  num_mats_v_dir = MAX(Ndbl / 2, 1);
	  num_mats       = num_mats_h_dir * num_mats_v_dir;
	  num_do_b_mat   = MAX((num_subarrays/num_mats) * num_c_subarray / (deg_bl_muxing * Ndsam_lev_1 * Ndsam_lev_2), 1);

  if (!(fully_assoc|| pure_cam) && (num_do_b_mat < (num_subarrays/num_mats)))
  {
	  return;
  }


  int deg_sa_mux_l1_non_assoc;
  //TODO:the i/o for subbank is not necessary and should be removed.
	  if (!is_tag)
	  {
		  if (is_main_mem == true)
		  {
			  num_do_b_subbank = g_ip->int_prefetch_w * g_ip->out_w;
			  //CACTI3DD DRAM page size
			  if(g_ip->is_3d_mem)
				  num_do_b_subbank = g_ip->page_sz_bits;
			  deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
		  }
		  else
		  {
			  if (g_ip->fast_access == true)
			  {
				  num_do_b_subbank = g_ip->out_w * g_ip->data_assoc;
				  deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
			  }
			  else
			  {

				  num_do_b_subbank = g_ip->out_w;
				  deg_sa_mux_l1_non_assoc = Ndsam_lev_1 / g_ip->data_assoc;
				  if (deg_sa_mux_l1_non_assoc < 1)
				  {
					  return;
				  }

			  }
		  }
	  }
	  else
	  {
		  num_do_b_subbank = tagbits * g_ip->tag_assoc;
		  if (num_do_b_mat < tagbits)
		  {
			  return;
		  }
		  deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
		  //num_do_b_mat = g_ip->tag_assoc / num_mats_h_dir;
	  }

  deg_senseamp_muxing_non_associativity = deg_sa_mux_l1_non_assoc;

	  num_act_mats_hor_dir = num_do_b_subbank / num_do_b_mat;
	  if (g_ip->is_3d_mem && num_act_mats_hor_dir == 0)
		  num_act_mats_hor_dir = 1;
	  if (num_act_mats_hor_dir == 0)
	  {
		  return;
	  }

  //compute num_do_mat for tag
  if (is_tag)
  {
	  if (!(fully_assoc || pure_cam))
	  {
		  num_do_b_mat     = g_ip->tag_assoc / num_act_mats_hor_dir;
		  num_do_b_subbank = num_act_mats_hor_dir * num_do_b_mat;
	  }
  }

  if ((g_ip->is_cache == false && is_main_mem == true) || (PAGE_MODE == 1 && is_dram))
  {
	  if (num_act_mats_hor_dir * num_do_b_mat * Ndsam_lev_1 * Ndsam_lev_2 != (int)g_ip->page_sz_bits)
	  {
		  return;
	  }
  }

//  if (is_tag == false && g_ip->is_cache == true && !fully_assoc && !pure_cam && //TODO: TODO burst transfer should also apply to RAM arrays
  if (is_tag == false && g_ip->is_main_mem == true &&
		  num_act_mats_hor_dir*num_do_b_mat*Ndsam_lev_1*Ndsam_lev_2 < ((int) g_ip->out_w * (int) g_ip->burst_len * (int) g_ip->data_assoc))
  {
	  return;
  }

  if (num_act_mats_hor_dir > num_mats_h_dir)
  {
	  return;
  }


  //compute di for mat subbank and bank
	  if(!is_tag)
	  {
		  if(g_ip->fast_access == true)
		  {
			  num_di_b_mat = num_do_b_mat / g_ip->data_assoc;
		  }
		  else
		  {
			  num_di_b_mat = num_do_b_mat;
		  }
	  }
	  else
	  {
		  num_di_b_mat = tagbits;
	  }

  num_di_b_subbank       = num_di_b_mat * num_act_mats_hor_dir;//normal cache or normal r/w for FA
  num_si_b_subbank       = num_si_b_mat; //* num_act_mats_hor_dir_sl; inside the data is broadcast

  int num_addr_b_row_dec     = _log2(num_r_subarray);
  if  ((fully_assoc ||pure_cam))
	  num_addr_b_row_dec     +=_log2(num_subarrays/num_mats);
  int number_subbanks        = num_mats / num_act_mats_hor_dir;
  number_subbanks_decode = _log2(number_subbanks);//TODO: add log2(num_subarray_per_bank) to FA/CAM

  num_rw_ports = g_ip->num_rw_ports;
  num_rd_ports = g_ip->num_rd_ports;
  num_wr_ports = g_ip->num_wr_ports;
  num_se_rd_ports = g_ip->num_se_rd_ports;
  num_search_ports = g_ip->num_search_ports;

  if (is_dram && is_main_mem)
  {
	  number_addr_bits_mat = MAX((unsigned int) num_addr_b_row_dec,
			  _log2(deg_bl_muxing) + _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2));
	  if (g_ip->print_detail_debug)
	  {
	  cout << "parameter.cc: number_addr_bits_mat = " << num_addr_b_row_dec << endl;
	  cout << "parameter.cc: num_addr_b_row_dec = " << num_addr_b_row_dec << endl;
	  cout << "parameter.cc: num_addr_b_mux_sel = " << _log2(deg_bl_muxing) + _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2) << endl;
	  }
  }
  else
  {
	  number_addr_bits_mat = num_addr_b_row_dec + _log2(deg_bl_muxing) +
	  _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2);
  }

	  if (is_tag)
	  {
		  num_di_b_bank_per_port = tagbits;
		  num_do_b_bank_per_port = g_ip->data_assoc;
	  }
	  else
	  {
		  num_di_b_bank_per_port = g_ip->out_w + g_ip->data_assoc;
		  num_do_b_bank_per_port = g_ip->out_w;
	  }

  if ((!is_tag) && (g_ip->data_assoc > 1) && (!g_ip->fast_access))
  {
	  number_way_select_signals_mat = g_ip->data_assoc;
  }

  // add ECC adjustment to all data signals that traverse on H-trees.
  if (g_ip->add_ecc_b_ == true) ECC_adjustment();

  is_valid = true;
}

void
DynamicParameter::ECC_adjustment() {
	  num_do_b_mat += (int) (ceil(num_do_b_mat / num_bits_per_ecc_b_));
	  num_di_b_mat += (int) (ceil(num_di_b_mat / num_bits_per_ecc_b_));
	  num_di_b_subbank += (int) (ceil(num_di_b_subbank / num_bits_per_ecc_b_));
	  num_do_b_subbank += (int) (ceil(num_do_b_subbank / num_bits_per_ecc_b_));
	  num_di_b_bank_per_port += (int) (ceil(num_di_b_bank_per_port / num_bits_per_ecc_b_));
	  num_do_b_bank_per_port += (int) (ceil(num_do_b_bank_per_port / num_bits_per_ecc_b_));

	  num_so_b_mat += (int) (ceil(num_so_b_mat / num_bits_per_ecc_b_));
	  num_si_b_mat += (int) (ceil(num_si_b_mat / num_bits_per_ecc_b_));
	  num_si_b_subbank += (int) (ceil(num_si_b_subbank / num_bits_per_ecc_b_));
	  num_so_b_subbank += (int) (ceil(num_so_b_subbank / num_bits_per_ecc_b_));
	  num_si_b_bank_per_port += (int) (ceil(num_si_b_bank_per_port / num_bits_per_ecc_b_));
	  num_so_b_bank_per_port += (int) (ceil(num_so_b_bank_per_port / num_bits_per_ecc_b_));
}

//DynamicParameter::DynamicParameter(
//    bool is_tag_,
//    int pure_ram_,
//    int pure_cam_,
//    double Nspd_,
//    unsigned int Ndwl_,
//    unsigned int Ndbl_,
//    unsigned int Ndcm_,
//    unsigned int Ndsam_lev_1_,
//    unsigned int Ndsam_lev_2_,
//    Wire_type wt,
//    bool is_main_mem_):
//  is_tag(is_tag_), pure_ram(pure_ram_), pure_cam(pure_cam_), tagbits(0), Nspd(Nspd_), Ndwl(Ndwl_), Ndbl(Ndbl_),Ndcm(Ndcm_),
//  Ndsam_lev_1(Ndsam_lev_1_), Ndsam_lev_2(Ndsam_lev_2_),wtype(wt),
//  number_way_select_signals_mat(0), V_b_sense(0), use_inp_params(0),
//  is_main_mem(is_main_mem_), cell(), is_valid(false)
//  ram_cell_tech_type = (is_tag) ? g_ip->tag_arr_ram_cell_tech_type : g_ip->data_arr_ram_cell_tech_type;
//  is_dram            = ((ram_cell_tech_type == lp_dram) || (ram_cell_tech_type == comm_dram));
//
//  unsigned int capacity_per_die = g_ip->cache_sz / NUMBER_STACKED_DIE_LAYERS;  // capacity per stacked die layer
//  const /*TechnologyParameter::*/InterconnectType & wire_local = g_tp.wire_local;
//  fully_assoc = (g_ip->fully_assoc) ? true : false;
//
//  if (fully_assoc || pure_cam)
//  { // fully-assocative cache -- ref: CACTi 2.0 report
//	  if (Ndwl != 1 ||            //Ndwl is fixed to 1 for FA
//			  Ndcm != 1 ||            //Ndcm is fixed to 1 for FA
//			  Nspd < 1 || Nspd > 1 || //Nspd is fixed to 1 for FA
//			  Ndsam_lev_1 != 1 ||     //Ndsam_lev_1 is fixed to one
//			  Ndsam_lev_2 != 1 ||     //Ndsam_lev_2 is fixed to one
//			  Ndbl < 2)
//	  {
//          return;
//	  }
//  }
//
//  if ((is_dram) && (!is_tag) && (Ndcm > 1))
//  {
//	  return;  // For a DRAM array, each bitline has its own sense-amp
//  }
//
//  // If it's not an FA tag/data array, Ndwl should be at least two and Ndbl should be
//  // at least two because an array is assumed to have at least one mat. And a mat
//  // is formed out of two horizontal subarrays and two vertical subarrays
//  if (fully_assoc == false && (Ndwl < 1 || Ndbl < 1))
//  {
//	  return;
//  }
//
//  //***********compute row, col of an subarray
//  if (!(fully_assoc || pure_cam))//Not fully_asso nor cam
//  {
//	  // if data array, let tagbits = 0
//	  if (is_tag)
//	  {
//		  if (g_ip->specific_tag)
//		  {
//			  tagbits = g_ip->tag_w;
//		  }
//		  else
//		  {
//			  tagbits = ADDRESS_BITS + EXTRA_TAG_BITS - _log2(capacity_per_die) +
//			  _log2(g_ip->tag_assoc*2 - 1) - _log2(g_ip->nbanks);
//
//		  }
//		  tagbits = (((tagbits + 3) >> 2) << 2);
//
//		  num_r_subarray = (int)ceil(capacity_per_die / (g_ip->nbanks *
//				  g_ip->block_sz * g_ip->tag_assoc * Ndbl * Nspd));// + EPSILON);
//		  num_c_subarray = (int)ceil((tagbits * g_ip->tag_assoc * Nspd / Ndwl));// + EPSILON);
//		  //burst_length = 1;
//	  }
//	  else
//	  {
//		  num_r_subarray = (int)ceil(capacity_per_die / (g_ip->nbanks *
//				  g_ip->block_sz * g_ip->data_assoc * Ndbl * Nspd));// + EPSILON);
//		  num_c_subarray = (int)ceil((8 * g_ip->block_sz * g_ip->data_assoc * Nspd / Ndwl));// + EPSILON); + EPSILON);
//		  // burst_length = g_ip->block_sz * 8 / g_ip->out_w;
//		  if(g_ip->is_3d_mem)
//		  {
//			  double capacity_per_die_double = (double)g_ip->cache_sz / g_ip->num_die_3d;
//			  //num_c_subarray = 1 << (int)ceil((double)_log2( 8*capacity_per_die / (g_ip->nbanks * Ndbl * Ndwl) )/2 ) ;
//			  //num_r_subarray = 1 << (int)ceil((double)_log2( 8*capacity_per_die / (g_ip->nbanks * Ndbl * Ndwl * num_c_subarray)	) );
//			  num_c_subarray = g_ip->page_sz_bits/Ndwl;
//			  num_r_subarray = 1 << (int)floor(_log2((double) g_ip->cache_sz / g_ip->num_die_3d
//					  / num_c_subarray / g_ip->nbanks / Ndbl / Ndwl * 1024 * 1024 * 1024) +0.5);
//			  if (g_ip->print_detail_debug)
//			  {
//			  cout  << "parameter.cc: capacity_per_die_double = " << capacity_per_die_double << " Gbit"<< endl;
//			  cout  << "parameter.cc: g_ip->nbanks * Ndbl * Ndwl = " << (g_ip->nbanks * Ndbl * Ndwl) << endl;
//			  //cout  << "parameter.cc: subarray capacity = " << 8*capacity_per_die / (g_ip->nbanks * Ndbl * Ndwl) << endl;
//			  //cout  << "parameter.cc: total bit add per subarray = " << _log2( 8*capacity_per_die / (g_ip->nbanks * Ndbl * Ndwl) ) << endl;
//			  cout  << "parameter.cc: num_r_subarray = " << num_r_subarray << endl;
//			  cout  << "parameter.cc: num_c_subarray = " << num_c_subarray << endl;
//			  }
//
//		  }
//	  }
//
//	  if (num_r_subarray < MINSUBARRAYROWS) return;
//	  if (num_r_subarray == 0) return;
//	  if (num_r_subarray > MAXSUBARRAYROWS) return;
//	  if (num_c_subarray < MINSUBARRAYCOLS) return;
//	  if (num_c_subarray > MAXSUBARRAYCOLS) return;
//
//  }
//
//  else
//  {//either fully-asso or cam
//	  if (pure_cam)
//	  {
//		  if (g_ip->specific_tag)
//		  {
//			  tagbits = int(ceil(g_ip->tag_w/8.0)*8);
//		  }
//		  else
//		  {
//			  tagbits = int(ceil((ADDRESS_BITS + EXTRA_TAG_BITS)/8.0)*8);
////			  cout<<"Pure CAM needs tag width to be specified"<<endl;
////			  exit(0);
//		  }
//		  //tagbits = (((tagbits + 3) >> 2) << 2);
//
//		  tag_num_r_subarray = (int)ceil(capacity_per_die / (g_ip->nbanks*tagbits/8.0 * Ndbl));//TODO: error check input of tagbits and blocksize //TODO: for pure CAM, g_ip->block should be number of entries.
//		  //tag_num_c_subarray = (int)(tagbits  + EPSILON);
//		  tag_num_c_subarray = tagbits;
//		  if (tag_num_r_subarray == 0) return;
//		  if (tag_num_r_subarray > MAXSUBARRAYROWS) return;
//		  if (tag_num_c_subarray < MINSUBARRAYCOLS) return;
//		  if (tag_num_c_subarray > MAXSUBARRAYCOLS) return;
//		  num_r_subarray = tag_num_r_subarray;
//	  }
//	  else //fully associative
//	  {
//		  if (g_ip->specific_tag)
//		  {
//			  tagbits = g_ip->tag_w;
//		  }
//		  else
//		  {
//			  tagbits = ADDRESS_BITS + EXTRA_TAG_BITS - _log2(g_ip->block_sz);//TODO: should be the page_offset=log2(page size), but this info is not avail with CACTI, for McPAT this is no problem.
//		  }
//		  tagbits = (((tagbits + 3) >> 2) << 2);
//
//		  tag_num_r_subarray = (int)(capacity_per_die / (g_ip->nbanks*g_ip->block_sz * Ndbl));
//		  tag_num_c_subarray = (int)ceil((tagbits * Nspd / Ndwl));// + EPSILON);
//		  if (tag_num_r_subarray == 0) return;
//		  if (tag_num_r_subarray > MAXSUBARRAYROWS) return;
//		  if (tag_num_c_subarray < MINSUBARRAYCOLS) return;
//		  if (tag_num_c_subarray > MAXSUBARRAYCOLS) return;
//
//		  data_num_r_subarray = tag_num_r_subarray;
//		  data_num_c_subarray = 8 * g_ip->block_sz;
//		  if (data_num_r_subarray == 0) return;
//		  if (data_num_r_subarray > MAXSUBARRAYROWS) return;
//		  if (data_num_c_subarray < MINSUBARRAYCOLS) return;
//		  if (data_num_c_subarray > MAXSUBARRAYCOLS) return;
//		  num_r_subarray = tag_num_r_subarray;
//	  }
//  }
//
//  num_subarrays = Ndwl * Ndbl;
//  //****************end of computation of row, col of an subarray
//
//  // calculate wire parameters
//  if (fully_assoc || pure_cam)
//  {
//	  cam_cell.h = g_tp.cam.b_h + 2 * wire_local.pitch * (g_ip->num_rw_ports-1 + g_ip->num_rd_ports + g_ip->num_wr_ports)
//	  + 2 * wire_local.pitch*(g_ip->num_search_ports-1) + wire_local.pitch * g_ip->num_se_rd_ports;
//	  cam_cell.w = g_tp.cam.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports-1 + g_ip->num_rd_ports + g_ip->num_wr_ports)
//	  + 2 * wire_local.pitch*(g_ip->num_search_ports-1) + wire_local.pitch * g_ip->num_se_rd_ports;
//
//	  cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_wr_ports +g_ip->num_rw_ports-1 + g_ip->num_rd_ports)
//	  + 2 * wire_local.pitch*(g_ip->num_search_ports-1);
//	  cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports -1 + (g_ip->num_rd_ports - g_ip->num_se_rd_ports)
//			  + g_ip->num_wr_ports) + g_tp.wire_local.pitch * g_ip->num_se_rd_ports + 2 * wire_local.pitch*(g_ip->num_search_ports-1);
//  }
//  else
//  {
//	  if(is_tag)
//	  {
//		  cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 + g_ip->num_rd_ports +
//				  g_ip->num_wr_ports);
//		  cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 + g_ip->num_wr_ports +
//				  (g_ip->num_rd_ports - g_ip->num_se_rd_ports)) +
//				  wire_local.pitch * g_ip->num_se_rd_ports;
//	  }
//	  else
//	  {
//		  if (is_dram)
//		  {
//			  cell.h = g_tp.dram.b_h;
//			  cell.w = g_tp.dram.b_w;
//		  }
//		  else
//		  {
//			  cell.h = g_tp.sram.b_h + 2 * wire_local.pitch * (g_ip->num_wr_ports +
//					  g_ip->num_rw_ports - 1 + g_ip->num_rd_ports);
//			  cell.w = g_tp.sram.b_w + 2 * wire_local.pitch * (g_ip->num_rw_ports - 1 +
//					  (g_ip->num_rd_ports - g_ip->num_se_rd_ports) +
//					  g_ip->num_wr_ports) + g_tp.wire_local.pitch * g_ip->num_se_rd_ports;
//		  }
//	  }
//  }
//
//  double c_b_metal = cell.h * wire_local.C_per_um;
//  double C_bl;
//
//  if (!(fully_assoc || pure_cam))
//  {
//	  if (is_dram)
//	  {
//		  deg_bl_muxing = 1;
//		  if (ram_cell_tech_type == comm_dram)
//		  {
//			  double Cbitrow_drain_cap = drain_C_(g_tp.dram.cell_a_w, NCH, 1, 0, cell.w, true, true) / 2.0;
//			  C_bl  = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
//			  //C_bl  = num_r_subarray * c_b_metal;
//			  V_b_sense = (g_tp.dram_cell_Vdd/2) * g_tp.dram_cell_C / (g_tp.dram_cell_C + C_bl);
//			  if (V_b_sense < VBITSENSEMIN && !(g_ip->is_3d_mem && g_ip->force_cache_config) )
//			  {
//				  return;
//			  }
//
//			  /*
//			  C_bl  = num_r_subarray * c_b_metal;
//			  V_b_sense = (g_tp.dram_cell_Vdd/2) * g_tp.dram_cell_C / (g_tp.dram_cell_C + C_bl);
//			  if (V_b_sense < VBITSENSEMIN)
//			  {
//				  return;
//			  }
//			  V_b_sense = VBITSENSEMIN;  // in any case, we fix sense amp input signal to a constant value
//			  */
//			  dram_refresh_period = 64e-3;
//
//		  }
//		  else
//		  {
//			  double Cbitrow_drain_cap = drain_C_(g_tp.dram.cell_a_w, NCH, 1, 0, cell.w, true, true) / 2.0;
//			  C_bl  = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
//			  V_b_sense = (g_tp.dram_cell_Vdd/2) * g_tp.dram_cell_C /(g_tp.dram_cell_C + C_bl);
//
//			  if (V_b_sense < VBITSENSEMIN)
//			  {
//				  return; //Sense amp input signal is smaller that minimum allowable sense amp input signal
//			  }
//			  V_b_sense = VBITSENSEMIN; // in any case, we fix sense amp input signal to a constant value
//			  //v_storage_worst = g_tp.dram_cell_Vdd / 2 - VBITSENSEMIN * (g_tp.dram_cell_C + C_bl) / g_tp.dram_cell_C;
//			  //dram_refresh_period = 1.1 * g_tp.dram_cell_C * v_storage_worst / g_tp.dram_cell_I_off_worst_case_len_temp;
//			  dram_refresh_period = 0.9 * g_tp.dram_cell_C * VDD_STORAGE_LOSS_FRACTION_WORST * g_tp.dram_cell_Vdd / g_tp.dram_cell_I_off_worst_case_len_temp;
//		  }
//	  }
//	  else
//	  { //SRAM
//		  V_b_sense = (0.05 * g_tp.sram_cell.Vdd > VBITSENSEMIN) ? 0.05 * g_tp.sram_cell.Vdd : VBITSENSEMIN;
//		  deg_bl_muxing = Ndcm;
//		  // "/ 2.0" below is due to the fact that two adjacent access transistors share drain
//		  // contacts in a physical layout
//		  double Cbitrow_drain_cap = drain_C_(g_tp.sram.cell_a_w, NCH, 1, 0, cell.w, false, true) / 2.0;
//		  C_bl = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
//		  dram_refresh_period = 0;
//	  }
//  }
//  else
//  {
//	  c_b_metal = cam_cell.h * wire_local.C_per_um;//IBM and SUN design, SRAM array uses dummy cells to fill the blank space due to mismatch on CAM-RAM
//	  V_b_sense = (0.05 * g_tp.sram_cell.Vdd > VBITSENSEMIN) ? 0.05 * g_tp.sram_cell.Vdd : VBITSENSEMIN;
//	  deg_bl_muxing = 1;//FA fix as 1
//	  // "/ 2.0" below is due to the fact that two adjacent access transistors share drain
//	  // contacts in a physical layout
//	  double Cbitrow_drain_cap = drain_C_(g_tp.cam.cell_a_w, NCH, 1, 0, cam_cell.w, false, true) / 2.0;//TODO: comment out these two lines
//	  C_bl = num_r_subarray * (Cbitrow_drain_cap + c_b_metal);
//	  dram_refresh_period = 0;
//  }
//
//
//  // do/di: data in/out, for fully associative they are the data width for normal read and write
//  // so/si: search data in/out, for fully associative they are the data width for the search ops
//  // for CAM, si=di, but so = matching address. do = data out = di (for normal read/write)
//  // so/si needs broadcase while do/di do not
//
//  if (fully_assoc || pure_cam)
//  {
//	    switch (Ndbl) {
//	      case (0):
//	        cout <<  "   Invalid Ndbl \n"<<endl;
//	        exit(0);
//	        break;
//	      case (1):
//	    	  num_mats_h_dir = 1;//one subarray per mat
//	    	  num_mats_v_dir = 1;
//	        break;
//	      case (2):
//	    	  num_mats_h_dir = 1;//two subarrays per mat
//	    	  num_mats_v_dir = 1;
//	    	  break;
//	      default:
//	    	  num_mats_h_dir = int(floor(sqrt(Ndbl/4.0)));//4 subbarrys per mat
//	    	  num_mats_v_dir = int(Ndbl/4.0 / num_mats_h_dir);
//	    	  break;
//	    }
//	    num_mats = num_mats_h_dir * num_mats_v_dir;
//
//	    if (fully_assoc)
//	    {
//	    	num_so_b_mat   = data_num_c_subarray;
//	    	num_do_b_mat   = data_num_c_subarray + tagbits;
//	    }
//	    else
//	    {
//	    	num_so_b_mat = int(ceil(log2(num_r_subarray)) + ceil(log2(num_subarrays)));//the address contains the matched data
//	    	num_do_b_mat = tagbits;
//	    }
//  }
//  else
//  {
//	  num_mats_h_dir = MAX(Ndwl / 2, 1);
//	  num_mats_v_dir = MAX(Ndbl / 2, 1);
//	  num_mats       = num_mats_h_dir * num_mats_v_dir;
//	  num_do_b_mat   = MAX((num_subarrays/num_mats) * num_c_subarray / (deg_bl_muxing * Ndsam_lev_1 * Ndsam_lev_2), 1);
//  }
//
//  if (!(fully_assoc|| pure_cam) && (num_do_b_mat < (num_subarrays/num_mats)))
//  {
//	  return;
//  }
//
//
//  int deg_sa_mux_l1_non_assoc;
//  //TODO:the i/o for subbank is not necessary and should be removed.
//  if (!(fully_assoc || pure_cam))
//  {
//	  if (!is_tag)
//	  {
//		  if (is_main_mem == true)
//		  {
//			  num_do_b_subbank = g_ip->int_prefetch_w * g_ip->out_w;
//			  //CACTI3DD DRAM page size
//			  if(g_ip->is_3d_mem)
//				  num_do_b_subbank = g_ip->page_sz_bits;
//			  deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
//		  }
//		  else
//		  {
//			  if (g_ip->fast_access == true)
//			  {
//				  num_do_b_subbank = g_ip->out_w * g_ip->data_assoc;
//				  deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
//			  }
//			  else
//			  {
//
//				  num_do_b_subbank = g_ip->out_w;
//				  deg_sa_mux_l1_non_assoc = Ndsam_lev_1 / g_ip->data_assoc;
//				  if (deg_sa_mux_l1_non_assoc < 1)
//				  {
//					  return;
//				  }
//
//			  }
//		  }
//	  }
//	  else
//	  {
//		  num_do_b_subbank = tagbits * g_ip->tag_assoc;
//		  if (num_do_b_mat < tagbits)
//		  {
//			  return;
//		  }
//		  deg_sa_mux_l1_non_assoc = Ndsam_lev_1;
//		  //num_do_b_mat = g_ip->tag_assoc / num_mats_h_dir;
//	  }
//  }
//  else
//  {
//	  if (fully_assoc)
//	  {
//		  num_so_b_subbank = 8 * g_ip->block_sz;//TODO:internal perfetch should be considered also for fa
//		  num_do_b_subbank = num_so_b_subbank + tag_num_c_subarray;
//	  }
//	  else
//	  {
//		  num_so_b_subbank = int(ceil(log2(num_r_subarray)) + ceil(log2(num_subarrays)));//the address contains the matched data
//		  num_do_b_subbank = tag_num_c_subarray;
//	  }
//
//	  deg_sa_mux_l1_non_assoc = 1;
//  }
//
//  deg_senseamp_muxing_non_associativity = deg_sa_mux_l1_non_assoc;
//
//  if (fully_assoc || pure_cam)
//  {
//	  num_act_mats_hor_dir = 1;
//	  num_act_mats_hor_dir_sl = num_mats_h_dir;//TODO: this is unnecessary, since search op, num_mats is used
//  }
//  else
//  {
//	  num_act_mats_hor_dir = num_do_b_subbank / num_do_b_mat;
//	  if (g_ip->is_3d_mem && num_act_mats_hor_dir == 0)
//		  num_act_mats_hor_dir = 1;
//	  if (num_act_mats_hor_dir == 0)
//	  {
//		  return;
//	  }
//  }
//
//  //compute num_do_mat for tag
//  if (is_tag)
//  {
//	  if (!(fully_assoc || pure_cam))
//	  {
//		  num_do_b_mat     = g_ip->tag_assoc / num_act_mats_hor_dir;
//		  num_do_b_subbank = num_act_mats_hor_dir * num_do_b_mat;
//	  }
//  }
//
//  if ((g_ip->is_cache == false && is_main_mem == true) || (PAGE_MODE == 1 && is_dram))
//  {
//	  if (num_act_mats_hor_dir * num_do_b_mat * Ndsam_lev_1 * Ndsam_lev_2 != (int)g_ip->page_sz_bits)
//	  {
//		  return;
//	  }
//  }
//
////  if (is_tag == false && g_ip->is_cache == true && !fully_assoc && !pure_cam && //TODO: TODO burst transfer should also apply to RAM arrays
//  if (is_tag == false && g_ip->is_main_mem == true &&
//		  num_act_mats_hor_dir*num_do_b_mat*Ndsam_lev_1*Ndsam_lev_2 < ((int) g_ip->out_w * (int) g_ip->burst_len * (int) g_ip->data_assoc))
//  {
//	  return;
//  }
//
//  if (num_act_mats_hor_dir > num_mats_h_dir)
//  {
//	  return;
//  }
//
//
//  //compute di for mat subbank and bank
//  if (!(fully_assoc ||pure_cam))
//  {
//	  if(!is_tag)
//	  {
//		  if(g_ip->fast_access == true)
//		  {
//			  num_di_b_mat = num_do_b_mat / g_ip->data_assoc;
//		  }
//		  else
//		  {
//			  num_di_b_mat = num_do_b_mat;
//		  }
//	  }
//	  else
//	  {
//		  num_di_b_mat = tagbits;
//	  }
//  }
//  else
//  {
//	  if (fully_assoc)
//	  {
//		  num_di_b_mat = num_do_b_mat;
//		  //*num_subarrays/num_mats; bits per mat of CAM/FA is as same as cache,
//		  //but inside the mat wire tracks need to be reserved for search data bus
//		  num_si_b_mat = tagbits;
//	  }
//	  else
//	  {
//		  num_di_b_mat = tagbits;
//		  num_si_b_mat = tagbits;//*num_subarrays/num_mats;
//	  }
//
//  }
//
//  num_di_b_subbank       = num_di_b_mat * num_act_mats_hor_dir;//normal cache or normal r/w for FA
//  num_si_b_subbank       = num_si_b_mat; //* num_act_mats_hor_dir_sl; inside the data is broadcast
//
//  int num_addr_b_row_dec     = _log2(num_r_subarray);
//  if  ((fully_assoc ||pure_cam))
//	  num_addr_b_row_dec     +=_log2(num_subarrays/num_mats);
//  int number_subbanks        = num_mats / num_act_mats_hor_dir;
//  number_subbanks_decode = _log2(number_subbanks);//TODO: add log2(num_subarray_per_bank) to FA/CAM
//
//  num_rw_ports = g_ip->num_rw_ports;
//  num_rd_ports = g_ip->num_rd_ports;
//  num_wr_ports = g_ip->num_wr_ports;
//  num_se_rd_ports = g_ip->num_se_rd_ports;
//  num_search_ports = g_ip->num_search_ports;
//
//  if (is_dram && is_main_mem)
//  {
//	  number_addr_bits_mat = MAX((unsigned int) num_addr_b_row_dec,
//			  _log2(deg_bl_muxing) + _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2));
//	  if (g_ip->print_detail_debug)
//	  {
//	  cout << "parameter.cc: number_addr_bits_mat = " << num_addr_b_row_dec << endl;
//	  cout << "parameter.cc: num_addr_b_row_dec = " << num_addr_b_row_dec << endl;
//	  cout << "parameter.cc: num_addr_b_mux_sel = " << _log2(deg_bl_muxing) + _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2) << endl;
//	  }
//  }
//  else
//  {
//	  number_addr_bits_mat = num_addr_b_row_dec + _log2(deg_bl_muxing) +
//	  _log2(deg_sa_mux_l1_non_assoc) + _log2(Ndsam_lev_2);
//  }
//
//  if (!(fully_assoc ||pure_cam))
//  {
//	  if (is_tag)
//	  {
//		  num_di_b_bank_per_port = tagbits;
//		  num_do_b_bank_per_port = g_ip->data_assoc;
//	  }
//	  else
//	  {
//		  num_di_b_bank_per_port = g_ip->out_w + g_ip->data_assoc;
//		  num_do_b_bank_per_port = g_ip->out_w;
//	  }
//  }
//  else
//  {
//	  if (fully_assoc)
//	  {
//		  num_di_b_bank_per_port = g_ip->out_w + tagbits;//TODO: out_w or block_sz?
//		  num_si_b_bank_per_port = tagbits;
//		  num_do_b_bank_per_port = g_ip->out_w + tagbits;
//		  num_so_b_bank_per_port = g_ip->out_w;
//	  }
//	  else
//	  {
//		  num_di_b_bank_per_port = tagbits;
//		  num_si_b_bank_per_port = tagbits;
//		  num_do_b_bank_per_port = tagbits;
//		  num_so_b_bank_per_port = int(ceil(log2(num_r_subarray)) + ceil(log2(num_subarrays)));
//	  }
//  }
//
//  if ((!is_tag) && (g_ip->data_assoc > 1) && (!g_ip->fast_access))
//  {
//	  number_way_select_signals_mat = g_ip->data_assoc;
//  }
//
//  // add ECC adjustment to all data signals that traverse on H-trees.
//  if (g_ip->add_ecc_b_ == true)
//  {
//	  num_do_b_mat += (int) (ceil(num_do_b_mat / num_bits_per_ecc_b_));
//	  num_di_b_mat += (int) (ceil(num_di_b_mat / num_bits_per_ecc_b_));
//	  num_di_b_subbank += (int) (ceil(num_di_b_subbank / num_bits_per_ecc_b_));
//	  num_do_b_subbank += (int) (ceil(num_do_b_subbank / num_bits_per_ecc_b_));
//	  num_di_b_bank_per_port += (int) (ceil(num_di_b_bank_per_port / num_bits_per_ecc_b_));
//	  num_do_b_bank_per_port += (int) (ceil(num_do_b_bank_per_port / num_bits_per_ecc_b_));
//
//	  num_so_b_mat += (int) (ceil(num_so_b_mat / num_bits_per_ecc_b_));
//	  num_si_b_mat += (int) (ceil(num_si_b_mat / num_bits_per_ecc_b_));
//	  num_si_b_subbank += (int) (ceil(num_si_b_subbank / num_bits_per_ecc_b_));
//	  num_so_b_subbank += (int) (ceil(num_so_b_subbank / num_bits_per_ecc_b_));
//	  num_si_b_bank_per_port += (int) (ceil(num_si_b_bank_per_port / num_bits_per_ecc_b_));
//	  num_so_b_bank_per_port += (int) (ceil(num_so_b_bank_per_port / num_bits_per_ecc_b_));
//  }
//
//  is_valid = true;
//}
