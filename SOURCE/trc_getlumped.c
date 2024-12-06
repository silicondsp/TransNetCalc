
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

float Trc_Getval(char string[], int index) {
  float value;
  char valString[100];
  int i, j;
  char c;

  i = 1;
  do {
    c = string[index + i];
    valString[i - 1] = c;
    i++;
  } while ((c != '_') && (c != '\0'));

  valString[i] = NULL;

  sscanf(valString, "%e", &value);

  return (value);
}

void Trc_CnGetLumpedValues(struct node *current_P) {
  float r, l, c, g;
  char string[200];
  int ch;
  int i, j, k, index, length;

  strcpy(string, current_P->typeName);
  r = c = l = g = 0.0;

  length = strlen(string);
  for (k = 0; k < length; k++) {

    ch = (int)string[k];
    switch (ch) {
    case 'r':
    case 'R':
      r = Trc_Getval(string, k);
      break;
    case 'l':
    case 'L':
      l = Trc_Getval(string, k);
      break;
    case 'c':
    case 'C':
      c = Trc_Getval(string, k);
      break;
    case 'g':
    case 'G':
      g = Trc_Getval(string, k);
      break;
    }
  }
  current_P->r = r;
  current_P->l = l;
  current_P->c = c;
  current_P->g = g;

  return;
}
