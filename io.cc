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



#include <fstream>
#include <iostream>
#include <sstream>


#include "io.h"
#include "area.h"
#include "basic_circuit.h"
#include "parameter.h"
#include "Ucache.h"
#include "nuca.h"
#include "crossbar.h"
#include "arbiter.h"
//#include "highradix.h"
#include "TSV.h"
#include "memorybus.h"
#include "version_cacti.h"

#include "extio.h"
#include "extio_technology.h"
#include "memcad.h"

using namespace std;


InputParameter::InputParameter()
:  array_power_gated(false),
 bitline_floating(false),
 wl_power_gated(false),
 cl_power_gated(false),
 interconect_power_gated(false),
 power_gating(false),
 cl_vertical (true)
{

}

/* Parses "cache.cfg" file */
  void
InputParameter::parse_cfg(const string & in_file)
{
  FILE *fp = fopen(in_file.c_str(), "r");
  char line[5000];
  char jk[5000];
  char temp_var[5000];

  if(!fp) {
    cout << in_file << " is missing!\n";
    exit(-1);
  }

  while(fscanf(fp, "%[^\n]\n", line) != EOF) {

    if (!strncmp("-size", line, strlen("-size"))) {
      sscanf(line, "-size %[(:-~)*]%u", jk, &(cache_sz));
      if (g_ip->print_detail_debug)
          	cout << "cache size: " << g_ip->cache_sz << "GB" << endl;
      continue;
    }



    if (!strncmp("-page size", line, strlen("-page size"))) {
      sscanf(line, "-page size %[(:-~)*]%u", jk, &(page_sz_bits));
      continue;
    }

    if (!strncmp("-burst length", line, strlen("-burst length"))) {
      sscanf(line, "-burst %[(:-~)*]%u", jk, &(burst_len));
      continue;
    }

    if (!strncmp("-internal prefetch width", line, strlen("-internal prefetch width"))) {
      sscanf(line, "-internal prefetch %[(:-~)*]%u", jk, &(int_prefetch_w));
      continue;
    }

    if (!strncmp("-block", line, strlen("-block"))) {
      sscanf(line, "-block size (bytes) %d", &(line_sz));
      continue;
    }

    if (!strncmp("-associativity", line, strlen("-associativity"))) {
      sscanf(line, "-associativity %d", &(assoc));
      continue;
    }

    if (!strncmp("-read-write", line, strlen("-read-write"))) {
      sscanf(line, "-read-write port %d", &(num_rw_ports));
      continue;
    }

    if (!strncmp("-exclusive read", line, strlen("exclusive read"))) {
      sscanf(line, "-exclusive read port %d", &(num_rd_ports));
      continue;
    }

    if(!strncmp("-exclusive write", line, strlen("-exclusive write"))) {
      sscanf(line, "-exclusive write port %d", &(num_wr_ports));
      continue;
    }

    if (!strncmp("-single ended", line, strlen("-single ended"))) {
      sscanf(line, "-single %[(:-~)*]%d", jk,
          &(num_se_rd_ports));
      continue;
    }

    if (!strncmp("-search", line, strlen("-search"))) {
      sscanf(line, "-search port %d", &(num_search_ports));
      continue;
    }

    if (!strncmp("-UCA bank", line, strlen("-UCA bank"))) {
      sscanf(line, "-UCA bank%[((:-~)| )*]%d", jk, &(nbanks));
      continue;
    }

    if (!strncmp("-technology", line, strlen("-technology"))) {
      sscanf(line, "-technology (u) %lf", &(F_sz_um));
      F_sz_nm = F_sz_um*1000;
      continue;
    }

    if (!strncmp("-output/input", line, strlen("-output/input"))) {
      sscanf(line, "-output/input bus %[(:-~)*]%d", jk, &(out_w));
      continue;
    }

    if (!strncmp("-operating temperature", line, strlen("-operating temperature"))) {
      sscanf(line, "-operating temperature %[(:-~)*]%d", jk, &(temp));
      continue;
    }

    if (!strncmp("-cache type", line, strlen("-cache type"))) {
      sscanf(line, "-cache type%[^\"]\"%[^\"]\"", jk, temp_var);

      if (!strncmp("cache", temp_var, sizeof("cache"))) {
        is_cache = true;
      }
      else
      {
        is_cache = false;
      }

      if (!strncmp("main memory", temp_var, sizeof("main memory"))) {
        is_main_mem = true;
      }
      else {
        is_main_mem = false;
      }

      if (!strncmp("3D memory or 2D main memory", temp_var, sizeof("3D memory or 2D main memory"))) {
    	  is_3d_mem = true;
    	  is_main_mem = true;
      }
      else {
    	  is_3d_mem = false;
    	  //is_main_mem = false;
      }

	  if (g_ip->print_detail_debug)
	  {cout << "io.cc: is_3d_mem = " << is_3d_mem << endl;}

      if (!strncmp("cam", temp_var, sizeof("cam"))) {
        pure_cam = true;
      }
      else {
        pure_cam = false;
      }

      if (!strncmp("ram", temp_var, sizeof("ram"))) {
        pure_ram = true;
      }
      else {
    	  if (!is_main_mem)
    		  pure_ram = false;
    	  else
    		  pure_ram = true;
      }

      continue;
    }

    if (!strncmp("-print option", line, strlen("-print option"))) {
    	sscanf(line, "-print option%[^\"]\"%[^\"]\"", jk, temp_var);

    	if (!strncmp("debug detail", temp_var, sizeof("debug detail"))) {
    		print_detail_debug = true;
    	}
    	else {
    		print_detail_debug = false;
    	}
    	if (g_ip->print_detail_debug)
    	{cout << "io.cc: print_detail_debug = " << print_detail_debug << endl;}
    	continue;
    }

    if (!strncmp("-burst depth", line, strlen("-burst depth"))) {
      sscanf(line, "-burst %[(:-~)*]%u", jk, &(burst_depth));
      continue;
    }

    if (!strncmp("-IO width", line, strlen("-IO width"))) {
      sscanf(line, "-IO %[(:-~)*]%u", jk, &(io_width));
      continue;
    }

    if (!strncmp("-system frequency", line, strlen("-system frequency"))) {
      sscanf(line, "-system frequency %[(:-~)*]%u", jk, &(sys_freq_MHz));
      if(g_ip->print_detail_debug)
    	  cout << "system frequency: " << g_ip->sys_freq_MHz  << endl;
      continue;
    }



    if (!strncmp("-stacked die", line, strlen("-stacked die"))) {
      sscanf(line, "-stacked die %[(:-~)*]%u", jk, &(num_die_3d));
      if(g_ip->print_detail_debug)
    	  cout << "num_die_3d: " << g_ip->num_die_3d  << endl;
      continue;
    }

    if (!strncmp("-partitioning granularity", line, strlen("-partitioning granularity"))) {
      sscanf(line, "-partitioning %[(:-~)*]%u", jk, &(partition_gran));
      if(g_ip->print_detail_debug)
    	  cout << "partitioning granularity: " << g_ip->partition_gran  << endl;
      continue;
    }

    if (!strncmp("-TSV projection", line, strlen("-TSV projection"))) {
        sscanf(line, "-TSV %[(:-~)*]%u", jk, &(TSV_proj_type));
        if(g_ip->print_detail_debug)
        	cout << "TSV projection: " << g_ip->TSV_proj_type  << endl;
        continue;
    }


    //g_ip->print_detail_debug = debug_detail;


    //g_ip->partition_gran = 1;

    // --- These two parameters are supposed for bank level partitioning, currently not shown to public
    g_ip->num_tier_row_sprd = 1;
    g_ip->num_tier_col_sprd = 1;

    if (!strncmp("-tag size", line, strlen("-tag size"))) {
      sscanf(line, "-tag size%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("default", temp_var, sizeof("default"))) {
        specific_tag = false;
        tag_w = 42; /* the actual value is calculated
                     * later based on the cache size, bank count, and associativity
                     */
      }
      else {
        specific_tag = true;
        sscanf(line, "-tag size (b) %d", &(tag_w));
      }
      continue;
    }

    if (!strncmp("-access mode", line, strlen("-access mode"))) {
      sscanf(line, "-access %[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("fast", temp_var, strlen("fast"))) {
        access_mode = 2;
      }
      else if (!strncmp("sequential", temp_var, strlen("sequential"))) {
        access_mode = 1;
      }
      else if(!strncmp("normal", temp_var, strlen("normal"))) {
        access_mode = 0;
      }
      else {
        cout << "ERROR: Invalid access mode!\n";
        exit(0);
      }
      continue;
    }

    if (!strncmp("-Data array cell type", line, strlen("-Data array cell type"))) {
      sscanf(line, "-Data array cell type %[^\"]\"%[^\"]\"", jk, temp_var);

      if(!strncmp("itrs-hp", temp_var, strlen("itrs-hp"))) {
        data_arr_ram_cell_tech_type = 0;
      }
      else if(!strncmp("itrs-lstp", temp_var, strlen("itrs-lstp"))) {
        data_arr_ram_cell_tech_type = 1;
      }
      else if(!strncmp("itrs-lop", temp_var, strlen("itrs-lop"))) {
        data_arr_ram_cell_tech_type = 2;
      }
      else if(!strncmp("lp-dram", temp_var, strlen("lp-dram"))) {
        data_arr_ram_cell_tech_type = 3;
      }
      else if(!strncmp("comm-dram", temp_var, strlen("comm-dram"))) {
        data_arr_ram_cell_tech_type = 4;
      }
      else {
        cout << "ERROR: Invalid type!\n";
        exit(0);
      }
      continue;
    }

    if (!strncmp("-Data array peripheral type", line, strlen("-Data array peripheral type"))) {
      sscanf(line, "-Data array peripheral type %[^\"]\"%[^\"]\"", jk, temp_var);

      if(!strncmp("itrs-hp", temp_var, strlen("itrs-hp"))) {
        data_arr_peri_global_tech_type = 0;
      }
      else if(!strncmp("itrs-lstp", temp_var, strlen("itrs-lstp"))) {
        data_arr_peri_global_tech_type = 1;
      }
      else if(!strncmp("itrs-lop", temp_var, strlen("itrs-lop"))) {
        data_arr_peri_global_tech_type = 2;
      }
      else {
        cout << "ERROR: Invalid type!\n";
        exit(0);
      }
      continue;
    }

    if (!strncmp("-Tag array cell type", line, strlen("-Tag array cell type"))) {
      sscanf(line, "-Tag array cell type %[^\"]\"%[^\"]\"", jk, temp_var);

      if(!strncmp("itrs-hp", temp_var, strlen("itrs-hp"))) {
        tag_arr_ram_cell_tech_type = 0;
      }
      else if(!strncmp("itrs-lstp", temp_var, strlen("itrs-lstp"))) {
        tag_arr_ram_cell_tech_type = 1;
      }
      else if(!strncmp("itrs-lop", temp_var, strlen("itrs-lop"))) {
        tag_arr_ram_cell_tech_type = 2;
      }
      else if(!strncmp("lp-dram", temp_var, strlen("lp-dram"))) {
        tag_arr_ram_cell_tech_type = 3;
      }
      else if(!strncmp("comm-dram", temp_var, strlen("comm-dram"))) {
        tag_arr_ram_cell_tech_type = 4;
      }
      else {
        cout << "ERROR: Invalid type!\n";
        exit(0);
      }
      continue;
    }

    if (!strncmp("-Tag array peripheral type", line, strlen("-Tag array peripheral type"))) {
      sscanf(line, "-Tag array peripheral type %[^\"]\"%[^\"]\"", jk, temp_var);

      if(!strncmp("itrs-hp", temp_var, strlen("itrs-hp"))) {
        tag_arr_peri_global_tech_type = 0;
      }
      else if(!strncmp("itrs-lstp", temp_var, strlen("itrs-lstp"))) {
        tag_arr_peri_global_tech_type = 1;
      }
      else if(!strncmp("itrs-lop", temp_var, strlen("itrs-lop"))) {
        tag_arr_peri_global_tech_type = 2;
      }
      else {
        cout << "ERROR: Invalid type!\n";
        exit(0);
      }
      continue;
    }
    if(!strncmp("-design", line, strlen("-design"))) {
      sscanf(line, "-%[((:-~)| |,)*]%d:%d:%d:%d:%d", jk,
          &(delay_wt), &(dynamic_power_wt),
          &(leakage_power_wt),
          &(cycle_time_wt), &(area_wt));
      continue;
    }

    if(!strncmp("-deviate", line, strlen("-deviate"))) {
      sscanf(line, "-%[((:-~)| |,)*]%d:%d:%d:%d:%d", jk,
          &(delay_dev), &(dynamic_power_dev),
          &(leakage_power_dev),
          &(cycle_time_dev), &(area_dev));
      continue;
    }

    if(!strncmp("-Optimize", line, strlen("-Optimize"))) {
      sscanf(line, "-Optimize  %[^\"]\"%[^\"]\"", jk, temp_var);

      if(!strncmp("ED^2", temp_var, strlen("ED^2"))) {
        ed = 2;
      }
      else if(!strncmp("ED", temp_var, strlen("ED"))) {
        ed = 1;
      }
      else {
        ed = 0;
      }
    }

    if(!strncmp("-NUCAdesign", line, strlen("-NUCAdesign"))) {
      sscanf(line, "-%[((:-~)| |,)*]%d:%d:%d:%d:%d", jk,
          &(delay_wt_nuca), &(dynamic_power_wt_nuca),
          &(leakage_power_wt_nuca),
          &(cycle_time_wt_nuca), &(area_wt_nuca));
      continue;
    }

    if(!strncmp("-NUCAdeviate", line, strlen("-NUCAdeviate"))) {
      sscanf(line, "-%[((:-~)| |,)*]%d:%d:%d:%d:%d", jk,
          &(delay_dev_nuca), &(dynamic_power_dev_nuca),
          &(leakage_power_dev_nuca),
          &(cycle_time_dev_nuca), &(area_dev_nuca));
      continue;
    }

    if(!strncmp("-Cache model", line, strlen("-cache model"))) {
      sscanf(line, "-Cache model %[^\"]\"%[^\"]\"", jk, temp_var);

      if (!strncmp("UCA", temp_var, strlen("UCA"))) {
        nuca = 0;
      }
      else {
        nuca = 1;
      }
      continue;
    }

    if(!strncmp("-NUCA bank", line, strlen("-NUCA bank"))) {
      sscanf(line, "-NUCA bank count %d", &(nuca_bank_count));

      if (nuca_bank_count != 0) {
        force_nuca_bank = 1;
      }
      continue;
    }

    if(!strncmp("-Wire inside mat", line, strlen("-Wire inside mat"))) {
      sscanf(line, "-Wire%[^\"]\"%[^\"]\"", jk, temp_var);

      if (!strncmp("global", temp_var, strlen("global"))) {
        wire_is_mat_type = 2;
        continue;
      }
      else if (!strncmp("local", temp_var, strlen("local"))) {
        wire_is_mat_type = 0;
        continue;
      }
      else {
        wire_is_mat_type = 1;
        continue;
      }
    }

    if(!strncmp("-Wire outside mat", line, strlen("-Wire outside mat"))) {
      sscanf(line, "-Wire%[^\"]\"%[^\"]\"", jk, temp_var);

      if (!strncmp("global", temp_var, strlen("global"))) {
        wire_os_mat_type = 2;
      }
      else {
        wire_os_mat_type = 1;
      }
      continue;
    }

    if(!strncmp("-Interconnect projection", line, strlen("-Interconnect projection"))) {
      sscanf(line, "-Interconnect projection%[^\"]\"%[^\"]\"", jk, temp_var);

      if (!strncmp("aggressive", temp_var, strlen("aggressive"))) {
        ic_proj_type = 0;
      }
      else {
        ic_proj_type = 1;
      }
      continue;
    }

    if(!strncmp("-Wire signaling", line, strlen("-wire signaling"))) {
      sscanf(line, "-Wire%[^\"]\"%[^\"]\"", jk, temp_var);

      if (!strncmp("default", temp_var, strlen("default"))) {
        force_wiretype = 0;
        wt = Global;
      }
      else if (!(strncmp("Global_10", temp_var, strlen("Global_10")))) {
        force_wiretype = 1;
        wt = Global_10;
      }
      else if (!(strncmp("Global_20", temp_var, strlen("Global_20")))) {
        force_wiretype = 1;
        wt = Global_20;
      }
      else if (!(strncmp("Global_30", temp_var, strlen("Global_30")))) {
        force_wiretype = 1;
        wt = Global_30;
      }
      else if (!(strncmp("Global_5", temp_var, strlen("Global_5")))) {
        force_wiretype = 1;
        wt = Global_5;
      }
      else if (!(strncmp("Global", temp_var, strlen("Global")))) {
        force_wiretype = 1;
        wt = Global;
      }
      else if (!(strncmp("fullswing", temp_var, strlen("fullswing")))) {
        force_wiretype = 1;
        wt = Full_swing;        
      }
      else if (!(strncmp("lowswing", temp_var, strlen("lowswing")))) {
        force_wiretype = 1;
        wt = Low_swing;        
      }
      else {
        cout << "Unknown wire type!\n";
        exit(0);
      }
      continue;
    }



    if(!strncmp("-Core", line, strlen("-Core"))) {
      sscanf(line, "-Core count %d\n", &(cores));
      if (cores > 16) {
        printf("No. of cores should be less than 16!\n");
      }
      continue;
    }

    if(!strncmp("-Cache level", line, strlen("-Cache level"))) {
      sscanf(line, "-Cache l%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("L2", temp_var, strlen("L2"))) {
        cache_level = 0;
      }
      else {
        cache_level = 1;
      }
    }

    if(!strncmp("-Print level", line, strlen("-Print level"))) {
      sscanf(line, "-Print l%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("DETAILED", temp_var, strlen("DETAILED"))) {
        print_detail = 1;
      }
      else {
        print_detail = 0;
      }

    }
    if(!strncmp("-Add ECC", line, strlen("-Add ECC"))) {
      sscanf(line, "-Add ECC %[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("true", temp_var, strlen("true"))) {
        add_ecc_b_ = true;
      }
      else {
        add_ecc_b_ = false;
      }
    }

    if(!strncmp("-CLDriver vertical", line, strlen("-CLDriver vertical"))) {
    	sscanf(line, "-CLDriver vertical %[^\"]\"%[^\"]\"", jk, temp_var);
    	if (!strncmp("true", temp_var, strlen("true"))) {
    		cl_vertical = true;
    	}
    	else {
    		cl_vertical = false;
    	}
    }

    if(!strncmp("-Array Power Gating", line, strlen("-Array Power Gating"))) {
    	sscanf(line, "-Array Power Gating %[^\"]\"%[^\"]\"", jk, temp_var);
    	if (!strncmp("true", temp_var, strlen("true"))) {
    		array_power_gated = true;
    	}
    	else {
    		array_power_gated = false;
    	}
    }

    if(!strncmp("-Bitline floating", line, strlen("-Bitline floating"))) {
    	sscanf(line, "-Bitline floating %[^\"]\"%[^\"]\"", jk, temp_var);
    	if (!strncmp("true", temp_var, strlen("true"))) {
    		bitline_floating = true;
    	}
    	else {
    		bitline_floating = false;
    	}
    }

    if(!strncmp("-WL Power Gating", line, strlen("-WL Power Gating"))) {
    	sscanf(line, "-WL Power Gating %[^\"]\"%[^\"]\"", jk, temp_var);
    	if (!strncmp("true", temp_var, strlen("true"))) {
    		wl_power_gated = true;
    	}
    	else {
    		wl_power_gated = false;
    	}
    }

    if(!strncmp("-CL Power Gating", line, strlen("-CL Power Gating"))) {
    	sscanf(line, "-CL Power Gating %[^\"]\"%[^\"]\"", jk, temp_var);
    	if (!strncmp("true", temp_var, strlen("true"))) {
    		cl_power_gated = true;
    	}
    	else {
    		cl_power_gated = false;
    	}
    }

    if(!strncmp("-Interconnect Power Gating", line, strlen("-Interconnect Power Gating"))) {
    	sscanf(line, "-Interconnect Power Gating %[^\"]\"%[^\"]\"", jk, temp_var);
    	if (!strncmp("true", temp_var, strlen("true"))) {
    		interconect_power_gated = true;
    	}
    	else {
    		interconect_power_gated = false;
    	}
    }

    if(!strncmp("-Power Gating Performance Loss", line, strlen("-Power Gating Performance Loss"))) {
        sscanf(line, "-Power Gating Performance Loss %lf", &(perfloss));
        continue;
    }

    if(!strncmp("-Print input parameters", line, strlen("-Print input parameters"))) {
      sscanf(line, "-Print input %[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("true", temp_var, strlen("true"))) {
        print_input_args = true;
      }
      else {
        print_input_args = false;
      }
    }

    if(!strncmp("-Force cache config", line, strlen("-Force cache config"))) {
      sscanf(line, "-Force cache %[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("true", temp_var, strlen("true"))) {
        force_cache_config = true;
      }
      else {
        force_cache_config = false;
      }
    }

    if(!strncmp("-Ndbl", line, strlen("-Ndbl"))) {
      sscanf(line, "-Ndbl %d\n", &(ndbl));
      continue;
    }
    if(!strncmp("-Ndwl", line, strlen("-Ndwl"))) {
      sscanf(line, "-Ndwl %d\n", &(ndwl));
      continue;
    }
    if(!strncmp("-Nspd", line, strlen("-Nspd"))) {
      sscanf(line, "-Nspd %d\n", &(nspd));
      continue;
    }
    if(!strncmp("-Ndsam1", line, strlen("-Ndsam1"))) {
      sscanf(line, "-Ndsam1 %d\n", &(ndsam1));
      continue;
    }
    if(!strncmp("-Ndsam2", line, strlen("-Ndsam2"))) {
      sscanf(line, "-Ndsam2 %d\n", &(ndsam2));
      continue;
    }
   if(!strncmp("-Ndcm", line, strlen("-Ndcm"))) {
      sscanf(line, "-Ndcm %d\n", &(ndcm));
      continue;
    }

	// Parameters related to off-chip interconnect
    
    if(!strncmp("-dram type", line, strlen("-dram type"))) {
      sscanf(line, "-dram type%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("DDR3", temp_var, strlen("DDR3"))) 
      {
		  io_type = DDR3;
	  }
	  else if(!strncmp("DDR4", temp_var, strlen("DDR4"))) 
	  {
		  io_type = DDR4;
	  }
	  else if(!strncmp("LPDDR2", temp_var, strlen("LPDDR2"))) 
	  {
		  io_type = LPDDR2;
	  }
	  else if(!strncmp("WideIO", temp_var, strlen("WideIO"))) 
	  {
		  io_type = WideIO;
	  }
	  else if(!strncmp("Low_Swing_Diff", temp_var, strlen("Low_Swing_Diff"))) 
	  {
		  io_type = Low_Swing_Diff;
	  }
	   else if(!strncmp("Serial", temp_var, strlen("Serial"))) 
	  {
		  io_type = Serial;
	  }
	  else
	  {
		  cout << "Invalid Input for dram type!" << endl;
		  exit(1);
	  }
     // sscanf(line, "-io_type \"%c\"\n", &(io_type));
    }
    if(!strncmp("-io state", line, strlen("-io state"))) {
      sscanf(line, "-io state%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("READ", temp_var, strlen("READ"))) 
      {
		  iostate = READ;
	  }
	  else if(!strncmp("WRITE", temp_var, strlen("WRITE"))) 
	  {
		  iostate = WRITE;
	  }
	  else if(!strncmp("IDLE", temp_var, strlen("IDLE"))) 
	  {
		  iostate = IDLE;
	  }
	  else if(!strncmp("SLEEP", temp_var, strlen("SLEEP"))) 
	  {
		  iostate = SLEEP;
	  }
	  else
	  {
		  cout << "Invalid Input for io state!" << endl;
		  exit(1);
	  }
      //sscanf(line, "-iostate \"%c\"\n", &(iostate));
    }
    if(!strncmp("-addr_timing", line, strlen("-addr_timing"))) {
      sscanf(line, "-addr_timing %lf", &(addr_timing));
    }
    if(!strncmp("-dram ecc", line, strlen("-dram ecc"))) {
	  sscanf(line, "-dram ecc%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("NO_ECC", temp_var, strlen("NO_ECC"))) 
      {
		  dram_ecc = NO_ECC;
	  }
	  else if(!strncmp("SECDED", temp_var, strlen("SECDED"))) 
	  {
		  dram_ecc = SECDED;
	  }
	  else if(!strncmp("CHIP_KILL", temp_var, strlen("CHIP_KILL"))) 
	  {
		  dram_ecc = CHIP_KILL;
	  }
	  else
	  {
		  cout << "Invalid Input for dram ecc!" << endl;
		  exit(1);
	  }
      //sscanf(line, "-dram_ecc \"%c\"\n", &(dram_ecc));
    }
    if(!strncmp("-dram dimm", line, strlen("-dram dimm"))) {
	  sscanf(line, "-dram dimm%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("UDIMM", temp_var, strlen("UDIMM"))) 
      {
		  dram_dimm = UDIMM;
	  }
	  else if(!strncmp("RDIMM", temp_var, strlen("RDIMM"))) 
	  {
		  dram_dimm = RDIMM;
	  }
	  else if(!strncmp("LRDIMM", temp_var, strlen("LRDIMM"))) 
	  {
		  dram_dimm = LRDIMM;
	  }
	  else
	  {
		  cout << "Invalid Input for dram dimm!" << endl;
		  exit(1);
	  }
      //sscanf(line, "-dram_ecc \"%c\"\n", &(dram_ecc));
    }
    
   
    if(!strncmp("-bus_bw", line, strlen("-bus_bw"))) {
      sscanf(line, "-bus_bw %lf", &(bus_bw));
    }
    if(!strncmp("-duty_cycle", line, strlen("-duty_cycle"))) {
      sscanf(line, "-duty_cycle %lf", &(duty_cycle));
    }
    if(!strncmp("-mem_density", line, strlen("-mem_density"))) {
      sscanf(line, "-mem_density %lf", &(mem_density));
    }
    if(!strncmp("-activity_dq", line, strlen("-activity_dq"))) {
      sscanf(line, "-activity_dq %lf", &activity_dq);
    }
    if(!strncmp("-activity_ca", line, strlen("-activity_ca"))) {
      sscanf(line, "-activity_ca %lf", &activity_ca);
    }
    if(!strncmp("-bus_freq", line, strlen("-bus_freq"))) {
      sscanf(line, "-bus_freq %lf", &bus_freq);
    }
    if(!strncmp("-num_dq", line, strlen("-num_dq"))) {
      sscanf(line, "-num_dq %d", &num_dq);
    }
    if(!strncmp("-num_dqs", line, strlen("-num_dqs"))) {
      sscanf(line, "-num_dqs %d", &num_dqs);
    }
    if(!strncmp("-num_ca", line, strlen("-num_ca"))) {
      sscanf(line, "-num_ca %d", &num_ca);
    }
    if(!strncmp("-num_clk", line, strlen("-num_clk"))) {
      sscanf(line, "-num_clk %d", &num_clk);
      if(num_clk<=0)
      {
		  cout << "num_clk should be greater than zero!\n";
		  exit(1);
	  }
    }
    if(!strncmp("-num_mem_dq", line, strlen("-num_mem_dq"))) {
      sscanf(line, "-num_mem_dq %d", &num_mem_dq);
    }
    if(!strncmp("-mem_data_width", line, strlen("-mem_data_width"))) {
      sscanf(line, "-mem_data_width %d", &mem_data_width);
    }
	
	// added just for memcad
	
	if(!strncmp("-num_bobs", line, strlen("-num_bobs"))) {
      sscanf(line, "-num_bobs %d", &num_bobs);
    }
	if(!strncmp("-capacity", line, strlen("-capacity"))) {
      sscanf(line, "-capacity %d", &capacity);
    }
	if(!strncmp("-num_channels_per_bob", line, strlen("-num_channels_per_bob"))) {
      sscanf(line, "-num_channels_per_bob %d", &num_channels_per_bob);
    }
	if(!strncmp("-first metric", line, strlen("-first metric"))) {
	  sscanf(line, "-first metric%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("Cost", temp_var, strlen("Cost"))) 
      {
		  first_metric = Cost;
	  }
	  else if(!strncmp("Energy", temp_var, strlen("Energy"))) 
	  {
		  first_metric = Energy;
	  }
	  else if(!strncmp("Bandwidth", temp_var, strlen("Bandwidth"))) 
	  {
		  first_metric = Bandwidth;
	  }
	  else
	  {
		  cout << "Invalid Input for first metric!" << endl;
		  exit(1);
	  }
     
    }
    if(!strncmp("-second metric", line, strlen("-second metric"))) {
	  sscanf(line, "-second metric%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("Cost", temp_var, strlen("Cost"))) 
      {
		  second_metric = Cost;
	  }
	  else if(!strncmp("Energy", temp_var, strlen("Energy"))) 
	  {
		  second_metric = Energy;
	  }
	  else if(!strncmp("Bandwidth", temp_var, strlen("Bandwidth"))) 
	  {
		  second_metric = Bandwidth;
	  }
	  else
	  {
		  cout << "Invalid Input for second metric!" << endl;
		  exit(1);
	  }
     
    }
    if(!strncmp("-third metric", line, strlen("-third metric"))) {
	  sscanf(line, "-third metric%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("Cost", temp_var, strlen("Cost"))) 
      {
		  third_metric = Cost;
	  }
	  else if(!strncmp("Energy", temp_var, strlen("Energy"))) 
	  {
		  third_metric = Energy;
	  }
	  else if(!strncmp("Bandwidth", temp_var, strlen("Bandwidth"))) 
	  {
		  third_metric = Bandwidth;
	  }
	  else
	  {
		  cout << "Invalid Input for third metric!" << endl;
		  exit(1);
	  }
     
    }
	if(!strncmp("-DIMM model", line, strlen("-DIMM model"))) {
	  sscanf(line, "-DIMM model%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("JUST_UDIMM", temp_var, strlen("JUST_UDIMM"))) 
      {
		  dimm_model = JUST_UDIMM;
	  }
	  else if(!strncmp("JUST_RDIMM", temp_var, strlen("JUST_RDIMM"))) 
	  {
		  dimm_model = JUST_RDIMM;
	  }
	  else if(!strncmp("JUST_LRDIMM", temp_var, strlen("JUST_LRDIMM"))) 
	  {
		  dimm_model = JUST_LRDIMM;
	  }
	  else if(!strncmp("ALL", temp_var, strlen("ALL"))) 
	  {
		  dimm_model = ALL;
	  }
	  else
	  {
		  cout << "Invalid Input for DIMM model!" << endl;
		  exit(1);
	  }
     
    }
	if(!strncmp("-Low Power Permitted", line, strlen("-Low Power Permitted"))) {
	  sscanf(line, "-Low Power Permitted%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("T", temp_var, strlen("T"))) 
      {
		  low_power_permitted = true;
	  }
	  else if(!strncmp("F", temp_var, strlen("F"))) 
	  {
		  low_power_permitted = false;
	  }
	  else
	  {
		  cout << "Invalid Input for Low Power Permitted!" << endl;
		  exit(1);
	  }
     
    }
	if(!strncmp("-load", line, strlen("-load"))) {
      sscanf(line, "-load %lf", &(load));
    }
	if(!strncmp("-row_buffer_hit_rate", line, strlen("-row_buffer_hit_rate"))) {
      sscanf(line, "-row_buffer_hit_rate %lf", &(row_buffer_hit_rate));
    }
	if(!strncmp("-rd_2_wr_ratio", line, strlen("-rd_2_wr_ratio"))) {
      sscanf(line, "-rd_2_wr_ratio %lf", &(rd_2_wr_ratio));
    }
	if(!strncmp("-same_bw_in_bob", line, strlen("-same_bw_in_bob"))) {
	  sscanf(line, "-same_bw_in_bob%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("T", temp_var, strlen("T"))) 
      {
		  same_bw_in_bob = true;
	  }
	  else if(!strncmp("F", temp_var, strlen("F"))) 
	  {
		  same_bw_in_bob = false;
	  }
	  else
	  {
		  cout << "Invalid Input for same_bw_in_bob!" << endl;
		  exit(1);
	  }
     
    }
	if(!strncmp("-mirror_in_bob", line, strlen("-mirror_in_bob"))) {
	  sscanf(line, "-mirror_in_bob%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("T", temp_var, strlen("T"))) 
      {
		  mirror_in_bob = true;
	  }
	  else if(!strncmp("F", temp_var, strlen("F"))) 
	  {
		  mirror_in_bob = false;
	  }
	  else
	  {
		  cout << "Invalid Input for mirror_in_bob!" << endl;
		  exit(1);
	  }
     
    }
    if(!strncmp("-total_power", line, strlen("-total_power"))) {
	  sscanf(line, "-total_power%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("T", temp_var, strlen("T"))) 
      {
		  total_power = true;
	  }
	  else if(!strncmp("F", temp_var, strlen("F"))) 
	  {
		  total_power = false;
	  }
	  else
	  {
		  cout << "Invalid Input for total_power!" << endl;
		  exit(1);
	  }
     
    }
    if(!strncmp("-verbose", line, strlen("-verbose"))) {
	  sscanf(line, "-verbose%[^\"]\"%[^\"]\"", jk, temp_var);
      if (!strncmp("T", temp_var, strlen("T"))) 
      {
		  verbose = true;
	  }
	  else if(!strncmp("F", temp_var, strlen("F"))) 
	  {
		  verbose = false;
	  }
	  else
	  {
		  cout << "Invalid Input for same_bw_in_bob!" << endl;
		  exit(1);
	  }
     
    }
	
	
	
	
  }
  rpters_in_htree = true;
  fclose(fp);
}

  void
InputParameter::display_ip()
{
  cout << "Cache size                    : " << cache_sz << endl;
  cout << "Block size                    : " << line_sz << endl;
  cout << "Associativity                 : " << assoc << endl;
  cout << "Read only ports               : " << num_rd_ports << endl;
  cout << "Write only ports              : " << num_wr_ports << endl;
  cout << "Read write ports              : " << num_rw_ports << endl;
  cout << "Single ended read ports       : " << num_se_rd_ports << endl;
  if (fully_assoc||pure_cam)
  {
	  cout << "Search ports                  : " << num_search_ports << endl;
  }
  cout << "Cache banks (UCA)             : " << nbanks << endl;
  cout << "Technology                    : " << F_sz_um << endl;
  cout << "Temperature                   : " << temp << endl;
  cout << "Tag size                      : " << tag_w << endl;
  if (is_cache) 
  {
    cout << "array type                    : " << "Cache" << endl;
  }
  if (pure_ram) 
  {
    cout << "array type                    : " << "Scratch RAM" << endl;
  }
  if (pure_cam)
  {
      cout << "array type                    : " << "CAM" << endl;
  }
  cout << "Model as memory               : " << is_main_mem << endl;
  cout << "Model as 3D memory       	 : " << is_3d_mem << endl;
  cout << "Access mode                   : " << access_mode << endl;
  cout << "Data array cell type          : " << data_arr_ram_cell_tech_type << endl;
  cout << "Data array peripheral type    : " << data_arr_peri_global_tech_type << endl;
  cout << "Tag array cell type           : " << tag_arr_ram_cell_tech_type << endl;
  cout << "Tag array peripheral type     : " << tag_arr_peri_global_tech_type << endl;
  cout << "Optimization target           : " << ed << endl;
  cout << "Design objective (UCA wt)     : " << delay_wt << " "
                                                << dynamic_power_wt << " " << leakage_power_wt << " " << cycle_time_wt
                                                << " " << area_wt << endl;
  cout << "Design objective (UCA dev)    : " << delay_dev << " "
                                                << dynamic_power_dev << " " << leakage_power_dev << " " << cycle_time_dev
                                                << " " << area_dev << endl;
  if (nuca)
    {
    cout << "Cores                         : " << cores << endl;


    cout << "Design objective (NUCA wt)    : " << delay_wt_nuca << " "
                                                << dynamic_power_wt_nuca << " " << leakage_power_wt_nuca << " " << cycle_time_wt_nuca
                                                << " " << area_wt_nuca << endl;
    cout << "Design objective (NUCA dev)   : " << delay_dev_nuca << " "
                                                << dynamic_power_dev_nuca << " " << leakage_power_dev_nuca << " " << cycle_time_dev_nuca
                                       << " " << area_dev_nuca << endl;
    }
  cout << "Cache model                   : " << nuca << endl;
  cout << "Nuca bank                     : " << nuca_bank_count << endl;
  cout << "Wire inside mat               : " << wire_is_mat_type << endl;
  cout << "Wire outside mat              : " << wire_os_mat_type << endl;
  cout << "Interconnect projection       : " << ic_proj_type << endl;
  cout << "Wire signaling               : " << force_wiretype << endl;
  cout << "Print level                   : " << print_detail << endl;
  cout << "ECC overhead                  : " << add_ecc_b_ << endl;
  cout << "Page size                     : " << page_sz_bits << endl;
  cout << "Burst length                  : " << burst_len << endl;
  cout << "Internal prefetch width       : " << int_prefetch_w << endl;
  cout << "Force cache config            : " << g_ip->force_cache_config << endl;
  if (g_ip->force_cache_config) {
    cout << "Ndwl                          : " << g_ip->ndwl << endl;
    cout << "Ndbl                          : " << g_ip->ndbl << endl;
    cout << "Nspd                          : " << g_ip->nspd << endl;
    cout << "Ndcm                          : " << g_ip->ndcm << endl;
    cout << "Ndsam1                        : " << g_ip->ndsam1 << endl;
    cout << "Ndsam2                        : " << g_ip->ndsam2 << endl;
  }
  cout << "Subarray Driver direction       : " << g_ip->cl_vertical << endl;

 // CACTI-I/O
  cout << "iostate                       : " ;
  switch(iostate)
  {
	  case(READ): cout << "READ" << endl; break;
	  case(WRITE): cout << "WRITE" << endl; break;
	  case(IDLE): cout << "IDLE" << endl; break;
	  case(SLEEP): cout << "SLEEP" << endl; break;
	  default: assert(false);
  } 
  cout << "dram_ecc                      : " ;
  switch(dram_ecc)
  {
	  case(NO_ECC): cout << "NO_ECC" << endl; break;
	  case(SECDED): cout << "SECDED" << endl; break;
	  case(CHIP_KILL): cout << "CHIP_KILL" << endl; break;
	  default: assert(false);
  } 
  cout << "io_type                     : " ;
  switch(io_type)
  {
	  case(DDR3): cout << "DDR3" << endl; break;
	  case(DDR4): cout << "DDR4" << endl; break;
	  case(LPDDR2): cout << "LPDDR2" << endl; break;
	  case(WideIO): cout << "WideIO" << endl; break;
	  case(Low_Swing_Diff): cout << "Low_Swing_Diff" << endl; break;
	  default: assert(false);
  } 	
  cout << "dram_dimm                      : " ;
  switch(dram_dimm)
  {
	  case(UDIMM): cout << "UDIMM" << endl; break;
	  case(RDIMM): cout << "RDIMM" << endl; break;
	  case(LRDIMM): cout << "LRDIMM" << endl; break;
	  default: assert(false);
  } 
 
	

}



powerComponents operator+(const powerComponents & x, const powerComponents & y)
{
  powerComponents z;

  z.dynamic = x.dynamic + y.dynamic;
  z.leakage = x.leakage + y.leakage;
  z.gate_leakage  = x.gate_leakage  + y.gate_leakage;
  z.short_circuit = x.short_circuit + y.short_circuit;
  z.longer_channel_leakage = x.longer_channel_leakage + y.longer_channel_leakage;

  return z;
}

powerComponents operator*(const powerComponents & x, double const * const y)
{
  powerComponents z;

  z.dynamic = x.dynamic*y[0];
  z.leakage = x.leakage*y[1];
  z.gate_leakage  = x.gate_leakage*y[2];
  z.short_circuit = x.short_circuit*y[3];
  z.longer_channel_leakage = x.longer_channel_leakage*y[1];//longer channel leakage has the same behavior as normal leakage

  return z;
}


powerDef operator+(const powerDef & x, const powerDef & y)
{
  powerDef z;

  z.readOp   = x.readOp  + y.readOp;
  z.writeOp  = x.writeOp + y.writeOp;
  z.searchOp = x.searchOp + y.searchOp;
  return z;
}

powerDef operator*(const powerDef & x, double const * const y)
{
  powerDef z;

  z.readOp   = x.readOp*y;
  z.writeOp  = x.writeOp*y;
  z.searchOp = x.searchOp*y;
  return z;
}

uca_org_t cacti_interface(const string & infile_name)
{

	//cout<<"TSV_proj_type: " << g_ip->TSV_proj_type << endl;
  uca_org_t fin_res;
  //uca_org_t result;
  fin_res.valid = false;

  g_ip = new InputParameter();
  g_ip->parse_cfg(infile_name);
  if(!g_ip->error_checking())
	  exit(0);
 // if (g_ip->print_input_args)
    g_ip->display_ip();
	

  init_tech_params(g_ip->F_sz_um, false);
  Wire winit; // Do not delete this line. It initializes wires.
//  cout << winit.wire_res(256*8*64e-9) << endl;
//  exit(0);


  //CACTI3DD
  // --- These two parameters are supposed for two different TSV technologies within one DRAM fabrication, currently assume one individual TSV geometry size for cost efficiency
  g_ip->tsv_is_subarray_type = g_ip->TSV_proj_type;
  g_ip->tsv_os_bank_type = g_ip->TSV_proj_type;
  TSV tsv_test(Coarse);// ********* double len_ /* in um*/, 	double diam_, 	double TSV_pitch_,
  if(g_ip->print_detail_debug)
  {
	  tsv_test.print_TSV();
  }

//  For HighRadix Only
//  ////  Wire wirea(g_ip->wt, 1000);
//  ////  wirea.print_wire();
//  ////  cout << "Wire Area " << wirea.area.get_area() << " sq. u" << endl;
//  //  winit.print_wire();
//  //
//    HighRadix *hr;
//      hr = new HighRadix();
//      hr->compute_power();
//      hr->print_router();
//    exit(0);
//
//    double sub_switch_sz = 2;
//    double rows = 32;
//    for (int i=0; i<6; i++) {
//      sub_switch_sz = pow(2, i);
//      rows = 64/sub_switch_sz;
//      hr = new HighRadix(sub_switch_sz, rows, .8/* freq */, 64, 2, 64, 0.7);
//      hr->compute_power();
//      hr->print_router();
//      delete hr;
//    }
//  //  HighRadix yarc;
//  //  yarc.compute_power();
//  //  yarc.print_router();
//    winit.print_wire();
//    exit(0);
//  For HighRadix Only End

  if (g_ip->nuca == 1)
  {
    Nuca n(&g_tp.peri_global);
    n.sim_nuca();
  }
  
  //g_ip->display_ip();
	
	
   
  IOTechParam iot(g_ip, g_ip->io_type, g_ip->num_mem_dq, g_ip->mem_data_width, g_ip->num_dq,g_ip->dram_dimm, 1,g_ip->bus_freq ); 
  Extio testextio(&iot);  
  testextio.extio_area();
  testextio.extio_eye();
  testextio.extio_power_dynamic();
  testextio.extio_power_phy();
  testextio.extio_power_term();	   
  
 
  /*
  int freq[][4]={{400,533,667,800},{800,933,1066,1200}};
	
  Mem_IO_type types[2]={DDR3,DDR4};
    
  int max_load[3]={3,3,8};
    
  for(int j=0;j<1;j++)  
  {
	  for(int connection=0;connection<3;connection++)
	  {
		  for(int frq=3;frq<4;frq++)
		  {
			  for(int load=1;load<=max_load[connection];load++)
			  {
				  IOTechParam iot(g_ip, types[j], load, 8, 72, connection, load, freq[j][frq]); 
				  Extio testextio(&iot);  
				 // testextio.extio_area();
				 // testextio.extio_eye();
				  testextio.extio_power_dynamic();
				  testextio.extio_power_phy();
				  testextio.extio_power_term();	 
				 
			  }
			  cout << endl;
		  }
		  cout << endl;
	  }
	   cout << endl;
  } 
  */ 

  ///double total_io_p, total_phy_p, total_io_area, total_vmargin, total_tmargin;
  //testextio.extio_power_area_timing(total_io_p, total_phy_p, total_io_area, total_vmargin, total_tmargin);
  
  solve(&fin_res);

  output_UCA(&fin_res);
  output_data_csv(fin_res, infile_name + ".out");


  // Memcad Optimization
  MemCadParameters memcad_params(g_ip);
  solve_memcad(&memcad_params);


  delete (g_ip);
  return fin_res;
}

//CACTI3DD's plain interface, please keep !!!
uca_org_t cacti_interface(
    int dram_cap_tot_byte,
    int line_size,
    int associativity,
    int rw_ports,
    int excl_read_ports,// para5
    int excl_write_ports,
    int single_ended_read_ports,
    int search_ports,
    int banks,
    double tech_node,//para10
    int output_width,
    int specific_tag,
    int tag_width,
    int access_mode,
    int cache,      //para15
    int main_mem,
    int obj_func_delay,
    int obj_func_dynamic_power,
    int obj_func_leakage_power,
    int obj_func_cycle_time, //para20
    int obj_func_area,
    int dev_func_delay,
    int dev_func_dynamic_power,
    int dev_func_leakage_power,
    int dev_func_area, //para25
    int dev_func_cycle_time,
    int ed_ed2_none, // 0 - ED, 1 - ED^2, 2 - use weight and deviate
    int temp,
    int wt, //0 - default(search across everything), 1 - global, 2 - 5% delay penalty, 3 - 10%, 4 - 20 %, 5 - 30%, 6 - low-swing
    int data_arr_ram_cell_tech_flavor_in,//para30
    int data_arr_peri_global_tech_flavor_in,
    int tag_arr_ram_cell_tech_flavor_in,
    int tag_arr_peri_global_tech_flavor_in,
    int interconnect_projection_type_in,
    int wire_inside_mat_type_in,//para35
    int wire_outside_mat_type_in,
    int REPEATERS_IN_HTREE_SEGMENTS_in,
    int VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in,
    int BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in,
    int PAGE_SIZE_BITS_in,//para40
    int BURST_LENGTH_in,
    int INTERNAL_PREFETCH_WIDTH_in,
    int force_wiretype,
    int wiretype,
    int force_config,//para45
    int ndwl,
    int ndbl,
    int nspd,
    int ndcm,
    int ndsam1,//para50
    int ndsam2,
    int ecc,
    int is_3d_dram,
    int burst_depth,
    int IO_width,
    int sys_freq,
    int debug_detail,
    int num_dies,
    int tsv_gran_is_subarray,
    int tsv_gran_os_bank,
    int num_tier_row_sprd,
    int num_tier_col_sprd,
    int partition_level
    )
{
  g_ip = new InputParameter();

  uca_org_t fin_res;
  fin_res.valid = false;

  g_ip->data_arr_ram_cell_tech_type    = data_arr_ram_cell_tech_flavor_in;
  g_ip->data_arr_peri_global_tech_type = data_arr_peri_global_tech_flavor_in;
  g_ip->tag_arr_ram_cell_tech_type     = tag_arr_ram_cell_tech_flavor_in;
  g_ip->tag_arr_peri_global_tech_type  = tag_arr_peri_global_tech_flavor_in;

  g_ip->ic_proj_type     = interconnect_projection_type_in;
  g_ip->wire_is_mat_type = wire_inside_mat_type_in;
  g_ip->wire_os_mat_type = wire_outside_mat_type_in;
  g_ip->burst_len        = BURST_LENGTH_in;
  g_ip->int_prefetch_w   = INTERNAL_PREFETCH_WIDTH_in;
  g_ip->page_sz_bits     = PAGE_SIZE_BITS_in;

  g_ip->num_die_3d = num_dies;
  g_ip->cache_sz            = dram_cap_tot_byte;
  g_ip->line_sz             = line_size;
  g_ip->assoc               = associativity;
  g_ip->nbanks              = banks;
  g_ip->out_w               = output_width;
  g_ip->specific_tag        = specific_tag;
  if (specific_tag == 0) {
    g_ip->tag_w = 42;
  }
  else {
    g_ip->tag_w               = tag_width;
  }

  g_ip->access_mode         = access_mode;
  g_ip->delay_wt = obj_func_delay;
  g_ip->dynamic_power_wt = obj_func_dynamic_power;
  g_ip->leakage_power_wt = obj_func_leakage_power;
  g_ip->area_wt = obj_func_area;
  g_ip->cycle_time_wt    = obj_func_cycle_time;
  g_ip->delay_dev = dev_func_delay;
  g_ip->dynamic_power_dev = dev_func_dynamic_power;
  g_ip->leakage_power_dev = dev_func_leakage_power;
  g_ip->area_dev = dev_func_area;
  g_ip->cycle_time_dev    = dev_func_cycle_time;
  g_ip->temp = temp;
  g_ip->ed = ed_ed2_none;

  g_ip->F_sz_nm         = tech_node;
  g_ip->F_sz_um         = tech_node / 1000;
  g_ip->is_main_mem     = (main_mem != 0) ? true : false;
  g_ip->is_cache        = (cache ==1) ? true : false;
  g_ip->pure_ram        = (cache ==0) ? true : false;
  g_ip->pure_cam        = (cache ==2) ? true : false;
  g_ip->rpters_in_htree = (REPEATERS_IN_HTREE_SEGMENTS_in != 0) ? true : false;
  g_ip->ver_htree_wires_over_array = VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in;
  g_ip->broadcast_addr_din_over_ver_htrees = BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in;

  g_ip->num_rw_ports    = rw_ports;
  g_ip->num_rd_ports    = excl_read_ports;
  g_ip->num_wr_ports    = excl_write_ports;
  g_ip->num_se_rd_ports = single_ended_read_ports;
  g_ip->num_search_ports = search_ports;

  g_ip->print_detail = 1;
  g_ip->nuca = 0;

  if (force_wiretype == 0)
  {
	  g_ip->wt = Global;
      g_ip->force_wiretype = false;
  }
  else
  {   g_ip->force_wiretype = true;
	  if (wiretype==10) {
		  g_ip->wt = Global_10;
	        }
	  if (wiretype==20) {
		  g_ip->wt = Global_20;
	        }
	  if (wiretype==30) {
		  g_ip->wt = Global_30;
	        }
	  if (wiretype==5) {
	      g_ip->wt = Global_5;
	        }
	  if (wiretype==0) {
		  g_ip->wt = Low_swing;
	  }
  }
  //g_ip->wt = Global_5;
  if (force_config == 0)
    {
  	  g_ip->force_cache_config = false;
    }
    else
    {
    	g_ip->force_cache_config = true;
    	g_ip->ndbl=ndbl;
    	g_ip->ndwl=ndwl;
    	g_ip->nspd=nspd;
    	g_ip->ndcm=ndcm;
    	g_ip->ndsam1=ndsam1;
    	g_ip->ndsam2=ndsam2;


    }

  if (ecc==0){
	  g_ip->add_ecc_b_=false;
  }
  else
  {
	  g_ip->add_ecc_b_=true;
  }

  //CACTI3DD
  g_ip->is_3d_mem = is_3d_dram;
  g_ip->burst_depth = burst_depth;
  g_ip->io_width =IO_width;
  g_ip->sys_freq_MHz = sys_freq;
  g_ip->print_detail_debug = debug_detail;

  g_ip->tsv_is_subarray_type = tsv_gran_is_subarray;
  g_ip->tsv_os_bank_type = tsv_gran_os_bank;

  g_ip->partition_gran = partition_level;
  g_ip->num_tier_row_sprd = num_tier_row_sprd;
  g_ip->num_tier_col_sprd = num_tier_col_sprd;
  if(partition_level == 3)
	  g_ip->fine_gran_bank_lvl = true;
  else
	  g_ip->fine_gran_bank_lvl = false;

  if(!g_ip->error_checking())
	  exit(0);

  init_tech_params(g_ip->F_sz_um, false);
  Wire winit; // Do not delete this line. It initializes wires.

  //tsv
  //TSV tsv_test(Coarse);
  //tsv_test.print_TSV();

  g_ip->display_ip();
  solve(&fin_res);
  output_UCA(&fin_res);
  output_data_csv_3dd(fin_res);
  delete (g_ip);

  return fin_res;
}

//cacti6.5's plain interface, please keep !!!
uca_org_t cacti_interface(
    int cache_size,
    int line_size,
    int associativity,
    int rw_ports,
    int excl_read_ports,
    int excl_write_ports,
    int single_ended_read_ports,
    int banks,
    double tech_node, // in nm
    int page_sz,
    int burst_length,
    int pre_width,
    int output_width,
    int specific_tag,
    int tag_width,
    int access_mode, //0 normal, 1 seq, 2 fast
    int cache, //scratch ram or cache
    int main_mem,
    int obj_func_delay,
    int obj_func_dynamic_power,
    int obj_func_leakage_power,
    int obj_func_area,
    int obj_func_cycle_time,
    int dev_func_delay,
    int dev_func_dynamic_power,
    int dev_func_leakage_power,
    int dev_func_area,
    int dev_func_cycle_time,
    int ed_ed2_none, // 0 - ED, 1 - ED^2, 2 - use weight and deviate
    int temp,
    int wt, //0 - default(search across everything), 1 - global, 2 - 5% delay penalty, 3 - 10%, 4 - 20 %, 5 - 30%, 6 - low-swing
    int data_arr_ram_cell_tech_flavor_in, // 0-4
    int data_arr_peri_global_tech_flavor_in,
    int tag_arr_ram_cell_tech_flavor_in,
    int tag_arr_peri_global_tech_flavor_in,
    int interconnect_projection_type_in, // 0 - aggressive, 1 - normal
    int wire_inside_mat_type_in,
    int wire_outside_mat_type_in,
    int is_nuca, // 0 - UCA, 1 - NUCA
    int core_count,
    int cache_level, // 0 - L2, 1 - L3
    int nuca_bank_count,
    int nuca_obj_func_delay,
    int nuca_obj_func_dynamic_power,
    int nuca_obj_func_leakage_power,
    int nuca_obj_func_area,
    int nuca_obj_func_cycle_time,
    int nuca_dev_func_delay,
    int nuca_dev_func_dynamic_power,
    int nuca_dev_func_leakage_power,
    int nuca_dev_func_area,
    int nuca_dev_func_cycle_time,
    int REPEATERS_IN_HTREE_SEGMENTS_in,//TODO for now only wires with repeaters are supported
    int p_input)
{
  g_ip = new InputParameter();
  g_ip->add_ecc_b_ = true;

  g_ip->data_arr_ram_cell_tech_type    = data_arr_ram_cell_tech_flavor_in;
  g_ip->data_arr_peri_global_tech_type = data_arr_peri_global_tech_flavor_in;
  g_ip->tag_arr_ram_cell_tech_type     = tag_arr_ram_cell_tech_flavor_in;
  g_ip->tag_arr_peri_global_tech_type  = tag_arr_peri_global_tech_flavor_in;

  g_ip->ic_proj_type     = interconnect_projection_type_in;
  g_ip->wire_is_mat_type = wire_inside_mat_type_in;
  g_ip->wire_os_mat_type = wire_outside_mat_type_in;
  g_ip->burst_len        = burst_length;
  g_ip->int_prefetch_w   = pre_width;
  g_ip->page_sz_bits     = page_sz;

  g_ip->cache_sz            = cache_size;
  g_ip->line_sz             = line_size;
  g_ip->assoc               = associativity;
  g_ip->nbanks              = banks;
  g_ip->out_w               = output_width;
  g_ip->specific_tag        = specific_tag;
  if (tag_width == 0) {
    g_ip->tag_w = 42;
  }
  else {
    g_ip->tag_w               = tag_width;
  }

  g_ip->access_mode         = access_mode;
  g_ip->delay_wt = obj_func_delay;
  g_ip->dynamic_power_wt = obj_func_dynamic_power;
  g_ip->leakage_power_wt = obj_func_leakage_power;
  g_ip->area_wt = obj_func_area;
  g_ip->cycle_time_wt    = obj_func_cycle_time;
  g_ip->delay_dev = dev_func_delay;
  g_ip->dynamic_power_dev = dev_func_dynamic_power;
  g_ip->leakage_power_dev = dev_func_leakage_power;
  g_ip->area_dev = dev_func_area;
  g_ip->cycle_time_dev    = dev_func_cycle_time;
  g_ip->ed = ed_ed2_none;

  switch(wt) {
    case (0):
      g_ip->force_wiretype = 0;
      g_ip->wt = Global;
      break;
    case (1):
      g_ip->force_wiretype = 1;
      g_ip->wt = Global;
      break;
    case (2):
      g_ip->force_wiretype = 1;
      g_ip->wt = Global_5;
      break;
    case (3):
      g_ip->force_wiretype = 1;
      g_ip->wt = Global_10;
      break;
    case (4):
      g_ip->force_wiretype = 1;
      g_ip->wt = Global_20;
      break;
    case (5):
      g_ip->force_wiretype = 1;
      g_ip->wt = Global_30;
      break;
    case (6):
      g_ip->force_wiretype = 1;
      g_ip->wt = Low_swing;
      break;
    default:
      cout << "Unknown wire type!\n";
      exit(0);
  }

  g_ip->delay_wt_nuca = nuca_obj_func_delay;
  g_ip->dynamic_power_wt_nuca = nuca_obj_func_dynamic_power;
  g_ip->leakage_power_wt_nuca = nuca_obj_func_leakage_power;
  g_ip->area_wt_nuca = nuca_obj_func_area;
  g_ip->cycle_time_wt_nuca    = nuca_obj_func_cycle_time;
  g_ip->delay_dev_nuca = dev_func_delay;
  g_ip->dynamic_power_dev_nuca = nuca_dev_func_dynamic_power;
  g_ip->leakage_power_dev_nuca = nuca_dev_func_leakage_power;
  g_ip->area_dev_nuca = nuca_dev_func_area;
  g_ip->cycle_time_dev_nuca    = nuca_dev_func_cycle_time;
  g_ip->nuca = is_nuca;
  g_ip->nuca_bank_count = nuca_bank_count;
  if(nuca_bank_count > 0) {
    g_ip->force_nuca_bank = 1;
  }
  g_ip->cores = core_count;
  g_ip->cache_level = cache_level;

  g_ip->temp = temp;

  g_ip->F_sz_nm         = tech_node;
  g_ip->F_sz_um         = tech_node / 1000;
  g_ip->is_main_mem     = (main_mem != 0) ? true : false;
  g_ip->is_cache        = (cache != 0) ? true : false;
  g_ip->rpters_in_htree = (REPEATERS_IN_HTREE_SEGMENTS_in != 0) ? true : false;

  g_ip->num_rw_ports    = rw_ports;
  g_ip->num_rd_ports    = excl_read_ports;
  g_ip->num_wr_ports    = excl_write_ports;
  g_ip->num_se_rd_ports = single_ended_read_ports;
  g_ip->print_detail = 1;
  g_ip->nuca = 0;

  g_ip->wt = Global_5;
  g_ip->force_cache_config = false;
  g_ip->force_wiretype = false;
  g_ip->print_input_args = p_input;


  uca_org_t fin_res;
  fin_res.valid = false;

  if (g_ip->error_checking() == false) exit(0);
  if (g_ip->print_input_args)
    g_ip->display_ip();
  init_tech_params(g_ip->F_sz_um, false);
  Wire winit; // Do not delete this line. It initializes wires.

  if (g_ip->nuca == 1)
  {
    Nuca n(&g_tp.peri_global);
    n.sim_nuca();
  }
  solve(&fin_res);

  output_UCA(&fin_res);

  delete (g_ip);
  return fin_res;
}

//McPAT's plain interface, please keep !!!
uca_org_t cacti_interface(
    int cache_size,
    int line_size,
    int associativity,
    int rw_ports,
    int excl_read_ports,// para5
    int excl_write_ports,
    int single_ended_read_ports,
    int search_ports,
    int banks,
    double tech_node,//para10
    int output_width,
    int specific_tag,
    int tag_width,
    int access_mode,
    int cache,      //para15
    int main_mem,
    int obj_func_delay,
    int obj_func_dynamic_power,
    int obj_func_leakage_power,
    int obj_func_cycle_time, //para20
    int obj_func_area,
    int dev_func_delay,
    int dev_func_dynamic_power,
    int dev_func_leakage_power,
    int dev_func_area, //para25
    int dev_func_cycle_time,
    int ed_ed2_none, // 0 - ED, 1 - ED^2, 2 - use weight and deviate
    int temp,
    int wt, //0 - default(search across everything), 1 - global, 2 - 5% delay penalty, 3 - 10%, 4 - 20 %, 5 - 30%, 6 - low-swing
    int data_arr_ram_cell_tech_flavor_in,//para30
    int data_arr_peri_global_tech_flavor_in,
    int tag_arr_ram_cell_tech_flavor_in,
    int tag_arr_peri_global_tech_flavor_in,
    int interconnect_projection_type_in,
    int wire_inside_mat_type_in,//para35
    int wire_outside_mat_type_in,
    int REPEATERS_IN_HTREE_SEGMENTS_in,
    int VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in,
    int BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in,
    int PAGE_SIZE_BITS_in,//para40
    int BURST_LENGTH_in,
    int INTERNAL_PREFETCH_WIDTH_in,
    int force_wiretype,
    int wiretype,
    int force_config,//para45
    int ndwl,
    int ndbl,
    int nspd,
    int ndcm,
    int ndsam1,//para50
    int ndsam2,
    int ecc)
{
  g_ip = new InputParameter();

  uca_org_t fin_res;
  fin_res.valid = false;

  g_ip->data_arr_ram_cell_tech_type    = data_arr_ram_cell_tech_flavor_in;
  g_ip->data_arr_peri_global_tech_type = data_arr_peri_global_tech_flavor_in;
  g_ip->tag_arr_ram_cell_tech_type     = tag_arr_ram_cell_tech_flavor_in;
  g_ip->tag_arr_peri_global_tech_type  = tag_arr_peri_global_tech_flavor_in;

  g_ip->ic_proj_type     = interconnect_projection_type_in;
  g_ip->wire_is_mat_type = wire_inside_mat_type_in;
  g_ip->wire_os_mat_type = wire_outside_mat_type_in;
  g_ip->burst_len        = BURST_LENGTH_in;
  g_ip->int_prefetch_w   = INTERNAL_PREFETCH_WIDTH_in;
  g_ip->page_sz_bits     = PAGE_SIZE_BITS_in;

  g_ip->cache_sz            = cache_size;
  g_ip->line_sz             = line_size;
  g_ip->assoc               = associativity;
  g_ip->nbanks              = banks;
  g_ip->out_w               = output_width;
  g_ip->specific_tag        = specific_tag;
  if (specific_tag == 0) {
    g_ip->tag_w = 42;
  }
  else {
    g_ip->tag_w               = tag_width;
  }

  g_ip->access_mode         = access_mode;
  g_ip->delay_wt = obj_func_delay;
  g_ip->dynamic_power_wt = obj_func_dynamic_power;
  g_ip->leakage_power_wt = obj_func_leakage_power;
  g_ip->area_wt = obj_func_area;
  g_ip->cycle_time_wt    = obj_func_cycle_time;
  g_ip->delay_dev = dev_func_delay;
  g_ip->dynamic_power_dev = dev_func_dynamic_power;
  g_ip->leakage_power_dev = dev_func_leakage_power;
  g_ip->area_dev = dev_func_area;
  g_ip->cycle_time_dev    = dev_func_cycle_time;
  g_ip->temp = temp;
  g_ip->ed = ed_ed2_none;

  g_ip->F_sz_nm         = tech_node;
  g_ip->F_sz_um         = tech_node / 1000;
  g_ip->is_main_mem     = (main_mem != 0) ? true : false;
  g_ip->is_cache        = (cache ==1) ? true : false;
  g_ip->pure_ram        = (cache ==0) ? true : false;
  g_ip->pure_cam        = (cache ==2) ? true : false;
  g_ip->rpters_in_htree = (REPEATERS_IN_HTREE_SEGMENTS_in != 0) ? true : false;
  g_ip->ver_htree_wires_over_array = VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in;
  g_ip->broadcast_addr_din_over_ver_htrees = BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in;

  g_ip->num_rw_ports    = rw_ports;
  g_ip->num_rd_ports    = excl_read_ports;
  g_ip->num_wr_ports    = excl_write_ports;
  g_ip->num_se_rd_ports = single_ended_read_ports;
  g_ip->num_search_ports = search_ports;

  g_ip->print_detail = 1;
  g_ip->nuca = 0;

  if (force_wiretype == 0)
  {
	  g_ip->wt = Global;
      g_ip->force_wiretype = false;
  }
  else
  {   g_ip->force_wiretype = true;
	  if (wiretype==10) {
		  g_ip->wt = Global_10;
	        }
	  if (wiretype==20) {
		  g_ip->wt = Global_20;
	        }
	  if (wiretype==30) {
		  g_ip->wt = Global_30;
	        }
	  if (wiretype==5) {
	      g_ip->wt = Global_5;
	        }
	  if (wiretype==0) {
		  g_ip->wt = Low_swing;
	  }
  }
  //g_ip->wt = Global_5;
  if (force_config == 0)
    {
  	  g_ip->force_cache_config = false;
    }
    else
    {
    	g_ip->force_cache_config = true;
    	g_ip->ndbl=ndbl;
    	g_ip->ndwl=ndwl;
    	g_ip->nspd=nspd;
    	g_ip->ndcm=ndcm;
    	g_ip->ndsam1=ndsam1;
    	g_ip->ndsam2=ndsam2;


    }

  if (ecc==0){
	  g_ip->add_ecc_b_=false;
  }
  else
  {
	  g_ip->add_ecc_b_=true;
  }


  if(!g_ip->error_checking())
	  exit(0);

  init_tech_params(g_ip->F_sz_um, false);
  Wire winit; // Do not delete this line. It initializes wires.

  g_ip->display_ip();
  solve(&fin_res);
  output_UCA(&fin_res);
  output_data_csv(fin_res);
  delete (g_ip);

  return fin_res;
}



bool InputParameter::error_checking()
{
  int  A;
  bool seq_access  = false;
  fast_access = true;

  switch (access_mode)
  {
    case 0:
      seq_access  = false;
      fast_access = false;
      break;
    case 1:
      seq_access  = true;
      fast_access = false;
      break;
    case 2:
      seq_access  = false;
      fast_access = true;
      break;
  }

  if(is_main_mem)
  {
	if(ic_proj_type == 0 && !g_ip->is_3d_mem)
    {
      cerr << "DRAM model supports only conservative interconnect projection!\n\n";
      return false;
    }
  }


  uint32_t B = line_sz;

  if (B < 1)
  {
    cerr << "Block size must >= 1" << endl;
    return false;
  }
  else if (B*8 < out_w)
  {
    cerr << "Block size must be at least " << out_w/8 << endl;
    return false;
  }

  if (F_sz_um <= 0)
  {
    cerr << "Feature size must be > 0" << endl;
    return false;
  }
  else if (F_sz_um > 0.091)
  {
    cerr << "Feature size must be <= 90 nm" << endl;
    return false;
  }


  uint32_t RWP  = num_rw_ports;
  uint32_t ERP  = num_rd_ports;
  uint32_t EWP  = num_wr_ports;
  uint32_t NSER = num_se_rd_ports;
  uint32_t SCHP = num_search_ports;

//TODO: revisit this. This is an important feature.  thought this should be used
//  // If multiple banks and multiple ports are specified, then if number of ports is less than or equal to
//  // the number of banks, we assume that the multiple ports are implemented via the multiple banks.
//  // In such a case we assume that each bank has 1 RWP port.
//  if ((RWP + ERP + EWP) <= nbanks && nbanks>1)
//  {
//    RWP  = 1;
//    ERP  = 0;
//    EWP  = 0;
//    NSER = 0;
//  }
//  else if ((RWP < 0) || (EWP < 0) || (ERP < 0))
//  {
//    cerr << "Ports must >=0" << endl;
//    return false;
//  }
//  else if (RWP > 2)
//  {
//    cerr << "Maximum of 2 read/write ports" << endl;
//    return false;
//  }
//  else if ((RWP+ERP+EWP) < 1)
  // Changed to new implementation:
  // The number of ports specified at input is per bank
  if ((RWP+ERP+EWP) < 1)
  {
    cerr << "Must have at least one port" << endl;
    return false;
  }

  if (is_pow2(nbanks) == false)
  {
    cerr << "Number of subbanks should be greater than or equal to 1 and should be a power of 2" << endl;
    return false;
  }

  int C = cache_sz/nbanks;
  if (C < 64 && !g_ip->is_3d_mem)
  {
    cerr << "Cache size must >=64" << endl;
    return false;
  }

//TODO: revisit this
//   if (pure_ram==true && assoc!=1)
//    {
//  	  cerr << "Pure RAM must have assoc as 1" << endl;
//  	  return false;
//    }

    //fully assoc and cam check
    if (is_cache && assoc==0)
    	fully_assoc =true;
    else
    	fully_assoc = false;

    if (pure_cam==true && assoc!=0)
    {
  	  cerr << "Pure CAM must have associativity as 0" << endl;
  	  return false;
    }

    if (assoc==0 && (pure_cam==false && is_cache ==false))
    {
  	  cerr << "Only CAM or Fully associative cache can have associativity as 0" << endl;
  	  return false;
    }

    if ((fully_assoc==true || pure_cam==true)
  		  &&  (data_arr_ram_cell_tech_type!= tag_arr_ram_cell_tech_type
  				 || data_arr_peri_global_tech_type != tag_arr_peri_global_tech_type  ))
    {
  	  cerr << "CAM and fully associative cache must have same device type for both data and tag array" << endl;
  	  return false;
    }

    if ((fully_assoc==true || pure_cam==true)
  		  &&  (data_arr_ram_cell_tech_type== lp_dram || data_arr_ram_cell_tech_type== comm_dram))
    {
  	  cerr << "DRAM based CAM and fully associative cache are not supported" << endl;
  	  return false;
    }

    if ((fully_assoc==true || pure_cam==true)
  		  &&  (is_main_mem==true))
    {
  	  cerr << "CAM and fully associative cache cannot be as main memory" << endl;
  	  return false;
    }

    if ((fully_assoc || pure_cam) && SCHP<1)
    {
	  cerr << "CAM and fully associative must have at least 1 search port" << endl;
	  return false;
    }

   if (RWP==0 && ERP==0 && SCHP>0 && ((fully_assoc || pure_cam)))
    {
  	  ERP=SCHP;
    }

//    if ((!(fully_assoc || pure_cam)) && SCHP>=1)
//    {
//	  cerr << "None CAM and fully associative cannot have search ports" << endl;
//	  return false;
//    }

  if (assoc == 0)
  {
    A = C/B;
    //fully_assoc = true;
  }
  else
  {
    if (assoc == 1)
    {
      A = 1;
      //fully_assoc = false;
    }
    else
    {
      //fully_assoc = false;
      A = assoc;
      if (is_pow2(A) == false)
      {
        cerr << "Associativity must be a power of 2" << endl;
        return false;
      }
    }
  }

  if (C/(B*A) <= 1 && assoc!=0 && !g_ip->is_3d_mem)
  {
    cerr << "Number of sets is too small: " << endl;
    cerr << " Need to either increase cache size, or decrease associativity or block size" << endl;
    cerr << " (or use fully associative cache)" << endl;
    return false;
  }

  block_sz = B;

  /*dt: testing sequential access mode*/
  if(seq_access)
  {
    tag_assoc  = A;
    data_assoc = 1;
    is_seq_acc = true;
  }
  else
  {
    tag_assoc  = A;
    data_assoc = A;
    is_seq_acc = false;
  }

  if (assoc==0)
  {
    data_assoc = 1;
  }
  num_rw_ports     = RWP;
  num_rd_ports     = ERP;
  num_wr_ports     = EWP;
  num_se_rd_ports  = NSER;
  if (!(fully_assoc || pure_cam))
    num_search_ports = 0;
  nsets            = C/(B*A);

  if (temp < 300 || temp > 400 || temp%10 != 0)
  {
    cerr << temp << " Temperature must be between 300 and 400 Kelvin and multiple of 10." << endl;
    return false;
  }

  if (nsets < 1 && !g_ip->is_3d_mem)
  {
    cerr << "Less than one set..." << endl;
    return false;
  }

  power_gating = (array_power_gated
				  || bitline_floating
				  || wl_power_gated
				  || cl_power_gated
				  || interconect_power_gated)?true:false;

  return true;
}

void output_data_csv_3dd(const uca_org_t & fin_res)
{
  //TODO: the csv output should remain
  fstream file("out.csv", ios::in);
  bool    print_index = file.fail();
  file.close();

  file.open("out.csv", ios::out|ios::app);
  if (file.fail() == true)
  {
    cerr << "File out.csv could not be opened successfully" << endl;
  }
  else
  {
	//print_index = false;
    if (print_index == true)
    {
      file << "Tech node (nm), ";
      file << "Number of tiers, ";
      file << "Capacity (MB) per die, ";
      file << "Number of banks, ";
      file << "Page size in bits, ";
      //file << "Output width (bits), ";
      file << "Burst depth, ";
      file << "IO width, ";
      file << "Ndwl, ";
      file << "Ndbl, ";
      file << "N rows in subarray, ";
      file << "N cols in subarray, ";
//      file << "Access time (ns), ";
//      file << "Random cycle time (ns), ";
//      file << "Multisubbank interleave cycle time (ns), ";

//      file << "Delay request network (ns), ";
//      file << "Delay inside mat (ns), ";
//      file << "Delay reply network (ns), ";
//      file << "Tag array access time (ns), ";
//      file << "Data array access time (ns), ";
//      file << "Refresh period (microsec), ";
//      file << "DRAM array availability (%), ";



//      file << "Dynamic search energy (nJ), ";
//      file << "Dynamic read energy (nJ), ";
//      file << "Dynamic write energy (nJ), ";
//      file << "Tag Dynamic read energy (nJ), ";
//      file << "Data Dynamic read energy (nJ), ";
//      file << "Dynamic read power (mW), ";
//      file << "Standby leakage per bank(mW), ";
//      file << "Leakage per bank with leak power management (mW), ";
//      file << "Leakage per bank with leak power management (mW), ";
//      file << "Refresh power as percentage of standby leakage, ";
      file << "Area (mm2), ";

//      file << "Nspd, ";
//      file << "Ndcm, ";
//      file << "Ndsam_level_1, ";
//      file << "Ndsam_level_2, ";
      file << "Data arrary area efficiency %, ";
//      file << "Ntwl, ";
//      file << "Ntbl, ";
//      file << "Ntspd, ";
//      file << "Ntcm, ";
//      file << "Ntsam_level_1, ";
//      file << "Ntsam_level_2, ";
//      file << "Tag arrary area efficiency %, ";

//      file << "Resistance per unit micron (ohm-micron), ";
//      file << "Capacitance per unit micron (fF per micron), ";
//      file << "Unit-length wire delay (ps), ";
//      file << "FO4 delay (ps), ";
//      file << "delay route to bank (including crossb delay) (ps), ";
//      file << "Crossbar delay (ps), ";
//      file << "Dyn read energy per access from closed page (nJ), ";
//      file << "Dyn read energy per access from open page (nJ), ";
//      file << "Leak power of an subbank with page closed (mW), ";
//      file << "Leak power of a subbank with page  open (mW), ";
//      file << "Leak power of request and reply networks (mW), ";
//      file << "Number of subbanks, ";

      file << "Number of TSVs in total, ";
      file << "Delay of TSVs (ns) worst case, ";
      file << "Area of TSVs (mm2) in total, ";
      file << "Energy of TSVs (nJ) per access, ";

      file << "t_RCD (ns), ";
      file << "t_RAS (ns), ";
      file << "t_RC (ns), ";
      file << "t_CAS (ns), ";
      file << "t_RP (ns), ";


      file << "Activate energy (nJ), ";
      file << "Read energy (nJ), ";
      file << "Write energy (nJ), ";
      file << "Precharge energy (nJ), ";
      //file << "tRCD, ";
      //file << "CAS latency, ";
      //file << "Precharge delay, ";
//      file << "Perc dyn energy bitlines, ";
//      file << "perc dyn energy wordlines, ";
//      file << "perc dyn energy outside mat, ";
//      file << "Area opt (perc), ";
//      file << "Delay opt (perc), ";
//      file << "Repeater opt (perc), ";
      //file << "Aspect ratio";
      file << "t_RRD (ns), ";
      file << "Number tiers for a row, ";
      file << "Number tiers for a column, ";
      file << "delay_row_activate_net, " ;
      file << "delay_row_predecode_driver_and_block, " ;
      file << "delay_row_decoder, " ;
      file << "delay_local_wordline , " ;
      file << "delay_bitlines, " ;
      file << "delay_sense_amp, " ;

      file << "delay_column_access_net, " ;
      file << "delay_column_predecoder, " ;
      file << "delay_column_decoder, " ;
      file << "delay_column_selectline, " ;
      file << "delay_datapath_net, " ;
      file << "delay_global_data, " ;
      file << "delay_local_data_and_drv, " ;
      file << "delay_data_buffer, " ;
      file << "delay_subarray_output_driver, " ;

      file << "energy_row_activate_net, ";
      file << "energy_row_predecode_driver_and_block, ";
      file << "energy_row_decoder, ";
      file << "energy_local_wordline, ";
      file << "energy_bitlines, ";
      file << "energy_sense_amp, ";

      file << "energy_column_access_net, ";
      file << "energy_column_predecoder, ";
      file << "energy_column_decoder, ";
      file << "energy_column_selectline, ";
      file << "energy_datapath_net, ";
      file << "energy_global_data, ";
      file << "energy_local_data_and_drv, ";
      file << "energy_subarray_output_driver, ";
      file << "energy_data_buffer, ";

      file << "area_subarray, ";
      file << "area_lwl_drv, ";
      file << "area_row_predec_dec, ";
      file << "area_col_predec_dec, ";
      file << "area_bus, ";
      file << "area_address_bus, ";
      file << "area_data_bus, ";
      file << "area_data_drv, ";
      file << "area_IOSA, ";
      file << endl;
    }
    file << g_ip->F_sz_nm << ", ";
    file << g_ip->num_die_3d << ", ";
    file << g_ip->cache_sz * 1024 / g_ip->num_die_3d << ", ";
    file << g_ip->nbanks << ", ";
    file << g_ip->page_sz_bits << ", " ;
//    file << g_ip->tag_assoc << ", ";
    //file << g_ip->out_w << ", ";
    file << g_ip->burst_depth << ", ";
    file << g_ip->io_width << ", ";

    file << fin_res.data_array2->Ndwl << ", ";
    file << fin_res.data_array2->Ndbl << ", ";
    file << fin_res.data_array2->num_row_subarray << ", ";
    file << fin_res.data_array2->num_col_subarray << ", ";
//    file << fin_res.access_time*1e+9 << ", ";
//    file << fin_res.cycle_time*1e+9 << ", ";
//    file << fin_res.data_array2->multisubbank_interleave_cycle_time*1e+9 << ", ";
//    file << fin_res.data_array2->delay_request_network*1e+9 << ", ";
//    file << fin_res.data_array2->delay_inside_mat*1e+9 <<  ", ";
//    file << fin_res.data_array2.delay_reply_network*1e+9 << ", ";

//    if (!(g_ip->fully_assoc || g_ip->pure_cam || g_ip->pure_ram))
//        {
//    	  file << fin_res.tag_array2->access_time*1e+9 << ", ";
//        }
//    else
//    {
//    	file << 0 << ", ";
//    }
//    file << fin_res.data_array2->access_time*1e+9 << ", ";
//    file << fin_res.data_array2->dram_refresh_period*1e+6 << ", ";
//    file << fin_res.data_array2->dram_array_availability <<  ", ";
/*    if (g_ip->fully_assoc || g_ip->pure_cam)
    {
    	file << fin_res.power.searchOp.dynamic*1e+9 << ", ";
    }
    	else
    {
    		file << "N/A" << ", ";
    }
    */
//    file << fin_res.power.readOp.dynamic*1e+9 << ", ";
//    file << fin_res.power.writeOp.dynamic*1e+9 << ", ";
//    if (!(g_ip->fully_assoc || g_ip->pure_cam || g_ip->pure_ram))
//        {
//        	file << fin_res.tag_array2->power.readOp.dynamic*1e+9 << ", ";
//        }
//        	else
//        {
//        		file << "NA" << ", ";
//        }
//    file << fin_res.data_array2->power.readOp.dynamic*1e+9 << ", ";
//    if (g_ip->fully_assoc || g_ip->pure_cam)
//        {
//    	    file << fin_res.power.searchOp.dynamic*1000/fin_res.cycle_time << ", ";
//        }
//        	else
//        {
//        	file << fin_res.power.readOp.dynamic*1000/fin_res.cycle_time << ", ";
//        }

//    file <<( fin_res.power.readOp.leakage + fin_res.power.readOp.gate_leakage )*1000 << ", ";
//    file << fin_res.leak_power_with_sleep_transistors_in_mats*1000 << ", ";
//    file << fin_res.data_array.refresh_power / fin_res.data_array.total_power.readOp.leakage << ", ";
    file << fin_res.data_array2->area *1e-6 << ", ";

//    file << fin_res.data_array2->Nspd << ", ";
//    file << fin_res.data_array2->deg_bl_muxing << ", ";
//    file << fin_res.data_array2->Ndsam_lev_1 << ", ";
//    file << fin_res.data_array2->Ndsam_lev_2 << ", ";
    file << fin_res.data_array2->area_efficiency << ", ";
/*    if (!(g_ip->fully_assoc || g_ip->pure_cam || g_ip->pure_ram))
    {
    file << fin_res.tag_array2->Ndwl << ", ";
    file << fin_res.tag_array2->Ndbl << ", ";
    file << fin_res.tag_array2->Nspd << ", ";
    file << fin_res.tag_array2->deg_bl_muxing << ", ";
    file << fin_res.tag_array2->Ndsam_lev_1 << ", ";
    file << fin_res.tag_array2->Ndsam_lev_2 << ", ";
    file << fin_res.tag_array2->area_efficiency << ", ";
    }
    else
    {
    	file << "N/A" << ", ";
    	file << "N/A"<< ", ";
    	file << "N/A" << ", ";
    	file << "N/A" << ", ";
    	file << "N/A" << ", ";
    	file << "N/A" << ", ";
    	file << "N/A" << ", ";
    }
*/
    file << fin_res.data_array2->num_TSV_tot << ", ";
    file << fin_res.data_array2->delay_TSV_tot *1e9 << ", ";
    file << fin_res.data_array2->area_TSV_tot *1e-6 << ", ";
    file << fin_res.data_array2->dyn_pow_TSV_per_access *1e9 << ", ";

    file << fin_res.data_array2->t_RCD *1e9 << ", ";
    file << fin_res.data_array2->t_RAS *1e9 << ", ";
    file << fin_res.data_array2->t_RC *1e9 << ", ";
    file << fin_res.data_array2->t_CAS *1e9 << ", ";
    file << fin_res.data_array2->t_RP *1e9 << ", ";



//    file << g_tp.wire_inside_mat.R_per_um << ", ";
//    file << g_tp.wire_inside_mat.C_per_um / 1e-15 << ", ";
//    file << g_tp.unit_len_wire_del / 1e-12 << ", ";
//    file << g_tp.FO4 / 1e-12 << ", ";
//    file << fin_res.data_array.delay_route_to_bank / 1e-9 << ", ";
//    file << fin_res.data_array.delay_crossbar / 1e-9 << ", ";
//    file << fin_res.data_array.dyn_read_energy_from_closed_page / 1e-9 << ", ";
//    file << fin_res.data_array.dyn_read_energy_from_open_page / 1e-9 << ", ";
//    file << fin_res.data_array.leak_power_subbank_closed_page / 1e-3 << ", ";
//    file << fin_res.data_array.leak_power_subbank_open_page / 1e-3 << ", ";
//    file << fin_res.data_array.leak_power_request_and_reply_networks / 1e-3 << ", ";
//    file << fin_res.data_array.number_subbanks << ", " ;
    //file << fin_res.data_array.page_size_in_bits << ", " ;

    file << fin_res.data_array2->activate_energy * 1e9 << ", " ;
    file << fin_res.data_array2->read_energy * 1e9 << ", " ;
    file << fin_res.data_array2->write_energy * 1e9 << ", " ;
    file << fin_res.data_array2->precharge_energy * 1e9 << ", " ;
    //file << fin_res.data_array.trcd * 1e9 << ", " ;
    //file << fin_res.data_array.cas_latency * 1e9 << ", " ;
    //file << fin_res.data_array.precharge_delay * 1e9 << ", " ;
    //file << fin_res.data_array.all_banks_height / fin_res.data_array.all_banks_width;

    file << fin_res.data_array2->t_RRD * 1e9 << ", " ;
    file << g_ip->num_tier_row_sprd << ", " ;
    file << g_ip->num_tier_col_sprd << ", " ;

    file << fin_res.data_array2->delay_row_activate_net * 1e9 << ", " ;
    file << fin_res.data_array2->delay_row_predecode_driver_and_block * 1e9 << ", " ;
    file << fin_res.data_array2->delay_row_decoder * 1e9 << ", " ;
    file << fin_res.data_array2->delay_local_wordline * 1e9 << ", " ;
    file << fin_res.data_array2->delay_bitlines * 1e9 << ", " ;
    file << fin_res.data_array2->delay_sense_amp * 1e9 << ", " ;
    file << fin_res.data_array2->delay_column_access_net * 1e9 << ", " ;
    file << fin_res.data_array2->delay_column_predecoder * 1e9 << ", " ;
    file << fin_res.data_array2->delay_column_decoder * 1e9 << ", " ;
    file << fin_res.data_array2->delay_column_selectline * 1e9 << ", " ;
    file << fin_res.data_array2->delay_datapath_net * 1e9 << ", " ;
    file << fin_res.data_array2->delay_global_data * 1e9 << ", " ;
    file << fin_res.data_array2->delay_local_data_and_drv * 1e9 << ", " ;
    file << fin_res.data_array2->delay_data_buffer * 1e9 << ", " ;
    file << fin_res.data_array2->delay_subarray_output_driver * 1e9 << ", " ;

	file << fin_res.data_array2->energy_row_activate_net * 1e9 << ", " ;
	file << fin_res.data_array2->energy_row_predecode_driver_and_block * 1e9 << ", " ;
	file << fin_res.data_array2->energy_row_decoder * 1e9 << ", " ;
	file << fin_res.data_array2->energy_local_wordline * 1e9 << ", " ;
	file << fin_res.data_array2->energy_bitlines * 1e9 << ", " ;
	file << fin_res.data_array2->energy_sense_amp * 1e9 << ", " ;

	file << fin_res.data_array2->energy_column_access_net * 1e9 << ", " ;
	file << fin_res.data_array2->energy_column_predecoder * 1e9 << ", " ;
	file << fin_res.data_array2->energy_column_decoder * 1e9 << ", " ;
	file << fin_res.data_array2->energy_column_selectline * 1e9 << ", " ;
	file << fin_res.data_array2->energy_datapath_net * 1e9 << ", " ;
	file << fin_res.data_array2->energy_global_data * 1e9 << ", " ;
	file << fin_res.data_array2->energy_local_data_and_drv * 1e9 << ", " ;
	file << fin_res.data_array2->energy_subarray_output_driver * 1e9 << ", " ;
	file << fin_res.data_array2->energy_data_buffer * 1e9 << ", " ;

	file << fin_res.data_array2->area_subarray / 1e6 << ", " ;
	file << fin_res.data_array2->area_lwl_drv / 1e6 << ", " ;
	file << fin_res.data_array2->area_row_predec_dec / 1e6 << ", " ;
	file << fin_res.data_array2->area_col_predec_dec / 1e6 << ", " ;
	file << fin_res.data_array2->area_bus / 1e6 << ", " ;
	file << fin_res.data_array2->area_address_bus / 1e6 << ", " ;
	file << fin_res.data_array2->area_data_bus / 1e6 << ", " ;
	file << fin_res.data_array2->area_data_drv / 1e6 << ", " ;
	file << fin_res.data_array2->area_IOSA / 1e6 << ", " ;
	file << fin_res.data_array2->area_sense_amp / 1e6 << ", " ;
    file<<endl;
  }
  file.close();
}

void output_data_csv(const uca_org_t & fin_res, string fn)
{
  //TODO: the csv output should remain
  fstream file(fn.c_str(), ios::in);
  bool    print_index = file.fail();
  file.close();

  file.open(fn.c_str(), ios::out|ios::app);
  if (file.fail() == true)
  {
    cerr << "File out.csv could not be opened successfully" << endl;
  }
  else
  {
    if (print_index == true)
    {
      file << "Tech node (nm), ";
      file << "Capacity (bytes), ";
      file << "Number of banks, ";
      file << "Associativity, ";
      file << "Output width (bits), ";
      file << "Access time (ns), ";
      file << "Random cycle time (ns), ";
//      file << "Multisubbank interleave cycle time (ns), ";

//      file << "Delay request network (ns), ";
//      file << "Delay inside mat (ns), ";
//      file << "Delay reply network (ns), ";
//      file << "Tag array access time (ns), ";
//      file << "Data array access time (ns), ";
//      file << "Refresh period (microsec), ";
//      file << "DRAM array availability (%), ";
      file << "Dynamic search energy (nJ), ";
      file << "Dynamic read energy (nJ), ";
      file << "Dynamic write energy (nJ), ";
//      file << "Tag Dynamic read energy (nJ), ";
//      file << "Data Dynamic read energy (nJ), ";
//      file << "Dynamic read power (mW), ";
      file << "Standby leakage per bank(mW), ";
//      file << "Leakage per bank with leak power management (mW), ";
//      file << "Leakage per bank with leak power management (mW), ";
//      file << "Refresh power as percentage of standby leakage, ";
      file << "Area (mm2), ";
      file << "Ndwl, ";
      file << "Ndbl, ";
      file << "Nspd, ";
      file << "Ndcm, ";
      file << "Ndsam_level_1, ";
      file << "Ndsam_level_2, ";
      file << "Data arrary area efficiency %, ";
      file << "Ntwl, ";
      file << "Ntbl, ";
      file << "Ntspd, ";
      file << "Ntcm, ";
      file << "Ntsam_level_1, ";
      file << "Ntsam_level_2, ";
      file << "Tag arrary area efficiency %, ";

//      file << "Resistance per unit micron (ohm-micron), ";
//      file << "Capacitance per unit micron (fF per micron), ";
//      file << "Unit-length wire delay (ps), ";
//      file << "FO4 delay (ps), ";
//      file << "delay route to bank (including crossb delay) (ps), ";
//      file << "Crossbar delay (ps), ";
//      file << "Dyn read energy per access from closed page (nJ), ";
//      file << "Dyn read energy per access from open page (nJ), ";
//      file << "Leak power of an subbank with page closed (mW), ";
//      file << "Leak power of a subbank with page  open (mW), ";
//      file << "Leak power of request and reply networks (mW), ";
//      file << "Number of subbanks, ";
//      file << "Page size in bits, ";
//      file << "Activate power, ";
//      file << "Read power, ";
//      file << "Write power, ";
//      file << "Precharge power, ";
//      file << "tRCD, ";
//      file << "CAS latency, ";
//      file << "Precharge delay, ";
//      file << "Perc dyn energy bitlines, ";
//      file << "perc dyn energy wordlines, ";
//      file << "perc dyn energy outside mat, ";
//      file << "Area opt (perc), ";
//      file << "Delay opt (perc), ";
//      file << "Repeater opt (perc), ";
//      file << "Aspect ratio";
      file << endl;
    }
    file << g_ip->F_sz_nm << ", ";
    file << g_ip->cache_sz << ", ";
    file << g_ip->nbanks << ", ";
    file << g_ip->tag_assoc << ", ";
    file << g_ip->out_w << ", ";
    file << fin_res.access_time*1e+9 << ", ";
    file << fin_res.cycle_time*1e+9 << ", ";
//    file << fin_res.data_array2->multisubbank_interleave_cycle_time*1e+9 << ", ";
//    file << fin_res.data_array2->delay_request_network*1e+9 << ", ";
//    file << fin_res.data_array2->delay_inside_mat*1e+9 <<  ", ";
//    file << fin_res.data_array2.delay_reply_network*1e+9 << ", ";

//    if (!(g_ip->fully_assoc || g_ip->pure_cam || g_ip->pure_ram))
//        {
//    	  file << fin_res.tag_array2->access_time*1e+9 << ", ";
//        }
//    else
//    {
//    	file << 0 << ", ";
//    }
//    file << fin_res.data_array2->access_time*1e+9 << ", ";
//    file << fin_res.data_array2->dram_refresh_period*1e+6 << ", ";
//    file << fin_res.data_array2->dram_array_availability <<  ", ";
    if (g_ip->fully_assoc || g_ip->pure_cam)
    {
    	file << fin_res.power.searchOp.dynamic*1e+9 << ", ";
    }
    	else
    {
    		file << "N/A" << ", ";
    }
    file << fin_res.power.readOp.dynamic*1e+9 << ", ";
    file << fin_res.power.writeOp.dynamic*1e+9 << ", ";
//    if (!(g_ip->fully_assoc || g_ip->pure_cam || g_ip->pure_ram))
//        {
//        	file << fin_res.tag_array2->power.readOp.dynamic*1e+9 << ", ";
//        }
//        	else
//        {
//        		file << "NA" << ", ";
//        }
//    file << fin_res.data_array2->power.readOp.dynamic*1e+9 << ", ";
//    if (g_ip->fully_assoc || g_ip->pure_cam)
//        {
//    	    file << fin_res.power.searchOp.dynamic*1000/fin_res.cycle_time << ", ";
//        }
//        	else
//        {
//        	file << fin_res.power.readOp.dynamic*1000/fin_res.cycle_time << ", ";
//        }

    file <<( fin_res.power.readOp.leakage + fin_res.power.readOp.gate_leakage )*1000 << ", ";
//    file << fin_res.leak_power_with_sleep_transistors_in_mats*1000 << ", ";
//    file << fin_res.data_array.refresh_power / fin_res.data_array.total_power.readOp.leakage << ", ";
    file << fin_res.area*1e-6 << ", ";

    file << fin_res.data_array2->Ndwl << ", ";
    file << fin_res.data_array2->Ndbl << ", ";
    file << fin_res.data_array2->Nspd << ", ";
    file << fin_res.data_array2->deg_bl_muxing << ", ";
    file << fin_res.data_array2->Ndsam_lev_1 << ", ";
    file << fin_res.data_array2->Ndsam_lev_2 << ", ";
    file << fin_res.data_array2->area_efficiency << ", ";
    if (!(g_ip->fully_assoc || g_ip->pure_cam || g_ip->pure_ram))
    {
    file << fin_res.tag_array2->Ndwl << ", ";
    file << fin_res.tag_array2->Ndbl << ", ";
    file << fin_res.tag_array2->Nspd << ", ";
    file << fin_res.tag_array2->deg_bl_muxing << ", ";
    file << fin_res.tag_array2->Ndsam_lev_1 << ", ";
    file << fin_res.tag_array2->Ndsam_lev_2 << ", ";
    file << fin_res.tag_array2->area_efficiency << ", ";
    }
    else
    {
    	file << "N/A" << ", ";
    	file << "N/A"<< ", ";
    	file << "N/A" << ", ";
    	file << "N/A" << ", ";
    	file << "N/A" << ", ";
    	file << "N/A" << ", ";
    	file << "N/A" << ", ";
    }

//    file << g_tp.wire_inside_mat.R_per_um << ", ";
//    file << g_tp.wire_inside_mat.C_per_um / 1e-15 << ", ";
//    file << g_tp.unit_len_wire_del / 1e-12 << ", ";
//    file << g_tp.FO4 / 1e-12 << ", ";
//    file << fin_res.data_array.delay_route_to_bank / 1e-9 << ", ";
//    file << fin_res.data_array.delay_crossbar / 1e-9 << ", ";
//    file << fin_res.data_array.dyn_read_energy_from_closed_page / 1e-9 << ", ";
//    file << fin_res.data_array.dyn_read_energy_from_open_page / 1e-9 << ", ";
//    file << fin_res.data_array.leak_power_subbank_closed_page / 1e-3 << ", ";
//    file << fin_res.data_array.leak_power_subbank_open_page / 1e-3 << ", ";
//    file << fin_res.data_array.leak_power_request_and_reply_networks / 1e-3 << ", ";
//    file << fin_res.data_array.number_subbanks << ", " ;
//    file << fin_res.data_array.page_size_in_bits << ", " ;
//    file << fin_res.data_array.activate_energy * 1e9 << ", " ;
//    file << fin_res.data_array.read_energy * 1e9 << ", " ;
//    file << fin_res.data_array.write_energy * 1e9 << ", " ;
//    file << fin_res.data_array.precharge_energy * 1e9 << ", " ;
//    file << fin_res.data_array.trcd * 1e9 << ", " ;
//    file << fin_res.data_array.cas_latency * 1e9 << ", " ;
//    file << fin_res.data_array.precharge_delay * 1e9 << ", " ;
//    file << fin_res.data_array.all_banks_height / fin_res.data_array.all_banks_width;
    file<<endl;
  }
  file.close();
}



void output_UCA(uca_org_t *fr)
{
	if(g_ip->is_3d_mem)
	{

		cout<<"-------  CACTI (version "<< VER_MAJOR_CACTI <<"."<< VER_MINOR_CACTI<<"."VER_COMMENT_CACTI
								<< " of " << VER_UPDATE_CACTI << ") 3D DRAM Main Memory  -------"<<endl;

		cout << "\nMemory Parameters:\n";
		cout << "	Total memory size (Gb): " <<
				(int) (g_ip->cache_sz) << endl;
		if(g_ip->num_die_3d>1)
		{
			cout << "	Stacked die count: " << (int) g_ip->num_die_3d << endl;
			if(g_ip->TSV_proj_type == 1)
				cout << "	TSV projection: industrial conservative"  << endl;
			else
				cout << "	TSV projection: ITRS aggressive"  << endl;
		}
		cout << "	Number of banks: " << (int) g_ip->nbanks << endl;
		cout << "	Technology size (nm): " << g_ip->F_sz_nm << endl;
		cout << "	Page size (bits): " << g_ip->page_sz_bits << endl;
		cout << "	Burst depth: " << g_ip->burst_depth << endl;
		cout << "	Chip IO width: " << g_ip->io_width << endl;
		cout << "	Best Ndwl: " << fr->data_array2->Ndwl << endl;
		cout << "	Best Ndbl: " << fr->data_array2->Ndbl << endl;
		cout << "	# rows in subarray: " << fr->data_array2->num_row_subarray << endl;
		cout << "	# columns in subarray: " << fr->data_array2->num_col_subarray << endl;

		cout <<"\nResults:\n";
		cout<<"Timing Components:"<<endl;
	    cout<<"	   t_RCD (Row to column command delay): "<< fr->data_array2->t_RCD * 1e9 << " ns" <<endl;
		cout<<"	   t_RAS (Row access strobe latency): "<< fr->data_array2->t_RAS * 1e9  << " ns" <<endl;
		cout<<"	   t_RC (Row cycle): "<< fr->data_array2->t_RC * 1e9  << " ns" <<endl;
		cout<<"	   t_CAS (Column access strobe latency): "<< fr->data_array2->t_CAS * 1e9  << " ns" <<endl;
		cout<<"	   t_RP (Row precharge latency): "<< fr->data_array2->t_RP* 1e9  << " ns" <<endl;
		//cout<<"	   t_RRD (Rank to rank latency): "<< fr->data_array2->t_RRD* 1e9  << " ns" <<endl;
		cout<<"	   t_RRD (Row activation to row activation delay): "<< fr->data_array2->t_RRD * 1e9 << " ns"<<endl;

		cout<<"Power Components:"<<endl;
		cout<<"	   Activation energy: "<< fr->data_array2->activate_energy * 1e9  << " nJ" <<endl;
		cout<<"	   Read energy: "<< fr->data_array2->read_energy * 1e9  << " nJ" <<endl;
		cout<<"	   Write energy: "<< fr->data_array2->write_energy * 1e9  << " nJ" <<endl;
		cout<<"	   Precharge energy: "<< fr->data_array2->precharge_energy * 1e9  << " nJ" <<endl;
		//cout<<"	Activation power: "<<  fr->data_array2->activate_power * 1e3   << " mW" <<endl;
		//cout<<"	Read power: "<<  fr->data_array2->read_power * 1e3 << " mW" <<endl;
		//cout<<"	Write power: "<<  fr->data_array2->write_power * 1e3  << " mW" <<endl;
		//cout<<"	Peak read power: "<< read_energy/((g_ip->burst_depth)/(g_ip->sys_freq_MHz*1e6)/2) * 1e3  << " mW" <<endl;
		cout<<"Area Components:"<<endl;
		//cout<<"	Height: "<<area.h/1e3<<" mm"<<endl;
		//cout<<"	Length: "<<area.w/1e3<<" mm"<<endl;
		//cout<<"	DRAM+peri Area: "<< fr->data_array2->area/1e6<<" mm2"<<endl;

		//double DRAM_area_per_die = (g_ip->partition_gran>0) ? fr->data_array2->area : (fr->data_array2->area/0.5);
		double DRAM_area_per_die = (g_ip->partition_gran>0) ? fr->data_array2->area : (fr->data_array2->area + fr->data_array2->area_ram_cells*0.65);
		//double DRAM_area_per_die = (g_ip->partition_gran>0) ? fr->data_array2->area : (fr->data_array2->area + 2.5e9*(double)(g_ip->F_sz_um)*(g_ip->F_sz_um));
		double area_efficiency_per_die = (g_ip->partition_gran>0) ? fr->data_array2->area_efficiency : (fr->data_array2->area_ram_cells / DRAM_area_per_die *100);
		double DRAM_width = (g_ip->partition_gran>0) ? fr->data_array2->all_banks_width : (fr->data_array2->all_banks_width + (DRAM_area_per_die-fr->data_array2->area)/fr->data_array2->all_banks_height);
		cout<<"	   DRAM core area: "<< fr->data_array2->area/1e6 <<" mm2"<<endl;
		if (g_ip->partition_gran == 0)
			cout<<"	   DRAM area per die: "<< DRAM_area_per_die/1e6 <<" mm2"<<endl;
		cout<<"	   Area efficiency: "<< area_efficiency_per_die <<"%"<<endl;
		cout<<"	   DRAM die width: "<< DRAM_width/1e3 <<" mm"<<endl;
		cout<<"	   DRAM die height: "<< fr->data_array2->all_banks_height/1e3 <<" mm"<<endl;
		if (g_ip->num_die_3d>1)
		{
			cout<<"TSV Components:"<<endl;
			cout<<"	   TSV area overhead: "<< fr->data_array2->area_TSV_tot /1e6 <<" mm2"<<endl;
			cout<<"	   TSV latency overhead: "<< fr->data_array2->delay_TSV_tot * 1e9 <<" ns"<<endl;
			cout<<"	   TSV energy overhead per access: "<< fr->data_array2->dyn_pow_TSV_per_access * 1e9 <<" nJ"<<endl;
		}
	}
	else // if(!g_ip->is_3d_mem)
	{
  //    if (NUCA)
  if (0) {
    cout << "\n\n Detailed Bank Stats:\n";
    cout << "    Bank Size (bytes): %d\n" <<
                                     (int) (g_ip->cache_sz);
  }
  else {
    if (g_ip->data_arr_ram_cell_tech_type == 3) {
      cout << "\n---------- CACTI (version "<< VER_MAJOR_CACTI <<"."<< VER_MINOR_CACTI<<"."VER_COMMENT_CACTI
								<< " of " << VER_UPDATE_CACTI << "), Uniform Cache Access " <<
        "Logic Process Based DRAM Model ----------\n";
    }
    else if (g_ip->data_arr_ram_cell_tech_type == 4) {
      cout << "\n---------- CACTI (version "<< VER_MAJOR_CACTI <<"."<< VER_MINOR_CACTI<<"."VER_COMMENT_CACTI
								<< " of " << VER_UPDATE_CACTI << "), Uniform" <<
        "Cache Access Commodity DRAM Model ----------\n";
    }
    else {
      cout << "\n---------- CACTI (version "<< VER_MAJOR_CACTI <<"."<< VER_MINOR_CACTI<<"."VER_COMMENT_CACTI
								<< " of " << VER_UPDATE_CACTI << "), Uniform Cache Access "
        "SRAM Model ----------\n";
    }
    cout << "\nCache Parameters:\n";
    cout << "    Total cache size (bytes): " <<
      (int) (g_ip->cache_sz) << endl;
  }

  cout << "    Number of banks: " << (int) g_ip->nbanks << endl;
  if (g_ip->fully_assoc|| g_ip->pure_cam)
    cout << "    Associativity: fully associative\n";
  else {
    if (g_ip->tag_assoc == 1)
      cout << "    Associativity: direct mapped\n";
    else
      cout << "    Associativity: " <<
        g_ip->tag_assoc << endl;
  }


  cout << "    Block size (bytes): " << g_ip->line_sz << endl;
  cout << "    Read/write Ports: " <<
    g_ip->num_rw_ports << endl;
  cout << "    Read ports: " <<
    g_ip->num_rd_ports << endl;
  cout << "    Write ports: " <<
    g_ip->num_wr_ports << endl;
  if (g_ip->fully_assoc|| g_ip->pure_cam)
	  cout << "    search ports: " <<
	      g_ip->num_search_ports << endl;
  cout << "    Technology size (nm): " <<
    g_ip->F_sz_nm << endl << endl;

  cout << "    Access time (ns): " << fr->access_time*1e9 << endl;
  cout << "    Cycle time (ns):  " << fr->cycle_time*1e9 << endl;
  if (g_ip->data_arr_ram_cell_tech_type >= 4) {
    cout << "    Precharge Delay (ns): " << fr->data_array2->precharge_delay*1e9 << endl;
    cout << "    Activate Energy (nJ): " << fr->data_array2->activate_energy*1e9 << endl;
    cout << "    Read Energy (nJ): " << fr->data_array2->read_energy*1e9 << endl;
    cout << "    Write Energy (nJ): " << fr->data_array2->write_energy*1e9 << endl;
    cout << "    Precharge Energy (nJ): " << fr->data_array2->precharge_energy*1e9 << endl;
    cout << "    Leakage Power Closed Page (mW): " << fr->data_array2->leak_power_subbank_closed_page*1e3 << endl;
    cout << "    Leakage Power Open Page (mW): " << fr->data_array2->leak_power_subbank_open_page*1e3 << endl;
    cout << "    Leakage Power I/O (mW): " << fr->data_array2->leak_power_request_and_reply_networks*1e3 << endl;
    cout << "    Refresh power (mW): " <<
      fr->data_array2->refresh_power*1e3 << endl;
  }
  else {
	  if ((g_ip->fully_assoc|| g_ip->pure_cam))
	  {
		  cout << "    Total dynamic associative search energy per access (nJ): " <<
		  fr->power.searchOp.dynamic*1e9 << endl;
//		  cout << "    Total dynamic read energy per access (nJ): " <<
//		  fr->power.readOp.dynamic*1e9 << endl;
//		  cout << "    Total dynamic write energy per access (nJ): " <<
//		  fr->power.writeOp.dynamic*1e9 << endl;
	  }
//	  else
//	  {
		  cout << "    Total dynamic read energy per access (nJ): " <<
		  fr->power.readOp.dynamic*1e9 << endl;
		  cout << "    Total dynamic write energy per access (nJ): " <<
		  fr->power.writeOp.dynamic*1e9 << endl;
//	  }
	  cout << "    Total leakage power of a bank"
	  " (mW): " << fr->power.readOp.leakage*1e3 << endl;
	  cout << "    Total gate leakage power of a bank"
	  " (mW): " << fr->power.readOp.gate_leakage*1e3 << endl;
  }

  if (g_ip->data_arr_ram_cell_tech_type ==3 || g_ip->data_arr_ram_cell_tech_type ==4)
  {
  }
  cout <<  "    Cache height x width (mm): " <<
    fr->cache_ht*1e-3 << " x " << fr->cache_len*1e-3 << endl << endl;


  cout << "    Best Ndwl : " << fr->data_array2->Ndwl << endl;
  cout << "    Best Ndbl : " << fr->data_array2->Ndbl << endl;
  cout << "    Best Nspd : " << fr->data_array2->Nspd << endl;
  cout << "    Best Ndcm : " << fr->data_array2->deg_bl_muxing << endl;
  cout << "    Best Ndsam L1 : " << fr->data_array2->Ndsam_lev_1 << endl;
  cout << "    Best Ndsam L2 : " << fr->data_array2->Ndsam_lev_2 << endl << endl;

  if ((!(g_ip->pure_ram|| g_ip->pure_cam || g_ip->fully_assoc)) && !g_ip->is_main_mem)
  {
    cout << "    Best Ntwl : " << fr->tag_array2->Ndwl << endl;
    cout << "    Best Ntbl : " << fr->tag_array2->Ndbl << endl;
    cout << "    Best Ntspd : " << fr->tag_array2->Nspd << endl;
    cout << "    Best Ntcm : " << fr->tag_array2->deg_bl_muxing << endl;
    cout << "    Best Ntsam L1 : " << fr->tag_array2->Ndsam_lev_1 << endl;
    cout << "    Best Ntsam L2 : " << fr->tag_array2->Ndsam_lev_2 << endl;
  }

  switch (fr->data_array2->wt) {
    case (0):
      cout <<  "    Data array, H-tree wire type: Delay optimized global wires\n";
      break;
    case (1):
      cout <<  "    Data array, H-tree wire type: Global wires with 5\% delay penalty\n";
      break;
    case (2):
      cout <<  "    Data array, H-tree wire type: Global wires with 10\% delay penalty\n";
      break;
    case (3):
      cout <<  "    Data array, H-tree wire type: Global wires with 20\% delay penalty\n";
      break;
    case (4):
      cout <<  "    Data array, H-tree wire type: Global wires with 30\% delay penalty\n";
      break;
    case (5):
      cout <<  "    Data array, wire type: Low swing wires\n";
      break;
    default:
      cout << "ERROR - Unknown wire type " << (int) fr->data_array2->wt <<endl;
      exit(0);
  }

  if (!(g_ip->pure_ram|| g_ip->pure_cam || g_ip->fully_assoc)) {
    switch (fr->tag_array2->wt) {
      case (0):
        cout <<  "    Tag array, H-tree wire type: Delay optimized global wires\n";
        break;
      case (1):
        cout <<  "    Tag array, H-tree wire type: Global wires with 5\% delay penalty\n";
        break;
      case (2):
        cout <<  "    Tag array, H-tree wire type: Global wires with 10\% delay penalty\n";
        break;
      case (3):
        cout <<  "    Tag array, H-tree wire type: Global wires with 20\% delay penalty\n";
        break;
      case (4):
        cout <<  "    Tag array, H-tree wire type: Global wires with 30\% delay penalty\n";
        break;
      case (5):
        cout <<  "    Tag array, wire type: Low swing wires\n";
        break;
      default:
        cout << "ERROR - Unknown wire type " << (int) fr->tag_array2->wt <<endl;
        exit(-1);
    }
  }
	} //end if(!g_ip->is_3d_mem)
  if (g_ip->print_detail)
  {
    //if(g_ip->fully_assoc) return;

	  if (g_ip->is_3d_mem)
	  {
		  cout << endl << endl << "3D DRAM Detail Components:" << endl << endl;
		  cout << endl << "Time Components:" << endl << endl;
		  cout << "\t row activation bus delay (ns): "  << fr->data_array2->delay_row_activate_net*1e9 << endl;
		  cout << "\t row predecoder delay (ns): "  << fr->data_array2->delay_row_predecode_driver_and_block*1e9 << endl;
		  cout << "\t row decoder delay (ns): "  << fr->data_array2->delay_row_decoder*1e9 << endl;
		  cout << "\t local wordline delay (ns): "  << fr->data_array2->delay_local_wordline*1e9 << endl;
		  cout << "\t bitline delay (ns): "  << fr->data_array2->delay_bitlines*1e9 << endl;
		  cout << "\t sense amp delay (ns): "  << fr->data_array2->delay_sense_amp*1e9 << endl;
		  cout << "\t column access bus delay (ns): "  << fr->data_array2->delay_column_access_net*1e9 << endl;
		  cout << "\t column predecoder delay (ns): "  << fr->data_array2->delay_column_predecoder*1e9 << endl;
		  cout << "\t column decoder delay (ns): "  << fr->data_array2->delay_column_decoder*1e9 << endl;
		  //cout << "\t column selectline delay (ns): "  << fr->data_array2->delay_column_selectline*1e9 << endl;
		  cout << "\t datapath bus delay (ns): "  << fr->data_array2->delay_datapath_net*1e9 << endl;
		  cout << "\t global dataline delay (ns): "  << fr->data_array2->delay_global_data*1e9 << endl;
		  cout << "\t local dataline delay (ns): "  << fr->data_array2->delay_local_data_and_drv*1e9 << endl;
		  cout << "\t data buffer delay (ns): "  << fr->data_array2->delay_data_buffer*1e9 << endl;
		  cout << "\t subarray output driver delay (ns): "  << fr->data_array2->delay_subarray_output_driver*1e9 << endl;

		  cout << endl << "Energy Components:" << endl << endl;
		  cout << "\t row activation bus energy (nJ): "  << fr->data_array2->energy_row_activate_net*1e9 << endl;
		  cout << "\t row predecoder energy (nJ): "  << fr->data_array2->energy_row_predecode_driver_and_block*1e9 << endl;
		  cout << "\t row decoder energy (nJ): "  << fr->data_array2->energy_row_decoder*1e9 << endl;
		  cout << "\t local wordline energy (nJ): "  << fr->data_array2->energy_local_wordline*1e9 << endl;
		  cout << "\t bitline energy (nJ): "  << fr->data_array2->energy_bitlines*1e9 << endl;
		  cout << "\t sense amp energy (nJ): "  << fr->data_array2->energy_sense_amp*1e9 << endl;
		  cout << "\t column access bus energy (nJ): "  << fr->data_array2->energy_column_access_net*1e9 << endl;
		  cout << "\t column predecoder energy (nJ): "  << fr->data_array2->energy_column_predecoder*1e9 << endl;
		  cout << "\t column decoder energy (nJ): "  << fr->data_array2->energy_column_decoder*1e9 << endl;
		  cout << "\t column selectline energy (nJ): "  << fr->data_array2->energy_column_selectline*1e9 << endl;
		  cout << "\t datapath bus energy (nJ): "  << fr->data_array2->energy_datapath_net*1e9 << endl;
		  cout << "\t global dataline energy (nJ): "  << fr->data_array2->energy_global_data*1e9 << endl;
		  cout << "\t local dataline energy (nJ): "  << fr->data_array2->energy_local_data_and_drv*1e9 << endl;
		  cout << "\t data buffer energy (nJ): "  << fr->data_array2->energy_subarray_output_driver*1e9 << endl;
		  //cout << "\t subarray output driver energy (nJ): "  << fr->data_array2->energy_data_buffer*1e9 << endl;

		  cout << endl << "Area Components:" << endl << endl;
		  //cout << "\t subarray area (mm2): "  << fr->data_array2->area_subarray/1e6 << endl;
		  cout << "\t DRAM cell area (mm2): "  << fr->data_array2->area_ram_cells/1e6 << endl;
		  cout << "\t local WL driver area (mm2): "  << fr->data_array2->area_lwl_drv/1e6 << endl;
		  cout << "\t subarray sense amp area (mm2): "  << fr->data_array2->area_sense_amp/1e6 << endl;
		  cout << "\t row predecoder/decoder area (mm2): "  << fr->data_array2->area_row_predec_dec/1e6 << endl;
		  cout << "\t column predecoder/decoder area (mm2): "  << fr->data_array2->area_col_predec_dec/1e6 << endl;
		  cout << "\t center stripe bus area (mm2): "  << fr->data_array2->area_bus/1e6 << endl;
		  cout << "\t address bus area (mm2): "  << fr->data_array2->area_address_bus/1e6 << endl;
		  cout << "\t data bus area (mm2): "  << fr->data_array2->area_data_bus/1e6 << endl;
		  cout << "\t data driver area (mm2): "  << fr->data_array2->area_data_drv/1e6 << endl;
		  cout << "\t IO secondary sense amp area (mm2): "  << fr->data_array2->area_IOSA/1e6 << endl;
		  cout << "\t TSV area (mm2): "<< fr->data_array2->area_TSV_tot /1e6 << endl;

	  }
	  else //if (!g_ip->is_3d_mem)
	  {
	  if (g_ip->power_gating)
	  {
		  /* Energy/Power stats */
		  cout << endl << endl << "Power-gating Components:" << endl << endl;
		  /* Data array power-gating stats */
		  if (!(g_ip->pure_cam || g_ip->fully_assoc))
			  cout <<  "  Data array: " << endl;
		  else if (g_ip->pure_cam)
			  cout <<  "  CAM array: " << endl;
		  else
			  cout <<  "  Fully associative cache array: " << endl;

		  cout <<  "\t Sub-array Sleep Tx size (um) - " <<
		  fr->data_array2->sram_sleep_tx_width << endl;

		  //    cout <<  "\t Sub-array Sleep Tx total size (um) - " <<
		  //      fr->data_array2->sram_sleep_tx_width << endl;

		  cout <<  "\t Sub-array Sleep Tx total area (mm^2) - " <<
		  fr->data_array2->sram_sleep_tx_area*1e-6 << endl;

		  cout <<  "\t Sub-array wakeup time (ns) - " <<
		  fr->data_array2->sram_sleep_wakeup_latency*1e9 << endl;

		  cout <<  "\t Sub-array Tx energy (nJ) - " <<
		  fr->data_array2->sram_sleep_wakeup_energy*1e9 << endl;
		  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		  cout << endl;
		  cout <<  "\t WL Sleep Tx size (um) - " <<
		  fr->data_array2->wl_sleep_tx_width << endl;

		  //    cout <<  "\t WL Sleep total Tx size (um) - " <<
		  //      fr->data_array2->wl_sleep_tx_width << endl;

		  cout <<  "\t WL Sleep Tx total area (mm^2) - " <<
		  fr->data_array2->wl_sleep_tx_area*1e-6 << endl;

		  cout <<  "\t WL wakeup time (ns) - " <<
		  fr->data_array2->wl_sleep_wakeup_latency*1e9 << endl;

		  cout <<  "\t WL Tx energy (nJ) - " <<
		  fr->data_array2->wl_sleep_wakeup_energy*1e9 << endl;
		  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		  cout << endl;
		  cout <<  "\t BL floating wakeup time (ns) - " <<
		  fr->data_array2->bl_floating_wakeup_latency*1e9 << endl;

		  cout <<  "\t BL floating Tx energy (nJ) - " <<
		  fr->data_array2->bl_floating_wakeup_energy*1e9 << endl;
		  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		  cout << endl;

		  cout <<  "\t Active mats per access - " << fr->data_array2->num_active_mats<<endl;
		  cout <<  "\t Active subarrays per mat - " << fr->data_array2->num_submarray_mats<<endl;
		  cout << endl;
		  /* Tag array area stats */
		  if ((!(g_ip->pure_ram|| g_ip->pure_cam || g_ip->fully_assoc)) && !g_ip->is_main_mem)
		  {
			  cout <<  "  Tag array: " << endl;
			  cout <<  "\t Sub-array Sleep Tx size (um) - " <<
			  fr->tag_array2->sram_sleep_tx_width << endl;

			  //    cout <<  "\t Sub-array Sleep Tx total size (um) - " <<
			  //      fr->tag_array2->sram_sleep_tx_width << endl;

			  cout <<  "\t Sub-array Sleep Tx total area (mm^2) - " <<
			  fr->tag_array2->sram_sleep_tx_area*1e-6 << endl;

			  cout <<  "\t Sub-array wakeup time (ns) - " <<
			  fr->tag_array2->sram_sleep_wakeup_latency*1e9 << endl;

			  cout <<  "\t Sub-array Tx energy (nJ) - " <<
			  fr->tag_array2->sram_sleep_wakeup_energy*1e9 << endl;
			  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			  cout << endl;
			  cout <<  "\t WL Sleep Tx size (um) - " <<
			  fr->tag_array2->wl_sleep_tx_width << endl;

			  //    cout <<  "\t WL Sleep total Tx size (um) - " <<
			  //      fr->tag_array2->wl_sleep_tx_width << endl;

			  cout <<  "\t WL Sleep Tx total area (mm^2) - " <<
			  fr->tag_array2->wl_sleep_tx_area*1e-6 << endl;

			  cout <<  "\t WL wakeup time (ns) - " <<
			  fr->tag_array2->wl_sleep_wakeup_latency*1e9 << endl;

			  cout <<  "\t WL Tx energy (nJ) - " <<
			  fr->tag_array2->wl_sleep_wakeup_energy*1e9 << endl;
			  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			  cout << endl;
			  cout <<  "\t BL floating wakeup time (ns) - " <<
			  fr->tag_array2->bl_floating_wakeup_latency*1e9 << endl;

			  cout <<  "\t BL floating Tx energy (nJ) - " <<
			  fr->tag_array2->bl_floating_wakeup_energy*1e9 << endl;
			  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			  cout << endl;

			  cout <<  "\t Active mats per access - " << fr->tag_array2->num_active_mats<<endl;
			  cout <<  "\t Active subarrays per mat - " << fr->tag_array2->num_submarray_mats<<endl;
			  cout << endl;
		  }
	  }

    /* Delay stats */
    /* data array stats */
    cout << endl << "Time Components:" << endl << endl;

    cout << "  Data side (with Output driver) (ns): " <<
      fr->data_array2->access_time/1e-9 << endl;

    cout <<  "\tH-tree input delay (ns): " <<
      fr->data_array2->delay_route_to_bank * 1e9 +
      fr->data_array2->delay_input_htree * 1e9 << endl;

    if (!(g_ip->pure_cam || g_ip->fully_assoc))
    {
      cout <<  "\tDecoder + wordline delay (ns): " <<
        fr->data_array2->delay_row_predecode_driver_and_block * 1e9 +
        fr->data_array2->delay_row_decoder * 1e9 << endl;
    }
    else
    {
        cout <<  "\tCAM search delay (ns): " <<
          fr->data_array2->delay_matchlines * 1e9 << endl;
    }

    cout <<  "\tBitline delay (ns): " <<
      fr->data_array2->delay_bitlines/1e-9 << endl;

    cout <<  "\tSense Amplifier delay (ns): " <<
      fr->data_array2->delay_sense_amp * 1e9 << endl;


    cout <<  "\tH-tree output delay (ns): " <<
      fr->data_array2->delay_subarray_output_driver * 1e9 +
      fr->data_array2->delay_dout_htree * 1e9 << endl;

    if ((!(g_ip->pure_ram|| g_ip->pure_cam || g_ip->fully_assoc)) && !g_ip->is_main_mem)
    {
      /* tag array stats */
      cout << endl << "  Tag side (with Output driver) (ns): " <<
        fr->tag_array2->access_time/1e-9 << endl;

      cout <<  "\tH-tree input delay (ns): " <<
        fr->tag_array2->delay_route_to_bank * 1e9 +
        fr->tag_array2->delay_input_htree * 1e9 << endl;

      cout <<  "\tDecoder + wordline delay (ns): " <<
        fr->tag_array2->delay_row_predecode_driver_and_block * 1e9 +
        fr->tag_array2->delay_row_decoder * 1e9 << endl;

      cout <<  "\tBitline delay (ns): " <<
        fr->tag_array2->delay_bitlines/1e-9 << endl;

      cout <<  "\tSense Amplifier delay (ns): " <<
        fr->tag_array2->delay_sense_amp * 1e9 << endl;

      cout <<  "\tComparator delay (ns): " <<
        fr->tag_array2->delay_comparator * 1e9 << endl;

      cout <<  "\tH-tree output delay (ns): " <<
        fr->tag_array2->delay_subarray_output_driver * 1e9 +
        fr->tag_array2->delay_dout_htree * 1e9 << endl;
    }



    /* Energy/Power stats */
    cout << endl << endl << "Power Components:" << endl << endl;

    if (!(g_ip->pure_cam || g_ip->fully_assoc))
    {
    	cout << "  Data array: Total dynamic read energy/access  (nJ): " <<
    	      fr->data_array2->power.readOp.dynamic * 1e9 << endl;

    	cout << "\tTotal energy in H-tree (that includes both "
    	      "address and data transfer) (nJ): " <<
    	        (fr->data_array2->power_addr_input_htree.readOp.dynamic +
    	         fr->data_array2->power_data_output_htree.readOp.dynamic +
    	         fr->data_array2->power_routing_to_bank.readOp.dynamic) * 1e9 << endl;

    	cout << "\tOutput Htree inside bank Energy (nJ): " <<
    	   fr->data_array2->power_data_output_htree.readOp.dynamic * 1e9 << endl;
    	cout <<  "\tDecoder (nJ): " <<
    	   fr->data_array2->power_row_predecoder_drivers.readOp.dynamic * 1e9 +
    	   fr->data_array2->power_row_predecoder_blocks.readOp.dynamic * 1e9 << endl;
    	cout <<  "\tWordline (nJ): " <<
    	   fr->data_array2->power_row_decoders.readOp.dynamic * 1e9 << endl;
    	cout <<  "\tBitline mux & associated drivers (nJ): " <<
    	   fr->data_array2->power_bit_mux_predecoder_drivers.readOp.dynamic * 1e9 +
    	   fr->data_array2->power_bit_mux_predecoder_blocks.readOp.dynamic * 1e9 +
    	   fr->data_array2->power_bit_mux_decoders.readOp.dynamic * 1e9 << endl;
    	cout <<  "\tSense amp mux & associated drivers (nJ): " <<
    	   fr->data_array2->power_senseamp_mux_lev_1_predecoder_drivers.readOp.dynamic * 1e9 +
    	   fr->data_array2->power_senseamp_mux_lev_1_predecoder_blocks.readOp.dynamic * 1e9 +
    	   fr->data_array2->power_senseamp_mux_lev_1_decoders.readOp.dynamic * 1e9  +
    	   fr->data_array2->power_senseamp_mux_lev_2_predecoder_drivers.readOp.dynamic * 1e9 +
    	   fr->data_array2->power_senseamp_mux_lev_2_predecoder_blocks.readOp.dynamic * 1e9 +
    	   fr->data_array2->power_senseamp_mux_lev_2_decoders.readOp.dynamic * 1e9 << endl;

    	cout <<  "\tBitlines precharge and equalization circuit (nJ): " <<
    	    	   fr->data_array2->power_prechg_eq_drivers.readOp.dynamic * 1e9 << endl;
    	cout <<  "\tBitlines (nJ): " <<
    	   fr->data_array2->power_bitlines.readOp.dynamic * 1e9 << endl;
    	cout <<  "\tSense amplifier energy (nJ): " <<
    	   fr->data_array2->power_sense_amps.readOp.dynamic * 1e9 << endl;
    	cout <<  "\tSub-array output driver (nJ): " <<
    	   fr->data_array2->power_output_drivers_at_subarray.readOp.dynamic * 1e9 << endl;

    	cout << "\tTotal leakage power of a bank (mW): " <<
    	        fr->data_array2->power.readOp.leakage * 1e3 << endl;
    	cout << "\tTotal leakage power in H-tree (that includes both "
    	      "address and data network) ((mW)): " <<
    	        (fr->data_array2->power_addr_input_htree.readOp.leakage +
    	         fr->data_array2->power_data_output_htree.readOp.leakage +
    	         fr->data_array2->power_routing_to_bank.readOp.leakage) * 1e3 << endl;

    	cout << "\tTotal leakage power in cells (mW): " <<
    	        (fr->data_array2->array_leakage) * 1e3 << endl;
    	cout << "\tTotal leakage power in row logic(mW): " <<
    	        (fr->data_array2->wl_leakage) * 1e3 << endl;
    	cout << "\tTotal leakage power in column logic(mW): " <<
    	        (fr->data_array2->cl_leakage) * 1e3 << endl;

    	cout << "\tTotal gate leakage power in H-tree (that includes both "
    	      "address and data network) ((mW)): " <<
    	        (fr->data_array2->power_addr_input_htree.readOp.gate_leakage +
    	         fr->data_array2->power_data_output_htree.readOp.gate_leakage +
    	         fr->data_array2->power_routing_to_bank.readOp.gate_leakage) * 1e3 << endl;
    }

        else if (g_ip->pure_cam)
        {

           	cout << "  CAM array:"<<endl;
            	cout << "  Total dynamic associative search energy/access  (nJ): " <<
                      fr->data_array2->power.searchOp.dynamic * 1e9 << endl;
    	        cout << "\tTotal energy in H-tree (that includes both "
    	            	      "match key and data transfer) (nJ): " <<
    	              (fr->data_array2->power_htree_in_search.searchOp.dynamic +
    	               fr->data_array2->power_htree_out_search.searchOp.dynamic +
    	               fr->data_array2->power_routing_to_bank.searchOp.dynamic) * 1e9 << endl;
    	        cout << "\tKeyword input and result output Htrees inside bank Energy (nJ): " <<
    	              (fr->data_array2->power_htree_in_search.searchOp.dynamic +
    	       	               fr->data_array2->power_htree_out_search.searchOp.dynamic) * 1e9 << endl;
    	        cout <<  "\tSearchlines (nJ): " <<
    	          	   fr->data_array2->power_searchline.searchOp.dynamic * 1e9 +
    	          	   fr->data_array2->power_searchline_precharge.searchOp.dynamic * 1e9 << endl;
    	        cout <<  "\tMatchlines  (nJ): " <<
    	               fr->data_array2->power_matchlines.searchOp.dynamic * 1e9 +
    	        	   fr->data_array2->power_matchline_precharge.searchOp.dynamic * 1e9 << endl;
    	        cout <<  "\tSub-array output driver (nJ): " <<
    	          	   fr->data_array2->power_output_drivers_at_subarray.searchOp.dynamic * 1e9 << endl;


            	cout <<endl<< "  Total dynamic read energy/access  (nJ): " <<
            	      fr->data_array2->power.readOp.dynamic * 1e9 << endl;
    	        cout << "\tTotal energy in H-tree (that includes both "
    	            	      "address and data transfer) (nJ): " <<
    	              (fr->data_array2->power_addr_input_htree.readOp.dynamic +
    	               fr->data_array2->power_data_output_htree.readOp.dynamic +
    	               fr->data_array2->power_routing_to_bank.readOp.dynamic) * 1e9 << endl;
    	        cout << "\tOutput Htree inside bank Energy (nJ): " <<
    	          	   fr->data_array2->power_data_output_htree.readOp.dynamic * 1e9 << endl;
    	        cout <<  "\tDecoder (nJ): " <<
    	          	   fr->data_array2->power_row_predecoder_drivers.readOp.dynamic * 1e9 +
    	          	   fr->data_array2->power_row_predecoder_blocks.readOp.dynamic * 1e9 << endl;
    	        cout <<  "\tWordline (nJ): " <<
    	          	   fr->data_array2->power_row_decoders.readOp.dynamic * 1e9 << endl;
    	        cout <<  "\tBitline mux & associated drivers (nJ): " <<
    	          	   fr->data_array2->power_bit_mux_predecoder_drivers.readOp.dynamic * 1e9 +
    	          	   fr->data_array2->power_bit_mux_predecoder_blocks.readOp.dynamic * 1e9 +
    	           	   fr->data_array2->power_bit_mux_decoders.readOp.dynamic * 1e9 << endl;
    	        cout <<  "\tSense amp mux & associated drivers (nJ): " <<
    	         	   fr->data_array2->power_senseamp_mux_lev_1_predecoder_drivers.readOp.dynamic * 1e9 +
    	          	   fr->data_array2->power_senseamp_mux_lev_1_predecoder_blocks.readOp.dynamic * 1e9 +
    	          	   fr->data_array2->power_senseamp_mux_lev_1_decoders.readOp.dynamic * 1e9  +
    	           	   fr->data_array2->power_senseamp_mux_lev_2_predecoder_drivers.readOp.dynamic * 1e9 +
    	           	   fr->data_array2->power_senseamp_mux_lev_2_predecoder_blocks.readOp.dynamic * 1e9 +
    	          	   fr->data_array2->power_senseamp_mux_lev_2_decoders.readOp.dynamic * 1e9 << endl;
    	        cout <<  "\tBitlines (nJ): " <<
    	          	   fr->data_array2->power_bitlines.readOp.dynamic * 1e9 +
    	          	   fr->data_array2->power_prechg_eq_drivers.readOp.dynamic * 1e9<< endl;
    	        cout <<  "\tSense amplifier energy (nJ): " <<
    	          	   fr->data_array2->power_sense_amps.readOp.dynamic * 1e9 << endl;
    	        cout <<  "\tSub-array output driver (nJ): " <<
    	          	   fr->data_array2->power_output_drivers_at_subarray.readOp.dynamic * 1e9 << endl;

            	cout << endl <<"  Total leakage power of a bank (mW): " <<
                      fr->data_array2->power.readOp.leakage * 1e3 << endl;
        }
        else
        {
        	cout << "  Fully associative array:"<<endl;
        	cout << "  Total dynamic associative search energy/access  (nJ): " <<
                  fr->data_array2->power.searchOp.dynamic * 1e9 << endl;
	        cout << "\tTotal energy in H-tree (that includes both "
	            	      "match key and data transfer) (nJ): " <<
	              (fr->data_array2->power_htree_in_search.searchOp.dynamic +
	               fr->data_array2->power_htree_out_search.searchOp.dynamic +
	               fr->data_array2->power_routing_to_bank.searchOp.dynamic) * 1e9 << endl;
	        cout << "\tKeyword input and result output Htrees inside bank Energy (nJ): " <<
	              (fr->data_array2->power_htree_in_search.searchOp.dynamic +
	       	               fr->data_array2->power_htree_out_search.searchOp.dynamic) * 1e9 << endl;
	        cout <<  "\tSearchlines (nJ): " <<
	          	   fr->data_array2->power_searchline.searchOp.dynamic * 1e9 +
	          	   fr->data_array2->power_searchline_precharge.searchOp.dynamic * 1e9 << endl;
	        cout <<  "\tMatchlines  (nJ): " <<
	               fr->data_array2->power_matchlines.searchOp.dynamic * 1e9 +
	        	   fr->data_array2->power_matchline_precharge.searchOp.dynamic * 1e9 << endl;
	        cout <<  "\tData portion wordline (nJ): " <<
	          	   fr->data_array2->power_matchline_to_wordline_drv.searchOp.dynamic * 1e9 << endl;
	        cout <<  "\tData Bitlines (nJ): " <<
	          	   fr->data_array2->power_bitlines.searchOp.dynamic * 1e9 +
	          	   fr->data_array2->power_prechg_eq_drivers.searchOp.dynamic * 1e9 << endl;
	        cout <<  "\tSense amplifier energy (nJ): " <<
	          	   fr->data_array2->power_sense_amps.searchOp.dynamic * 1e9 << endl;
	        cout <<  "\tSub-array output driver (nJ): " <<
	          	   fr->data_array2->power_output_drivers_at_subarray.searchOp.dynamic * 1e9 << endl;


        	cout <<endl<< "  Total dynamic read energy/access  (nJ): " <<
        	      fr->data_array2->power.readOp.dynamic * 1e9 << endl;
	        cout << "\tTotal energy in H-tree (that includes both "
	            	      "address and data transfer) (nJ): " <<
	              (fr->data_array2->power_addr_input_htree.readOp.dynamic +
	               fr->data_array2->power_data_output_htree.readOp.dynamic +
	               fr->data_array2->power_routing_to_bank.readOp.dynamic) * 1e9 << endl;
	        cout << "\tOutput Htree inside bank Energy (nJ): " <<
	          	   fr->data_array2->power_data_output_htree.readOp.dynamic * 1e9 << endl;
	        cout <<  "\tDecoder (nJ): " <<
	          	   fr->data_array2->power_row_predecoder_drivers.readOp.dynamic * 1e9 +
	          	   fr->data_array2->power_row_predecoder_blocks.readOp.dynamic * 1e9 << endl;
	        cout <<  "\tWordline (nJ): " <<
	          	   fr->data_array2->power_row_decoders.readOp.dynamic * 1e9 << endl;
	        cout <<  "\tBitline mux & associated drivers (nJ): " <<
	          	   fr->data_array2->power_bit_mux_predecoder_drivers.readOp.dynamic * 1e9 +
	          	   fr->data_array2->power_bit_mux_predecoder_blocks.readOp.dynamic * 1e9 +
	           	   fr->data_array2->power_bit_mux_decoders.readOp.dynamic * 1e9 << endl;
	        cout <<  "\tSense amp mux & associated drivers (nJ): " <<
	         	   fr->data_array2->power_senseamp_mux_lev_1_predecoder_drivers.readOp.dynamic * 1e9 +
	          	   fr->data_array2->power_senseamp_mux_lev_1_predecoder_blocks.readOp.dynamic * 1e9 +
	          	   fr->data_array2->power_senseamp_mux_lev_1_decoders.readOp.dynamic * 1e9  +
	           	   fr->data_array2->power_senseamp_mux_lev_2_predecoder_drivers.readOp.dynamic * 1e9 +
	           	   fr->data_array2->power_senseamp_mux_lev_2_predecoder_blocks.readOp.dynamic * 1e9 +
	          	   fr->data_array2->power_senseamp_mux_lev_2_decoders.readOp.dynamic * 1e9 << endl;
	        cout <<  "\tBitlines (nJ): " <<
	          	   fr->data_array2->power_bitlines.readOp.dynamic * 1e9 +
	          	   fr->data_array2->power_prechg_eq_drivers.readOp.dynamic * 1e9<< endl;
	        cout <<  "\tSense amplifier energy (nJ): " <<
	          	   fr->data_array2->power_sense_amps.readOp.dynamic * 1e9 << endl;
	        cout <<  "\tSub-array output driver (nJ): " <<
	          	   fr->data_array2->power_output_drivers_at_subarray.readOp.dynamic * 1e9 << endl;

        	cout << endl <<"  Total leakage power of a bank (mW): " <<
                  fr->data_array2->power.readOp.leakage * 1e3 << endl;
      }


    if ((!(g_ip->pure_ram|| g_ip->pure_cam || g_ip->fully_assoc)) && !g_ip->is_main_mem)
    {
      cout << endl << "  Tag array:  Total dynamic read energy/access (nJ): " <<
        fr->tag_array2->power.readOp.dynamic * 1e9 << endl;
      cout << "\tTotal leakage read/write power of a bank (mW): " <<
          fr->tag_array2->power.readOp.leakage * 1e3 << endl;
      cout << "\tTotal energy in H-tree (that includes both "
        "address and data transfer) (nJ): " <<
          (fr->tag_array2->power_addr_input_htree.readOp.dynamic +
           fr->tag_array2->power_data_output_htree.readOp.dynamic +
           fr->tag_array2->power_routing_to_bank.readOp.dynamic) * 1e9 << endl;

      cout << "\tOutput Htree inside a bank Energy (nJ): " <<
        fr->tag_array2->power_data_output_htree.readOp.dynamic * 1e9 << endl;
      cout <<  "\tDecoder (nJ): " <<
        fr->tag_array2->power_row_predecoder_drivers.readOp.dynamic * 1e9 +
        fr->tag_array2->power_row_predecoder_blocks.readOp.dynamic * 1e9 << endl;
      cout <<  "\tWordline (nJ): " <<
        fr->tag_array2->power_row_decoders.readOp.dynamic * 1e9 << endl;
      cout <<  "\tBitline mux & associated drivers (nJ): " <<
        fr->tag_array2->power_bit_mux_predecoder_drivers.readOp.dynamic * 1e9 +
        fr->tag_array2->power_bit_mux_predecoder_blocks.readOp.dynamic * 1e9 +
        fr->tag_array2->power_bit_mux_decoders.readOp.dynamic * 1e9 << endl;
      cout <<  "\tSense amp mux & associated drivers (nJ): " <<
        fr->tag_array2->power_senseamp_mux_lev_1_predecoder_drivers.readOp.dynamic * 1e9 +
        fr->tag_array2->power_senseamp_mux_lev_1_predecoder_blocks.readOp.dynamic * 1e9 +
        fr->tag_array2->power_senseamp_mux_lev_1_decoders.readOp.dynamic * 1e9  +
        fr->tag_array2->power_senseamp_mux_lev_2_predecoder_drivers.readOp.dynamic * 1e9 +
        fr->tag_array2->power_senseamp_mux_lev_2_predecoder_blocks.readOp.dynamic * 1e9 +
        fr->tag_array2->power_senseamp_mux_lev_2_decoders.readOp.dynamic * 1e9 << endl;
      cout <<  "\tBitlines precharge and equalization circuit (nJ): " <<
        fr->tag_array2->power_prechg_eq_drivers.readOp.dynamic * 1e9 << endl;
      cout <<  "\tBitlines (nJ): " <<
        fr->tag_array2->power_bitlines.readOp.dynamic * 1e9 << endl;
      cout <<  "\tSense amplifier energy (nJ): " <<
        fr->tag_array2->power_sense_amps.readOp.dynamic * 1e9 << endl;
      cout <<  "\tSub-array output driver (nJ): " <<
        fr->tag_array2->power_output_drivers_at_subarray.readOp.dynamic * 1e9 << endl;

    	cout << "\tTotal leakage power of a bank (mW): " <<
    	        fr->tag_array2->power.readOp.leakage * 1e3 << endl;
    	cout << "\tTotal leakage power in H-tree (that includes both "
    	      "address and data network) ((mW)): " <<
    	        (fr->tag_array2->power_addr_input_htree.readOp.leakage +
    	         fr->tag_array2->power_data_output_htree.readOp.leakage +
    	         fr->tag_array2->power_routing_to_bank.readOp.leakage) * 1e3 << endl;

    	cout << "\tTotal leakage power in cells (mW): " <<
    	        (fr->tag_array2->array_leakage) * 1e3 << endl;
    	cout << "\tTotal leakage power in row logic(mW): " <<
    	        (fr->tag_array2->wl_leakage) * 1e3 << endl;
    	cout << "\tTotal leakage power in column logic(mW): " <<
    	        (fr->tag_array2->cl_leakage) * 1e3 << endl;
    	cout << "\tTotal gate leakage power in H-tree (that includes both "
    	      "address and data network) ((mW)): " <<
    	        (fr->tag_array2->power_addr_input_htree.readOp.gate_leakage +
    	         fr->tag_array2->power_data_output_htree.readOp.gate_leakage +
    	         fr->tag_array2->power_routing_to_bank.readOp.gate_leakage) * 1e3 << endl;
    }

    cout << endl << endl <<  "Area Components:" << endl << endl;
    /* Data array area stats */
    if (!(g_ip->pure_cam || g_ip->fully_assoc))
    	cout <<  "  Data array: Area (mm2): " << fr->data_array2->area * 1e-6 << endl;
    else if (g_ip->pure_cam)
    	cout <<  "  CAM array: Area (mm2): " << fr->data_array2->area * 1e-6 << endl;
    else
    	cout <<  "  Fully associative cache array: Area (mm2): " << fr->data_array2->area * 1e-6 << endl;
    cout <<  "\tHeight (mm): " <<
      fr->data_array2->all_banks_height*1e-3 << endl;
    cout <<  "\tWidth (mm): " <<
      fr->data_array2->all_banks_width*1e-3 << endl;
    if (g_ip->print_detail) {
      cout <<  "\tArea efficiency (Memory cell area/Total area) - " <<
        fr->data_array2->area_efficiency << " %" << endl;
      cout << "\t\tMAT Height (mm): " <<
        fr->data_array2->mat_height*1e-3 << endl;
      cout << "\t\tMAT Length (mm): " <<
        fr->data_array2->mat_length*1e-3 << endl;
      cout << "\t\tSubarray Height (mm): " <<
        fr->data_array2->subarray_height*1e-3 << endl;
      cout << "\t\tSubarray Length (mm): " <<
        fr->data_array2->subarray_length*1e-3 << endl;
    }

    /* Tag array area stats */
    if ((!(g_ip->pure_ram|| g_ip->pure_cam || g_ip->fully_assoc)) && !g_ip->is_main_mem)
    {
      cout << endl << "  Tag array: Area (mm2): " << fr->tag_array2->area * 1e-6 << endl;
      cout <<  "\tHeight (mm): " <<
        fr->tag_array2->all_banks_height*1e-3 << endl;
      cout <<  "\tWidth (mm): " <<
        fr->tag_array2->all_banks_width*1e-3 << endl;
      if (g_ip->print_detail)
      {
        cout <<  "\tArea efficiency (Memory cell area/Total area) - " <<
          fr->tag_array2->area_efficiency << " %" << endl;
      cout << "\t\tMAT Height (mm): " <<
        fr->tag_array2->mat_height*1e-3 << endl;
      cout << "\t\tMAT Length (mm): " <<
        fr->tag_array2->mat_length*1e-3 << endl;
      cout << "\t\tSubarray Height (mm): " <<
        fr->tag_array2->subarray_height*1e-3 << endl;
      cout << "\t\tSubarray Length (mm): " <<
        fr->tag_array2->subarray_length*1e-3 << endl;
      }
    }

	  }//if (!g_ip->is_3d_mem)



    Wire wpr;
    wpr.print_wire();

    //cout << "FO4 = " << g_tp.FO4 << endl;
  }
}

//McPAT's plain interface, please keep !!!
uca_org_t cacti_interface(InputParameter  * const local_interface)
{
//  g_ip = new InputParameter();
  //g_ip->add_ecc_b_ = true;

  uca_org_t fin_res;
  fin_res.valid = false;

  g_ip = local_interface;

//  g_ip->data_arr_ram_cell_tech_type    = data_arr_ram_cell_tech_flavor_in;
//  g_ip->data_arr_peri_global_tech_type = data_arr_peri_global_tech_flavor_in;
//  g_ip->tag_arr_ram_cell_tech_type     = tag_arr_ram_cell_tech_flavor_in;
//  g_ip->tag_arr_peri_global_tech_type  = tag_arr_peri_global_tech_flavor_in;
//
//  g_ip->ic_proj_type     = interconnect_projection_type_in;
//  g_ip->wire_is_mat_type = wire_inside_mat_type_in;
//  g_ip->wire_os_mat_type = wire_outside_mat_type_in;
//  g_ip->burst_len        = BURST_LENGTH_in;
//  g_ip->int_prefetch_w   = INTERNAL_PREFETCH_WIDTH_in;
//  g_ip->page_sz_bits     = PAGE_SIZE_BITS_in;
//
//  g_ip->cache_sz            = cache_size;
//  g_ip->line_sz             = line_size;
//  g_ip->assoc               = associativity;
//  g_ip->nbanks              = banks;
//  g_ip->out_w               = output_width;
//  g_ip->specific_tag        = specific_tag;
//  if (tag_width == 0) {
//    g_ip->tag_w = 42;
//  }
//  else {
//    g_ip->tag_w               = tag_width;
//  }
//
//  g_ip->access_mode         = access_mode;
//  g_ip->delay_wt = obj_func_delay;
//  g_ip->dynamic_power_wt = obj_func_dynamic_power;
//  g_ip->leakage_power_wt = obj_func_leakage_power;
//  g_ip->area_wt = obj_func_area;
//  g_ip->cycle_time_wt    = obj_func_cycle_time;
//  g_ip->delay_dev = dev_func_delay;
//  g_ip->dynamic_power_dev = dev_func_dynamic_power;
//  g_ip->leakage_power_dev = dev_func_leakage_power;
//  g_ip->area_dev = dev_func_area;
//  g_ip->cycle_time_dev    = dev_func_cycle_time;
//  g_ip->temp = temp;
//
//  g_ip->F_sz_nm         = tech_node;
//  g_ip->F_sz_um         = tech_node / 1000;
//  g_ip->is_main_mem     = (main_mem != 0) ? true : false;
//  g_ip->is_cache        = (cache ==1) ? true : false;
//  g_ip->pure_ram        = (cache ==0) ? true : false;
//  g_ip->pure_cam        = (cache ==2) ? true : false;
//  g_ip->rpters_in_htree = (REPEATERS_IN_HTREE_SEGMENTS_in != 0) ? true : false;
//  g_ip->ver_htree_wires_over_array = VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in;
//  g_ip->broadcast_addr_din_over_ver_htrees = BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in;
//
//  g_ip->num_rw_ports    = rw_ports;
//  g_ip->num_rd_ports    = excl_read_ports;
//  g_ip->num_wr_ports    = excl_write_ports;
//  g_ip->num_se_rd_ports = single_ended_read_ports;
//  g_ip->num_search_ports = search_ports;
//
//  g_ip->print_detail = 1;
//    g_ip->nuca = 0;
//    g_ip->is_cache=true;
//
//  if (force_wiretype == 0)
//  {
//	  g_ip->wt = Global;
//      g_ip->force_wiretype = false;
//  }
//  else
//  {   g_ip->force_wiretype = true;
//	  if (wiretype==10) {
//		  g_ip->wt = Global_10;
//	        }
//	  if (wiretype==20) {
//		  g_ip->wt = Global_20;
//	        }
//	  if (wiretype==30) {
//		  g_ip->wt = Global_30;
//	        }
//	  if (wiretype==5) {
//	      g_ip->wt = Global_5;
//	        }
//	  if (wiretype==0) {
//		  g_ip->wt = Low_swing;
//	  }
//  }
//  //g_ip->wt = Global_5;
//  if (force_config == 0)
//    {
//  	  g_ip->force_cache_config = false;
//    }
//    else
//    {
//    	g_ip->force_cache_config = true;
//    	g_ip->ndbl=ndbl;
//    	g_ip->ndwl=ndwl;
//    	g_ip->nspd=nspd;
//    	g_ip->ndcm=ndcm;
//    	g_ip->ndsam1=ndsam1;
//    	g_ip->ndsam2=ndsam2;
//
//
//    }
//
//  if (ecc==0){
//	  g_ip->add_ecc_b_=false;
//  }
//  else
//  {
//	  g_ip->add_ecc_b_=true;
//  }


  g_ip->error_checking();

  init_tech_params(g_ip->F_sz_um, false);
  Wire winit; // Do not delete this line. It initializes wires.

  solve(&fin_res);

//  g_ip->display_ip();
//  output_UCA(&fin_res);
//  output_data_csv(fin_res);

 // delete (g_ip);

  return fin_res;
}

//McPAT's plain interface, please keep !!!
uca_org_t init_interface(InputParameter* const local_interface)
{
 // g_ip = new InputParameter();
  //g_ip->add_ecc_b_ = true;

  uca_org_t fin_res;
  fin_res.valid = false;

   g_ip = local_interface;


//  g_ip->data_arr_ram_cell_tech_type    = data_arr_ram_cell_tech_flavor_in;
//  g_ip->data_arr_peri_global_tech_type = data_arr_peri_global_tech_flavor_in;
//  g_ip->tag_arr_ram_cell_tech_type     = tag_arr_ram_cell_tech_flavor_in;
//  g_ip->tag_arr_peri_global_tech_type  = tag_arr_peri_global_tech_flavor_in;
//
//  g_ip->ic_proj_type     = interconnect_projection_type_in;
//  g_ip->wire_is_mat_type = wire_inside_mat_type_in;
//  g_ip->wire_os_mat_type = wire_outside_mat_type_in;
//  g_ip->burst_len        = BURST_LENGTH_in;
//  g_ip->int_prefetch_w   = INTERNAL_PREFETCH_WIDTH_in;
//  g_ip->page_sz_bits     = PAGE_SIZE_BITS_in;
//
//  g_ip->cache_sz            = cache_size;
//  g_ip->line_sz             = line_size;
//  g_ip->assoc               = associativity;
//  g_ip->nbanks              = banks;
//  g_ip->out_w               = output_width;
//  g_ip->specific_tag        = specific_tag;
//  if (tag_width == 0) {
//    g_ip->tag_w = 42;
//  }
//  else {
//    g_ip->tag_w               = tag_width;
//  }
//
//  g_ip->access_mode         = access_mode;
//  g_ip->delay_wt = obj_func_delay;
//  g_ip->dynamic_power_wt = obj_func_dynamic_power;
//  g_ip->leakage_power_wt = obj_func_leakage_power;
//  g_ip->area_wt = obj_func_area;
//  g_ip->cycle_time_wt    = obj_func_cycle_time;
//  g_ip->delay_dev = dev_func_delay;
//  g_ip->dynamic_power_dev = dev_func_dynamic_power;
//  g_ip->leakage_power_dev = dev_func_leakage_power;
//  g_ip->area_dev = dev_func_area;
//  g_ip->cycle_time_dev    = dev_func_cycle_time;
//  g_ip->temp = temp;
//
//  g_ip->F_sz_nm         = tech_node;
//  g_ip->F_sz_um         = tech_node / 1000;
//  g_ip->is_main_mem     = (main_mem != 0) ? true : false;
//  g_ip->is_cache        = (cache ==1) ? true : false;
//  g_ip->pure_ram        = (cache ==0) ? true : false;
//  g_ip->pure_cam        = (cache ==2) ? true : false;
//  g_ip->rpters_in_htree = (REPEATERS_IN_HTREE_SEGMENTS_in != 0) ? true : false;
//  g_ip->ver_htree_wires_over_array = VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in;
//  g_ip->broadcast_addr_din_over_ver_htrees = BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in;
//
//  g_ip->num_rw_ports    = rw_ports;
//  g_ip->num_rd_ports    = excl_read_ports;
//  g_ip->num_wr_ports    = excl_write_ports;
//  g_ip->num_se_rd_ports = single_ended_read_ports;
//  g_ip->num_search_ports = search_ports;
//
//  g_ip->print_detail = 1;
//  g_ip->nuca = 0;
//
//  if (force_wiretype == 0)
//  {
//	  g_ip->wt = Global;
//      g_ip->force_wiretype = false;
//  }
//  else
//  {   g_ip->force_wiretype = true;
//	  if (wiretype==10) {
//		  g_ip->wt = Global_10;
//	        }
//	  if (wiretype==20) {
//		  g_ip->wt = Global_20;
//	        }
//	  if (wiretype==30) {
//		  g_ip->wt = Global_30;
//	        }
//	  if (wiretype==5) {
//	      g_ip->wt = Global_5;
//	        }
//	  if (wiretype==0) {
//		  g_ip->wt = Low_swing;
//	  }
//  }
//  //g_ip->wt = Global_5;
//  if (force_config == 0)
//    {
//  	  g_ip->force_cache_config = false;
//    }
//    else
//    {
//    	g_ip->force_cache_config = true;
//    	g_ip->ndbl=ndbl;
//    	g_ip->ndwl=ndwl;
//    	g_ip->nspd=nspd;
//    	g_ip->ndcm=ndcm;
//    	g_ip->ndsam1=ndsam1;
//    	g_ip->ndsam2=ndsam2;
//
//
//    }
//
//  if (ecc==0){
//	  g_ip->add_ecc_b_=false;
//  }
//  else
//  {
//	  g_ip->add_ecc_b_=true;
//  }


  g_ip->error_checking();

  init_tech_params(g_ip->F_sz_um, false);
  Wire winit; // Do not delete this line. It initializes wires.
  //solve(&fin_res);
  //g_ip->display_ip();

  //solve(&fin_res);
  //output_UCA(&fin_res);
  //output_data_csv(fin_res);
 // delete (g_ip);

  return fin_res;
}

void reconfigure(InputParameter *local_interface, uca_org_t *fin_res)
{
  // Copy the InputParameter to global interface (g_ip) and do error checking.
  g_ip = local_interface;
  g_ip->error_checking();

  // Initialize technology parameters
  init_tech_params(g_ip->F_sz_um,false);

  Wire winit; // Do not delete this line. It initializes wires.

  // This corresponds to solve() in the initialization process.
  update(fin_res);
}

