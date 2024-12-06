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
#include "trantopcalc_tcl.h"
#include <kiss_fft.h>
#include <tcl.h>

#define RAND_LARGE 0x7fffffff
#define DEC

Tcl_ObjType *trc_vecObjType_P;
Tcl_ObjType *trc_nodeObjType_P;

#define TEXT_SIZE 10000

doubleVector_t *Trc_GetVector(Tcl_Obj *obj_P) {
  if (obj_P == NULL)
    return (NULL);
  return (obj_P->internalRep.otherValuePtr);
}

cxVector_t *Trc_GetCxVector(Tcl_Obj *obj_P) {
  if (obj_P == NULL)
    return (NULL);
  return (obj_P->internalRep.otherValuePtr);
}

short_t *Trc_GetPtr(Tcl_Obj *obj_P) {
  if (obj_P == NULL)
    return (NULL);
  return (obj_P->internalRep.otherValuePtr);
}
cxScalar_t *Trc_GetCxScalar(Tcl_Obj *obj_P) {
  if (obj_P == NULL)
    return (NULL);
  return (obj_P->internalRep.otherValuePtr);
}

void Trc_PrintVector(doubleVector_t *vec_P, char format[], FILE *fp,
                     int nolen) {
  int n;
  int i;
  char format1[256];

  strcpy(format1, "%");
  strcat(format1, format);
  strcat(format1, " ");
  // sprintf(format1,"\%%s \n",format);
  n = vec_P->length;
  if (nolen == 0)
    fprintf(fp, "%d\n", n);

  printf("FORMAT=%s FORMAT1=%s\n", format, format1);
  printf("Printing vector with n=%d elements\n", n);
  for (i = 0; i < n; i++) {

    if (!vec_P->transpose) {
      fprintf(fp, format1, vec_P->vector_P[i]);
      fprintf(fp, "\n");
    } else
      fprintf(fp, format1, vec_P->vector_P[i]);
  }
  if (vec_P->transpose)
    fprintf(fp, "\n");
}

void Trc_StripSpaces(char origText[], char desText[], int maxlength) {

  int len;
  int j;
  int i;
  int c;

  len = strlen(origText);
  // get rid of all spaces including tabs
  j = 0;
  for (i = 0; i < len; i++) {
    if (i > maxlength - 1)
      continue;
    c = origText[i];
    switch (c) {
    case ' ':

      break;
    case '\t':

      break;
    default:
      desText[j] = c;
      j++;

      break;
    }
  }
  desText[j] = NULL;
}
void Trc_PrintCxVector(cxVector_t *vec_P, char format[], FILE *fp, int twocol) {
  int n;
  int i;
  char format1[256];
  char format2[256];
  char complexNumber[1024];
  char buffer[1024];
  char tsign[3];

  strcpy(format1, "%");
  strcat(format1, format);

  n = vec_P->length;
  if (twocol == 0)
    fprintf(fp, "%d\n", n);

  printf("Printing Complex Vector (Index Real Imag) with n=%d elements t=%d\n",
         n, vec_P->transpose);
  for (i = 0; i < n; i++) {
    if (twocol == 0) {
      if (vec_P->vector_P[i].im < 0)
        strcpy(tsign, " ");
      else
        strcpy(tsign, "+");
      sprintf(format2, "%s %s %s i ", format1, tsign, format1);
      // printf("FORMAT=%s\n",format2);
      if (vec_P->transpose == 0) {
        // printf("%f %f\n",vec_P->vector_P[i].re,vec_P->vector_P[i].im);
        sprintf(complexNumber, format2, vec_P->vector_P[i].re,
                vec_P->vector_P[i].im);

        Trc_StripSpaces(complexNumber, buffer, 1024);
        fprintf(fp, "%s", buffer);

        fprintf(fp, "\n");
      } else {

        sprintf(complexNumber, format2, vec_P->vector_P[i].re, tsign,
                vec_P->vector_P[i].im);

        Trc_StripSpaces(complexNumber, buffer, 1024);
        fprintf(fp, "%s ", buffer);

        // fprintf(fp,format2,vec_P->vector_P[i].re,vec_P->vector_P[i].im);
      }

    } // end twocol
    else {
      sprintf(format2, "%s\t%s\n", format1, format1);
      fprintf(fp, format2, vec_P->vector_P[i].re, vec_P->vector_P[i].im);
    }
  }
  if (vec_P->transpose)
    fprintf(fp, "\n");
}

void Trc_PrintComplexScalar(cxScalar_t *x_P, char format[], FILE *fp) {
  int n;
  int i;
  char format1[256];
  char format2[256];
  char complexNumber[1024];
  char buffer[1024];
  char tsign[3];

  strcpy(format1, "%");
  strcat(format1, format);

  printf("Printing Complex Scalar\n");

  if (x_P->value.im < 0)
    strcpy(tsign, " ");
  else
    strcpy(tsign, "+");
  sprintf(format2, "%s %s %s i ", format1, tsign, format1);
  // printf("FORMAT=%s\n",format2);
  sprintf(complexNumber, format2, x_P->value.re, x_P->value.im);

  //	printf("complex number=%s\n",complexNumber);
  Trc_StripSpaces(complexNumber, buffer, 1024);
  fprintf(fp, "%s", buffer);
  fprintf(fp, "\n");
}

int Trc_VectorFromString(Tcl_Interp *interp, Tcl_Obj *obj_P) {
  char *data;
  Tcl_Obj *resultPtr;
  int rand, error;
  int range = 0;
  int i;
  int n;
  float x;
  int length;

  int argc, code;
  char **argv;
  char *list;

  doubleVector_t *vector;
  double *vec;

  list = Tcl_GetStringFromObj(obj_P, &length);
  code = Tcl_SplitList(interp, list, &argc, &argv);
  printf("VectorCmd()=%s\n", list);
  n = argc;

  vec = (double *)malloc(n * sizeof(double));
  vector = (doubleVector_t *)malloc(1 * sizeof(doubleVector_t));
  vector->length = n;
  vector->transpose = 0;
  vector->vector_P = vec;
  vector->type = TRC_TCL_TYPE_REAL_VECTOR;

  for (i = 0; i < argc; i++) {
    printf("%s\n", argv[i]);

    sscanf(argv[i], "%f", &x);
    vec[i] = x;
  }
  obj_P->internalRep.otherValuePtr = vector;

  //	obj_P->typePtr=Tcl_GetObjType(VECTOR_TYPE_NAME);

#if 0000
        vec=(float)calloc(objc-1, sizeof(float);
	vector=(doubleVector_t *)calloc(1,sizeof(doubleVector_t));
	
	
	if(!vec || !vector) return TCL_ERROR;
	n=objc-1;
	vector->vector_P=vec;
	vector->length=n;
	
	for(i=0; i<n i++) {
    if (Tcl_GetFloatFromObj(interp, objv[i + 1], &x)) != TCL_OK) { return TCL_ERROR;}
    vector->vector_P[i] = x;
	}

#endif	
	
//	resultPtr = Tcl_GetObjResult(interp);
//	Tcl_SetIntObj(resultPtr, argc);
//	return TCL_OK;
        return 0;
}

#if 1
int Trc_VectorFromFile(Tcl_Interp *interp, Tcl_Obj *obj_P, char *fileName) {
  int i;
  int n;
  float x;
  int length;

  doubleVector_t *vector;
  double *vec;
  FILE *fp;

  fp = fopen(fileName, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open file:%s for reading.\n", fileName);
    return (1);
  }
  fscanf(fp, "%d", &n);

  vec = (double *)calloc(n, sizeof(double));

  if (!vec) {
    fprintf(stderr, "Could not allocate space for vector length=%d.\n", n);
    return (2);
  }
  vector = (doubleVector_t *)malloc(1 * sizeof(doubleVector_t));
  vector->length = n;
  vector->transpose = 0;
  vector->vector_P = vec;
  vector->type = TRC_TCL_TYPE_REAL_VECTOR;
  obj_P->internalRep.otherValuePtr = vector;

  for (i = 0; i < n; i++) {
    if (feof(fp))
      continue;
    fscanf(fp, "%f", &x);
    vec[i] = x;
  }
  fclose(fp);
  return (0);
}

#endif

Tcl_RegExp imagPartRegExp;
Tcl_RegExp imagPartRegExp2;

int Trc_InitRegExp(Tcl_Interp *interp) {

  imagPartRegExp =
      Tcl_RegExpCompile(interp, "[+-][0-9]*_?[0-9]+([eE][-+]?[0-9]+)?i");
  imagPartRegExp2 =
      Tcl_RegExpCompile(interp, "[-+]?[0-9]*_?[0-9]?([eE][-+]?[0-9]+)?i");

  if (imagPartRegExp == NULL) {
    fprintf(stderr, "Bad Regular expression for imag part\n");
    return TCL_ERROR;
  }
}
//+++++++++
int Trc_GetRealImagFromString(Tcl_Interp *interp, char theText[], float *x_P,
                              float *y_P) {
  int i, k;
  int n;
  float x, y;
  int length;

  char *imText;
  char *imTextEnd;

  char *origText;
  int matched;
  char realPart[100];
  char imagPart[100];
  long index1, index2;
  int c;
  int len;
  Tcl_Obj *cxObj_P;
  Tcl_RegExpInfo info;
  Tcl_RegExpIndices match;
  char *matchString;
  Tcl_Obj *matchObj_P;

#if 0000

  typedef struct Tcl_RegExpInfo {
    int nsubs;
    Tcl_RegExpIndices *matches;
    long extendStart;
  } Tcl_RegExpInfo;

  typedef struct Tcl_RegExpIndices {
    long start;
    long end;
  } Tcl_RegExpIndices;

#endif

//	imagPartRegExp=Tcl_RegExpCompile(interp,"[-+][0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?i");
#if 000

  imagPartRegExp =
      Tcl_RegExpCompile(NULL, "[+-][0-9]*_?[0-9]+([eE][-+]?[0-9]+)?i");
  imagPartRegExp2 =
      Tcl_RegExpCompile(NULL, "[-+]?[0-9]*_?[0-9]?([eE][-+]?[0-9]+)?i");

  if (imagPartRegExp == NULL) {
    fprintf(stderr, "Bad Regular expression for imag part\n");
    return TCL_ERROR;
  }
#endif

  //-----------

  length = strlen(theText);
  for (k = 0; k < length; k++) {
    c = theText[k];
    if (c == '.')
      theText[k] = '_';
  }
  printf("TheText=%s\n", theText);

  strcpy(imagPart, "0");
  strcpy(realPart, "0");

  matched = Tcl_RegExpExec(interp, imagPartRegExp, theText, theText);
  if (!matched) {
    matched = Tcl_RegExpExec(interp, imagPartRegExp2, theText, theText);
    Tcl_RegExpGetInfo(imagPartRegExp2, &info);
  } else {
    Tcl_RegExpGetInfo(imagPartRegExp, &info);
  }
  for (k = 0; k < length; k++) {
    c = theText[k];
    if (c == '_')
      theText[k] = '.';
  }
  if (matched) {

    match = info.matches[0];
    printf("start=%d end=%d \n", (int)match.start, (int)match.end);

    if (match.start) {
      printf("Found imaginary starting at->%s", &theText[match.start]);
      if (imTextEnd) {
        printf("<-ending at->%s\n", &theText[match.end]);
      } else {
        printf("\n");
      }
    }
    {

      index1 = match.end - match.start;
      for (k = 0; k < index1; k++)
        imagPart[k] = theText[match.start + k];
      imagPart[k] = NULL;
    }
    printf("THE IMAG PART IS->%s<- (%d)\n", imagPart, index1);

    if (match.start) {
      strcpy(realPart, theText);
      realPart[match.start] = NULL;
    } else
      strcpy(realPart, "0");
    printf("THE REAL PART IS->%s<- (%d)\n", realPart, index1);

    // get rid of i
    len = strlen(imagPart);
    imagPart[len - 1] = NULL;

    // imagPart[len+1]=NULL;
    printf("CORRECTED  IMAG PART IS->%s\n", imagPart);

  } else {
    strcpy(realPart, theText);
    strcpy(imagPart, "0");
  }

  printf("String results:%s<---->%s\n", realPart, imagPart);
  sscanf(realPart, "%f", x_P);
  sscanf(imagPart, "%f", y_P);
}

//+++++++++
#define REG_EXPRESSION_IMAG "[+-][0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?i"

int Trc_CxVectorFromFile(Tcl_Interp *interp, Tcl_Obj *obj_P, char *fileName) {
  int i, k;
  int n;
  float x, y;
  int length;

  Tcl_RegExp imagPartRegExp;
  Tcl_RegExp imagPartRegExp2;

  char *imText;
  char *imTextEnd;
  char theText[200];

  char *origText;
  int matched;
  char realPart[100];
  char imagPart[100];
  long index1, index2;
  int c;
  int len;
  Tcl_Obj *cxObj_P;
  Tcl_RegExpInfo info;
  Tcl_RegExpIndices match;
  char *matchString;
  Tcl_Obj *matchObj_P;
  int status;

  cxVector_t *vector;
  cx_t *vec;
  FILE *fp;

  fp = fopen(fileName, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open file:%s for reading.\n", fileName);
    return (1);
  }
  fscanf(fp, "%d", &n);

  vec = (cx_t *)calloc(n, sizeof(cx_t));

  if (!vec) {
    fprintf(stderr, "Could not allocate space for vector length=%d.\n", n);
    return (2);
  }
  vector = (cxVector_t *)malloc(1 * sizeof(cxVector_t));
  vector->length = n;
  vector->transpose = 0;
  vector->vector_P = vec;
  vector->type = TRC_TCL_TYPE_COMPLEX_VECTOR;
  obj_P->internalRep.otherValuePtr = vector;

  for (i = 0; i < n; i++) {
    if (feof(fp))
      continue;
    fscanf(fp, "%s", theText);

    status = Trc_GetRealImagFromString(interp, theText, &x, &y);
    // if(status==TCL_ERROR) return(TCL_ERROR);
    vec[i].re = x;
    vec[i].im = y;

    //-----------
  }
  fclose(fp);
  return (0);
}

void Trc_StringFromObject(Tcl_Obj *obj_P) {

  char *text;
  char fval[100];
  int i;
  int n;
  int textSize = 0;
  doubleVector_t *vector;

  printf("I am in VecUpdateStringProc\n");

  text = (char *)malloc(TEXT_SIZE * sizeof(char));
  vector = (doubleVector_t *)obj_P->internalRep.otherValuePtr;

  if (vector == NULL)
    return;
  n = vector->length;
  for (i = 0; i < n; i++) {
    if (textSize > TEXT_SIZE - 100)
      continue;
    sprintf(fval, "%f ", vector->vector_P[i]);
    strcat(text, fval);
    textSize += strlen(fval);
  }

  Tcl_SetStringObj(obj_P, text, strlen(text) + 1);
}

int Trc_CxVectorFromString(Tcl_Interp *interp, Tcl_Obj *obj_P) {
  char *data;
  Tcl_Obj *resultPtr;
  int rand, error;
  int range = 0;
  int i, j, ii;
  int n;
  float x, y;
  int length;

  int argc, code;
  char **argv;
  char *list;
  Tcl_RegExp imagPartRegExp;

  char *imText;
  char *imTextEnd;
  char theText[200];
  char *origText;
  int matched;
  char realPart[100];
  char imagPart[100];
  long index1, index2;
  int c;
  int len;

  cxVector_t *vector;
  cx_t *vec;

  list = Tcl_GetStringFromObj(obj_P, &length);
  code = Tcl_SplitList(interp, list, &argc, &argv);
  printf("VectorCmd()=%s\n", list);
  n = argc;

  vec = (cx_t *)malloc(n * sizeof(cx_t));
  vector = (cxVector_t *)malloc(1 * sizeof(cxVector_t));
  vector->length = n;
  vector->transpose = 0;
  vector->vector_P = vec;
  vector->type = TRC_TCL_TYPE_COMPLEX_VECTOR;

  for (i = 0; i < argc; i++) {
    /*
     * Complex examples:
     */

    printf("%s\n", argv[i]);
    origText = argv[i];
    len = strlen(origText);
    // get rid of all spaces including tabs
    j = 0;
    for (ii = 0; ii < len; ii++) {
      if (ii > 200 - 1)
        continue;
      c = origText[ii];
      switch (c) {
      case ' ':

        break;
      case '\t':

        break;
      default:
        theText[j] = c;
        j++;

        break;
      }
    }
    theText[j] = NULL;
    printf("Removed spaces->%s<-\n", theText);
    strcpy(imagPart, "0");
    strcpy(realPart, "0");

    //((((((((((
    Trc_GetRealImagFromString(interp, theText, &x, &y);

    vec[i].re = x;
    vec[i].im = y;
    //   printf("results: %f\t%f\n",vec[i].re,vec[i].im);
    //  printf("results: %f\t%f\n",x,y);
    // sscanf(argv[i],"%f",&vec[i]);
  }
  obj_P->internalRep.otherValuePtr = vector;

  return 0;
}

static int VecSetFromAnyProc(Tcl_Interp *interp, Tcl_Obj *obj_P) {}

static void VecUpdateStringProc(Tcl_Obj *obj_P) {}

static void VecDupIntRepProc(Tcl_Obj *srcObj_P, Tcl_Obj *dupObj_P) {

  printf("I am in VecDupIntRepProc\n");

  // dupObj_P->typePtr=Tcl_GetObjType(VECTOR_TYPE_NAME);
  // dupObj_P->internalRep.otherValuePtr = srcObj_P->internalRep.otherValuePtr ;
}

static int VecFreeIntRepProc(Tcl_Obj *obj_P) {
  cxVector_t *cxvec_P;
  doubleVector_t *vec_P;
  short_t *type_P;
  cxScalar_t *sc;

  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  type_P = Trc_GetPtr(obj_P);
  switch (type_P->type) {
  case TRC_TCL_TYPE_REAL_VECTOR:
    //	      printf("Free real vector\n");
    vec_P = Trc_GetVector(obj_P);
    if (vec_P->vector_P)
      free(vec_P->vector_P);
    if (vec_P)
      free(vec_P);

    break;
  case TRC_TCL_TYPE_COMPLEX_VECTOR:
    //	      printf("Free complex vector\n");
    cxvec_P = Trc_GetCxVector(obj_P);
    if (cxvec_P->vector_P)
      free(cxvec_P->vector_P);
    if (cxvec_P)
      free(cxvec_P);

    break;

  case TRC_TCL_TYPE_COMPLEX_SCALAR:
    //	      printf("Free complex scalar\n");
    cxvec_P = Trc_GetCxScalar(obj_P);
    if (cxvec_P)
      free(cxvec_P);

    break;

  default:
    printf("Type=%d\n", type_P->type);
    fprintf(stderr, "Error: Variable not a vector or matrix");
    return TCL_ERROR;
  }
}

int NodeSetFromAnyProc(Tcl_Interp *interp, Tcl_Obj *obj_P) {}

void NodeUpdateStringProc(Tcl_Obj *obj_P) {}

void NodeDupIntRepProc(Tcl_Obj *srcObj_P, Tcl_Obj *dupObj_P) {

  printf("I am in NodeDupIntRepProc\n");

  // dupObj_P->typePtr=Tcl_GetObjType(VECTOR_TYPE_NAME);
  // dupObj_P->internalRep.otherValuePtr = srcObj_P->internalRep.otherValuePtr ;
}

void NodeFreeIntRepProc(Tcl_Obj *obj_P) {
  printf("I am in NodeFreeIntRepProc\n");
}

void Trc_VectorRegisterTypes(Tcl_Interp *interp) {
  static char name[100];
  static Tcl_ObjType vectorObjType;
  static Tcl_ObjType nodeObjType;
  Tcl_ObjType *objType_P;

  strcpy(name, VECTOR_TYPE_NAME);

  vectorObjType.name = VECTOR_TYPE_NAME;
  vectorObjType.freeIntRepProc = VecFreeIntRepProc;
  vectorObjType.dupIntRepProc = VecDupIntRepProc;
  vectorObjType.updateStringProc = VecUpdateStringProc;
  vectorObjType.setFromAnyProc = VecSetFromAnyProc;

  Tcl_RegisterObjType(&vectorObjType);
  printf("Registered obj type:%s\n", vectorObjType.name);

  nodeObjType.name = NODE_TYPE_NAME;
  nodeObjType.freeIntRepProc = NodeFreeIntRepProc;
  nodeObjType.dupIntRepProc = NodeDupIntRepProc;
  nodeObjType.updateStringProc = NodeUpdateStringProc;
  nodeObjType.setFromAnyProc = NodeSetFromAnyProc;

  Tcl_RegisterObjType(&nodeObjType);
  printf("Registered obj type:%s\n", nodeObjType.name);

  // if(Tcl_AppendAllObjTypes(interp,&vectorObjType)!= TCL_OK)
  //    fprintf(stderr,"Problem appending \n");

  //       objType_P=Tcl_GetObjType(VECTOR_TYPE_NAME);
  //	krn_vecObjType_P=objType_P;
  //	objType_P=Tcl_GetObjType("int");

  //	printf("In Tcl_Vector got obj type this is the
  //name:%s\n",krn_vecObjType_P->name);

  trc_vecObjType_P = &vectorObjType;
  trc_nodeObjType_P = &nodeObjType;
}

/*
 * Use following to only register network types (node etc).
 */
void Trc_NetworkRegisterTypes(Tcl_Interp *interp) {
  static char name[100];

  static Tcl_ObjType vectorObjType;
  static Tcl_ObjType nodeObjType;
  Tcl_ObjType *objType_P;

  strcpy(name, VECTOR_TYPE_NAME);

  nodeObjType.name = NODE_TYPE_NAME;
  nodeObjType.freeIntRepProc = NodeFreeIntRepProc;
  nodeObjType.dupIntRepProc = NodeDupIntRepProc;
  nodeObjType.updateStringProc = NodeUpdateStringProc;
  nodeObjType.setFromAnyProc = NodeSetFromAnyProc;

  Tcl_RegisterObjType(&nodeObjType);
  printf("Registered obj type:%s\n", nodeObjType.name);

  // if(Tcl_AppendAllObjTypes(interp,&vectorObjType)!= TCL_OK)
  //    fprintf(stderr,"Problem appending \n");

  //       objType_P=Tcl_GetObjType(VECTOR_TYPE_NAME);
  //	krn_vecObjType_P=objType_P;
  //	objType_P=Tcl_GetObjType("int");

  //	printf("In Tcl_Vector got obj type this is the
  //name:%s\n",krn_vecObjType_P->name);

  trc_nodeObjType_P = &nodeObjType;
}
