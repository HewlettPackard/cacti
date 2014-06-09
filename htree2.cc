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

#include "htree2.h"
#include "wire.h"
#include <assert.h>
#include <iostream>

Htree2::Htree2(
    enum Wire_type wire_model, double mat_w, double mat_h, 
    int a_bits, int d_inbits, int d_outbits, int bl, int wl, enum Htree_type htree_type,
    bool uca_tree_, TechnologyParameter::DeviceType *dt)
 :in_rise_time(0), out_rise_time(0),
  tree_type(htree_type), mat_width(mat_w), mat_height(mat_h),
  add_bits(a_bits), data_in_bits(d_inbits), data_out_bits(d_outbits), ndbl(bl), ndwl(wl),
  uca_tree(uca_tree_), wt(wire_model), deviceType(dt)
{
  assert(bl >= 2 && wl >= 2);
  
//  if (bl == 1 && wl == 1)
//  {
//    delay = 0;
//    power.readOp.dynamic = 0;
//    power.readOp.leakage = 0;
//    area.w = mat_w;
//    area.h = mat_h;
//    return;
//  }
//  if (bl == 1) ndbl++;
//  if (wl == 1) ndwl++;

  max_unpipelined_link_delay = 0; //TODO
  min_w_nmos = g_tp.min_w_nmos_;
  min_w_pmos = deviceType->n_to_p_eff_curr_drv_ratio * min_w_nmos;

  switch (htree_type)
  {
    case Add_htree:
      wire_bw = init_wire_bw = add_bits;
      in_htree();
      break;
    case Data_in_htree:
      wire_bw = init_wire_bw = data_in_bits;
      in_htree();
      break;
    case Data_out_htree:
      wire_bw = init_wire_bw = data_out_bits;
      out_htree();
      break;
    default:
      assert(0);
      break;
  }

  power_bit = power;
  power.readOp.dynamic *= init_wire_bw;

  assert(power.readOp.dynamic >= 0);
  assert(power.readOp.leakage >= 0);
}



// nand gate sizing calculation
void Htree2::input_nand(double s1, double s2, double l_eff)
{
  Wire w1(wt, l_eff); 
  double pton_size = deviceType->n_to_p_eff_curr_drv_ratio; 
  // input capacitance of a repeater  = input capacitance of nand.
  double nsize = s1*(1 + pton_size)/(2 + pton_size);
  nsize = (nsize < 1) ? 1 : nsize;

  double tc = 2*tr_R_on(nsize*min_w_nmos, NCH, 1) *
    (drain_C_(nsize*min_w_nmos, NCH, 1, 1, g_tp.cell_h_def)*2 +
     2 * gate_C(s2*(min_w_nmos + min_w_pmos), 0));
  delay+= horowitz (w1.out_rise_time, tc, 
      deviceType->Vth/deviceType->Vdd, deviceType->Vth/deviceType->Vdd, RISE);
  power.readOp.dynamic += 0.5 *
    (2*drain_C_(pton_size * nsize*min_w_pmos, PCH, 1, 1, g_tp.cell_h_def)
     + drain_C_(nsize*min_w_nmos, NCH, 1, 1, g_tp.cell_h_def)
     + 2*gate_C(s2*(min_w_nmos + min_w_pmos), 0)) * 
    deviceType->Vdd * deviceType->Vdd;

  power.readOp.leakage += (wire_bw*cmos_Ileak(min_w_nmos*(nsize*2), min_w_pmos * nsize * 2, false)*deviceType->Vdd);
}



// tristate buffer model consisting of not, nand, nor, and driver transistors 
void Htree2::output_buffer(double s1, double s2, double l_eff)
{
  Wire w1(wt, l_eff); 
  double pton_size = deviceType->n_to_p_eff_curr_drv_ratio; 
  // input capacitance of repeater = input capacitance of nand + nor.
  double size = s1*(1 + pton_size)/(2 + pton_size + 1 + 2*pton_size);
  double s_eff =  //stage eff of a repeater in a wire
    (gate_C(s2*(min_w_nmos + min_w_pmos), 0) + w1.wire_cap(l_eff*1e-6))/
    gate_C(s2*(min_w_nmos + min_w_pmos), 0);
  double tr_size = gate_C(s1*(min_w_nmos + min_w_pmos), 0) * 1/2/(s_eff*gate_C(min_w_pmos, 0)); 
  size = (size < 1) ? 1 : size;

  double res_nor = 2*tr_R_on(size*min_w_pmos, PCH, 1);
  double res_ptrans = tr_R_on(tr_size*min_w_nmos, NCH, 1);
  double cap_nand_out = drain_C_(size*min_w_nmos, NCH, 1, 1, g_tp.cell_h_def) +
                        drain_C_(size*min_w_pmos, PCH, 1, 1, g_tp.cell_h_def)*2 +
                        gate_C(tr_size*min_w_pmos, 0);
  double cap_ptrans_out = 2 *(drain_C_(tr_size*min_w_pmos, PCH, 1, 1, g_tp.cell_h_def) +
                              drain_C_(tr_size*min_w_nmos, NCH, 1, 1, g_tp.cell_h_def)) +
                          gate_C(s1*(min_w_nmos + min_w_pmos), 0);

  double tc = res_nor * cap_nand_out + (res_nor + res_ptrans) * cap_ptrans_out;


  delay += horowitz (w1.out_rise_time, tc, 
      deviceType->Vth/deviceType->Vdd, deviceType->Vth/deviceType->Vdd, RISE);

  //nand
  power.readOp.dynamic += 0.5 *
    (2*drain_C_(size*min_w_pmos, PCH, 1, 1, g_tp.cell_h_def) +
       drain_C_(size*min_w_nmos, NCH, 1, 1, g_tp.cell_h_def) +
     gate_C(tr_size*(min_w_pmos), 0)) * 
    deviceType->Vdd * deviceType->Vdd;

  //not
  power.readOp.dynamic += 0.5 *
    (drain_C_(size*min_w_pmos, PCH, 1, 1, g_tp.cell_h_def)
     +drain_C_(size*min_w_nmos, NCH, 1, 1, g_tp.cell_h_def)
     +gate_C(size*(min_w_nmos + min_w_pmos), 0)) * 
    deviceType->Vdd * deviceType->Vdd;

  //nor
  power.readOp.dynamic += 0.5 *
    (drain_C_(size*min_w_pmos, PCH, 1, 1, g_tp.cell_h_def)
     + 2*drain_C_(size*min_w_nmos, NCH, 1, 1, g_tp.cell_h_def)
     +gate_C(tr_size*(min_w_nmos + min_w_pmos), 0)) * 
    deviceType->Vdd * deviceType->Vdd;

  //output transistor
  power.readOp.dynamic += 0.5 *
    ((drain_C_(tr_size*min_w_pmos, PCH, 1, 1, g_tp.cell_h_def)
      +drain_C_(tr_size*min_w_nmos, NCH, 1, 1, g_tp.cell_h_def))*2
     + gate_C(s1*(min_w_nmos + min_w_pmos), 0)) * 
    deviceType->Vdd * deviceType->Vdd;

  if(uca_tree) {
    power.readOp.leakage += (deviceType->Vdd*cmos_Ileak(min_w_nmos*(tr_size*2/*inverter + output tr*/ + size*6 /*nand + nor*/), 
        min_w_pmos*(tr_size*2 + size*6), false)*wire_bw);
  }
  else {
    power.readOp.leakage += (deviceType->Vdd*cmos_Ileak(min_w_nmos*(tr_size*2/*inverter + output tr*/ + size*6 /*nand + nor*/), 
        min_w_pmos*(tr_size*2 + size*6), false)*wire_bw);
  }
}



/* calculates the input h-tree delay/power
 * A nand gate is used at each node to 
 * limit the signal 
 * The area of an unbalanced htree (rows != columns) 
 * depends on how data is traversed.
 * In the following function, if ( no. of rows < no. of columns),
 * then data first traverse in excess hor. links until vertical
 * and horizontal nodes are same.
 * If no. of rows is bigger, then data traverse in 
 * a hor. link followed by a ver. link in a repeated 
 * fashion (similar to a balanced tree) until there are no 
 * hor. links left. After this it goes through the remaining vertical
 * links.
 */
  void 
Htree2::in_htree()
{
  //temp var
  double s1 = 0, s2 = 0, s3 = 0;
  double l_eff = 0;
  Wire *wtemp1 = 0, *wtemp2 = 0, *wtemp3 = 0;
  double len = 0, ht = 0;
  int option = 0;

  int h = (int) _log2(ndwl/2); // horizontal nodes
  int v = (int) _log2(ndbl/2); // vertical nodes
  double len_temp;
  double ht_temp;
  if (uca_tree)
  {
    ht_temp = (mat_height*ndbl/2 +/* since uca_tree models interbank tree, mat_height => bank height */  
        ((add_bits + data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch *
         2 * (1-pow(0.5,h))))/2;
    len_temp = (mat_width*ndwl/2 +
        ((add_bits + data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch *
         2 * (1-pow(0.5,v))))/2;
  }
  else
  {
    if (ndwl == ndbl) {
      ht_temp = ((mat_height*ndbl/2) +
          (add_bits * (ndbl/2-1) * g_tp.wire_outside_mat.pitch) +
          ((data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * h)
          )/2;  
      len_temp = (mat_width*ndwl/2 +
        (add_bits * (ndwl/2-1) * g_tp.wire_outside_mat.pitch) +
        ((data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * v))/2;
    }
    else if (ndwl > ndbl) {
      double excess_part = (_log2(ndwl/2) - _log2(ndbl/2));
      ht_temp = ((mat_height*ndbl/2) +
          (add_bits * ((ndbl/2-1) + excess_part) * g_tp.wire_outside_mat.pitch) +
          (data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * 
          (2*(1 - pow(0.5, h-v)) + pow(0.5, v-h) * v))/2;
      len_temp = (mat_width*ndwl/2 +
        (add_bits * (ndwl/2-1) * g_tp.wire_outside_mat.pitch) +
        ((data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * v))/2;
    }
    else {
      double excess_part = (_log2(ndbl/2) - _log2(ndwl/2));
      ht_temp = ((mat_height*ndbl/2) +
          (add_bits * ((ndwl/2-1) + excess_part) * g_tp.wire_outside_mat.pitch) +
          ((data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * h)
          )/2;  
      len_temp = (mat_width*ndwl/2 + 
          (add_bits * ((ndwl/2-1) + excess_part) * g_tp.wire_outside_mat.pitch) +
          (data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * (h + 2*(1-pow(0.5, v-h))))/2;
    }
  }

  area.h   = ht_temp * 2;
  area.w   = len_temp * 2;
  delay = 0;
  power.readOp.dynamic = 0;
  power.readOp.leakage = 0;
  len = len_temp;
  ht  = ht_temp/2;

  while (v > 0 || h > 0)
  {
    if (wtemp1) delete wtemp1;
    if (wtemp2) delete wtemp2;
    if (wtemp3) delete wtemp3;

    if (h > v)
    {
      //the iteration considers only one horizontal link
      wtemp1 = new Wire(wt, len); // hor
      wtemp2 = new Wire(wt, len/2);  // next hor
      len_temp = len;
      len /= 2;
      wtemp3 = 0; 
      h--;
      option = 0;
    }
    else if (v>0 && h>0)
    {
      //considers one horizontal link and one vertical link
      wtemp1 = new Wire(wt, len); // hor
      wtemp2 = new Wire(wt, ht);  // ver
      wtemp3 = new Wire(wt, len/2);  // next hor
      len_temp = len;
      ht_temp = ht;
      len /= 2;
      ht  /= 2;
      v--;
      h--;
      option = 1;
    }
    else
    {
      // considers only one vertical link
      assert(h == 0);
      wtemp1 = new Wire(wt, ht); // ver
      wtemp2 = new Wire(wt, ht/2);  // next ver
      ht_temp = ht;
      ht /= 2;
      wtemp3 = 0; 
      v--;
      option = 2;
    }

    delay += wtemp1->delay;
    power.readOp.dynamic += wtemp1->power.readOp.dynamic;
    power.readOp.leakage += wtemp1->power.readOp.leakage*wire_bw;
    if (uca_tree == false && option == 2)
    {
      wire_bw*=2;  // wire bandwidth doubles only for vertical branches
    }

    if (uca_tree == false)
    {
      if (len_temp > wtemp1->repeater_spacing)
      {
        s1 = wtemp1->repeater_size;
        l_eff = wtemp1->repeater_spacing;
      }
      else
      {
        s1 = (len_temp/wtemp1->repeater_spacing) * wtemp1->repeater_size;
        l_eff = len_temp;
      }
      
      if (ht_temp > wtemp2->repeater_spacing)
      {
        s2 = wtemp2->repeater_size;
      }
      else
      {
        s2 = (len_temp/wtemp2->repeater_spacing) * wtemp2->repeater_size;
      }
      // first level
      input_nand(s1, s2, l_eff);
    }


    if (option != 1)
    {
      continue;
    }

    // second level
    delay += wtemp2->delay;
    power.readOp.dynamic += wtemp2->power.readOp.dynamic;
    power.readOp.leakage += wtemp2->power.readOp.leakage*wire_bw;

    if (uca_tree)
    {
      power.readOp.leakage += (wtemp2->power.readOp.leakage*wire_bw);
    }
    else
    {
      power.readOp.leakage += (wtemp2->power.readOp.leakage*wire_bw);
      wire_bw*=2;
      
      if (ht_temp > wtemp3->repeater_spacing)
      {
        s3    = wtemp3->repeater_size;
        l_eff = wtemp3->repeater_spacing;
      }
      else
      {
        s3    = (len_temp/wtemp3->repeater_spacing) * wtemp3->repeater_size;
        l_eff = ht_temp;
      }
      
      input_nand(s2, s3, l_eff);
    }
  }

  if (wtemp1) delete wtemp1;
  if (wtemp2) delete wtemp2;
  if (wtemp3) delete wtemp3;
}



/* a tristate buffer is used to handle fan-ins 
 * The area of an unbalanced htree (rows != columns) 
 * depends on how data is traversed.
 * In the following function, if ( no. of rows < no. of columns),
 * then data first traverse in excess hor. links until vertical
 * and horizontal nodes are same.
 * If no. of rows is bigger, then data traverse in 
 * a hor. link followed by a ver. link in a repeated 
 * fashion (similar to a balanced tree) until there are no 
 * hor. links left. After this it goes through the remaining vertical
 * links.
 */
void Htree2::out_htree()
{
  //temp var
  double s1 = 0, s2 = 0, s3 = 0;
  double l_eff = 0;
  Wire *wtemp1 = 0, *wtemp2 = 0, *wtemp3 = 0;
  double len = 0, ht = 0;
  int option = 0;

  int h = (int) _log2(ndwl/2);
  int v = (int) _log2(ndbl/2);
  double len_temp;
  double ht_temp;
  if (uca_tree)
  {
    ht_temp = (mat_height*ndbl/2 +/* since uca_tree models interbank tree, mat_height => bank height */  
        ((add_bits + data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch *
         2 * (1-pow(0.5,h))))/2;
    len_temp = (mat_width*ndwl/2 +
        ((add_bits + data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch *
         2 * (1-pow(0.5,v))))/2;
  }
  else
  {
    if (ndwl == ndbl) {
      ht_temp = ((mat_height*ndbl/2) +
          (add_bits * (ndbl/2-1) * g_tp.wire_outside_mat.pitch) +
          ((data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * h)
          )/2;  
      len_temp = (mat_width*ndwl/2 +
        (add_bits * (ndwl/2-1) * g_tp.wire_outside_mat.pitch) +
        ((data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * v))/2;
    }
    else if (ndwl > ndbl) {
      double excess_part = (_log2(ndwl/2) - _log2(ndbl/2));
      ht_temp = ((mat_height*ndbl/2) +
          (add_bits * ((ndbl/2-1) + excess_part) * g_tp.wire_outside_mat.pitch) +
          (data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * 
          (2*(1 - pow(0.5, h-v)) + pow(0.5, v-h) * v))/2;
      len_temp = (mat_width*ndwl/2 +
        (add_bits * (ndwl/2-1) * g_tp.wire_outside_mat.pitch) +
        ((data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * v))/2;
    }
    else {
      double excess_part = (_log2(ndbl/2) - _log2(ndwl/2));
      ht_temp = ((mat_height*ndbl/2) +
          (add_bits * ((ndwl/2-1) + excess_part) * g_tp.wire_outside_mat.pitch) +
          ((data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * h)
          )/2;  
      len_temp = (mat_width*ndwl/2 + 
          (add_bits * ((ndwl/2-1) + excess_part) * g_tp.wire_outside_mat.pitch) +
          (data_in_bits + data_out_bits) * g_tp.wire_outside_mat.pitch * (h + 2*(1-pow(0.5, v-h))))/2;
    }
  }

  area.h = ht_temp * 2;
  area.w = len_temp * 2;
  delay = 0;
  power.readOp.dynamic = 0;
  power.readOp.leakage = 0;
  len = len_temp;
  ht = ht_temp/2;

  while (v > 0 || h > 0)
  { //finds delay/power of each link in the tree
    if (wtemp1) delete wtemp1;
    if (wtemp2) delete wtemp2;
    if (wtemp3) delete wtemp3;

    if(h > v) {
      //the iteration considers only one horizontal link
      wtemp1 = new Wire(wt, len); // hor
      wtemp2 = new Wire(wt, len/2);  // next hor
      len_temp = len;
      len /= 2;
      wtemp3 = 0; 
      h--;
      option = 0;
    }
    else if (v>0 && h>0) {
      //considers one horizontal link and one vertical link
      wtemp1 = new Wire(wt, len); // hor
      wtemp2 = new Wire(wt, ht);  // ver
      wtemp3 = new Wire(wt, len/2);  // next hor
      len_temp = len;
      ht_temp = ht;
      len /= 2;
      ht /= 2;
      v--;
      h--;
      option = 1;
    }
    else {
      // considers only one vertical link
      assert(h == 0);
      wtemp1 = new Wire(wt, ht); // ver
      wtemp2 = new Wire(wt, ht/2);  // next ver
      ht_temp = ht;
      ht /= 2;
      wtemp3 = 0; 
      v--;
      option = 2;
    }
    delay += wtemp1->delay;
    power.readOp.dynamic += wtemp1->power.readOp.dynamic;
    power.readOp.leakage += wtemp1->power.readOp.leakage*wire_bw;
    if (uca_tree == false && option == 2)
    {
      wire_bw*=2;
    }

    if (uca_tree == false)
    {
      if (len_temp > wtemp1->repeater_spacing)
      {
        s1 = wtemp1->repeater_size;
        l_eff = wtemp1->repeater_spacing;
      }
      else
      {
        s1 = (len_temp/wtemp1->repeater_spacing) * wtemp1->repeater_size;
        l_eff = len_temp;
      }
      if (ht_temp > wtemp2->repeater_spacing)
      {
        s2 = wtemp2->repeater_size;
      }
      else
      {
        s2 = (len_temp/wtemp2->repeater_spacing) * wtemp2->repeater_size;
      }
      // first level
      output_buffer(s1, s2, l_eff);
    }


    if (option != 1)
    {
      continue;
    }

    // second level
    delay += wtemp2->delay;
    power.readOp.dynamic += wtemp2->power.readOp.dynamic;
    power.readOp.leakage += wtemp2->power.readOp.leakage*wire_bw;
    if (uca_tree)
    {
      power.readOp.leakage += (wtemp2->power.readOp.leakage*wire_bw);
    }
    else
    {
      power.readOp.leakage += (wtemp2->power.readOp.leakage*wire_bw);
      wire_bw*=2;

      if (ht_temp > wtemp3->repeater_spacing)
      {
        s3 = wtemp3->repeater_size;
        l_eff = wtemp3->repeater_spacing;
      }
      else
      {
        s3 = (len_temp/wtemp3->repeater_spacing) * wtemp3->repeater_size;
        l_eff = ht_temp;
      }
      
      output_buffer(s2, s3, l_eff);
    }
  }

  if (wtemp1) delete wtemp1;
  if (wtemp2) delete wtemp2;
  if (wtemp3) delete wtemp3;
}

