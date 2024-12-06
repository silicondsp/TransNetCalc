
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
#include <string.h>

#include "cx.h"

float Getval(char string[], int index) {
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

  fprintf(stderr, "extracted string is:%s \n", valString);
  sscanf(valString, "%e", &value);

  return (value);
}

main() {
  float r, l, c, g;
  char string[200];
  char rstr[100], cstr[100], lstr[100], gstr[100];
  int ch;
  int i, j, k, index, length;

  r = c = l = g = 0.0;

  for (i = 0; i < 200; i++)
    string[i] = (char)NULL;
  fprintf(stderr, "Enter string \n");
  scanf("%s", string);
  length = strlen(string);
  for (k = 0; k < length; k++) {

    ch = (int)string[k];
    switch (ch) {
    case 'r':
    case 'R':
      r = Getval(string, k);
      break;
    case 'l':
    case 'L':
      l = Getval(string, k);
      break;
    case 'c':
    case 'C':
      c = Getval(string, k);
      break;
    case 'g':
    case 'G':
      g = Getval(string, k);
      break;
    }
  }
  fprintf(stderr, "rlcg %e %e %e %e \n", r, l, c, g);

} /* end main */
