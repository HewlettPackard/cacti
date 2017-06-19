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


#include "basic_circuit.h"

#include "parameter.h"

#include <cmath>
void init_tech_params(double technology, bool is_tag)
{
  g_tp.init(technology,is_tag);
}

void printing(const char * name, double value)
{
	cout << "tech " << name << " " << value << endl; 
}

void printing_int(const char * name, uint64_t value)
{
	cout << "tech " << name << " " << value << endl; 
}
void print_g_tp()
{
	printing("g_tp.peri_global.Vdd",g_tp.peri_global.Vdd);
	printing("g_tp.peri_global.Vcc_min",g_tp.peri_global.Vcc_min);
	printing("g_tp.peri_global.t_ox",g_tp.peri_global.t_ox);
	printing("g_tp.peri_global.Vth",g_tp.peri_global.Vth);
	printing("g_tp.peri_global.C_ox",g_tp.peri_global.C_ox);
	printing("g_tp.peri_global.C_g_ideal",g_tp.peri_global.C_g_ideal);
	printing("g_tp.peri_global.C_fringe",g_tp.peri_global.C_fringe);
	printing("g_tp.peri_global.C_junc",g_tp.peri_global.C_junc);
	printing("g_tp.peri_global.C_junc_sidewall",g_tp.peri_global.C_junc_sidewall);
	printing("g_tp.peri_global.l_phy",g_tp.peri_global.l_phy);
	printing("g_tp.peri_global.l_elec",g_tp.peri_global.l_elec);
	printing("g_tp.peri_global.I_on_n",g_tp.peri_global.I_on_n);
	printing("g_tp.peri_global.R_nch_on",g_tp.peri_global.R_nch_on);
	printing("g_tp.peri_global.R_pch_on",g_tp.peri_global.R_pch_on);
	printing("g_tp.peri_global.n_to_p_eff_curr_drv_ratio",g_tp.peri_global.n_to_p_eff_curr_drv_ratio);
	printing("g_tp.peri_global.long_channel_leakage_reduction",g_tp.peri_global.long_channel_leakage_reduction);
	printing("g_tp.peri_global.I_off_n",g_tp.peri_global.I_off_n);
	printing("g_tp.peri_global.I_off_p",g_tp.peri_global.I_off_p);
	printing("g_tp.peri_global.I_g_on_n",g_tp.peri_global.I_g_on_n);
	printing("g_tp.peri_global.I_g_on_p",g_tp.peri_global.I_g_on_p);

	printing("g_tp.peri_global.Mobility_n",g_tp.peri_global.Mobility_n);

	printing("g_tp.sleep_tx.Vdd",g_tp.sleep_tx.Vdd);
	printing("g_tp.sleep_tx.Vcc_min",g_tp.sleep_tx.Vcc_min);
	printing("g_tp.sleep_tx.t_ox",g_tp.sleep_tx.t_ox);
	printing("g_tp.sleep_tx.Vth",g_tp.sleep_tx.Vth);
	printing("g_tp.sleep_tx.C_ox",g_tp.sleep_tx.C_ox);
	printing("g_tp.sleep_tx.C_g_ideal",g_tp.sleep_tx.C_g_ideal);
	printing("g_tp.sleep_tx.C_fringe",g_tp.sleep_tx.C_fringe);
	printing("g_tp.sleep_tx.C_junc",g_tp.sleep_tx.C_junc);
	printing("g_tp.sleep_tx.C_junc_sidewall",g_tp.sleep_tx.C_junc_sidewall);
	printing("g_tp.sleep_tx.l_phy",g_tp.sleep_tx.l_phy);
	printing("g_tp.sleep_tx.l_elec",g_tp.sleep_tx.l_elec);
	printing("g_tp.sleep_tx.I_on_n",g_tp.sleep_tx.I_on_n);
	printing("g_tp.sleep_tx.R_nch_on",g_tp.sleep_tx.R_nch_on);
	printing("g_tp.sleep_tx.R_pch_on",g_tp.sleep_tx.R_pch_on);
	printing("g_tp.sleep_tx.n_to_p_eff_curr_drv_ratio",g_tp.sleep_tx.n_to_p_eff_curr_drv_ratio);
	printing("g_tp.sleep_tx.long_channel_leakage_reduction",g_tp.sleep_tx.long_channel_leakage_reduction);
	printing("g_tp.sleep_tx.I_off_n",g_tp.sleep_tx.I_off_n);
	printing("g_tp.sleep_tx.I_off_p",g_tp.sleep_tx.I_off_p);
	printing("g_tp.sleep_tx.I_g_on_n",g_tp.sleep_tx.I_g_on_n);
	printing("g_tp.sleep_tx.I_g_on_p",g_tp.sleep_tx.I_g_on_p);
	printing("g_tp.sleep_tx.Mobility_n",g_tp.sleep_tx.Mobility_n);

	printing("g_tp.sram_cell.Vdd",g_tp.sram_cell.Vdd);
	printing("g_tp.sram_cell.Vcc_min",g_tp.sram_cell.Vcc_min);
	printing("g_tp.sram_cell.l_phy",g_tp.sram_cell.l_phy);
	printing("g_tp.sram_cell.l_elec",g_tp.sram_cell.l_elec);
	printing("g_tp.sram_cell.t_ox",g_tp.sram_cell.t_ox);
	printing("g_tp.sram_cell.Vth",g_tp.sram_cell.Vth);
	printing("g_tp.sram_cell.C_g_ideal",g_tp.sram_cell.C_g_ideal);
	printing("g_tp.sram_cell.C_fringe",g_tp.sram_cell.C_fringe);
	printing("g_tp.sram_cell.C_junc",g_tp.sram_cell.C_junc);
	printing("g_tp.sram_cell.C_junc_sidewall",g_tp.sram_cell.C_junc_sidewall);
	printing("g_tp.sram_cell.I_on_n",g_tp.sram_cell.I_on_n);
	printing("g_tp.sram_cell.R_nch_on",g_tp.sram_cell.R_nch_on);
	printing("g_tp.sram_cell.R_pch_on",g_tp.sram_cell.R_pch_on);
	printing("g_tp.sram_cell.n_to_p_eff_curr_drv_ratio",g_tp.sram_cell.n_to_p_eff_curr_drv_ratio);
	printing("g_tp.sram_cell.long_channel_leakage_reduction",g_tp.sram_cell.long_channel_leakage_reduction);
	printing("g_tp.sram_cell.I_off_n",g_tp.sram_cell.I_off_n);
	printing("g_tp.sram_cell.I_off_p",g_tp.sram_cell.I_off_p);
	printing("g_tp.sram_cell.I_g_on_n",g_tp.sram_cell.I_g_on_n);
	printing("g_tp.sram_cell.I_g_on_p",g_tp.sram_cell.I_g_on_p);

	printing("g_tp.dram_cell_Vdd",g_tp.dram_cell_Vdd);
	printing("g_tp.dram_acc.Vth",g_tp.dram_acc.Vth);
	printing("g_tp.dram_acc.l_phy",g_tp.dram_acc.l_phy);
	printing("g_tp.dram_acc.l_elec",g_tp.dram_acc.l_elec);
	printing("g_tp.dram_acc.C_g_ideal",g_tp.dram_acc.C_g_ideal);
	printing("g_tp.dram_acc.C_fringe",g_tp.dram_acc.C_fringe);
	printing("g_tp.dram_acc.C_junc",g_tp.dram_acc.C_junc);
	printing("g_tp.dram_acc.C_junc_sidewall",g_tp.dram_acc.C_junc_sidewall);
	printing("g_tp.dram_cell_I_on",g_tp.dram_cell_I_on);
	printing("g_tp.dram_cell_I_off_worst_case_len_temp",g_tp.dram_cell_I_off_worst_case_len_temp);
	printing("g_tp.dram_acc.I_on_n",g_tp.dram_acc.I_on_n);
	printing("g_tp.dram_cell_C",g_tp.dram_cell_C);
	printing("g_tp.vpp",g_tp.vpp);
	printing("g_tp.dram_wl.l_phy",g_tp.dram_wl.l_phy);
	printing("g_tp.dram_wl.l_elec",g_tp.dram_wl.l_elec);
	printing("g_tp.dram_wl.C_g_ideal",g_tp.dram_wl.C_g_ideal);
	printing("g_tp.dram_wl.C_fringe",g_tp.dram_wl.C_fringe);
	printing("g_tp.dram_wl.C_junc",g_tp.dram_wl.C_junc);
	printing("g_tp.dram_wl.C_junc_sidewall",g_tp.dram_wl.C_junc_sidewall);
	printing("g_tp.dram_wl.I_on_n",g_tp.dram_wl.I_on_n);
	printing("g_tp.dram_wl.R_nch_on",g_tp.dram_wl.R_nch_on);
	printing("g_tp.dram_wl.R_pch_on",g_tp.dram_wl.R_pch_on);
	printing("g_tp.dram_wl.n_to_p_eff_curr_drv_ratio",g_tp.dram_wl.n_to_p_eff_curr_drv_ratio);
	printing("g_tp.dram_wl.long_channel_leakage_reduction",g_tp.dram_wl.long_channel_leakage_reduction);
	printing("g_tp.dram_wl.I_off_n",g_tp.dram_wl.I_off_n);
	printing("g_tp.dram_wl.I_off_p",g_tp.dram_wl.I_off_p);

	printing("g_tp.cam_cell.Vdd",g_tp.cam_cell.Vdd);
	printing("g_tp.cam_cell.l_phy",g_tp.cam_cell.l_phy);
	printing("g_tp.cam_cell.l_elec",g_tp.cam_cell.l_elec);
	printing("g_tp.cam_cell.t_ox",g_tp.cam_cell.t_ox);
	printing("g_tp.cam_cell.Vth",g_tp.cam_cell.Vth);
	printing("g_tp.cam_cell.C_g_ideal",g_tp.cam_cell.C_g_ideal);
	printing("g_tp.cam_cell.C_fringe",g_tp.cam_cell.C_fringe);
	printing("g_tp.cam_cell.C_junc",g_tp.cam_cell.C_junc);
	printing("g_tp.cam_cell.C_junc_sidewall",g_tp.cam_cell.C_junc_sidewall);
	printing("g_tp.cam_cell.I_on_n",g_tp.cam_cell.I_on_n);
	printing("g_tp.cam_cell.R_nch_on",g_tp.cam_cell.R_nch_on);
	printing("g_tp.cam_cell.R_pch_on",g_tp.cam_cell.R_pch_on);
	printing("g_tp.cam_cell.n_to_p_eff_curr_drv_ratio",g_tp.cam_cell.n_to_p_eff_curr_drv_ratio);
	printing("g_tp.cam_cell.long_channel_leakage_reduction",g_tp.cam_cell.long_channel_leakage_reduction);
	printing("g_tp.cam_cell.I_off_n",g_tp.cam_cell.I_off_n);
	printing("g_tp.cam_cell.I_off_p",g_tp.cam_cell.I_off_p);
	printing("g_tp.cam_cell.I_g_on_n",g_tp.cam_cell.I_g_on_n);
	printing("g_tp.cam_cell.I_g_on_p",g_tp.cam_cell.I_g_on_p);

	printing("g_tp.dram.cell_a_w",g_tp.dram.cell_a_w);
	printing("g_tp.dram.cell_pmos_w",g_tp.dram.cell_pmos_w);
	printing("g_tp.dram.cell_nmos_w",g_tp.dram.cell_nmos_w);
	

	printing("g_tp.sram.cell_a_w",g_tp.sram.cell_a_w);
	printing("g_tp.sram.cell_pmos_w",g_tp.sram.cell_pmos_w);
	printing("g_tp.sram.cell_nmos_w",g_tp.sram.cell_nmos_w);
	

	printing("g_tp.cam.cell_a_w",g_tp.cam.cell_a_w);
	printing("g_tp.cam.cell_pmos_w",g_tp.cam.cell_pmos_w);
	printing("g_tp.cam.cell_nmos_w",g_tp.cam.cell_nmos_w);
	
	printing("g_tp.scaling_factor.logic_scaling_co_eff",g_tp.scaling_factor.logic_scaling_co_eff);
	printing("g_tp.scaling_factor.core_tx_density",g_tp.scaling_factor.core_tx_density);
	printing("g_tp.chip_layout_overhead",g_tp.chip_layout_overhead);
	printing("g_tp.macro_layout_overhead",g_tp.macro_layout_overhead);
	printing("g_tp.sckt_co_eff",g_tp.sckt_co_eff);

	printing("g_tp.w_comp_inv_p1",g_tp.w_comp_inv_p1);
	printing("g_tp.w_comp_inv_n1",g_tp.w_comp_inv_n1);
	printing("g_tp.w_comp_inv_p2",g_tp.w_comp_inv_p2);
	printing("g_tp.w_comp_inv_n2",g_tp.w_comp_inv_n2);
	printing("g_tp.w_comp_inv_p3",g_tp.w_comp_inv_p3);
	printing("g_tp.w_comp_inv_n3",g_tp.w_comp_inv_n3);
	printing("g_tp.w_eval_inv_p",g_tp.w_eval_inv_p);
	printing("g_tp.w_eval_inv_n",g_tp.w_eval_inv_n);
	printing("g_tp.w_comp_n",g_tp.w_comp_n);
	printing("g_tp.w_comp_p",g_tp.w_comp_p);

	printing("g_tp.MIN_GAP_BET_P_AND_N_DIFFS",g_tp.MIN_GAP_BET_P_AND_N_DIFFS);
	printing("g_tp.MIN_GAP_BET_SAME_TYPE_DIFFS",g_tp.MIN_GAP_BET_SAME_TYPE_DIFFS);
	printing("g_tp.HPOWERRAIL",g_tp.HPOWERRAIL);
	printing("g_tp.cell_h_def",g_tp.cell_h_def);
	printing("g_tp.w_poly_contact",g_tp.w_poly_contact);
	printing("g_tp.spacing_poly_to_contact",g_tp.spacing_poly_to_contact);
	printing("g_tp.spacing_poly_to_poly",g_tp.spacing_poly_to_poly);
	printing("g_tp.ram_wl_stitching_overhead_",g_tp.ram_wl_stitching_overhead_);

	printing("g_tp.min_w_nmos_",g_tp.min_w_nmos_);
	printing("g_tp.max_w_nmos_",g_tp.max_w_nmos_);
	printing("g_tp.w_iso",g_tp.w_iso);
	printing("g_tp.w_sense_n",g_tp.w_sense_n);
	printing("g_tp.w_sense_p",g_tp.w_sense_p);
	printing("g_tp.w_sense_en",g_tp.w_sense_en);
	printing("g_tp.w_nmos_b_mux",g_tp.w_nmos_b_mux);
	printing("g_tp.w_nmos_sa_mux",g_tp.w_nmos_sa_mux);

	printing("g_tp.max_w_nmos_dec",g_tp.max_w_nmos_dec);
	printing_int("g_tp.h_dec",g_tp.h_dec);

	printing("g_tp.peri_global.C_overlap",g_tp.peri_global.C_overlap);
	printing("g_tp.sram_cell.C_overlap",g_tp.sram_cell.C_overlap);
	printing("g_tp.cam_cell.C_overlap",g_tp.cam_cell.C_overlap);

	printing("g_tp.dram_acc.C_overlap",g_tp.dram_acc.C_overlap);
	printing("g_tp.dram_acc.R_nch_on",g_tp.dram_acc.R_nch_on);
	
	printing("g_tp.dram_wl.C_overlap",g_tp.dram_wl.C_overlap);

	printing("g_tp.gm_sense_amp_latch",g_tp.gm_sense_amp_latch);

	printing("g_tp.dram.b_w",g_tp.dram.b_w);
	printing("g_tp.dram.b_h",g_tp.dram.b_h);
	printing("g_tp.sram.b_w",g_tp.sram.b_w);
	printing("g_tp.sram.b_h",g_tp.sram.b_h);
	printing("g_tp.cam.b_w",g_tp.cam.b_w);
	printing("g_tp.cam.b_h",g_tp.cam.b_h);

	printing("g_tp.dram.Vbitpre",g_tp.dram.Vbitpre);
	printing("g_tp.sram.Vbitpre",g_tp.sram.Vbitpre);
	printing("g_tp.sram.Vbitfloating",g_tp.sram.Vbitfloating);
	printing("g_tp.cam.Vbitpre",g_tp.cam.Vbitpre);
	
	printing("g_tp.w_pmos_bl_precharge",g_tp.w_pmos_bl_precharge);
	printing("g_tp.w_pmos_bl_eq",g_tp.w_pmos_bl_eq);

	printing("g_tp.wire_local.pitch",g_tp.wire_local.pitch);
	printing("g_tp.wire_local.R_per_um",g_tp.wire_local.R_per_um);
	printing("g_tp.wire_local.C_per_um",g_tp.wire_local.C_per_um);
	printing("g_tp.wire_local.aspect_ratio",g_tp.wire_local.aspect_ratio);
	printing("g_tp.wire_local.ild_thickness",g_tp.wire_local.ild_thickness);
	printing("g_tp.wire_local.miller_value",g_tp.wire_local.miller_value);
	printing("g_tp.wire_local.horiz_dielectric_constant",g_tp.wire_local.horiz_dielectric_constant);
	printing("g_tp.wire_local.vert_dielectric_constant",g_tp.wire_local.vert_dielectric_constant);
	
	printing("g_tp.wire_inside_mat.pitch",g_tp.wire_inside_mat.pitch);
	printing("g_tp.wire_inside_mat.R_per_um",g_tp.wire_inside_mat.R_per_um);
	printing("g_tp.wire_inside_mat.C_per_um",g_tp.wire_inside_mat.C_per_um);
	printing("g_tp.wire_inside_mat.aspect_ratio",g_tp.wire_inside_mat.aspect_ratio);
	printing("g_tp.wire_inside_mat.ild_thickness",g_tp.wire_inside_mat.ild_thickness);
	printing("g_tp.wire_inside_mat.miller_value",g_tp.wire_inside_mat.miller_value);
	printing("g_tp.wire_inside_mat.horiz_dielectric_constant",g_tp.wire_inside_mat.horiz_dielectric_constant);
	printing("g_tp.wire_inside_mat.vert_dielectric_constant",g_tp.wire_inside_mat.vert_dielectric_constant);
	
	printing("g_tp.wire_outside_mat.pitch",g_tp.wire_outside_mat.pitch);
	printing("g_tp.wire_outside_mat.R_per_um",g_tp.wire_outside_mat.R_per_um);
	printing("g_tp.wire_outside_mat.C_per_um",g_tp.wire_outside_mat.C_per_um);
	printing("g_tp.wire_outside_mat.aspect_ratio",g_tp.wire_outside_mat.aspect_ratio);
	printing("g_tp.wire_outside_mat.ild_thickness",g_tp.wire_outside_mat.ild_thickness);
	printing("g_tp.wire_outside_mat.miller_value",g_tp.wire_outside_mat.miller_value);
	printing("g_tp.wire_outside_mat.horiz_dielectric_constant",g_tp.wire_outside_mat.horiz_dielectric_constant);
	printing("g_tp.wire_outside_mat.vert_dielectric_constant",g_tp.wire_outside_mat.vert_dielectric_constant);

	printing("g_tp.unit_len_wire_del",g_tp.unit_len_wire_del);

	printing("g_tp.sense_delay",g_tp.sense_delay);
	printing("g_tp.sense_dy_power",g_tp.sense_dy_power);
	
	printing("g_tp.tsv_parasitic_resistance_fine",g_tp.tsv_parasitic_resistance_fine);
	printing("g_tp.tsv_parasitic_capacitance_fine",g_tp.tsv_parasitic_capacitance_fine);
	printing("g_tp.tsv_minimum_area_fine",g_tp.tsv_minimum_area_fine);
	
	printing("g_tp.tsv_parasitic_resistance_coarse",g_tp.tsv_parasitic_resistance_coarse);
	printing("g_tp.tsv_parasitic_capacitance_coarse",g_tp.tsv_parasitic_capacitance_coarse);
	printing("g_tp.tsv_minimum_area_coarse",g_tp.tsv_minimum_area_coarse);
	
	printing("g_tp.tsv_minimum_area_coarse",g_tp.tsv_minimum_area_coarse);
	printing("g_tp.fringe_cap",g_tp.fringe_cap);
	printing("g_tp.kinv",g_tp.kinv);
	printing("g_tp.FO4",g_tp.FO4);

}
