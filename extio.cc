#include "extio.h"
#include <cassert>


Extio::Extio(IOTechParam *iot):
io_param(iot){}


//External IO AREA. Does not include PHY or decap, includes only IO active circuit. More details can be found in the CACTI-IO technical report (), Chapter 2.3.

void Extio::extio_area()
{

	//Area per IO, assuming drive stage and ODT are shared
	double single_io_area = io_param->ioarea_c + 
         (io_param->ioarea_k0/io_param->r_on)+(1/io_param->r_on)*
            (io_param->ioarea_k1*io_param->frequency + 
            io_param->ioarea_k2*io_param->frequency*io_param->frequency + 
            io_param->ioarea_k3*io_param->frequency*
            io_param->frequency*io_param->frequency); // IO Area in sq.mm.

  //Area per IO if ODT requirements are more stringent than the Ron
  //requirements in determining size of driver
	if (2*io_param->rtt1_dq_read < io_param->r_on) {
	  single_io_area = io_param->ioarea_c + 
    (io_param->ioarea_k0/(2*io_param->rtt1_dq_read))+
    (1/io_param->r_on)*(io_param->ioarea_k1*io_param->frequency + 
    io_param->ioarea_k2*io_param->frequency*io_param->frequency + 
    io_param->ioarea_k3*io_param->frequency*io_param->frequency*io_param->frequency);
	}

	//Total IO area
	io_area = (g_ip->num_dq + g_ip->num_dqs + g_ip->num_ca + g_ip->num_clk) *
            single_io_area;
	
	printf("IO Area (sq.mm) = ");
	cout << io_area << endl;

}

//External IO Termination Power. More details can be found in the CACTI-IO technical report (), Chapter 2.1.

void Extio::extio_power_term()
{

	//IO Termination and Bias Power

  //Bias and Leakage Power
	power_bias = io_param->i_bias * io_param->vdd_io + 
    io_param->i_leak * (g_ip->num_dq + 
        g_ip->num_dqs + 
        g_ip->num_clk + 
        g_ip->num_ca) * io_param->vdd_io/1000000;
	

	//Termination Power
	power_termination_read = 1000 * (g_ip->num_dq + g_ip->num_dqs) * 
        io_param->vdd_io * io_param->vdd_io * 0.25 * 
        (1/(io_param->r_on + io_param->rpar_read + io_param->rs1_dq) + 
          1/(io_param->rtt1_dq_read) + 1/(io_param->rtt2_dq_read)) + 
        1000 * g_ip->num_ca * io_param->vdd_io * io_param->vdd_io * 
        (0.5 / (2 * (io_param->r_on_ca + io_param->rtt_ca)));

	power_termination_write = 1000 * (g_ip->num_dq + g_ip->num_dqs) * 
    io_param->vdd_io * io_param->vdd_io * 0.25 * 
    (1/(io_param->r_on + io_param->rpar_write) + 
      1/(io_param->rtt1_dq_write) + 1/(io_param->rtt2_dq_write)) + 
    1000 * g_ip->num_ca * io_param->vdd_io * io_param->vdd_io * 
    (0.5 / (2 * (io_param->r_on_ca + io_param->rtt_ca)));

   power_clk_bias = io_param->vdd_io * io_param->v_sw_clk / io_param->r_diff_term * 1000;

	
   if (io_param->io_type == Serial)
   { power_termination_read= 1000*(g_ip->num_dq)*io_param->vdd_io*io_param->v_sw_clk/io_param->r_diff_term;
	power_termination_write= 1000*(g_ip->num_dq)*io_param->vdd_io*io_param->v_sw_clk/io_param->r_diff_term;
	power_clk_bias=0;
   }

   if (io_param->io_type == DDR4)
   { 
	   power_termination_read=1000 * (g_ip->num_dq + g_ip->num_dqs) * 
        io_param->vdd_io * io_param->vdd_io *0.5 * (1/(io_param->r_on + io_param->rpar_read + io_param->rs1_dq))
		+ 1000 * g_ip->num_ca * io_param->vdd_io * io_param->vdd_io * 
        (0.5 / (2 * (io_param->r_on_ca + io_param->rtt_ca)));
	
	
 
		power_termination_write = 1000 * (g_ip->num_dq + g_ip->num_dqs) * 
		io_param->vdd_io * io_param->vdd_io * 0.5 * 
		(1/(io_param->r_on + io_param->rpar_write)) + 
		1000 * g_ip->num_ca * io_param->vdd_io * io_param->vdd_io * 
		(0.5 / (2 * (io_param->r_on_ca + io_param->rtt_ca)));
   
		
   
   }


	//Combining the power terms based on STATE (READ/WRITE/IDLE/SLEEP)
	if (g_ip->iostate == READ)
	  {
	    io_power_term = g_ip->duty_cycle * 
        (power_termination_read + power_bias + power_clk_bias);
	  }
	else if (g_ip->iostate == WRITE)
	  {
	    io_power_term = g_ip->duty_cycle * 
        (power_termination_write + power_bias + power_clk_bias);
	  }
	else if (g_ip->iostate == IDLE)
	  {
	    io_power_term = g_ip->duty_cycle * 
        (power_termination_write + power_bias + power_clk_bias);
		if (io_param->io_type == DDR4)
		{ io_power_term = 1e-6*io_param->i_leak*io_param->vdd_io; // IDLE IO power for DDR4 is leakage since bus can be parked at VDDQ
		}
	  }
	else if (g_ip->iostate == SLEEP)
	  {
	    io_power_term = 1e-6*io_param->i_leak*io_param->vdd_io; //nA to mW
	  }
	else
	  {
	    io_power_term = 0;
	  }


	printf("IO Termination and Bias Power (mW) = ");
	cout << io_power_term << endl;
}


//External PHY Power and Wakeup Times. More details can be found in the CACTI-IO technical report (), Chapter 2.1.

void Extio::extio_power_phy ()
{


  phy_static_power = io_param->phy_datapath_s + io_param->phy_phase_rotator_s +
      io_param->phy_clock_tree_s + io_param->phy_rx_s + io_param->phy_dcc_s + 
      io_param->phy_deskew_s + io_param->phy_leveling_s + io_param->phy_pll_s; // in mW

  phy_dynamic_power = io_param->phy_datapath_d + io_param->phy_phase_rotator_d + 
      io_param->phy_clock_tree_d + io_param->phy_rx_d + io_param->phy_dcc_d + 
      io_param->phy_deskew_d + io_param->phy_leveling_d + 
      io_param->phy_pll_d; // in mW/Gbps



//Combining the power terms based on STATE (READ/WRITE/IDLE/SLEEP)
	if (g_ip->iostate == READ)
	  {
	    phy_power = phy_static_power + 2 * io_param->frequency * g_ip->num_dq * phy_dynamic_power / 1000; // Total PHY power in mW
	  }
	else if (g_ip->iostate == WRITE)
	  {
	    phy_power = phy_static_power + 2 * io_param->frequency * g_ip->num_dq * phy_dynamic_power / 1000; // Total PHY power in mW
	  }
	else if (g_ip->iostate == IDLE)
	  {
	     phy_power = phy_static_power; // Total PHY power in mW

	  }
	else if (g_ip->iostate == SLEEP)
	  {
	    phy_power = 0; // Total PHY power in mW;
	  }
	else
	  {
	    phy_power = 0; // Total PHY power in mW;
	  }


  phy_wtime = io_param->phy_pll_wtime + io_param->phy_phase_rotator_wtime + io_param->phy_rx_wtime + io_param->phy_bandgap_wtime + io_param->phy_deskew_wtime + io_param->phy_vrefgen_wtime; // Total Wakeup time from SLEEP to ACTIVE. Some of the Wakeup time can be hidden if all components do not need to be serially brought out of SLEEP. This depends on the implementation and user can modify the Wakeup times accordingly. 


	printf("PHY Power (mW) = ");
	cout << phy_power << " ";
	printf("PHY Wakeup Time (us) = ");
	cout << phy_wtime << endl;

}


//External IO Dynamic Power. Does not include termination or PHY. More details can be found in the CACTI-IO technical report (), Chapter 2.1.

void Extio::extio_power_dynamic()
{

	if (io_param->io_type == Serial)
	{
	power_dq_write = 0;

	power_dqs_write = 0;

	power_ca_write = 0;

	power_dq_read = 0;

	power_dqs_read = 0;

	power_ca_read = 0;

	power_clk = 0;

	}
	else
	{


	//Line capacitance calculations for effective c_line

  double c_line =1e6/(io_param->z0*2*io_param->frequency); //For DDR signals: DQ, DQS, CLK
  double c_line_ca=c_line; //For DDR CA
	double c_line_sdr=1e6/(io_param->z0*io_param->frequency); //For SDR CA
	double c_line_2T=1e6*2/(io_param->z0*io_param->frequency); //For 2T timing
	double c_line_3T=1e6*3/(io_param->z0*io_param->frequency); //For 3T timing

	//Line capacitance if flight time is less than half the bit period
	
	if (io_param->t_flight < 1e3/(4*io_param->frequency)){
	  c_line = 1e3*io_param->t_flight/io_param->z0;
	}

	if (io_param->t_flight_ca < 1e3/(4*io_param->frequency)){
	  c_line_ca = 1e3*io_param->t_flight/io_param->z0;
	}

	if (io_param->t_flight_ca < 1e3/(2*io_param->frequency)){
	  c_line_sdr = 1e3*io_param->t_flight/io_param->z0;
	}

	if (io_param->t_flight_ca < 1e3*2/(2*io_param->frequency)){
	  c_line_2T = 1e3*io_param->t_flight/io_param->z0;
	}

	if (io_param->t_flight_ca < 1e3*3/(2*io_param->frequency)){
	  c_line_3T = 1e3*io_param->t_flight/io_param->z0;
	}

	//Line capacitance calculation for the address bus, depending on what address timing is chosen (DDR/SDR/2T/3T)

	if (g_ip->addr_timing==1.0) {
	  c_line_ca = c_line_sdr;
	}
	else if (g_ip->addr_timing==2.0){
	  c_line_ca = c_line_2T;
	}
	else if (g_ip->addr_timing==3.0){
	  c_line_ca = c_line_3T;
	}

	//Dynamic power per signal group for WRITE and READ modes

	power_dq_write = g_ip->num_dq * g_ip->activity_dq * 
          (io_param->c_tx +  c_line) * io_param->vdd_io * 
          io_param->v_sw_data_write_line * io_param->frequency / 1000 + 
        g_ip->num_dq * g_ip->activity_dq * io_param->c_data * 
          io_param->vdd_io * io_param->v_sw_data_write_load1 * 
          io_param->frequency / 1000 + 
        g_ip->num_dq * g_ip->activity_dq * ((g_ip->num_mem_dq-1) * 
          io_param->c_data) * io_param->vdd_io * 
          io_param->v_sw_data_write_load2 * io_param->frequency / 1000  + 
        g_ip->num_dq * g_ip->activity_dq * io_param->c_int * 
          io_param->vdd_io * io_param->vdd_io * io_param->frequency / 1000;

	power_dqs_write = g_ip->num_dqs * (io_param->c_tx +  c_line) * 
          io_param->vdd_io * io_param->v_sw_data_write_line * 
          io_param->frequency / 1000 + 
        g_ip->num_dqs * io_param->c_data * io_param->vdd_io * 
          io_param->v_sw_data_write_load1 * io_param->frequency / 1000 + 
        g_ip->num_dqs * ((g_ip->num_mem_dq-1) * io_param->c_data) * 
          io_param->vdd_io * io_param->v_sw_data_write_load2 *
          io_param->frequency / 1000  + 
        g_ip->num_dqs * io_param->c_int * io_param->vdd_io * 
          io_param->vdd_io * io_param->frequency / 1000;

	power_ca_write = g_ip->num_ca * g_ip->activity_ca * 
          (io_param->c_tx + io_param->num_mem_ca * io_param->c_addr + 
            c_line_ca) * 
          io_param->vdd_io * io_param->v_sw_addr * io_param->frequency / 1000 + 
        g_ip->num_ca * g_ip->activity_ca * io_param->c_int * 
          io_param->vdd_io * io_param->vdd_io * io_param->frequency / 1000;

	power_dq_read = g_ip->num_dq * g_ip->activity_dq * 
          (io_param->c_tx +  c_line) * io_param->vdd_io * 
          io_param->v_sw_data_read_line * io_param->frequency / 1000.0 + 
        g_ip->num_dq * g_ip->activity_dq * io_param->c_data * 
          io_param->vdd_io * io_param->v_sw_data_read_load1 * io_param->frequency / 1000.0 + 
        g_ip->num_dq *g_ip->activity_dq * ((g_ip->num_mem_dq-1) * io_param->c_data) * 
          io_param->vdd_io * io_param->v_sw_data_read_load2 * io_param->frequency / 1000.0  + 
        g_ip->num_dq * g_ip->activity_dq * io_param->c_int * io_param->vdd_io * 
          io_param->vdd_io * io_param->frequency / 1000.0;

	power_dqs_read = g_ip->num_dqs * (io_param->c_tx +  c_line) * 
          io_param->vdd_io * io_param->v_sw_data_read_line * 
          io_param->frequency / 1000.0 + 
        g_ip->num_dqs * io_param->c_data * io_param->vdd_io * 
          io_param->v_sw_data_read_load1 * io_param->frequency / 1000.0 + 
        g_ip->num_dqs * ((g_ip->num_mem_dq-1) * io_param->c_data) * 
          io_param->vdd_io * io_param->v_sw_data_read_load2 * io_param->frequency / 1000.0  + 
        g_ip->num_dqs * io_param->c_int * io_param->vdd_io * io_param->vdd_io * 
          io_param->frequency / 1000.0;

	power_ca_read = g_ip->num_ca * g_ip->activity_ca * 
          (io_param->c_tx + io_param->num_mem_ca * 
            io_param->c_addr + c_line_ca) * 
          io_param->vdd_io * io_param->v_sw_addr * io_param->frequency / 1000 + 
        g_ip->num_ca * g_ip->activity_ca * io_param->c_int * 
          io_param->vdd_io * io_param->vdd_io * io_param->frequency / 1000;

	power_clk = g_ip->num_clk * 
          (io_param->c_tx + io_param->num_mem_clk * 
            io_param->c_data + c_line) * 
          io_param->vdd_io * io_param->v_sw_clk *io_param->frequency / 1000 + 
        g_ip->num_clk * io_param->c_int * io_param->vdd_io * 
          io_param->vdd_io * io_param->frequency / 1000;

	

	}

	//Combining the power terms based on STATE (READ/WRITE/IDLE/SLEEP)

  if (g_ip->iostate == READ) {
    io_power_dynamic = g_ip->duty_cycle * (power_dq_read + 
        power_ca_read + power_dqs_read + power_clk);
       
  }
  else if (g_ip->iostate == WRITE) {
    io_power_dynamic = g_ip->duty_cycle * 
        (power_dq_write + power_ca_write + power_dqs_write + power_clk);
  }
  else if (g_ip->iostate == IDLE) {
    io_power_dynamic = g_ip->duty_cycle * (power_clk);
  }
  else if (g_ip->iostate == SLEEP) {
    io_power_dynamic = 0;
  }
  else {
    io_power_dynamic = 0;
  }


	printf("IO Dynamic Power (mW) = ");
	cout << io_power_dynamic << " ";
}


//External IO Timing and Voltage Margins. More details can be found in the CACTI-IO technical report (), Chapter 2.2.

void Extio::extio_eye()
{

	if (io_param->io_type == Serial)
	{io_vmargin=0;
	}
	else
	{

	//VOLTAGE MARGINS
  //Voltage noise calculations based on proportional and independent noise
  //sources for WRITE, READ and CA
	double v_noise_write = io_param->k_noise_write_sen * io_param->v_sw_data_write_line + 
                         io_param->v_noise_independent_write;
	double v_noise_read  = io_param->k_noise_read_sen * io_param->v_sw_data_read_line + 
                         io_param->v_noise_independent_read;
	double v_noise_addr  = io_param->k_noise_addr_sen * io_param->v_sw_addr + 
                         io_param->v_noise_independent_addr;
	
	
  //Worst-case voltage margin (Swing/2 - Voltage noise) calculations per state
  //depending on DQ voltage margin and CA voltage margin (lesser or the two is
  //reported)
  if (g_ip->iostate == READ)
  {
    if ((io_param->v_sw_data_read_line/2 - v_noise_read) <
        (io_param->v_sw_addr/2 - v_noise_addr)) {
      io_vmargin = io_param->v_sw_data_read_line/2 - v_noise_read;
    }
    else {
      io_vmargin = io_param->v_sw_addr/2 - v_noise_addr;
    }
  }
  else if (g_ip->iostate == WRITE) {
    if ((io_param->v_sw_data_write_line/2 - v_noise_write) < 
       (io_param->v_sw_addr/2 - v_noise_addr)) {
      io_vmargin = io_param->v_sw_data_write_line/2 - v_noise_write;
    }
    else {
      io_vmargin = io_param->v_sw_addr/2 - v_noise_addr;
    }		       
  }
  else {
    io_vmargin = 0;
  }

	}

	//TIMING MARGINS
	
	double t_margin_write_setup,t_margin_write_hold,t_margin_read_setup
		   ,t_margin_read_hold,t_margin_addr_setup,t_margin_addr_hold;
	
	if (io_param->io_type == Serial)
	{

	 t_margin_write_setup = (1e6/(4*io_param->frequency)) - 
         io_param->t_ds - 
         io_param->t_jitter_setup_sen;
	
	 t_margin_write_hold = (1e6/(4*io_param->frequency)) - 
         io_param->t_dh - io_param->t_dcd_soc -
         io_param->t_jitter_hold_sen;
	
	 t_margin_read_setup = (1e6/(4*io_param->frequency)) - 
         io_param->t_soc_setup - 
         io_param->t_jitter_setup_sen;

	 t_margin_read_hold = (1e6/(4*io_param->frequency)) - 
         io_param->t_soc_hold - io_param->t_dcd_dram - 
         io_param->t_dcd_soc - 
         io_param->t_jitter_hold_sen;

	
	
	 t_margin_addr_setup = (1e6*g_ip->addr_timing/(2*io_param->frequency));
	
	
	 t_margin_addr_hold = (1e6*g_ip->addr_timing/(2*io_param->frequency));



	}
	else
	{



  //Setup and Hold timing margins for DQ WRITE, DQ READ and CA based on timing
  //budget
	 t_margin_write_setup = (1e6/(4*io_param->frequency)) - 
         io_param->t_ds - io_param->t_error_soc - 
         io_param->t_jitter_setup_sen - io_param->t_skew_setup + io_param->t_cor_margin;
	
	 t_margin_write_hold = (1e6/(4*io_param->frequency)) - 
         io_param->t_dh - io_param->t_dcd_soc - io_param->t_error_soc - 
         io_param->t_jitter_hold_sen - io_param->t_skew_hold + io_param->t_cor_margin;
	
	 t_margin_read_setup = (1e6/(4*io_param->frequency)) - 
         io_param->t_soc_setup - io_param->t_error_soc - 
         io_param->t_jitter_setup_sen - io_param->t_skew_setup - 
         io_param->t_dqsq + io_param->t_cor_margin;

	 t_margin_read_hold = (1e6/(4*io_param->frequency)) - 
         io_param->t_soc_hold - io_param->t_dcd_dram - 
         io_param->t_dcd_soc - io_param->t_error_soc - 
         io_param->t_jitter_hold_sen - io_param->t_skew_hold + 		   io_param->t_cor_margin;

	
	
	 t_margin_addr_setup = (1e6*g_ip->addr_timing/(2*io_param->frequency)) - 
         io_param->t_is - io_param->t_error_soc - 
         io_param->t_jitter_addr_setup_sen - io_param->t_skew_setup + io_param->t_cor_margin;
	
	
	 t_margin_addr_hold = (1e6*g_ip->addr_timing/(2*io_param->frequency)) - 
         io_param->t_ih - io_param->t_dcd_soc - io_param->t_error_soc - 
         io_param->t_jitter_addr_hold_sen - io_param->t_skew_hold + io_param->t_cor_margin;
	}
	
  //Worst-case timing margin per state depending on DQ and CA timing margins
  if (g_ip->iostate == READ) {
    io_tmargin = t_margin_read_setup < t_margin_read_hold ? 
          t_margin_read_setup : t_margin_read_hold;
    io_tmargin = io_tmargin < t_margin_addr_setup ? 
          io_tmargin : t_margin_addr_setup;
    io_tmargin = io_tmargin < t_margin_addr_hold ? 
          io_tmargin : t_margin_addr_hold;
  }
  else if (g_ip->iostate == WRITE) {
    io_tmargin = t_margin_write_setup < t_margin_write_hold ? 
          t_margin_write_setup : t_margin_write_hold;
    io_tmargin = io_tmargin < t_margin_addr_setup ? 
          io_tmargin : t_margin_addr_setup;
    io_tmargin = io_tmargin < t_margin_addr_hold ? 
          io_tmargin : t_margin_addr_hold;
  }
  else {
    io_tmargin = 0;
  }





	//OUTPUTS


	printf("IO Timing Margin (ps) = ");
	cout << io_tmargin <<endl;
	printf("IO Votlage Margin (V) = ");
	cout << io_vmargin << endl;

}
