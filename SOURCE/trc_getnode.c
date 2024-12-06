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

#include "cx.h"
#include "trantopcalc.h"
#include <math.h>
#include <stdio.h>

#define MOVE_DOWN 275  /* curser down */
#define MOVE_LEFT 276  /* curser left */
#define MOVE_RIGHT 277 /* curser right*/
#define FREQ 'f'
#define OVER 'x'
#define DONE 1
#define NODE_INFO 0
#define VOLTAGE 1
#define CURRENT 2
#define CHANGE_FREQ 3
#define EXIT 4
#define TRY_AGAIN 10

Trc_GetNodeInfo(struct node *root_P, int *key_val) {
  int flag;
  short int key;
  char c;
  char nodeName[80];
  struct node *current_P;
  static char *selection_A[] = {"(0) Node Inform  ", "(1) Voltage Dist ",
                                "(2) Current Dist ", "(3) Freq Change  ",
                                "(4) Exit         ", ""};
  char fname[80], filecur[80], filevol[80];
  FILE *fcur, *fvol, *wind;
  int i;
  float x[100], y[100];

  int n = 50;
  int n1;
  int num_curves = 1;
  int action;

  n1 = n + 1;
  wind = stdout;

  flag = 0;
  while (flag != DONE) {
    Trc_Prompt("Enter  a node name and Hit Return");
    scanf("%s", nodeName);
    current_P = Trc_SearchTreeName(root_P, nodeName);
    action = Trc_Menu(selection_A);
    if (current_P == (struct node *)NULL) {
      Trc_Prompt("No match ... try again");
      action = TRY_AGAIN;
    }
    switch (action) {
    case NODE_INFO:
      Trc_PrintNode(current_P, wind);
      break;
    case CHANGE_FREQ:
      flag = DONE;
      *key_val = FREQ;
      break;
    case VOLTAGE:
      printf("Not implemented\n");
#if 0
				walk_voltage(current_P,x,y,n);
				clear();
				refresh();
				wclear(wind);
				wrefresh(wind);
				clear_ws(ws_id);
				graph_plot(x,y,&n1,&num_curves,
					"Voltage Distribution","Length",
					"Voltage");
				Trc_Prompt("Enter s to save");
				key=keystroke();
				if(key == 's') {

				Trc_Prompt("Enter file name (no attribute)\n");
				scanf("%s",fname);
				strcpy(filevol,fname);
				strcat(filevol,".vol");
				fvol=fopen(filevol,"w");
				for (i=0; i<=n; i++)
					fprintf(fvol," %e %e \n",x[i],y[i]);
				fclose(fvol);
				}
#endif
      break;
    case CURRENT:
      printf("Not implemented\n");
#if 0
				walk_current(current,x,y,n);
				clear();
				refresh();
				clear_ws(ws_id);
				graph_plot(x,y,&n1,&num_curves,
					"Current Distribution",
					"Length","Current");
				Trc_Prompt("Enter s to save");
				key=keystroke();
				if(key == 's') {

				Trc_Prompt("Enter file name (no attribute)\n");
				scanf("%s",fname);
				strcpy(filecur,fname);
				strcat(filecur,".cur");
				fcur=fopen(filecur,"w");
				for (i=0; i<=n; i++)
					fprintf(fcur," %e %e \n",x[i],y[i]);
				fclose(fcur);
				}
#endif
      break;
    case TRY_AGAIN:
      break;

    case EXIT:
      flag = DONE;
      *key_val = OVER;
      break;
    }
  }
}

void Trc_PrintNode(struct node *root_P, FILE *fp)
/* The function print_node  outputs the node data calculated during program
 * execution to the window defined by *wind
 */
{
  float wavelength;
  float epsr;

  if (root_P == (struct node *)NULL)
    return;
  fprintf(fp, "Node Name: %s\n", root_P->name);
  fprintf(fp, "Length: %11.4e\n", root_P->length);
  fprintf(fp, "Line Type : %s\n", root_P->typeName);
  fprintf(fp, "Z0: %11.4e %11.4e\n", root_P->zo.re, root_P->zo.im);
  fprintf(fp, "Gamma: \n %11.4e %11.4e \n", root_P->gamma.re, root_P->gamma.im);
  if (root_P->gamma.im == 0)
    fprintf(fp, "Wavelength = infinity\n");
  else {
    wavelength = 2 * PI / root_P->gamma.im;
    fprintf(fp, "Wavelength = %f\n", wavelength);
    epsr = 3.e8 / Trc_freq / wavelength;
    epsr = epsr * epsr;
    fprintf(fp, "epsr = %f\n", epsr);
  }

  switch (root_P->type) {
  case NODE_TYPE_LEAF:
    if (root_P->infinite)
      fprintf(fp, "Impedence: infinite\n");
    else
      fprintf(fp, "Z: %11.4e %11.4e\n", root_P->zLoad.re, root_P->zLoad.im);
    fprintf(fp, "Incoming current:\n  %11.4e %11.4e\n", root_P->inCurrent.re,
            root_P->inCurrent.im);

    break;
  case NODE_TYPE_UNARY:
    fprintf(fp, "Left branch: %s\n", root_P->leftName);
    fprintf(fp, "Z Left: \n %11.4e %11.4e\n", root_P->leftImp.re,
            root_P->leftImp.im);
    fprintf(fp, "Incoming current:\n  %11.4e %11.4e\n", root_P->inCurrent.re,
            root_P->inCurrent.im);
    fprintf(fp, "Outgoing left current: \n %11.4e %11.4e\n",
            root_P->leftCurrent.re, root_P->leftCurrent.im);

    break;
  case NODE_TYPE_BINARY:
    fprintf(fp, "Left branch: %s\n", root_P->leftName);
    fprintf(fp, "Right branch: %s\n", root_P->rightName);
    fprintf(fp, "Z Left: \n %11.4e %11.4e\n", root_P->leftImp.re,
            root_P->leftImp.im);
    fprintf(fp, "Z Right: \n %11.4e %11.4e\n", root_P->rightImp.re,
            root_P->rightImp.im);
    fprintf(fp, "Incoming current:\n  %11.4e %11.4e\n", root_P->inCurrent.re,
            root_P->inCurrent.im);
    fprintf(fp, "Outgoing left current: \n %11.4e %11.4e\n",
            root_P->leftCurrent.re, root_P->leftCurrent.im);
    fprintf(fp, "Outgoing right current: \n %11.4e %11.4e\n",
            root_P->rightCurrent.re, root_P->rightCurrent.im);

    break;
  }
  fprintf(fp, "Voltage: \n %11.4e %11.4e\n", root_P->voltage.re,
          root_P->voltage.im);
}

void Trc_PrintTree(struct node *root_P, char *nodename, FILE *fp)
/*
 * The function print_tree outputs the data calculated during program
 * execution.
 */
{
  float wavelength;
  float epsr;

  if (root_P == (struct node *)NULL)
    return;
  if ((strcmp(root_P->name, nodename) == 0) || (strcmp(nodename, "all") == 0)) {
    fprintf(fp, "\nName: %s\n", root_P->name);
    fprintf(fp, "Length: %.3f\n", root_P->length);
    fprintf(fp, "Characteristic impedence: %.3f %.3f\n", root_P->zo.re,
            root_P->zo.im);
    root_P->gamma.re = root_P->gamma.re;
    fprintf(fp, "Propagation constant: %.3f %.3f \n", root_P->gamma.re,
            root_P->gamma.im);
    if (root_P->gamma.im == 0)
      fprintf(fp, "Wavelength = infinity\n");
    else {
      wavelength = 2 * PI / root_P->gamma.im;
      fprintf(fp, "Wavelength = %f\n", wavelength);
      epsr = sqrt(3.e8 / Trc_freq / wavelength);
      fprintf(fp, "epsr = %f\n", epsr);
    }
    fprintf(fp, "------------------------------------\n");
    fprintf(fp, "Left branch: %s\n", root_P->leftName);
    fprintf(fp, "Right branch: %s\n", root_P->rightName);
    if (root_P->infinite)
      fprintf(fp, "Impedence: infinite\n");
    else
      fprintf(fp, "Impedence: %.3f %.3f\n", root_P->zLoad.re, root_P->zLoad.im);
    fprintf(fp, "Left impedence: %.3f %.3f\n", root_P->leftImp.re,
            root_P->leftImp.im);
    fprintf(fp, "Right impedence: %.3f %.3f\n", root_P->rightImp.re,
            root_P->rightImp.im);
    fprintf(fp, "Incoming current: %.3f %.3f\n", root_P->inCurrent.re,
            root_P->inCurrent.im);
    fprintf(fp, "Outgoing left current: %.3f %.3f\n", root_P->leftCurrent.re,
            root_P->leftCurrent.im);
    fprintf(fp, "Outgoing right current: %.3f %.3f\n", root_P->rightCurrent.re,
            root_P->rightCurrent.im);
    fprintf(fp, "Voltage: %.3f %.3f\n", root_P->voltage.re, root_P->voltage.im);
  }

  Trc_PrintTree(root_P->left_P, nodename, fp);
  Trc_PrintTree(root_P->right_P, nodename, fp);
}
