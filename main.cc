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

#include "io.h"
#include <iostream>

#include "Ucache.h"

using namespace std;


int main(int argc,char *argv[])
{

  uca_org_t result;
  if (argc != 53 && argc != 55 && argc !=64)
  {
    bool infile_specified = false;
    string infile_name("");

    for (int32_t i = 0; i < argc; i++)
    {
      if (argv[i] == string("-infile"))
      {
        infile_specified = true;
        i++;
        infile_name = argv[i];
      }
    }
    if (infile_specified == false)
    {
      cerr << " Invalid arguments -- how to use CACTI:" << endl;
      cerr << "  1) cacti -infile <input file name>" << endl;
      cerr << "  2) cacti arg1 ... arg52 -- please refer to the README file" << endl;
      cerr << " No. of arguments input - " << argc << endl;
      exit(1);
    }
    else
    {
      result = cacti_interface(infile_name);
    }
  }
  else if (argc == 53)
  {
	  result = cacti_interface(atoi(argv[ 1]),
			  atoi(argv[ 2]),
			  atoi(argv[ 3]),
			  atoi(argv[ 4]),
			  atoi(argv[ 5]),
			  atoi(argv[ 6]),
			  atoi(argv[ 7]),
			  atoi(argv[ 8]),
			  atoi(argv[ 9]),
			  atof(argv[10]),
			  atoi(argv[11]),
			  atoi(argv[12]),
			  atoi(argv[13]),
			  atoi(argv[14]),
			  atoi(argv[15]),
			  atoi(argv[16]),
			  atoi(argv[17]),
			  atoi(argv[18]),
			  atoi(argv[19]),
			  atoi(argv[20]),
			  atoi(argv[21]),
			  atoi(argv[22]),
			  atoi(argv[23]),
			  atoi(argv[24]),
			  atoi(argv[25]),
			  atoi(argv[26]),
			  atoi(argv[27]),
			  atoi(argv[28]),
			  atoi(argv[29]),
			  atoi(argv[30]),
			  atoi(argv[31]),
			  atoi(argv[32]),
			  atoi(argv[33]),
			  atoi(argv[34]),
			  atoi(argv[35]),
			  atoi(argv[36]),
			  atoi(argv[37]),
			  atoi(argv[38]),
			  atoi(argv[39]),
			  atoi(argv[40]),
			  atoi(argv[41]),
			  atoi(argv[42]),
			  atoi(argv[43]),
			  atoi(argv[44]),
			  atoi(argv[45]),
			  atoi(argv[46]),
			  atoi(argv[47]),
			  atoi(argv[48]),
			  atoi(argv[49]),
			  atoi(argv[50]),
			  atoi(argv[51]),
			  atoi(argv[52]));
  }
  else if (argc == 55)
  {
	  result = cacti_interface(atoi(argv[ 1]),
			  atoi(argv[ 2]),
			  atoi(argv[ 3]),
			  atoi(argv[ 4]),
			  atoi(argv[ 5]),
			  atoi(argv[ 6]),
			  atoi(argv[ 7]),
			  atoi(argv[ 8]),
			  atof(argv[ 9]),
			  atoi(argv[10]),
			  atoi(argv[11]),
			  atoi(argv[12]),
			  atoi(argv[13]),
			  atoi(argv[14]),
			  atoi(argv[15]),
			  atoi(argv[16]),
			  atoi(argv[17]),
			  atoi(argv[18]),
			  atoi(argv[19]),
			  atoi(argv[20]),
			  atoi(argv[21]),
			  atoi(argv[22]),
			  atoi(argv[23]),
			  atoi(argv[24]),
			  atoi(argv[25]),
			  atoi(argv[26]),
			  atoi(argv[27]),
			  atoi(argv[28]),
			  atoi(argv[29]),
			  atoi(argv[30]),
			  atoi(argv[31]),
			  atoi(argv[32]),
			  atoi(argv[33]),
			  atoi(argv[34]),
			  atoi(argv[35]),
			  atoi(argv[36]),
			  atoi(argv[37]),
			  atoi(argv[38]),
			  atoi(argv[39]),
			  atoi(argv[40]),
			  atoi(argv[41]),
			  atoi(argv[42]),
			  atoi(argv[43]),
			  atoi(argv[44]),
			  atoi(argv[45]),
			  atoi(argv[46]),
			  atoi(argv[47]),
			  atoi(argv[48]),
			  atoi(argv[49]),
			  atoi(argv[50]),
			  atoi(argv[51]),
			  atoi(argv[52]),
			  atoi(argv[53]),
			  atoi(argv[54]));
  }
  else if (argc == 64)
  {
	  result = cacti_interface(atoi(argv[ 1]),
			  atoi(argv[ 2]),
			  atoi(argv[ 3]),
			  atoi(argv[ 4]),
			  atoi(argv[ 5]),
			  atoi(argv[ 6]),
			  atoi(argv[ 7]),
			  atoi(argv[ 8]),
			  atof(argv[ 9]),
			  atoi(argv[10]),
			  atoi(argv[11]),
			  atoi(argv[12]),
			  atoi(argv[13]),
			  atoi(argv[14]),
			  atoi(argv[15]),
			  atoi(argv[16]),
			  atoi(argv[17]),
			  atoi(argv[18]),
			  atoi(argv[19]),
			  atoi(argv[20]),
			  atoi(argv[21]),
			  atoi(argv[22]),
			  atoi(argv[23]),
			  atoi(argv[24]),
			  atoi(argv[25]),
			  atoi(argv[26]),
			  atoi(argv[27]),
			  atoi(argv[28]),
			  atoi(argv[29]),
			  atoi(argv[30]),
			  atoi(argv[31]),
			  atoi(argv[32]),
			  atoi(argv[33]),
			  atoi(argv[34]),
			  atoi(argv[35]),
			  atoi(argv[36]),
			  atoi(argv[37]),
			  atoi(argv[38]),
			  atoi(argv[39]),
			  atoi(argv[40]),
			  atoi(argv[41]),
			  atoi(argv[42]),
			  atoi(argv[43]),
			  atoi(argv[44]),
			  atoi(argv[45]),
			  atoi(argv[46]),
			  atoi(argv[47]),
			  atoi(argv[48]),
			  atoi(argv[49]),
			  atoi(argv[50]),
			  atoi(argv[51]),
			  atoi(argv[52]),
			  atoi(argv[53]),
			  atoi(argv[54]),
			  atoi(argv[55]),
			  atoi(argv[56]),
			  atoi(argv[57]),
			  atoi(argv[58]),
			  atoi(argv[59]),
			  atoi(argv[60]),
			  atoi(argv[61]),
			  atoi(argv[62]),
			  atoi(argv[63]));
  }

  cout << "=============================================\n\n";
 // print_g_tp(); //function to test technology paramters.
//  g_tp.display();
  result.cleanup();
//  delete result.data_array2;
//  if (result.tag_array2!=NULL)
//	  delete result.tag_array2;

  return 0;
}

