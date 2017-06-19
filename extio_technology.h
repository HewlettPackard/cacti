#ifndef __EXTIO_TECH__
#define __EXTIO_TECH__

#include <iostream>
#include "parameter.h"
#include "const.h"

#define NUM_DIMM 1


extern const double rtt1_wr_lrdimm_ddr3[8][4];
extern const double rtt2_wr_lrdimm_ddr3[8][4];
extern const double rtt1_rd_lrdimm_ddr3[8][4];
extern const double rtt2_rd_lrdimm_ddr3[8][4];

extern const double rtt1_wr_host_dimm_ddr3[3][4];
extern const double rtt2_wr_host_dimm_ddr3[3][4];
extern const double rtt1_rd_host_dimm_ddr3[3][4];
extern const double rtt2_rd_host_dimm_ddr3[3][4];

extern const double rtt1_wr_bob_dimm_ddr3[3][4];
extern const double rtt2_wr_bob_dimm_ddr3[3][4];
extern const double rtt1_rd_bob_dimm_ddr3[3][4];
extern const double rtt2_rd_bob_dimm_ddr3[3][4];


extern const double rtt1_wr_lrdimm_ddr4[8][4];
extern const double rtt2_wr_lrdimm_ddr4[8][4];
extern const double rtt1_rd_lrdimm_ddr4[8][4];
extern const double rtt2_rd_lrdimm_ddr4[8][4];

extern const double rtt1_wr_host_dimm_ddr4[3][4];
extern const double rtt2_wr_host_dimm_ddr4[3][4];
extern const double rtt1_rd_host_dimm_ddr4[3][4];
extern const double rtt2_rd_host_dimm_ddr4[3][4];

extern const double rtt1_wr_bob_dimm_ddr4[3][4];
extern const double rtt2_wr_bob_dimm_ddr4[3][4];
extern const double rtt1_rd_bob_dimm_ddr4[3][4];
extern const double rtt2_rd_bob_dimm_ddr4[3][4];

class IOTechParam
{
  public:
		IOTechParam(InputParameter *);
		// connection : 0(bob-dimm), 1(host-dimm), 2(on-dimm)
		IOTechParam(InputParameter *, Mem_IO_type io_type, int num_mem_dq, int mem_data_width, int num_dq, int connection, int num_loads, double freq) ;
		~IOTechParam();
		double num_mem_ca; /* Number of loads on the address bus
			based on total number of memories in the channel.For 
			registered or buffered configurations, the num_mem_dq and num_mem_ca is per buffer. */

		double num_mem_clk; /* Number of loads on the clock as total
			memories in the channel / number of clock lines available */

		//Technology Parameters
		// IO Supply voltage (V) 
		double vdd_io; /* Voltage swing on CLK/CLKB (V) (swing on the CLK pin if it
			is differentially terminated) */
		double v_sw_clk;

		// Loading parameters

		double c_int; /*Internal IO loading (pF) (loading within the IO, due to
			predriver nets) */
		double c_tx; /* IO TX self-load including package (pF) (loading at the
			CPU TX pin) */
		double c_data; /* Device loading per memory data pin (pF) (DRAM device
			load for DQ per die) */
		double c_addr; /* Device loading per memory address pin (pF) (DRAM
			device load for CA per die) */
		double i_bias; /* Bias current (mA) (includes bias current for the whole memory
			bus due to RX Vref based receivers */
		double i_leak; // Active leakage current per pin (nA) 



      // IO Area coefficients

		double ioarea_c; /* sq.mm. (IO Area baseline coeeficient for control
			circuitry and overhead) */
		double ioarea_k0; /* sq.mm * ohms (IO Area coefficient for the driver, for
			unit drive strength or output impedance) */
		double ioarea_k1; /* sq.mm * ohms / MHz (IO Area coefficient for the
			predriver final stage, based on fanout needed) */
		double ioarea_k2; /* sq.mm * ohms / MHz^2 (IO Area coefficient for
			predriver middle stage, based on fanout needed) */
		double ioarea_k3; /* sq.mm * ohms / MHz^3 (IO Area coefficient for
			predriver first stage, based on fanout needed) */


      // Timing parameters (ps)

	    double t_ds;  //DQ setup time at DRAM
	    double t_is;  //CA setup time at DRAM
	    double t_dh;  //DQ hold time at DRAM
	    double t_ih;  //CA hold time at DRAM
	    double t_dcd_soc;   //Duty-cycle distortion at the CPU/SOC
	    double t_dcd_dram;  //Duty-cycle distortion at the DRAM
	    double t_error_soc; //Timing error due to edge placement uncertainty of the DLL
	    double t_skew_setup;//Setup skew between DQ/DQS or CA/CLK after deskewing the lines
	    double t_skew_hold; //Hold skew between DQ/DQS or CA/CLK after deskewing the lines
	    double t_dqsq;  //DQ-DQS skew at the DRAM output during Read
	    //double t_qhs;   //DQ-DQS hold factor at the DRAM output during Read FIXME: I am commenting it as the variable is never used.
	    double t_soc_setup; //Setup time at SOC input dueing Read
	    double t_soc_hold;  //Hold time at SOC input during Read
		double t_jitter_setup; /* Half-cycle jitter on the DQS at DRAM input
			affecting setup time */
		double t_jitter_hold; /* Half-cycle jitter on the DQS at the DRAM input
			affecting hold time */
		double t_jitter_addr_setup; /* Half-cycle jitter on the CLK at DRAM input
			affecting setup time */
		double t_jitter_addr_hold; /* Half-cycle jitter on the CLK at the DRAM
			input affecting hold time */
		double t_cor_margin;  // Statistical correlation margin


	    	//Termination Parameters

      double r_diff_term; /* Differential termination resister if
            used for CLK (Ohm) */


      // ODT related termination resistor values (Ohm)

	    double rtt1_dq_read; //DQ Read termination at CPU
	    double rtt2_dq_read; //DQ Read termination at inactive DRAM
	    double rtt1_dq_write; //DQ Write termination at active DRAM
	    double rtt2_dq_write; //DQ Write termination at inactive DRAM
	    double rtt_ca; //CA fly-by termination
	    double rs1_dq; //Series resistor at active DRAM
	    double rs2_dq; //Series resistor at inactive DRAM
	    double r_stub_ca; //Series resistor for the fly-by channel
	    double r_on; //Driver impedance
	    double r_on_ca; //CA driver impedance

		double z0; //Line impedance (ohms): Characteristic impedance of the route.
		double t_flight; /* Flight time of the interconnect (ns) (approximately
			180ps/inch for FR4) */
		double t_flight_ca; /* Flight time of the Control/Address (CA)
			interconnect (ns) (approximately 180ps/inch for FR4) */

     		// Voltage noise coeffecients

	    double k_noise_write; //Proportional noise coefficient for Write mode
	    double k_noise_read; //Proportional noise coefficient for Read mode
	    double k_noise_addr; //Proportional noise coefficient for Address bus
	    double v_noise_independent_write; //Independent noise voltage for Write mode
	    double v_noise_independent_read; //Independent noise voltage for Read mode
	    double v_noise_independent_addr; //Independent noise voltage for Address bus


	    //SENSITIVITY INPUTS FOR TIMING AND VOLTAGE NOISE

      /* This is a user-defined section that depends on the channel sensitivity
       * to IO and DRAM parameters. The t_jitter_* and k_noise_* are the
       * parameters that are impacted based on the channel analysis. The user
       * can define any relationship between the termination, loading and
       * configuration parameters AND the t_jitter/k_noise parameters. */

	    double k_noise_write_sen; 
	    double k_noise_read_sen; 
	    double k_noise_addr_sen;
	    double t_jitter_setup_sen;
	    double t_jitter_hold_sen;
	    double t_jitter_addr_setup_sen;
	    double t_jitter_addr_hold_sen;

	    //SWING AND TERMINATION CALCULATIONS
  	  //R|| calculation

		double rpar_write;
		double rpar_read;

	    //Swing calculation

		double v_sw_data_read_load1; //Swing for DQ at dram1 during READ
		double v_sw_data_read_load2; //Swing for DQ at dram2 during READ
		double v_sw_data_read_line; //Swing for DQ on the line during READ
		double v_sw_addr; //Swing for the address bus
		double v_sw_data_write_load1; //Swing for DQ at dram1 during WRITE
		double v_sw_data_write_load2; //Swing for DQ at dram2 during WRITE
		double v_sw_data_write_line; //Swing for DQ on the line during WRITE

		// PHY Static Power Coefficients (mW)

	    double phy_datapath_s; // Datapath Static Power
	    double phy_phase_rotator_s; // Phase Rotator Static Power
	    double phy_clock_tree_s; // Clock Tree Static Power
	    double phy_rx_s; // Receiver Static Power
	    double phy_dcc_s; // Duty Cycle Correction Static Power
	    double phy_deskew_s; // Deskewing Static Power
	    double phy_leveling_s; // Write and Read Leveling Static Power
	    double phy_pll_s; // PHY PLL Static Power


      // PHY Dynamic Power Coefficients (mW/Gbps)

	    double phy_datapath_d; // Datapath Dynamic Power
	    double phy_phase_rotator_d; // Phase Rotator Dynamic Power
	    double phy_clock_tree_d; // Clock Tree Dynamic Power
	    double phy_rx_d; // Receiver Dynamic Power
	    double phy_dcc_d; // Duty Cycle Correction Dynamic Power
	    double phy_deskew_d; // Deskewing Dynamic Power
	    double phy_leveling_d; // Write and Read Leveling Dynamic Power
	    double phy_pll_d; // PHY PLL Dynamic Power


	//PHY Wakeup Times (Sleep to Active) (microseconds)

		double phy_pll_wtime; // PHY PLL Wakeup Time
		double phy_phase_rotator_wtime; // Phase Rotator Wakeup Time
		double phy_rx_wtime; // Receiver Wakeup Time
		double phy_bandgap_wtime; // Bandgap Wakeup Time
		double phy_deskew_wtime; // Deskewing Wakeup Time
		double phy_vrefgen_wtime; // VREF Generator Wakeup Time


		// RTT values depends on the number of loads, frequency, and link_type
		double frequency;
		Mem_IO_type io_type;
		int frequnecy_index(Mem_IO_type type);
};

#endif
