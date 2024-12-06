/*  Transmission Line Topology Calculation and Analysis (TranTopCalc) Command
   Tool Copyright (C) 2006  Sasan H Ardalan

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    http://www.ccdsp.org
    Raleigh, North Carolina
*/

/* main.c */
#include  <stdlib.h>

#include "cx.h"
#include "trantopcalc_tcl.h"
#include <tcl.h>
#include <trantopcalc.h>

float Trc_freq;

main(int argc, char *argv[]) {
  /*
   * Initialize your application here.
   *
   * Then initialize and run Tcl.
   */
  printf("\nTransmission Line Calculation for \nBinary Tree Topologies:\n");
  printf("TransNetCalc\n");
  printf("This is free software; see the source for copying conditions. There "
         "is NO\n");
  printf("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR "
         "PURPOSE.\n");

  printf("(c)1985-2017 Sasan H. Ardalan\n");
  printf("http://trantopcalc.sourceforge.net\n");
  printf("http://www.ccdsp.org/TransTopCalc_Site\n");

  Trc_freq = 1000000.0;
  Tcl_Main(argc, argv, Trc_TclAppInit);
  exit(0);
}

void CsInfo(char *info) { fprintf(stdout, "%s\n", info); }

int Trc_Menu(char *selection_A[]) {
  int select;
  int i;

  i = 0;
  while (strcmp(selection_A[i], "") != 0) {
    fprintf(stderr, "%s\n", selection_A[i]);
    i++;
  }
  scanf("%d", &select);
  return (select);
}
