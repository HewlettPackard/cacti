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

#include <time.h>
#include <math.h>


#include "area.h"
#include "basic_circuit.h"
#include "component.h"
#include "const.h"
#include "parameter.h"
#include "cacti_interface.h"
#include "Ucache.h"

#include <pthread.h>
#include <iostream>
#include <algorithm>

using namespace std;


bool mem_array::lt(const mem_array * m1, const mem_array * m2)
{
  if (m1->Nspd < m2->Nspd) return true;
  else if (m1->Nspd > m2->Nspd) return false;
  else if (m1->Ndwl < m2->Ndwl) return true;
  else if (m1->Ndwl > m2->Ndwl) return false;
  else if (m1->Ndbl < m2->Ndbl) return true;
  else if (m1->Ndbl > m2->Ndbl) return false;
  else if (m1->deg_bl_muxing < m2->deg_bl_muxing) return true;
  else if (m1->deg_bl_muxing > m2->deg_bl_muxing) return false;
  else if (m1->Ndsam_lev_1 < m2->Ndsam_lev_1) return true;
  else if (m1->Ndsam_lev_1 > m2->Ndsam_lev_1) return false;
  else if (m1->Ndsam_lev_2 < m2->Ndsam_lev_2) return true;
  else return false;
}



void uca_org_t::find_delay()
{
  mem_array * data_arr = data_array2;
  mem_array * tag_arr  = tag_array2;

  // check whether it is a regular cache or scratch ram
  if (g_ip->is_cache == false)
  {
    access_time = data_arr->access_time;
  }
  // Both tag and data lookup happen in parallel
  // and the entire set is sent over the data array h-tree without
  // waiting for the way-select signal --TODO add the corresponding
  // power overhead Nav
  else if (g_ip->fast_access == true)
  {
    access_time = MAX(tag_arr->access_time, data_arr->access_time);
  }
  // Tag is accessed first. On a hit, way-select signal along with the
  // address is sent to read/write the appropriate block in the data
  // array
  else if (g_ip->is_seq_acc == true)
  {
    access_time = tag_arr->access_time + data_arr->access_time;
  }
  // Normal access: tag array access and data array access happen in parallel.
  // But, the data array will wait for the way-select and transfer only the
  // appropriate block over the h-tree.
  else
  {
    access_time = MAX(tag_arr->access_time + data_arr->delay_senseamp_mux_decoder,
                      data_arr->delay_before_subarray_output_driver) +
                  data_arr->delay_from_subarray_output_driver_to_output;
  }
}



void uca_org_t::find_energy()
{
  if (g_ip->is_cache) 
    power = data_array2->power + tag_array2->power;
  else 
    power = data_array2->power;
}



void uca_org_t::find_area()
{
  if (g_ip->is_cache == false)
  {
    cache_ht  = data_array2->height;
    cache_len = data_array2->width;
  }
  else
  {
    cache_ht  = MAX(tag_array2->height, data_array2->height);
    cache_len = tag_array2->width + data_array2->width;
  }
  area = cache_ht * cache_len;
}



void uca_org_t::find_cyc()
{
  if (g_ip->is_cache == false)
  {
    cycle_time = data_array2->cycle_time;
  }
  else
  {
    cycle_time = MAX(tag_array2->cycle_time, 
                    data_array2->cycle_time);
  }
}


