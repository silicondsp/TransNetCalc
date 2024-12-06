/*  Transmission Line Topology Calculation and Analysis (TransTopCalc TTC)
 * Library Copyright (C) 1985-2017  Sasan H Ardalan
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    http://www.SiliconDsp.com
 *    Silicon DSP  Corporation
 */

/*********************************************************************/
/*                                                                   */
/* NAME:  cn_graph.c                                                */
/*                                                                   */
/* graph the network                                                 */
/*                                                                   */
/* NOTE:  network topology must be entered as specified in           */
/*        users manual                                               */
/*                                                                   */
/* AUTHOR:  Sasan Ardalan 			                     */
/*          North Carolina State University                          */
/*                                                                   */
/* DATE:  Sept 24, 1986                                              */
/*                                                                   */
/*********************************************************************/

#include "cx.h"
#include "trantopcalc.h"
#include <math.h>
#include <stdio.h>

int Trc_GraphNet(char *netFileName) {
  struct node *root_P;
  FILE *net_F; /* pointer to file with network */
  struct node *current_P;

  /*
   * create the network as a recursive tree data structure
   */
  if ((net_F = fopen(netFileName, "r")) == NULL) {
    Trc_Message("Cannot find file !");
    return (1);
  }
  root_P = Trc_RecallTree(net_F);
  fclose(net_F);
  /*
  Cn_DrawTopology(root_P,netFileName);
  */
  return (0);
}
