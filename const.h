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
#ifndef __CONST_H__
#define __CONST_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*  The following are things you might want to change
 *  when compiling
 */

/*
 * Address bits in a word, and number of output bits from the cache 
 */

/*
was: #define ADDRESS_BITS 32
now: I'm using 42 bits as in the Power4, 
since that's bigger then the 36 bits on the Pentium 4 
and 40 bits on the Opteron
*/
const int ADDRESS_BITS = 42;

/*dt: In addition to the tag bits, the tags also include 1 valid bit, 1 dirty bit, 2 bits for a 4-state 
  cache coherency protocoll (MESI), 1 bit for MRU (change this to log(ways) for full LRU). 
  So in total we have 1 + 1 + 2 + 1 = 5 */
const int EXTRA_TAG_BITS = 5;

/* limits on the various N parameters */

const unsigned int MAXDATAN     = 512;      // maximum for Ndwl and Ndbl
const unsigned int MAXSUBARRAYS = 1048576;  // maximum subarrays for data and tag arrays
const unsigned int MAXDATASPD   = 256;      // maximum for Nspd
const unsigned int MAX_COL_MUX  = 256;



#define ROUTER_TYPES 3
#define WIRE_TYPES 6

const double Cpolywire = 0;


/* Threshold voltages (as a proportion of Vdd)
   If you don't know them, set all values to 0.5 */
#define VTHFA1         0.452
#define VTHFA2         0.304
#define VTHFA3         0.420
#define VTHFA4         0.413
#define VTHFA5         0.405
#define VTHFA6         0.452
#define VSINV          0.452   
#define VTHCOMPINV     0.437
#define VTHMUXNAND     0.548  // TODO : this constant must be revisited
#define VTHEVALINV     0.452
#define VTHSENSEEXTDRV 0.438


//WmuxdrvNANDn and WmuxdrvNANDp are no longer being used but it's part of the old
//delay_comparator function which we are using exactly as it used to be, so just setting these to 0
const double WmuxdrvNANDn = 0;
const double WmuxdrvNANDp = 0;


/*===================================================================*/
/*
 * The following are things you probably wouldn't want to change.  
 */

#define BIGNUM 1e30
#define INF 9999999
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

/* Used to communicate with the horowitz model */
#define RISE 1
#define FALL 0
#define NCH  1
#define PCH  0


#define EPSILON 0.5 //v4.1: This constant is being used in order to fix floating point -> integer
//conversion problems that were occuring within CACTI. Typical problem that was occuring was
//that with different compilers a floating point number like 3.0 would get represented as either 
//2.9999....or 3.00000001 and then the integer part of the floating point number (3.0) would 
//be computed differently depending on the compiler. What we are doing now is to replace 
//int (x) with (int) (x+EPSILON) where EPSILON is 0.5. This would fix such problems. Note that
//this works only when x is an integer >= 0. 

#define EPSILON2 0.1  
#define EPSILON3 0.6


#define MINSUBARRAYROWS 16 //For simplicity in modeling, for the row decoding structure, we assume
//that each row predecode block is composed of at least one 2-4 decoder. When the outputs from the
//row predecode blocks are combined this means that there are at least 4*4=16 row decode outputs
#define MAXSUBARRAYROWS 262144 //Each row predecode block produces a max of 2^9 outputs. So 
//the maximum number of row decode outputs will be 2^9*2^9
#define MINSUBARRAYCOLS 2
#define MAXSUBARRAYCOLS 262144


#define INV 0
#define NOR 1
#define NAND 2


#define NUMBER_TECH_FLAVORS 4

#define NUMBER_INTERCONNECT_PROJECTION_TYPES 2 //aggressive and conservative
//0 = Aggressive projections, 1 = Conservative projections
#define NUMBER_WIRE_TYPES 4 //local, semi-global and global
//1 = 'Semi-global' wire type, 2 = 'Global' wire type


const int dram_cell_tech_flavor = 3;


#define VBITSENSEMIN 0.08 //minimum bitline sense voltage is fixed to be 80 mV.

#define fopt 4.0

#define INPUT_WIRE_TO_INPUT_GATE_CAP_RATIO 0
#define BUFFER_SEPARATION_LENGTH_MULTIPLIER 1
#define NUMBER_MATS_PER_REDUNDANT_MAT 8

#define NUMBER_STACKED_DIE_LAYERS 1

// this variable can be set to carry out solution optimization for
// a maximum area allocation.
#define STACKED_DIE_LAYER_ALLOTED_AREA_mm2 0 //6.24 //6.21//71.5

// this variable can also be employed when solution optimization
// with maximum area allocation is carried out. 
#define MAX_PERCENT_AWAY_FROM_ALLOTED_AREA 50

// this variable can also be employed when solution optimization
// with maximum area allocation is carried out.
#define MIN_AREA_EFFICIENCY 20

// this variable can be employed when solution with a desired
// aspect ratio is required.
#define STACKED_DIE_LAYER_ASPECT_RATIO 1

// this variable can be employed when solution with a desired
// aspect ratio is required. 
#define MAX_PERCENT_AWAY_FROM_ASPECT_RATIO 101

// this variable can be employed to carry out solution optimization
// for a certain target random cycle time. 
#define TARGET_CYCLE_TIME_ns 1000000000

#define NUMBER_PIPELINE_STAGES 4

// this can be used to model the length of interconnect
// between a bank and a crossbar
#define LENGTH_INTERCONNECT_FROM_BANK_TO_CROSSBAR 0 //3791 // 2880//micron

#define IS_CROSSBAR 0
#define NUMBER_INPUT_PORTS_CROSSBAR 8
#define NUMBER_OUTPUT_PORTS_CROSSBAR 8
#define NUMBER_SIGNALS_PER_PORT_CROSSBAR 256


#define MAT_LEAKAGE_REDUCTION_DUE_TO_SLEEP_TRANSISTORS_FACTOR 1
#define LEAKAGE_REDUCTION_DUE_TO_LONG_CHANNEL_HP_TRANSISTORS_FACTOR 1

#define PAGE_MODE 0

#define MAIN_MEM_PER_CHIP_STANDBY_CURRENT_mA 60
// We are actually not using this variable in the CACTI code. We just want to acknowledge that 
// this current should be multiplied by the DDR(n) system VDD value to compute the standby power
// consumed during precharge. 


const double VDD_STORAGE_LOSS_FRACTION_WORST = 0.125;
const double CU_RESISTIVITY = 0.022; //ohm-micron
const double BULK_CU_RESISTIVITY = 0.018; //ohm-micron
const double PERMITTIVITY_FREE_SPACE = 8.854e-18; //F/micron

const static uint32_t sram_num_cells_wl_stitching_ = 16;
const static uint32_t dram_num_cells_wl_stitching_ = 64;
const static uint32_t comm_dram_num_cells_wl_stitching_ = 256;
const static uint32_t num_bits_per_ecc_b_          = 8;

#define MAX_NUMBER_GATES_STAGE 20
#define MAX_NUMBER_HTREE_NODES 20
#define NAND2_LEAK_STACK_FACTOR 0.2
#define NAND3_LEAK_STACK_FACTOR 0.2
#define NOR2_LEAK_STACK_FACTOR 0.2
#define INV_LEAK_STACK_FACTOR  0.5
#define MAX_NUMBER_ARRAY_PARTITIONS 1000000


// abbreviations used in this project
// ----------------------------------
//
//  num  : number
//  rw   : read/write
//  rd   : read
//  wr   : write
//  se   : single-ended
//  sz   : size
//  F    : feature
//  w    : width
//  h    : height or horizontal
//  v    : vertical or velocity


enum ram_cell_tech_type_num
{
  itrs_hp   = 0,
  itrs_lstp = 1,
  itrs_lop  = 2,
  lp_dram   = 3,
  comm_dram = 4
};



#endif
