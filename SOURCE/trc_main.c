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

#include <math.h>
#include <stdio.h>

#include "cx.h"
#include "trantopcalc.h"

#define MAX_STRING 80
#define MAX_INT 50
#define MAXLINE 80
#define HOME 2
#define CAPTURE 100
#define STORE 101
#define RECALL 1
#define GRAPH 103
#define CLEAR 104
#define ANALYZE 2
#define IMPULSE 3
#define LENGTH_ANALYSIS 4
#define EXIT 6
#define HOME 2
#define OPEN_WISS 2
#define SEG_NAME_BEGIN 1000
#define PRINT_TOPOLOGY 5
main() {
  struct node *root_P;
  float xin, yin, xn, yn;
  char topologyFileName[256];
  char *messg1, *messg2;
  int action, initialChoice, scact;
  FILE *fnet, *frec;

  static char *selection_A[] = {"(1) Load",
                                "(2) Analyze",
                                "(3) Impulse",
                                "(4) Length Analysis",
                                "(5) Print Topology",
                                "(6) Exit",
                                ""};
  /* Initialize variables */

  printf("\nTransmission Line Calculation for \nBinary Tree Topologies:\n");
  printf("TransNetCalc\n");

  printf("This is free software; see the source for copying conditions. There "
         "is NO\n");
  printf("warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR "
         "PURPOSE.\n");

  printf("(c)1985-2017 Sasan H. Ardalan\n");
  printf("http://trantopcalc.sourceforge.net\n");
  printf("http://www.ccdsp.org/TransTopCalc_Site\n");

  action = CAPTURE;
  initialChoice = CAPTURE;
  root_P = (struct node *)NULL;
  while (action != EXIT) {
    action = Trc_Menu(selection_A);

    switch (action) {
    case CAPTURE:
#if 0
			if(root != (struct node *) NULL) {
				recover_mem(ws_id,root);
				root = (struct node *) NULL;
			}
			root=capture_nodes(ws_id,fp);
			initialChoice=STORE;
#endif
      break;
    case STORE:
      Trc_Prompt("Enter file name to store network ");
      scanf("%s", topologyFileName);
      if ((fnet = fopen(topologyFileName, "w")) == NULL) {
        Trc_Message("Cannot open file!");
        initialChoice = STORE;
        break;
      }
      Trc_StoreTree(root_P, fnet);
      fclose(fnet);
      initialChoice = RECALL;
      Trc_Prompt("Network Stored");
      break;
    case RECALL:
      if (root_P != (struct node *)NULL) {
        Trc_RecoverMem(root_P);
        root_P = (struct node *)NULL;
      }
      Trc_Prompt("Enter file that contains network");
      scanf("%s", topologyFileName);
      if ((frec = fopen(topologyFileName, "r")) == NULL) {
        Trc_Message("Cannot find file !");
        initialChoice = RECALL;
        break;
      }
      root_P = Trc_RecallTree(frec);
      fclose(frec);
      /*
                              seg_name=SEG_NAME_BEGIN;
                              graph_tree_recall(ws_id,root,&seg_name);
      */
      break;
    case ANALYZE:
      if (root_P == (struct node *)NULL)
        break;
      Trc_NetAnal(root_P, topologyFileName);
      action = EXIT;
      break;
    case IMPULSE:
      if (root_P == (struct node *)NULL)
        break;
      Trc_NetImpCalc(root_P, topologyFileName);
      action = EXIT;
      break;
    case GRAPH:
      if (root_P == (struct node *)NULL)
        break;
#if 0
			del_tree_seg_ws(ws_id,root);
			graph_tree_recall(ws_id,root,&seg_name);
#endif
      break;
    case CLEAR:
#if 0
			clear_ws(ws_id);
#endif
      break;
    case LENGTH_ANALYSIS:
      if (root_P == (struct node *)NULL)
        break;
      Trc_NetLength(root_P);
#if 0
			edit_network(ws_id,root);
			clear();
			refresh();
			del_tree_seg_ws(ws_id,root);
			graph_tree_recall(ws_id,root,&seg_name);
#endif
      break;

    case PRINT_TOPOLOGY:
      if (root_P == (struct node *)NULL) {
        printf("Topology is empty\n");
        break;
      }
      Trc_StoreTree(root_P, stdout);

      break;

    case EXIT:
      break;
    }
  }

} /* end main */

void CsInfo(char *info) { fprintf(stdout, "%s\n", info); }
