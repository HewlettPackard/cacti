#include "extio_technology.h"
#include "cacti_interface.h"
#include <cassert>

/* This file contains configuration parameters, including
 * default configuration for DDR3, LPDDR2 and WIDEIO. The configuration
 * parameters include technology parameters - voltage, load capacitances, IO
 * area coefficients, timing parameters, as well as external io configuration parameters -
 * termination values, voltage noise coefficients and voltage/timing noise
 * sensitivity parameters.  More details can be found in the CACTI-IO technical
 * report (), especially Chapters 2 and 3. The user can define new dram types here. */



///////////// DDR3 ///////////////////

 const double rtt1_wr_lrdimm_ddr3[8][4] =
{
	{INF,INF,120,120},
	{INF,INF,120,120},
	{INF,120,120,80},
	{120,120,120,60},
	{120,120,120,60},
	{120,80,80,60},
	{120,80,80,60},
	{120,80,60,40}
};

 const double rtt2_wr_lrdimm_ddr3[8][4] =
{
	{INF,INF,INF,INF},//1
	{INF,INF,120,120},//2
	{120,120,120,80}, //3
	{120,120,80,60}, //4
	{120,120,80,60},
	{120,80,60,40}, //6
	{120,80,60,40},
	{80,80,40,30}//8
};

 const double rtt1_rd_lrdimm_ddr3[8][4] =
{
	{INF,INF,120,120},//1
	{INF,INF,120,120},//2
	{INF,120,120,80}, //3
	{120,120,120,60}, //4
	{120,120,120,60},
	{120,80,80,60}, //6
	{120,80,80,60},
	{120,80,60,40}//8
};

 const double rtt2_rd_lrdimm_ddr3[8][4] =
{
	{INF,INF,INF,INF},//1
	{INF,120,80,60},//2
	{120,80,80,40}, //3
	{120,80,60,40}, //4
	{120,80,60,40},
	{80,60,60,30}, //6
	{80,60,60,30},
	{80,60,40,20}//8
};


 const double rtt1_wr_host_dimm_ddr3[3][4]=
{
	{120,120,120,60},
	{120,80,80,60},
	{120,80,60,40}
};

const double rtt2_wr_host_dimm_ddr3[3][4]=
{
	{120,120,80,60},
	{120,80,60,40},
	{80,80,40,30}
};

 const double rtt1_rd_host_dimm_ddr3[3][4]=
{
	{120,120,120,60},
	{120,80,80,60},
	{120,80,60,40}
};

 const double rtt2_rd_host_dimm_ddr3[3][4]=
{
	{120,80,60,40},
	{80,60,60,30},
	{80,60,40,20}
};


 const double rtt1_wr_bob_dimm_ddr3[3][4]=
{
	{INF,120,120,80},
	{120,120,120,60},
	{120,80,80,60}
};

 const double rtt2_wr_bob_dimm_ddr3[3][4]=
{
	{120,120,120,80},
	{120,120,80,60},
	{120,80,60,40}
};

 const double rtt1_rd_bob_dimm_ddr3[3][4]=
{
	{INF,120,120,80},
	{120,120,120,60},
	{120,80,80,60}
};

 const double rtt2_rd_bob_dimm_ddr3[3][4]=
{
	{120,80,80,40},
	{120,80,60,40},
	{80,60,60,30}
};


///////////// DDR4 ///////////////////

 const double rtt1_wr_lrdimm_ddr4[8][4] =
{
	{120,120,80,80},//1
	{120,120,80,80},//2
	{120,80,80,60}, //3
	{80,60,60,60}, //4
	{80,60,60,60},
	{60,60,60,40}, //6
	{60,60,60,40},
	{40,40,40,40}//8
};

 const double rtt2_wr_lrdimm_ddr4[8][4] =
{
	{INF,INF,INF,INF},//1
	{120,120,120,80},//2
	{120,80,80,80},//3
	{80,80,80,60},//4
	{80,80,80,60},
	{60,60,60,40},//6
	{60,60,60,40},
	{60,40,40,30}//8
};

 const double rtt1_rd_lrdimm_ddr4[8][4] =
{
	{120,120,80,80},//1
	{120,120,80,60},//2
	{120,80,80,60}, //3
	{120,60,60,60}, //4
	{120,60,60,60},
	{80,60,60,40}, //6
	{80,60,60,40},
	{60,40,40,30}//8
};

 const double rtt2_rd_lrdimm_ddr4[8][4] =
{
	{INF,INF,INF,INF},//1
	{80,60,60,60},//2
	{60,60,40,40}, //3
	{60,40,40,40}, //4
	{60,40,40,40},
	{40,40,40,30}, //6
	{40,40,40,30},
	{40,30,30,20}//8
};

 

 const double rtt1_wr_host_dimm_ddr4[3][4]=
{
	{80,60,60,60},
	{60,60,60,60},
	{40,40,40,40}
};

 const double rtt2_wr_host_dimm_ddr4[3][4]=
{
	{80,80,80,60},
	{60,60,60,40},
	{60,40,40,30}
};

 const double rtt1_rd_host_dimm_ddr4[3][4]=
{
	{120,60,60,60},
	{80,60,60,40},
	{60,40,40,30}
};

 const double rtt2_rd_host_dimm_ddr4[3][4]=
{
	{60,40,40,40},
	{40,40,40,30},
	{40,30,30,20}
};


 const double rtt1_wr_bob_dimm_ddr4[3][4]=
{
	{120,80,80,60},
	{80,60,60,60},
	{60,60,60,40}
};

 const double rtt2_wr_bob_dimm_ddr4[3][4]=
{
	{120,80,80,80},
	{80,80,80,60},
	{60,60,60,40}
};

 const double rtt1_rd_bob_dimm_ddr4[3][4]=
{
	{120,80,80,60},
	{120,60,60,60},
	{80,60,60,40}
};

 const double rtt2_rd_bob_dimm_ddr4[3][4]=
{
	{60,60,40,40},
	{60,40,40,40},
	{40,40,40,30}
};


/////////////////////////////////////////////

int IOTechParam::frequnecy_index(Mem_IO_type type)
{
	if(type==DDR3)
	{
		if(frequency<=400)
			return 0;
		else if(frequency<=533)
			return 1;
		else if(frequency<=667)
			return 2;	
		else
			return 3;
	}
	else if(type==DDR4)
	{
		if(frequency<=800)
			return 0;
		else if(frequency<=933)
			return 1;
		else if(frequency<=1066)
			return 2;	
		else
			return 3;
	}
	else
	{
		assert(false);
	}
	return 0;
}



IOTechParam::IOTechParam(InputParameter * g_ip) 
{
  num_mem_ca  = g_ip->num_mem_dq * (g_ip->num_dq/g_ip->mem_data_width); 
  num_mem_clk =  g_ip->num_mem_dq *
                (g_ip->num_dq/g_ip->mem_data_width)/(g_ip->num_clk/2); 


  if (g_ip->io_type == LPDDR2) { //LPDDR
    //Technology Parameters

    vdd_io = 1.2;
    v_sw_clk =  1;

    // Loading paramters
    c_int = 1.5; 
    c_tx = 2;  
    c_data = 1.5;
    c_addr = 0.75;
    i_bias =  5; 
    i_leak = 1000; 

    // IO Area coefficients

    ioarea_c = 0.01; 
    ioarea_k0 = 0.5; 
    ioarea_k1 = 0.00008; 
    ioarea_k2 = 0.000000030; 
    ioarea_k3 = 0.000000000008; 

    // Timing parameters (ps)
    t_ds = 250; 
    t_is = 250;
    t_dh = 250;
    t_ih = 250; 
    t_dcd_soc = 50;
    t_dcd_dram = 50; 
    t_error_soc = 50;
    t_skew_setup = 50;
    t_skew_hold = 50;
    t_dqsq = 250; 
    t_soc_setup = 50;
    t_soc_hold = 50;
    t_jitter_setup = 200; 
    t_jitter_hold = 200; 
    t_jitter_addr_setup = 200;
    t_jitter_addr_hold = 200;
    t_cor_margin = 40; 

    //External IO Configuration Parameters 

    r_diff_term = 480;
    rtt1_dq_read = 100000; 
    rtt2_dq_read = 100000; 
    rtt1_dq_write = 100000; 
    rtt2_dq_write = 100000; 
    rtt_ca = 240; 
    rs1_dq = 0; 
    rs2_dq = 0; 
    r_stub_ca = 0; 
    r_on = 50; 
    r_on_ca = 50;
    z0 = 50;
    t_flight = 0.5;
    t_flight_ca = 0.5;

    // Voltage noise coeffecients
    k_noise_write = 0.2; 
    k_noise_read = 0.2; 
    k_noise_addr = 0.2; 
    v_noise_independent_write = 0.1; 
    v_noise_independent_read = 0.1; 
    v_noise_independent_addr = 0.1; 

    //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

/* This is a user-defined section that depends on the channel sensitivity
      * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
      * parameters that are impacted based on the channel analysis. The user
      * can define any relationship between the termination, loading and
      * configuration parameters AND the t_jitter/k_noise parameters. E.g. a
      * linear relationship, a non-linear analytical relationship or a lookup
      * table. The sensitivity coefficients are based on channel analysis
      * performed on the channel of interest.Given below is an example of such
      * a sensitivity relationship.
      * Such a linear fit can be found efficiently using an orthogonal design 
      * of experiments method shown in the technical report (), in Chapter 2.2. */

    k_noise_write_sen = k_noise_write * (1 + 0.2*(r_on/34 - 1) +
        0.2*(g_ip->num_mem_dq/2 - 1));
    k_noise_read_sen = k_noise_read * (1 + 0.2*(r_on/34 - 1) +
        0.2*(g_ip->num_mem_dq/2 - 1));
    k_noise_addr_sen = k_noise_addr * (1 + 0.1*(rtt_ca/100 - 1) +
        0.2*(r_on/34 - 1) + 0.2*(num_mem_ca/16 - 1));

    t_jitter_setup_sen = t_jitter_setup * (1  + 0.1*(r_on/34 - 1) +
        0.3*(g_ip->num_mem_dq/2 - 1));
    t_jitter_hold_sen = t_jitter_hold * (1 + 0.1*(r_on/34 - 1) +
        0.3*(g_ip->num_mem_dq/2 - 1));
    t_jitter_addr_setup_sen = t_jitter_addr_setup * (1 + 0.2*(rtt_ca/100 - 1) +
        0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));
    t_jitter_addr_hold_sen = t_jitter_addr_hold * (1 + 0.2*(rtt_ca/100 - 1) +
        0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

    // PHY Static Power Coefficients (mW)

    phy_datapath_s = 0; 
    phy_phase_rotator_s = 5; 
    phy_clock_tree_s = 0; 
    phy_rx_s = 3; 
    phy_dcc_s = 0; 
    phy_deskew_s = 0; 
    phy_leveling_s = 0;
    phy_pll_s = 2;


    // PHY Dynamic Power Coefficients (mW/Gbps)

    phy_datapath_d = 0.3;
    phy_phase_rotator_d = 0.01;
    phy_clock_tree_d = 0.4;
    phy_rx_d = 0.2;
    phy_dcc_d = 0;
    phy_deskew_d = 0;
    phy_leveling_d = 0;
    phy_pll_d = 0.05;


	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 5;
	phy_rx_wtime = 2;
	phy_bandgap_wtime = 10;
	phy_deskew_wtime = 0;
	phy_vrefgen_wtime = 0;


  }
   else if (g_ip->io_type == WideIO) { //WIDEIO 
     //Technology Parameters
     vdd_io = 1.2;
     v_sw_clk =  1.2;

     // Loading parameters
     c_int = 0.5; 
     c_tx = 0.5;  
     c_data = 0.5;  
     c_addr = 0.35; 
     i_bias =  0; 
     i_leak = 500; 

     // IO Area coefficients
     ioarea_c = 0.003; 
     ioarea_k0 = 0.2; 
     ioarea_k1 = 0.00004; 
     ioarea_k2 = 0.000000020; 
     ioarea_k3 = 0.000000000004; 

     // Timing parameters (ps)
     t_ds = 250; 
     t_is = 250;  
     t_dh = 250; 
     t_ih = 250; 
     t_dcd_soc = 50;  
     t_dcd_dram = 50; 
     t_error_soc = 50; 
     t_skew_setup = 50;  
     t_skew_hold = 50;  
     t_dqsq = 250;  
     t_soc_setup = 50; 
     t_soc_hold = 50; 
     t_jitter_setup = 200; 
     t_jitter_hold = 200; 
     t_jitter_addr_setup = 200; 
     t_jitter_addr_hold = 200;
	t_cor_margin = 50; 

     //External IO Configuration Parameters 

     r_diff_term = 100000;
     rtt1_dq_read = 100000; 
     rtt2_dq_read = 100000; 
     rtt1_dq_write = 100000; 
     rtt2_dq_write = 100000; 
     rtt_ca = 100000; 
     rs1_dq = 0; 
     rs2_dq = 0; 
     r_stub_ca = 0; 
     r_on = 75; 
     r_on_ca = 75; 
     z0 = 50;
     t_flight = 0.05;
     t_flight_ca = 0.05;

     // Voltage noise coeffecients
     k_noise_write = 0.2; 
     k_noise_read = 0.2; 
     k_noise_addr = 0.2; 
     v_noise_independent_write = 0.1; 
     v_noise_independent_read = 0.1; 
     v_noise_independent_addr = 0.1; 

     //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

     /* This is a user-defined section that depends on the channel sensitivity
      * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
      * parameters that are impacted based on the channel analysis. The user
      * can define any relationship between the termination, loading and
      * configuration parameters AND the t_jitter/k_noise parameters. E.g. a
      * linear relationship, a non-linear analytical relationship or a lookup
      * table. The sensitivity coefficients are based on channel analysis
      * performed on the channel of interest.Given below is an example of such
      * a sensitivity relationship.
      * Such a linear fit can be found efficiently using an orthogonal design 
      * of experiments method shown in the technical report (), in Chapter 2.2. */

     k_noise_write_sen = k_noise_write * (1 + 0.2*(r_on/50 - 1) + 
         0.2*(g_ip->num_mem_dq/2 - 1));
     k_noise_read_sen = k_noise_read * (1 + 0.2*(r_on/50 - 1) + 
         0.2*(g_ip->num_mem_dq/2 - 1));
     k_noise_addr_sen = k_noise_addr * (1 + 0.2*(r_on/50 - 1) + 
         0.2*(num_mem_ca/16 - 1));


     t_jitter_setup_sen = t_jitter_setup * (1  + 0.1*(r_on/50 - 1) + 
         0.3*(g_ip->num_mem_dq/2 - 1));
     t_jitter_hold_sen = t_jitter_hold * (1 + 0.1*(r_on/50 - 1) + 
         0.3*(g_ip->num_mem_dq/2 - 1));
     t_jitter_addr_setup_sen = t_jitter_addr_setup * (1 + 0.1*(r_on/50 - 1) + 
         0.4*(num_mem_ca/16 - 1));
     t_jitter_addr_hold_sen = t_jitter_addr_hold * (1 + 0.1*(r_on/50 - 1) + 
         0.4*(num_mem_ca/16 - 1));

     // PHY Static Power Coefficients (mW)
     phy_datapath_s = 0;
     phy_phase_rotator_s = 1;
     phy_clock_tree_s = 0;
     phy_rx_s = 0;
     phy_dcc_s = 0;
     phy_deskew_s = 0;
     phy_leveling_s = 0;
     phy_pll_s = 0;


     // PHY Dynamic Power Coefficients (mW/Gbps)
     phy_datapath_d = 0.3;
     phy_phase_rotator_d = 0.01;
     phy_clock_tree_d = 0.2;
     phy_rx_d = 0.1;
     phy_dcc_d = 0;
     phy_deskew_d = 0;
     phy_leveling_d = 0;
     phy_pll_d = 0;

	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 0;
	phy_rx_wtime = 0;
	phy_bandgap_wtime = 0;
	phy_deskew_wtime = 0;
	phy_vrefgen_wtime = 0;


   }
   else if (g_ip->io_type == DDR3)
   { //Default parameters for DDR3
     // IO Supply voltage (V) 
     vdd_io = 1.5;
     v_sw_clk =  0.75;

     // Loading parameters
     c_int = 1.5; 
     c_tx = 2;  
     c_data = 1.5; 
     c_addr = 0.75; 
     i_bias =  15; 
     i_leak = 1000; 

     // IO Area coefficients
     ioarea_c = 0.01; 
     ioarea_k0 = 0.5; 
     ioarea_k1 = 0.00015; 
     ioarea_k2 = 0.000000045; 
     ioarea_k3 = 0.000000000015; 

     // Timing parameters (ps)
     t_ds = 150;  
     t_is = 150; 
     t_dh = 150;
     t_ih = 150;
     t_dcd_soc = 50; 
     t_dcd_dram = 50;
     t_error_soc = 25; 
     t_skew_setup = 25; 
     t_skew_hold = 25; 
     t_dqsq = 100; 
     t_soc_setup = 50; 
     t_soc_hold = 50; 
     t_jitter_setup = 100; 
     t_jitter_hold = 100; 
     t_jitter_addr_setup = 100;
     t_jitter_addr_hold = 100;
	t_cor_margin = 30;


     //External IO Configuration Parameters 

     r_diff_term = 100;
     rtt1_dq_read = g_ip->rtt_value; 
     rtt2_dq_read = g_ip->rtt_value; 
     rtt1_dq_write = g_ip->rtt_value;
     rtt2_dq_write = g_ip->rtt_value; 
     rtt_ca = 50; 
     rs1_dq = 15; 
     rs2_dq = 15; 
     r_stub_ca = 0; 
     r_on = g_ip->ron_value; 
     r_on_ca = 50; 
     z0 = 50;
     t_flight = g_ip->tflight_value;
     t_flight_ca = 2;

     // Voltage noise coeffecients

     k_noise_write = 0.2; 
     k_noise_read = 0.2; 
     k_noise_addr = 0.2; 
     v_noise_independent_write = 0.1; 
     v_noise_independent_read = 0.1; 
     v_noise_independent_addr = 0.1; 

     //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

     /* This is a user-defined section that depends on the channel sensitivity
      * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
      * parameters that are impacted based on the channel analysis. The user
      * can define any relationship between the termination, loading and
      * configuration parameters AND the t_jitter/k_noise parameters. E.g. a
      * linear relationship, a non-linear analytical relationship or a lookup
      * table. The sensitivity coefficients are based on channel analysis
      * performed on the channel of interest.Given below is an example of such
      * a sensitivity relationship.
      * Such a linear fit can be found efficiently using an orthogonal design 
      * of experiments method shown in the technical report (), in Chapter 2.2. */

     k_noise_write_sen = k_noise_write * (1 + 0.1*(rtt1_dq_write/60 - 1) +
         0.2*(rtt2_dq_write/60 - 1) + 0.2*(r_on/34 - 1) +
         0.2*(g_ip->num_mem_dq/2 - 1));

     k_noise_read_sen = k_noise_read * (1 + 0.1*(rtt1_dq_read/60 - 1) +
         0.2*(rtt2_dq_read/60 - 1) + 0.2*(r_on/34 - 1) +
         0.2*(g_ip->num_mem_dq/2 - 1));

     k_noise_addr_sen = k_noise_addr * (1 + 0.1*(rtt_ca/50 - 1) +
         0.2*(r_on/34 - 1) + 0.2*(num_mem_ca/16 - 1));


     t_jitter_setup_sen = t_jitter_setup * (1 + 0.2*(rtt1_dq_write/60 - 1) + 
         0.3*(rtt2_dq_write/60 - 1) + 0.1*(r_on/34 - 1) + 
         0.3*(g_ip->num_mem_dq/2 - 1));

     t_jitter_hold_sen = t_jitter_hold * (1 + 0.2*(rtt1_dq_write/60 - 1) + 
         0.3*(rtt2_dq_write/60 - 1) + 
         0.1*(r_on/34 - 1) + 0.3*(g_ip->num_mem_dq/2 - 1));

     t_jitter_addr_setup_sen = t_jitter_addr_setup * (1 + 0.2*(rtt_ca/50 - 1) + 
         0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

     t_jitter_addr_hold_sen = t_jitter_addr_hold * (1 + 0.2*(rtt_ca/50 - 1) + 
         0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

     // PHY Static Power Coefficients (mW)
     phy_datapath_s = 0; 
     phy_phase_rotator_s = 10; 
     phy_clock_tree_s = 0; 
     phy_rx_s = 10; 
     phy_dcc_s = 0; 
     phy_deskew_s = 0; 
     phy_leveling_s = 0; 
     phy_pll_s = 10; 

     // PHY Dynamic Power Coefficients (mW/Gbps)
     phy_datapath_d = 0.5; 
     phy_phase_rotator_d = 0.01; 
     phy_clock_tree_d = 0.5; 
     phy_rx_d = 0.5; 
     phy_dcc_d = 0.05; 
     phy_deskew_d = 0.1; 
     phy_leveling_d = 0.05; 
     phy_pll_d = 0.05; 

	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 5;
	phy_rx_wtime = 2;
	phy_bandgap_wtime = 10;
	phy_deskew_wtime = 0.003;
	phy_vrefgen_wtime = 0.5;


   }
   else if (g_ip->io_type == DDR4)
   { //Default parameters for DDR4
     // IO Supply voltage (V) 
     vdd_io = 1.2;
     v_sw_clk =  0.6;

     // Loading parameters
     c_int = 1.5; 
     c_tx = 2;  
     c_data = 1; 
     c_addr = 0.75; 
     i_bias =  15; 
     i_leak = 1000; 

     // IO Area coefficients
     ioarea_c = 0.01; 
     ioarea_k0 = 0.35; 
     ioarea_k1 = 0.00008; 
     ioarea_k2 = 0.000000035; 
     ioarea_k3 = 0.000000000010; 

     // Timing parameters (ps)
     t_ds = 30;  
     t_is = 60; 
     t_dh = 30;
     t_ih = 60;
     t_dcd_soc = 20; 
     t_dcd_dram = 20;
     t_error_soc = 15; 
     t_skew_setup = 15; 
     t_skew_hold = 15; 
     t_dqsq = 50; 
     t_soc_setup = 20; 
     t_soc_hold = 10; 
     t_jitter_setup = 30; 
     t_jitter_hold = 30; 
     t_jitter_addr_setup = 60;
     t_jitter_addr_hold = 60;
	t_cor_margin = 10;


     //External IO Configuration Parameters 

     r_diff_term = 100;
     rtt1_dq_read = g_ip->rtt_value; 
     rtt2_dq_read = g_ip->rtt_value; 
     rtt1_dq_write = g_ip->rtt_value;
     rtt2_dq_write = g_ip->rtt_value; 
     rtt_ca = 50; 
     rs1_dq = 15; 
     rs2_dq = 15; 
     r_stub_ca = 0; 
     r_on = g_ip->ron_value; 
     r_on_ca = 50; 
     z0 = 50;
     t_flight = g_ip->tflight_value;
     t_flight_ca = 2;

     // Voltage noise coeffecients

     k_noise_write = 0.2; 
     k_noise_read = 0.2; 
     k_noise_addr = 0.2; 
     v_noise_independent_write = 0.1; 
     v_noise_independent_read = 0.1; 
     v_noise_independent_addr = 0.1; 

     //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

     /* This is a user-defined section that depends on the channel sensitivity
      * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
      * parameters that are impacted based on the channel analysis. The user
      * can define any relationship between the termination, loading and
      * configuration parameters AND the t_jitter/k_noise parameters. E.g. a
      * linear relationship, a non-linear analytical relationship or a lookup
      * table. The sensitivity coefficients are based on channel analysis
      * performed on the channel of interest.Given below is an example of such
      * a sensitivity relationship.
      * Such a linear fit can be found efficiently using an orthogonal design 
      * of experiments method shown in the technical report (), in Chapter 2.2. */

     k_noise_write_sen = k_noise_write * (1 + 0.1*(rtt1_dq_write/60 - 1) +
         0.2*(rtt2_dq_write/60 - 1) + 0.2*(r_on/34 - 1) +
         0.2*(g_ip->num_mem_dq/2 - 1));

     k_noise_read_sen = k_noise_read * (1 + 0.1*(rtt1_dq_read/60 - 1) +
         0.2*(rtt2_dq_read/60 - 1) + 0.2*(r_on/34 - 1) +
         0.2*(g_ip->num_mem_dq/2 - 1));

     k_noise_addr_sen = k_noise_addr * (1 + 0.1*(rtt_ca/50 - 1) +
         0.2*(r_on/34 - 1) + 0.2*(num_mem_ca/16 - 1));


     t_jitter_setup_sen = t_jitter_setup * (1 + 0.2*(rtt1_dq_write/60 - 1) + 
         0.3*(rtt2_dq_write/60 - 1) + 0.1*(r_on/34 - 1) + 
         0.3*(g_ip->num_mem_dq/2 - 1));

     t_jitter_hold_sen = t_jitter_hold * (1 + 0.2*(rtt1_dq_write/60 - 1) + 
         0.3*(rtt2_dq_write/60 - 1) + 
         0.1*(r_on/34 - 1) + 0.3*(g_ip->num_mem_dq/2 - 1));

     t_jitter_addr_setup_sen = t_jitter_addr_setup * (1 + 0.2*(rtt_ca/50 - 1) + 
         0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

     t_jitter_addr_hold_sen = t_jitter_addr_hold * (1 + 0.2*(rtt_ca/50 - 1) + 
         0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

     // PHY Static Power Coefficients (mW)
     phy_datapath_s = 0; 
     phy_phase_rotator_s = 10; 
     phy_clock_tree_s = 0; 
     phy_rx_s = 10; 
     phy_dcc_s = 0; 
     phy_deskew_s = 0; 
     phy_leveling_s = 0; 
     phy_pll_s = 10; 

     // PHY Dynamic Power Coefficients (mW/Gbps)
     phy_datapath_d = 0.5; 
     phy_phase_rotator_d = 0.01; 
     phy_clock_tree_d = 0.5; 
     phy_rx_d = 0.5; 
     phy_dcc_d = 0.05; 
     phy_deskew_d = 0.1; 
     phy_leveling_d = 0.05; 
     phy_pll_d = 0.05; 

	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 5;
	phy_rx_wtime = 2;
	phy_bandgap_wtime = 10;
	phy_deskew_wtime = 0.003;
	phy_vrefgen_wtime = 0.5;


   }
   else if (g_ip->io_type == Serial)
   { //Default parameters for Serial
     // IO Supply voltage (V) 
     vdd_io = 1.2;
     v_sw_clk =  0.75;

     // IO Area coefficients
     ioarea_c = 0.01; 
     ioarea_k0 = 0.15; 
     ioarea_k1 = 0.00005; 
     ioarea_k2 = 0.000000025; 
     ioarea_k3 = 0.000000000005; 

     // Timing parameters (ps)
     t_ds = 15;  
     t_dh = 15;
     t_dcd_soc = 10; 
     t_dcd_dram = 10; 
     t_soc_setup = 10; 
     t_soc_hold = 10; 
     t_jitter_setup = 20; 
     t_jitter_hold = 20;

     //External IO Configuration Parameters 

     r_diff_term = 100;


     t_jitter_setup_sen = t_jitter_setup;

     t_jitter_hold_sen = t_jitter_hold;

     t_jitter_addr_setup_sen = t_jitter_addr_setup;

     t_jitter_addr_hold_sen = t_jitter_addr_hold;

     // PHY Static Power Coefficients (mW)
     phy_datapath_s = 0; 
     phy_phase_rotator_s = 10; 
     phy_clock_tree_s = 0; 
     phy_rx_s = 10; 
     phy_dcc_s = 0; 
     phy_deskew_s = 0; 
     phy_leveling_s = 0; 
     phy_pll_s = 10; 

     // PHY Dynamic Power Coefficients (mW/Gbps)
     phy_datapath_d = 0.5; 
     phy_phase_rotator_d = 0.01; 
     phy_clock_tree_d = 0.5; 
     phy_rx_d = 0.5; 
     phy_dcc_d = 0.05; 
     phy_deskew_d = 0.1; 
     phy_leveling_d = 0.05; 
     phy_pll_d = 0.05; 

	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 5;
	phy_rx_wtime = 2;
	phy_bandgap_wtime = 10;
	phy_deskew_wtime = 0.003;
	phy_vrefgen_wtime = 0.5;


   }
	else
	{
		cout << "Not Yet supported" << endl;
		exit(1);
	}


   //SWING AND TERMINATION CALCULATIONS

   //R|| calculation
   rpar_write =(rtt1_dq_write + rs1_dq)*(rtt2_dq_write + rs2_dq)/
        (rtt1_dq_write + rs1_dq + rtt2_dq_write + rs2_dq);
   rpar_read =(rtt1_dq_read)*(rtt2_dq_read + rs2_dq)/
        (rtt1_dq_read + rtt2_dq_read + rs2_dq);

   //Swing calculation
   v_sw_data_read_load1 =vdd_io * (rtt1_dq_read)*(rtt2_dq_read + rs2_dq) / 
      ((rtt1_dq_read + rtt2_dq_read + rs2_dq)*(r_on + rs1_dq + rpar_read)); 
   v_sw_data_read_load2 =vdd_io * (rtt1_dq_read)*(rtt2_dq_read) / 
      ((rtt1_dq_read + rtt2_dq_read + rs2_dq)*(r_on + rs1_dq + rpar_read)); 
   v_sw_data_read_line =vdd_io * rpar_read / (r_on + rs1_dq + rpar_read); 
   v_sw_addr =vdd_io * rtt_ca / (50 + rtt_ca); 
   v_sw_data_write_load1 =vdd_io * (rtt1_dq_write)*(rtt2_dq_write + rs2_dq) / 
      ((rtt1_dq_write + rs1_dq + rtt2_dq_write + rs2_dq)*(r_on + rpar_write)); 
   v_sw_data_write_load2 =vdd_io * (rtt2_dq_write)*(rtt1_dq_write + rs1_dq) / 
      ((rtt1_dq_write + rs1_dq + rtt2_dq_write + rs2_dq)*(r_on + rpar_write));
   v_sw_data_write_line =vdd_io * rpar_write / (r_on + rpar_write); 

}

// This constructor recieves most of the input from g_ip.
// however it is possible to customize other some of the paremeters,
// that are mentioned as inputs.
// connection: 0 bob-dimm, 1 host-dimm, 2 lrdimm


IOTechParam::IOTechParam(InputParameter * g_ip, Mem_IO_type io_type1, int num_mem_dq, int mem_data_width
						, int num_dq, int connection, int num_loads, double freq) 
{
  num_mem_ca  = num_mem_dq * (mem_data_width); 
  num_mem_clk =  num_mem_dq *
                (num_dq/mem_data_width)/(g_ip->num_clk/2); 

  io_type = io_type1;
  frequency = freq;
  
  
  

  if (io_type == LPDDR2) { //LPDDR
    //Technology Parameters

    vdd_io = 1.2;
    v_sw_clk =  1;

    // Loading paramters
    c_int = 1.5; 
    c_tx = 2;  
    c_data = 1.5;
    c_addr = 0.75;
    i_bias =  5; 
    i_leak = 1000; 

    // IO Area coefficients

    ioarea_c = 0.01; 
    ioarea_k0 = 0.5; 
    ioarea_k1 = 0.00008; 
    ioarea_k2 = 0.000000030; 
    ioarea_k3 = 0.000000000008; 

    // Timing parameters (ps)
    t_ds = 250; 
    t_is = 250;
    t_dh = 250;
    t_ih = 250; 
    t_dcd_soc = 50;
    t_dcd_dram = 50; 
    t_error_soc = 50;
    t_skew_setup = 50;
    t_skew_hold = 50;
    t_dqsq = 250; 
    t_soc_setup = 50;
    t_soc_hold = 50;
    t_jitter_setup = 200; 
    t_jitter_hold = 200; 
    t_jitter_addr_setup = 200;
    t_jitter_addr_hold = 200;
    t_cor_margin = 40; 

    //External IO Configuration Parameters 

    r_diff_term = 480;
    rtt1_dq_read = 100000; 
    rtt2_dq_read = 100000; 
    rtt1_dq_write = 100000; 
    rtt2_dq_write = 100000; 
    rtt_ca = 240; 
    rs1_dq = 0; 
    rs2_dq = 0; 
    r_stub_ca = 0; 
    r_on = 50; 
    r_on_ca = 50;
    z0 = 50;
    t_flight = 0.5;
    t_flight_ca = 0.5;

    // Voltage noise coeffecients
    k_noise_write = 0.2; 
    k_noise_read = 0.2; 
    k_noise_addr = 0.2; 
    v_noise_independent_write = 0.1; 
    v_noise_independent_read = 0.1; 
    v_noise_independent_addr = 0.1; 

    //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

/* This is a user-defined section that depends on the channel sensitivity
      * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
      * parameters that are impacted based on the channel analysis. The user
      * can define any relationship between the termination, loading and
      * configuration parameters AND the t_jitter/k_noise parameters. E.g. a
      * linear relationship, a non-linear analytical relationship or a lookup
      * table. The sensitivity coefficients are based on channel analysis
      * performed on the channel of interest.Given below is an example of such
      * a sensitivity relationship.
      * Such a linear fit can be found efficiently using an orthogonal design 
      * of experiments method shown in the technical report (), in Chapter 2.2. */

    k_noise_write_sen = k_noise_write * (1 + 0.2*(r_on/34 - 1) +
        0.2*(num_mem_dq/2 - 1));
    k_noise_read_sen = k_noise_read * (1 + 0.2*(r_on/34 - 1) +
        0.2*(num_mem_dq/2 - 1));
    k_noise_addr_sen = k_noise_addr * (1 + 0.1*(rtt_ca/100 - 1) +
        0.2*(r_on/34 - 1) + 0.2*(num_mem_ca/16 - 1));

    t_jitter_setup_sen = t_jitter_setup * (1  + 0.1*(r_on/34 - 1) +
        0.3*(num_mem_dq/2 - 1));
    t_jitter_hold_sen = t_jitter_hold * (1 + 0.1*(r_on/34 - 1) +
        0.3*(num_mem_dq/2 - 1));
    t_jitter_addr_setup_sen = t_jitter_addr_setup * (1 + 0.2*(rtt_ca/100 - 1) +
        0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));
    t_jitter_addr_hold_sen = t_jitter_addr_hold * (1 + 0.2*(rtt_ca/100 - 1) +
        0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

    // PHY Static Power Coefficients (mW)

    phy_datapath_s = 0; 
    phy_phase_rotator_s = 5; 
    phy_clock_tree_s = 0; 
    phy_rx_s = 3; 
    phy_dcc_s = 0; 
    phy_deskew_s = 0; 
    phy_leveling_s = 0;
    phy_pll_s = 2;


    // PHY Dynamic Power Coefficients (mW/Gbps)

    phy_datapath_d = 0.3;
    phy_phase_rotator_d = 0.01;
    phy_clock_tree_d = 0.4;
    phy_rx_d = 0.2;
    phy_dcc_d = 0;
    phy_deskew_d = 0;
    phy_leveling_d = 0;
    phy_pll_d = 0.05;


	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 5;
	phy_rx_wtime = 2;
	phy_bandgap_wtime = 10;
	phy_deskew_wtime = 0;
	phy_vrefgen_wtime = 0;


  }
   else if (io_type == WideIO) { //WIDEIO 
     //Technology Parameters
     vdd_io = 1.2;
     v_sw_clk =  1.2;

     // Loading parameters
     c_int = 0.5; 
     c_tx = 0.5;  
     c_data = 0.5;  
     c_addr = 0.35; 
     i_bias =  0; 
     i_leak = 500; 

     // IO Area coefficients
     ioarea_c = 0.003; 
     ioarea_k0 = 0.2; 
     ioarea_k1 = 0.00004; 
     ioarea_k2 = 0.000000020; 
     ioarea_k3 = 0.000000000004; 

     // Timing parameters (ps)
     t_ds = 250; 
     t_is = 250;  
     t_dh = 250; 
     t_ih = 250; 
     t_dcd_soc = 50;  
     t_dcd_dram = 50; 
     t_error_soc = 50; 
     t_skew_setup = 50;  
     t_skew_hold = 50;  
     t_dqsq = 250;  
     t_soc_setup = 50; 
     t_soc_hold = 50; 
     t_jitter_setup = 200; 
     t_jitter_hold = 200; 
     t_jitter_addr_setup = 200; 
     t_jitter_addr_hold = 200;
	t_cor_margin = 50; 

     //External IO Configuration Parameters 

     r_diff_term = 100000;
     rtt1_dq_read = 100000; 
     rtt2_dq_read = 100000; 
     rtt1_dq_write = 100000; 
     rtt2_dq_write = 100000; 
     rtt_ca = 100000; 
     rs1_dq = 0; 
     rs2_dq = 0; 
     r_stub_ca = 0; 
     r_on = 75; 
     r_on_ca = 75; 
     z0 = 50;
     t_flight = 0.05;
     t_flight_ca = 0.05;

     // Voltage noise coeffecients
     k_noise_write = 0.2; 
     k_noise_read = 0.2; 
     k_noise_addr = 0.2; 
     v_noise_independent_write = 0.1; 
     v_noise_independent_read = 0.1; 
     v_noise_independent_addr = 0.1; 

     //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

     /* This is a user-defined section that depends on the channel sensitivity
      * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
      * parameters that are impacted based on the channel analysis. The user
      * can define any relationship between the termination, loading and
      * configuration parameters AND the t_jitter/k_noise parameters. E.g. a
      * linear relationship, a non-linear analytical relationship or a lookup
      * table. The sensitivity coefficients are based on channel analysis
      * performed on the channel of interest.Given below is an example of such
      * a sensitivity relationship.
      * Such a linear fit can be found efficiently using an orthogonal design 
      * of experiments method shown in the technical report (), in Chapter 2.2. */

     k_noise_write_sen = k_noise_write * (1 + 0.2*(r_on/50 - 1) + 
         0.2*(num_mem_dq/2 - 1));
     k_noise_read_sen = k_noise_read * (1 + 0.2*(r_on/50 - 1) + 
         0.2*(num_mem_dq/2 - 1));
     k_noise_addr_sen = k_noise_addr * (1 + 0.2*(r_on/50 - 1) + 
         0.2*(num_mem_ca/16 - 1));


     t_jitter_setup_sen = t_jitter_setup * (1  + 0.1*(r_on/50 - 1) + 
         0.3*(num_mem_dq/2 - 1));
     t_jitter_hold_sen = t_jitter_hold * (1 + 0.1*(r_on/50 - 1) + 
         0.3*(num_mem_dq/2 - 1));
     t_jitter_addr_setup_sen = t_jitter_addr_setup * (1 + 0.1*(r_on/50 - 1) + 
         0.4*(num_mem_ca/16 - 1));
     t_jitter_addr_hold_sen = t_jitter_addr_hold * (1 + 0.1*(r_on/50 - 1) + 
         0.4*(num_mem_ca/16 - 1));

     // PHY Static Power Coefficients (mW)
     phy_datapath_s = 0;
     phy_phase_rotator_s = 1;
     phy_clock_tree_s = 0;
     phy_rx_s = 0;
     phy_dcc_s = 0;
     phy_deskew_s = 0;
     phy_leveling_s = 0;
     phy_pll_s = 0;


     // PHY Dynamic Power Coefficients (mW/Gbps)
     phy_datapath_d = 0.3;
     phy_phase_rotator_d = 0.01;
     phy_clock_tree_d = 0.2;
     phy_rx_d = 0.1;
     phy_dcc_d = 0;
     phy_deskew_d = 0;
     phy_leveling_d = 0;
     phy_pll_d = 0;

	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 0;
	phy_rx_wtime = 0;
	phy_bandgap_wtime = 0;
	phy_deskew_wtime = 0;
	phy_vrefgen_wtime = 0;


   }
   else if (io_type == DDR3)
   { //Default parameters for DDR3
     // IO Supply voltage (V) 
     vdd_io = 1.5;
     v_sw_clk =  0.75;

     // Loading parameters
     c_int = 1.5; 
     c_tx = 2;  
     c_data = 1.5; 
     c_addr = 0.75; 
     i_bias =  15; 
     i_leak = 1000; 

     // IO Area coefficients
     ioarea_c = 0.01; 
     ioarea_k0 = 0.5; 
     ioarea_k1 = 0.00015; 
     ioarea_k2 = 0.000000045; 
     ioarea_k3 = 0.000000000015; 

     // Timing parameters (ps)
     t_ds = 150;  
     t_is = 150; 
     t_dh = 150;
     t_ih = 150;
     t_dcd_soc = 50; 
     t_dcd_dram = 50;
     t_error_soc = 25; 
     t_skew_setup = 25; 
     t_skew_hold = 25; 
     t_dqsq = 100; 
     t_soc_setup = 50; 
     t_soc_hold = 50; 
     t_jitter_setup = 100; 
     t_jitter_hold = 100; 
     t_jitter_addr_setup = 100;
     t_jitter_addr_hold = 100;
	t_cor_margin = 30;


     //External IO Configuration Parameters 

     r_diff_term = 100;
     
     /*
     rtt1_dq_read = g_ip->rtt_value; 
     rtt2_dq_read = g_ip->rtt_value; 
     rtt1_dq_write = g_ip->rtt_value;
     rtt2_dq_write = g_ip->rtt_value;
     */
     switch(connection)
     {
		 case(0):
			rtt1_dq_write = rtt1_wr_bob_dimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_write = rtt2_wr_bob_dimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt1_dq_read = rtt1_rd_bob_dimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_read = rtt2_rd_bob_dimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			break;
		 case(1):
			rtt1_dq_write = rtt1_wr_host_dimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_write = rtt2_wr_host_dimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt1_dq_read = rtt1_rd_host_dimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_read = rtt2_rd_host_dimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			break;
		 case(2):
			rtt1_dq_write = rtt1_wr_lrdimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_write = rtt2_wr_lrdimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt1_dq_read = rtt1_rd_lrdimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_read = rtt2_rd_lrdimm_ddr3[num_loads-1][frequnecy_index(io_type)];
			break;
		default:
			break;
	 }
     
      
     rtt_ca = 50; 
     rs1_dq = 15; 
     rs2_dq = 15; 
     r_stub_ca = 0; 
     r_on = g_ip->ron_value; 
     r_on_ca = 50; 
     z0 = 50;
     t_flight = g_ip->tflight_value;
     t_flight_ca = 2;

     // Voltage noise coeffecients

     k_noise_write = 0.2; 
     k_noise_read = 0.2; 
     k_noise_addr = 0.2; 
     v_noise_independent_write = 0.1; 
     v_noise_independent_read = 0.1; 
     v_noise_independent_addr = 0.1; 

     //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

     /* This is a user-defined section that depends on the channel sensitivity
      * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
      * parameters that are impacted based on the channel analysis. The user
      * can define any relationship between the termination, loading and
      * configuration parameters AND the t_jitter/k_noise parameters. E.g. a
      * linear relationship, a non-linear analytical relationship or a lookup
      * table. The sensitivity coefficients are based on channel analysis
      * performed on the channel of interest.Given below is an example of such
      * a sensitivity relationship.
      * Such a linear fit can be found efficiently using an orthogonal design 
      * of experiments method shown in the technical report (), in Chapter 2.2. */

     k_noise_write_sen = k_noise_write * (1 + 0.1*(rtt1_dq_write/60 - 1) +
         0.2*(rtt2_dq_write/60 - 1) + 0.2*(r_on/34 - 1) +
         0.2*(num_mem_dq/2 - 1));

     k_noise_read_sen = k_noise_read * (1 + 0.1*(rtt1_dq_read/60 - 1) +
         0.2*(rtt2_dq_read/60 - 1) + 0.2*(r_on/34 - 1) +
         0.2*(num_mem_dq/2 - 1));

     k_noise_addr_sen = k_noise_addr * (1 + 0.1*(rtt_ca/50 - 1) +
         0.2*(r_on/34 - 1) + 0.2*(num_mem_ca/16 - 1));


     t_jitter_setup_sen = t_jitter_setup * (1 + 0.2*(rtt1_dq_write/60 - 1) + 
         0.3*(rtt2_dq_write/60 - 1) + 0.1*(r_on/34 - 1) + 
         0.3*(num_mem_dq/2 - 1));

     t_jitter_hold_sen = t_jitter_hold * (1 + 0.2*(rtt1_dq_write/60 - 1) + 
         0.3*(rtt2_dq_write/60 - 1) + 
         0.1*(r_on/34 - 1) + 0.3*(num_mem_dq/2 - 1));

     t_jitter_addr_setup_sen = t_jitter_addr_setup * (1 + 0.2*(rtt_ca/50 - 1) + 
         0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

     t_jitter_addr_hold_sen = t_jitter_addr_hold * (1 + 0.2*(rtt_ca/50 - 1) + 
         0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

     // PHY Static Power Coefficients (mW)
     phy_datapath_s = 0; 
     phy_phase_rotator_s = 10; 
     phy_clock_tree_s = 0; 
     phy_rx_s = 10; 
     phy_dcc_s = 0; 
     phy_deskew_s = 0; 
     phy_leveling_s = 0; 
     phy_pll_s = 10; 

     // PHY Dynamic Power Coefficients (mW/Gbps)
     phy_datapath_d = 0.5; 
     phy_phase_rotator_d = 0.01; 
     phy_clock_tree_d = 0.5; 
     phy_rx_d = 0.5; 
     phy_dcc_d = 0.05; 
     phy_deskew_d = 0.1; 
     phy_leveling_d = 0.05; 
     phy_pll_d = 0.05; 

	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 5;
	phy_rx_wtime = 2;
	phy_bandgap_wtime = 10;
	phy_deskew_wtime = 0.003;
	phy_vrefgen_wtime = 0.5;


   }
   else if (io_type == DDR4)
   { //Default parameters for DDR4
     // IO Supply voltage (V) 
     vdd_io = 1.2;
     v_sw_clk =  0.6;

     // Loading parameters
     c_int = 1.5; 
     c_tx = 2;  
     c_data = 1; 
     c_addr = 0.75; 
     i_bias =  15; 
     i_leak = 1000; 

     // IO Area coefficients
     ioarea_c = 0.01; 
     ioarea_k0 = 0.35; 
     ioarea_k1 = 0.00008; 
     ioarea_k2 = 0.000000035; 
     ioarea_k3 = 0.000000000010; 

     // Timing parameters (ps)
     t_ds = 30;  
     t_is = 60; 
     t_dh = 30;
     t_ih = 60;
     t_dcd_soc = 20; 
     t_dcd_dram = 20;
     t_error_soc = 15; 
     t_skew_setup = 15; 
     t_skew_hold = 15; 
     t_dqsq = 50; 
     t_soc_setup = 20; 
     t_soc_hold = 10; 
     t_jitter_setup = 30; 
     t_jitter_hold = 30; 
     t_jitter_addr_setup = 60;
     t_jitter_addr_hold = 60;
	t_cor_margin = 10;


     //External IO Configuration Parameters 

     r_diff_term = 100;
     /*
     rtt1_dq_read = g_ip->rtt_value; 
     rtt2_dq_read = g_ip->rtt_value; 
     rtt1_dq_write = g_ip->rtt_value;
     rtt2_dq_write = g_ip->rtt_value; 
     */
     
     switch(connection)
     {
		 case(0):
			rtt1_dq_write = rtt1_wr_bob_dimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_write = rtt2_wr_bob_dimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt1_dq_read = rtt1_rd_bob_dimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_read = rtt2_rd_bob_dimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			break;
		 case(1):
			rtt1_dq_write = rtt1_wr_host_dimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_write = rtt2_wr_host_dimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt1_dq_read = rtt1_rd_host_dimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_read = rtt2_rd_host_dimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			break;
		 case(2):
			rtt1_dq_write = rtt1_wr_lrdimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_write = rtt2_wr_lrdimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt1_dq_read = rtt1_rd_lrdimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			rtt2_dq_read = rtt2_rd_lrdimm_ddr4[num_loads-1][frequnecy_index(io_type)];
			break;
		default:
			break;
	 }
     
     rtt_ca = 50; 
     rs1_dq = 15; 
     rs2_dq = 15; 
     r_stub_ca = 0; 
     r_on = g_ip->ron_value; 
     r_on_ca = 50; 
     z0 = 50;
     t_flight = g_ip->tflight_value;
     t_flight_ca = 2;

     // Voltage noise coeffecients

     k_noise_write = 0.2; 
     k_noise_read = 0.2; 
     k_noise_addr = 0.2; 
     v_noise_independent_write = 0.1; 
     v_noise_independent_read = 0.1; 
     v_noise_independent_addr = 0.1; 

     //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

     /* This is a user-defined section that depends on the channel sensitivity
      * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
      * parameters that are impacted based on the channel analysis. The user
      * can define any relationship between the termination, loading and
      * configuration parameters AND the t_jitter/k_noise parameters. E.g. a
      * linear relationship, a non-linear analytical relationship or a lookup
      * table. The sensitivity coefficients are based on channel analysis
      * performed on the channel of interest.Given below is an example of such
      * a sensitivity relationship.
      * Such a linear fit can be found efficiently using an orthogonal design 
      * of experiments method shown in the technical report (), in Chapter 2.2. */

     k_noise_write_sen = k_noise_write * (1 + 0.1*(rtt1_dq_write/60 - 1) +
         0.2*(rtt2_dq_write/60 - 1) + 0.2*(r_on/34 - 1) +
         0.2*(num_mem_dq/2 - 1));

     k_noise_read_sen = k_noise_read * (1 + 0.1*(rtt1_dq_read/60 - 1) +
         0.2*(rtt2_dq_read/60 - 1) + 0.2*(r_on/34 - 1) +
         0.2*(num_mem_dq/2 - 1));

     k_noise_addr_sen = k_noise_addr * (1 + 0.1*(rtt_ca/50 - 1) +
         0.2*(r_on/34 - 1) + 0.2*(num_mem_ca/16 - 1));


     t_jitter_setup_sen = t_jitter_setup * (1 + 0.2*(rtt1_dq_write/60 - 1) + 
         0.3*(rtt2_dq_write/60 - 1) + 0.1*(r_on/34 - 1) + 
         0.3*(num_mem_dq/2 - 1));

     t_jitter_hold_sen = t_jitter_hold * (1 + 0.2*(rtt1_dq_write/60 - 1) + 
         0.3*(rtt2_dq_write/60 - 1) + 
         0.1*(r_on/34 - 1) + 0.3*(num_mem_dq/2 - 1));

     t_jitter_addr_setup_sen = t_jitter_addr_setup * (1 + 0.2*(rtt_ca/50 - 1) + 
         0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

     t_jitter_addr_hold_sen = t_jitter_addr_hold * (1 + 0.2*(rtt_ca/50 - 1) + 
         0.1*(r_on/34 - 1) + 0.4*(num_mem_ca/16 - 1));

     // PHY Static Power Coefficients (mW)
     phy_datapath_s = 0; 
     phy_phase_rotator_s = 10; 
     phy_clock_tree_s = 0; 
     phy_rx_s = 10; 
     phy_dcc_s = 0; 
     phy_deskew_s = 0; 
     phy_leveling_s = 0; 
     phy_pll_s = 10; 

     // PHY Dynamic Power Coefficients (mW/Gbps)
     phy_datapath_d = 0.5; 
     phy_phase_rotator_d = 0.01; 
     phy_clock_tree_d = 0.5; 
     phy_rx_d = 0.5; 
     phy_dcc_d = 0.05; 
     phy_deskew_d = 0.1; 
     phy_leveling_d = 0.05; 
     phy_pll_d = 0.05; 

	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 5;
	phy_rx_wtime = 2;
	phy_bandgap_wtime = 10;
	phy_deskew_wtime = 0.003;
	phy_vrefgen_wtime = 0.5;


   }
   else if (io_type == Serial)
   { //Default parameters for Serial
     // IO Supply voltage (V) 
     vdd_io = 1.2;
     v_sw_clk =  0.75;

     // IO Area coefficients
     ioarea_c = 0.01; 
     ioarea_k0 = 0.15; 
     ioarea_k1 = 0.00005; 
     ioarea_k2 = 0.000000025; 
     ioarea_k3 = 0.000000000005; 

     // Timing parameters (ps)
     t_ds = 15;  
     t_dh = 15;
     t_dcd_soc = 10; 
     t_dcd_dram = 10; 
     t_soc_setup = 10; 
     t_soc_hold = 10; 
     t_jitter_setup = 20; 
     t_jitter_hold = 20;

     //External IO Configuration Parameters 

     r_diff_term = 100;


     t_jitter_setup_sen = t_jitter_setup;

     t_jitter_hold_sen = t_jitter_hold;

     t_jitter_addr_setup_sen = t_jitter_addr_setup;

     t_jitter_addr_hold_sen = t_jitter_addr_hold;

     // PHY Static Power Coefficients (mW)
     phy_datapath_s = 0; 
     phy_phase_rotator_s = 10; 
     phy_clock_tree_s = 0; 
     phy_rx_s = 10; 
     phy_dcc_s = 0; 
     phy_deskew_s = 0; 
     phy_leveling_s = 0; 
     phy_pll_s = 10; 

     // PHY Dynamic Power Coefficients (mW/Gbps)
     phy_datapath_d = 0.5; 
     phy_phase_rotator_d = 0.01; 
     phy_clock_tree_d = 0.5; 
     phy_rx_d = 0.5; 
     phy_dcc_d = 0.05; 
     phy_deskew_d = 0.1; 
     phy_leveling_d = 0.05; 
     phy_pll_d = 0.05; 

	//PHY Wakeup Times (Sleep to Active) (microseconds)

	phy_pll_wtime = 10;
	phy_phase_rotator_wtime = 5;
	phy_rx_wtime = 2;
	phy_bandgap_wtime = 10;
	phy_deskew_wtime = 0.003;
	phy_vrefgen_wtime = 0.5;


   }
	else
	{
		cout << "Not Yet supported" << endl;
		exit(1);
	}


   //SWING AND TERMINATION CALCULATIONS

   //R|| calculation
   rpar_write =(rtt1_dq_write + rs1_dq)*(rtt2_dq_write + rs2_dq)/
        (rtt1_dq_write + rs1_dq + rtt2_dq_write + rs2_dq);
   rpar_read =(rtt1_dq_read)*(rtt2_dq_read + rs2_dq)/
        (rtt1_dq_read + rtt2_dq_read + rs2_dq);

	

   //Swing calculation
   v_sw_data_read_load1 =vdd_io * (rtt1_dq_read)*(rtt2_dq_read + rs2_dq) / 
      ((rtt1_dq_read + rtt2_dq_read + rs2_dq)*(r_on + rs1_dq + rpar_read)); 
   v_sw_data_read_load2 =vdd_io * (rtt1_dq_read)*(rtt2_dq_read) / 
      ((rtt1_dq_read + rtt2_dq_read + rs2_dq)*(r_on + rs1_dq + rpar_read)); 
   v_sw_data_read_line =vdd_io * rpar_read / (r_on + rs1_dq + rpar_read); 
   v_sw_addr =vdd_io * rtt_ca / (50 + rtt_ca); 
   v_sw_data_write_load1 =vdd_io * (rtt1_dq_write)*(rtt2_dq_write + rs2_dq) / 
      ((rtt1_dq_write + rs1_dq + rtt2_dq_write + rs2_dq)*(r_on + rpar_write)); 
   v_sw_data_write_load2 =vdd_io * (rtt2_dq_write)*(rtt1_dq_write + rs1_dq) / 
      ((rtt1_dq_write + rs1_dq + rtt2_dq_write + rs2_dq)*(r_on + rpar_write));
   v_sw_data_write_line =vdd_io * rpar_write / (r_on + rpar_write); 

}



IOTechParam::~IOTechParam() 
{}
