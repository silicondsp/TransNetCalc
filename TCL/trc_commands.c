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
#include "trantopcalc.h"
#include "trantopcalc_tcl.h"
#include "trc_tclcommands.h"
#include <tcl.h>

/*
 *
 * The Vector Commands C  procedure.
 */

#define DEFAULT_SEED 36998711
//#define PI 3.14159265358979323

int Trc_CheckObjType(Tcl_Obj *obj_P) {
  if (obj_P->typePtr == NULL)
    return (1);
  if (strcmp(obj_P->typePtr->name, VECTOR_TYPE_NAME) == 0)
    return (0);
  else
    return (1);
}

int Trc_CheckObjNodeType(Tcl_Obj *obj_P) {
  if (obj_P->typePtr == NULL)
    return (1);
  if (strcmp(obj_P->typePtr->name, NODE_TYPE_NAME) == 0)
    return (0);
  else
    return (1);
}

/*
 * Vector Commands
 */
int Trc_VectorObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                     Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  int rand, error;
  int range = 0;
  int i;
  int n;
  double x;
  int length;

  int argc, code;
  char **argv;
  char *list;

  doubleVector_t *vector;
  double *vec;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  doubleVector_t *vec_P;

  if (objc < 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "?vector?");
    return TCL_ERROR;
  }
  list = Tcl_GetStringFromObj(objv[2], &length);

  varStrName = Tcl_GetStringFromObj(objv[1], &varLength);

  //	printf("Vector command variable name=%s\n",varStrName);

  obj_P = Tcl_NewStringObj(list, strlen(list) + 1);
  obj_P->typePtr = objType_P;

  Trc_VectorFromString(interp, obj_P);

  // trc_vecObjType_P->setFromAnyProc(interp,obj_P);

  varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
  obj_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, varNameObj_P, NULL, obj_P, TCL_NAMESPACE_ONLY);

  resultPtr = Tcl_GetObjResult(interp);
  Tcl_SetStringObj(resultPtr, list, strlen(list) + 1);
  //	Tcl_SetIntObj(resultPtr, argc);
  return TCL_OK;
}

/*
 * Vector Commands
 * syntax: vrd ?vector? fileName
 */
int Trc_ReadVectorObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                         Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  int rand, error;
  int range = 0;
  int i;
  int n;
  double x;
  int length;

  int argc, code;
  char **argv;
  char *list;

  doubleVector_t *vector;
  double *vec;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  doubleVector_t *vec_P;
  char *fileName;

  if (objc < 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "vrd Variable fileName");
    return TCL_ERROR;
  }
  fileName = Tcl_GetStringFromObj(objv[2], &length);

  printf("vrd fileName=%s\n", fileName);

  varStrName = Tcl_GetStringFromObj(objv[1], &varLength);

  //	printf("Vector command variable name=%s\n",varStrName);

  obj_P = Tcl_NewStringObj(varStrName, varLength);
  obj_P->typePtr = objType_P;

  if (Trc_VectorFromFile(interp, obj_P, fileName)) {
    return TCL_ERROR;
  }

  // trc_vecObjType_P->setFromAnyProc(interp,obj_P);

  varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
  obj_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, varNameObj_P, NULL, obj_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax: vcxrd ?vector? fileName
 */
int Trc_ReadCxVectorObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                           Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  int rand, error;
  int range = 0;
  int i;
  int n;
  double x;
  int length;

  int argc, code;
  char **argv;
  char *list;

  doubleVector_t *vector;
  double *vec;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  doubleVector_t *vec_P;
  char *fileName;

  if (objc < 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "vcxrd Variable fileName");
    return TCL_ERROR;
  }
  fileName = Tcl_GetStringFromObj(objv[2], &length);

  printf("vrd fileName=%s\n", fileName);

  varStrName = Tcl_GetStringFromObj(objv[1], &varLength);

  //	printf("Vector command variable name=%s\n",varStrName);

  obj_P = Tcl_NewStringObj(varStrName, varLength);
  obj_P->typePtr = objType_P;

  if (Trc_CxVectorFromFile(interp, obj_P, fileName)) {
    return TCL_ERROR;
  }

  // trc_vecObjType_P->setFromAnyProc(interp,obj_P);

  varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
  obj_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, varNameObj_P, NULL, obj_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

int Trc_PrintRealVector(Tcl_Obj *obj_P, char format[], FILE *fp, int nolen) {
  doubleVector_t *vec_P;
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetVector(obj_P);
  Trc_PrintVector(vec_P, format, fp, nolen);
}

int Trc_PrintComplexVector(Tcl_Obj *obj_P, char format[], FILE *fp,
                           int twocol) {
  cxVector_t *vec_P;
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetCxVector(obj_P);
  Trc_PrintCxVector(vec_P, format, fp, twocol);
}

int Trc_PrintComplexScalarObj(Tcl_Obj *obj_P, char format[], FILE *fp) {
  cxScalar_t *x_P;
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  x_P = Trc_GetCxScalar(obj_P);
  Trc_PrintComplexScalar(x_P, format, fp);
}

int Trc_VectorObjPrintCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                          Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  Tcl_Obj *obj_P;
  short_t *type_P;
  char theFormat[256];
  char *format;
  char *fileName;
  int length;
  FILE *fp;
  char *option;
  int nolen = 0;
  int twocol = 0;
  int objcwopt;

  objcwopt = objc;
  if (objc < 2 || objc > 6) {
    Tcl_WrongNumArgs(interp, 1, objv, "syntax vpr $vector");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  type_P = Trc_GetPtr(obj_P);
  //	fprintf(stderr,"Error: Variable not a vector or matrix (%x)\n",type_P);
  /*
   * check for an optin at the end
   */
  option = Tcl_GetStringFromObj(objv[objc - 1], &length);
  // printf("Option =%s\n",option);
  if (length > 1) {
    if (option[0] == '-') {
      // printf("Found Option =%s\n",option);
      if (strcmp(option, "-nolen") == 0) {
        nolen = TRUE;
        // printf("nolen is TRUE\n",option);
      } else if (strcmp(option, "-twocol") == 0) {
        twocol = TRUE;
        // printf("twocol is TRUE\n",option);
      }
      objcwopt = objc - 1;
    }
  }

  switch (objcwopt) {
  case 2:
    strcpy(theFormat, "e ");
    fp = stdout;

    break;

  case 3:
    format = Tcl_GetStringFromObj(objv[2], &length);
    if (length > 256) {
      fprintf(stderr, "Error: Format string too long\n");
      return TCL_ERROR;
    }
    strncpy(theFormat, format, length + 1);
    fp = stdout;
    break;

  case 4:
    format = Tcl_GetStringFromObj(objv[2], &length);
    if (length > 256) {
      fprintf(stderr, "Error: Format string too long\n");
      return TCL_ERROR;
    }
    strncpy(theFormat, format, length + 1);
    fileName = Tcl_GetStringFromObj(objv[3], &length);
    fp = fopen(fileName, "w");
    if (!fp) {
      fprintf(stderr, "Error: Could not open file for writing file name=%s\n",
              fileName);
      return TCL_ERROR;
    }

    break;

  default:

    break;
  }

  switch (type_P->type) {
  case TRC_TCL_TYPE_REAL_VECTOR:
    Trc_PrintRealVector(obj_P, theFormat, fp, nolen);
    break;
  case TRC_TCL_TYPE_COMPLEX_VECTOR:
    Trc_PrintComplexVector(obj_P, theFormat, fp, twocol);
    break;

  case TRC_TCL_TYPE_COMPLEX_SCALAR:
    Trc_PrintComplexScalarObj(obj_P, theFormat, fp);
    break;

  default:
    printf("Type=%d\n", type_P->type);
    fprintf(stderr, "Error: Variable not a vector or matrix");
    return TCL_ERROR;
  }
  if (fp != stdout)
    fclose(fp);

  return TCL_OK;
}

/*
 * Vector Commands
 */
int Trc_VectorCxObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                       Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  int rand, error;
  int range = 0;
  int i;
  int n;
  double x;
  int length;

  int argc, code;
  char **argv;
  char *list;

  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  if (objc < 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "?vector?");
    return TCL_ERROR;
  }
  list = Tcl_GetStringFromObj(objv[2], &length);

  varStrName = Tcl_GetStringFromObj(objv[1], &varLength);

  obj_P = Tcl_NewStringObj(list, strlen(list) + 1);
  obj_P->typePtr = objType_P;

  Trc_CxVectorFromString(interp, obj_P);

  varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
  obj_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, varNameObj_P, NULL, obj_P, TCL_NAMESPACE_ONLY);

  resultPtr = Tcl_GetObjResult(interp);
  Tcl_SetStringObj(resultPtr, list, strlen(list) + 1);
  //	Tcl_SetIntObj(resultPtr, argc);
  return TCL_OK;
}

int Trc_VectorCxObjPrintCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                            Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  int rand, error;
  int range = 0;
  int i;
  int n;
  double x;
  int length;

  int argc, code;
  char **argv;
  char *list;

  doubleVector_t *vector;
  double *vec;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  cxVector_t *vec_P;

  if (objc < 2 || objc > 3) {
    Tcl_WrongNumArgs(interp, 1, objv, "syntax vpr $vector");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetCxVector(obj_P);
  Trc_PrintCxVector(vec_P, "%f", stdout, 0);

  return TCL_OK;
}

/*
 * Set an element in a vector
 * syntax: vset $Vector $value $index
 */

int Trc_SetVectorElementReal(ClientData clientData, Tcl_Interp *interp,
                             int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;

  doubleVector_t *vec_P;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int len;
  int i, j, k;
  int K;
  int status;
  double value;

  if (objc < 2)
    return TCL_ERROR;

  obj_P = objv[1];
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetVector(obj_P);

  switch (objc) {

  case 2:
  case 3:
    fprintf(stderr, "vset $Vector $value $index \n");
    return TCL_ERROR;
    break;
  case 4:

    Tcl_GetDoubleFromObj(interp, objv[2], &value);
    Tcl_GetIntFromObj(interp, objv[3], &i);
    if (i >= vec_P->length || i < 0) {
      fprintf(stderr, "Index out of range\n");
      return TCL_ERROR;
    }

    vec_P->vector_P[i] = value;

    break;

  default:
    fprintf(stderr, "Too many  variables\n");
    return TCL_ERROR;
    break;
  }

  return TCL_OK;
}

/*
 * Get a vector element
 * syntax: vget $Vector  $i ?value?
 */

int Trc_GetVectorElementReal(ClientData clientData, Tcl_Interp *interp,
                             int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  doubleVector_t *vec_P;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  int len;
  int i, j, k;
  int status;
  double value;

  if (objc < 2)
    return TCL_ERROR;

  obj_P = objv[1];
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetVector(obj_P);

  switch (objc) {

  case 2:
  case 3:

    fprintf(stderr, "vget $Vector  $i  ?value?\n");
    return TCL_ERROR;
    break;
  case 4:

    Tcl_GetIntFromObj(interp, objv[2], &i);
    if (i >= vec_P->length || i < 0) {
      fprintf(stderr, "Index out of range\n");
      return TCL_ERROR;
    }

    value = vec_P->vector_P[i];

    varStrName = Tcl_GetStringFromObj(objv[3], &varLength);
    varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P = Tcl_NewObj();

    Tcl_SetDoubleObj(objVar_P, value);
    Tcl_ObjSetVar2(interp, varNameObj_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    fprintf(stderr, "Too many  variables\n");
    return TCL_ERROR;
    break;
  }

  return TCL_OK;
}

/*
 * Create Real Vector
 * syntax: vcr ?vector? $length  [$std $seed]
 */

int Trc_VectorCreate(ClientData clientData, Tcl_Interp *interp, int objc,
                     Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  doubleVector_t *vec_P;

  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  int len;
  int i;
  int K;
  int length;
  int status;
  char *value;
  int valLength;
  int seed;
  float std;

  switch (objc) {

  case 2:
    fprintf(stderr,
            "Missing vector variable: vcr ?vector? $length  [$std $seed]\n");
    return TCL_ERROR;
    break;
  case 3:

    value = Tcl_GetStringFromObj(objv[2], &valLength);
    sscanf(value, "%d", &length);

    vec_P = Dsp_AllocateVector(length);
    if (vec_P == NULL)
      return TCL_ERROR;

    varStrName = Tcl_GetStringFromObj(objv[1], &varLength);
    varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P = Tcl_NewObj();
    objVar_P->internalRep.otherValuePtr = vec_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, varNameObj_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  case 5:
  case 4:
#if 0000
    value = Tcl_GetStringFromObj(objv[2], &valLength);
    sscanf(value, "%d", &m);
    value = Tcl_GetStringFromObj(objv[3], &valLength);
    sscanf(value, "%d", &n);
#endif
    value = Tcl_GetStringFromObj(objv[2], &valLength);
    Tcl_GetInt(interp, value, &length);

    vec_P = Dsp_AllocateVector(length);
    if (vec_P == NULL)
      return TCL_ERROR;

    value = Tcl_GetStringFromObj(objv[3], &valLength);
    sscanf(value, "%f", &std);
    if (objc == 6) {
      value = Tcl_GetStringFromObj(objv[4], &valLength);
      sscanf(value, "%d", &seed);
    } else {
      seed = DEFAULT_SEED;
    }

    Dsp_GenGaussVector(vec_P, std, 0, (long)seed);

    varStrName = Tcl_GetStringFromObj(objv[1], &varLength);
    varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P = Tcl_NewObj();
    objVar_P->internalRep.otherValuePtr = vec_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, varNameObj_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    fprintf(stderr, "Too many  variables\n");
    return TCL_ERROR;
    break;
  }

  return TCL_OK;
}

/*
 * Create Real Vector
 * syntax: vcxcr ?vector? $length  [$std $seed]
 */

int Trc_VectorComplexCreate(ClientData clientData, Tcl_Interp *interp, int objc,
                            Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  cxVector_t *vec_P;

  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  int len;
  int i;
  int K;
  int length;
  int status;
  char *value;
  int valLength;
  int seed;
  float std;

  switch (objc) {

  case 2:
    fprintf(stderr, "Missing complex vector variable: vcxcr ?vector? $length  "
                    "[$std $seed]\n");
    return TCL_ERROR;
    break;
  case 3:

    value = Tcl_GetStringFromObj(objv[2], &valLength);
    sscanf(value, "%d", &length);

    vec_P = Dsp_AllocateCxVector(length);
    if (vec_P == NULL)
      return TCL_ERROR;

    varStrName = Tcl_GetStringFromObj(objv[1], &varLength);
    varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P = Tcl_NewObj();
    objVar_P->internalRep.otherValuePtr = vec_P;
    objVar_P->typePtr = trc_vecObjType_P;

    Tcl_ObjSetVar2(interp, varNameObj_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  case 5:
  case 4:
#if 0000
    value = Tcl_GetStringFromObj(objv[2], &valLength);
    sscanf(value, "%d", &m);
    value = Tcl_GetStringFromObj(objv[3], &valLength);
    sscanf(value, "%d", &n);
#endif
    value = Tcl_GetStringFromObj(objv[2], &valLength);
    Tcl_GetInt(interp, value, &length);

    vec_P = Dsp_AllocateCxVector(length);
    if (vec_P == NULL)
      return TCL_ERROR;

    value = Tcl_GetStringFromObj(objv[3], &valLength);
    sscanf(value, "%f", &std);
    if (objc == 6) {
      value = Tcl_GetStringFromObj(objv[4], &valLength);
      sscanf(value, "%d", &seed);
    } else {
      seed = DEFAULT_SEED;
    }

    Dsp_GenGaussComplexVector(vec_P, std, 0, (long)seed);

    varStrName = Tcl_GetStringFromObj(objv[1], &varLength);
    varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P = Tcl_NewObj();
    objVar_P->internalRep.otherValuePtr = vec_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, varNameObj_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    fprintf(stderr, "Too many  variables\n");
    return TCL_ERROR;
    break;
  }

  return TCL_OK;
}

//+++

/*
 * syntax: vdot $x $y ?z?
 * note x and y are real or complex vectors
 * z is complex or real. If both x and y are real then z is real
 */

int Trc_VectorObjDotProductCmd(ClientData clientData, Tcl_Interp *interp,
                               int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  doubleVector_t *vx_P;
  doubleVector_t *vy_P;

  cxVector_t *vxx_P;
  cxVector_t *vyx_P;

  double dotProductReal;
  cxScalar_t *dotProductComplex_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax vdot $x $y z where C=xTy");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[2];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  if (typeX_P->type == TRC_TCL_TYPE_REAL_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_REAL_VECTOR) {

    vx_P = Trc_GetVector(objX_P);
    vy_P = Trc_GetVector(objY_P);

    if (vx_P->length != vy_P->length || vx_P->transpose) {
      fprintf(
          stderr,
          "x vector and y vector dimensions not compatible for dot product\n");
      return TCL_ERROR;
    }
    Dsp_DotRealReal(vx_P, vy_P, &dotProductReal);
    varStrName = Tcl_GetStringFromObj(objv[3], &varLength);
    varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    Tcl_SetDoubleObj(objVar_P, dotProductReal);
    Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    return TCL_OK;
  }
  if (typeX_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR) {

    vxx_P = Trc_GetCxVector(objX_P);
    vyx_P = Trc_GetCxVector(objY_P);
    dotProductComplex_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    dotProductComplex_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;

    // z=xTy A
    if (vxx_P->length != vyx_P->length || vxx_P->transpose) {
      fprintf(
          stderr,
          "x vector and y vector dimensions not compatible for dot product\n");
      return TCL_ERROR;
    }
    Dsp_DotComplexComplex(vxx_P, vyx_P, dotProductComplex_P);
    varStrName = Tcl_GetStringFromObj(objv[3], &varLength);
    varNameObj_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = dotProductComplex_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    return TCL_OK;
  }
  if (typeX_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_REAL_VECTOR) {
  }
  if (typeX_P->type == TRC_TCL_TYPE_REAL_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR) {
  }

  fprintf(stderr,
          "NOT SUPPORTED X and Y must both be real or complex vectors \n");
  return TCL_ERROR;
}

/*
 * syntax: cx $re $im ?z?
 * note re and im are double values
 * z is complex
 */

int Trc_ScalarComplexObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                            Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double realPart;
  double imagPart;

  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  int status;

#if 0000
  Tcl_ObjType *objType_P;
  // objType_P=(Tcl_ObjType *)clientData;
  objType_P = Tcl_GetObjType(VECTOR_TYPE_NAME);
  printf("krn %x  client %x\n", clientData, trc_vecObjType_P);
  printf("got obj type this is the name:%s\n", objType_P->name);
  printf("HEEEEEEEEEEEEEEERRRRRRRRRRRREEEEEE\n");

#endif

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax cx $re $im ?z?");
    return TCL_ERROR;
  }

  Tcl_GetDoubleFromObj(interp, objv[1], &realPart);
  Tcl_GetDoubleFromObj(interp, objv[2], &imagPart);

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));

  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = realPart;
  z_P->value.im = imagPart;

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax cxpolar $m $angle ?z?
 * note re and im are double values
 * z is complex
 */

int Trc_ScalarComplexFromPolarObjCmd(ClientData clientData, Tcl_Interp *interp,
                                     int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double mag;
  double angle;

  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax cxpolar $m $angle ?z?");
    return TCL_ERROR;
  }

  Tcl_GetDoubleFromObj(interp, objv[1], &mag);
  Tcl_GetDoubleFromObj(interp, objv[2], &angle);

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));

  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;

  angle = angle * PI / 180.0;

  z_P->value.re = mag * cos(angle);
  z_P->value.im = mag * sin(angle);

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax: cxmult $x $y ?z?
 */

int Trc_ScalarComplexMultObjCmd(ClientData clientData, Tcl_Interp *interp,
                                int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double realPart;
  double imagPart;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax cxmult $re $im ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[2];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR ||
      typeY_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxmult variables are not complex scalars\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);
  y_P = Trc_GetCxScalar(objY_P);

  realPart = x_P->value.re * y_P->value.re - x_P->value.im * y_P->value.im;
  imagPart = x_P->value.im * y_P->value.re + x_P->value.re * y_P->value.im;

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = realPart;
  z_P->value.im = imagPart;

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax: cxadd $x $y ?z?
 */

int Trc_ScalarComplexAddObjCmd(ClientData clientData, Tcl_Interp *interp,
                               int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double realPart;
  double imagPart;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax cxadd $x $y ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[2];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR ||
      typeY_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxadd variables are not complex scalars\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);
  y_P = Trc_GetCxScalar(objY_P);

  realPart = x_P->value.re + y_P->value.re;
  imagPart = x_P->value.im + y_P->value.im;

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = realPart;
  z_P->value.im = imagPart;

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax cxsub $x $y ?z?
 */

int Trc_ScalarComplexSubObjCmd(ClientData clientData, Tcl_Interp *interp,
                               int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double realPart;
  double imagPart;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax cxsub $x $y ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[2];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR ||
      typeY_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxsub variables are not complex scalars\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);
  y_P = Trc_GetCxScalar(objY_P);

  realPart = x_P->value.re - y_P->value.re;
  imagPart = x_P->value.im - y_P->value.im;

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = realPart;
  z_P->value.im = imagPart;

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;

  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax: cxdiv $x $y ?z?
 */

int Trc_ScalarComplexDivObjCmd(ClientData clientData, Tcl_Interp *interp,
                               int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double realPart;
  double imagPart;
  double tmp;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax cxsub $x $y ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[2];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR ||
      typeY_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxdiv variables are not complex scalars\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);
  y_P = Trc_GetCxScalar(objY_P);

  tmp = y_P->value.re * y_P->value.re + y_P->value.im * y_P->value.im;
  if (tmp == 0.0) {
    printf("error, divide by 0.0 in cxdiv setting to 1.0e10\n");
    realPart = 1.0e10;
    imagPart = 1.0e10;

  } else {
    realPart =
        (x_P->value.re * y_P->value.re + x_P->value.im * y_P->value.im) / tmp;
    imagPart =
        (x_P->value.im * y_P->value.re - x_P->value.re * y_P->value.im) / tmp;
  }

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = realPart;
  z_P->value.im = imagPart;

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * Calculate e raised to the complex power x
 *
 * x = x1 + jx2
 * z = [exp(x1) * cos(x2)] + j[exp(x1) * sin(x2)]
 * syntax cxexp $x  ?z?
 */

int Trc_ScalarComplexExpObjCmd(ClientData clientData, Tcl_Interp *interp,
                               int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  Tcl_Obj *objX_P;

  double realPart;
  double imagPart;
  double tmp;

  cxScalar_t *x_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 3 || objc > 3) {
    fprintf(stderr, "syntax cxexp $x  ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxexp variable is not complex scalar\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);

#if 000
  if (x.re < -200) {
    z.re = 0.0;
    z.im = 0.0;
    return (z);
  }

  z.re = exp(x.re) * cos(x.im);
  z.im = exp(x.re) * sin(x.im);

#endif
  if (x_P->value.re < -200) {
    realPart = 0.0;
    imagPart = 0.0;
  } else {

    realPart = exp(x_P->value.re) * cos(x_P->value.im);
    imagPart = exp(x_P->value.re) * sin(x_P->value.im);
  }

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = realPart;
  z_P->value.im = imagPart;

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax cxtanh $x  ?z?
 * tanh(x)
 *        exp(x) - exp(-x)
 *      = ----------------
 *        exp(x) + exp(-x)
 */
int Trc_ScalarComplexTanhObjCmd(ClientData clientData, Tcl_Interp *interp,
                                int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  Tcl_Obj *objX_P;

  double realPart;
  double imagPart;
  double tmp;

  cx_t value;
  cx_t nx;
  cx_t x;

  cxScalar_t *x_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 3 || objc > 3) {
    fprintf(stderr, "syntax cxtanh $x  ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxtanh variable is not complex scalar\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);

#if 000
  cx_t nx;

  nx = x;
  nx.re = -nx.re;
  nx.im = -nx.im;

  return (Cx_Div(Cx_Sub(Cx_Exp(x), Cx_Exp(nx)), Cx_Add(Cx_Exp(x), Cx_Exp(nx))));

#endif
  x = x_P->value;
  nx = x;
  nx.re = -nx.re;
  nx.im = -nx.im;

  value = Dsp_Cx_Div(Dsp_Cx_Sub(Dsp_Cx_Exp(x), Dsp_Cx_Exp(nx)),
                     Dsp_Cx_Add(Dsp_Cx_Exp(x), Dsp_Cx_Exp(nx)));

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = value.re;
  z_P->value.im = value.im;

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * Calculate a square root
 * syntax cxsqrt $x  ?z?
 * convert to polar, take the square root of the magnitude and halve theta,
 * then convert back to rectangular
 */

int Trc_ScalarComplexSqrtObjCmd(ClientData clientData, Tcl_Interp *interp,
                                int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  Tcl_Obj *objX_P;

  double realPart;
  double imagPart;
  double tmp;

  cxScalar_t *x_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  register double m, theta;
  static double pi = 3.14159265358979323;

  int status;

  if (objc < 3 || objc > 3) {
    fprintf(stderr, "syntax cxsqrt $x  ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxsqrt variable is not complex scalar\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);

#if 000
  if (x.im == 0)
    if (x.re >= 0)
      theta = 0;
    else
      theta = pi;
  else if (x.re == 0)
    if (x.im >= 0)
      theta = pi / 2;
    else
      theta = 3 * pi / 2;
  else if (x.re > 0)
    theta = atan(x.im / x.re);
  else
    theta = atan(x.im / x.re) + pi;

  m = hypot(x.re, x.im);

  theta = theta / 2;
  m = sqrt(m);

  x.re = m * cos(theta);
  x.im = m * sin(theta);

#endif

  if (x_P->value.im == 0)
    if (x_P->value.re >= 0)
      theta = 0;
    else
      theta = pi;
  else if (x_P->value.re == 0)
    if (x_P->value.im >= 0)
      theta = pi / 2;
    else
      theta = 3 * pi / 2;
  else if (x_P->value.re > 0)
    theta = atan(x_P->value.im / x_P->value.re);
  else
    theta = atan(x_P->value.im / x_P->value.re) + pi;

  m = hypot(x_P->value.re, x_P->value.im);

  theta = theta / 2;
  m = sqrt(m);

  realPart = m * cos(theta);
  imagPart = m * sin(theta);

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = realPart;
  z_P->value.im = imagPart;

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

//........

/*
 * Calculate polar and return mag and angle
 * syntax: cxgetpolar $x  ?mag? ?angle?
 */

int Trc_ScalarComplexPolarObjCmd(ClientData clientData, Tcl_Interp *interp,
                                 int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  Tcl_Obj *objX_P;

  double realPart;
  double imagPart;
  double tmp;

  cxScalar_t *x_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *objVar2_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  register double m, theta;
  static double pi = 3.14159265358979323;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax cxpolar $x  ?mag? ?angle?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxpolar variable is not complex scalar\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);

  if (x_P->value.im == 0)
    if (x_P->value.re >= 0)
      theta = 0;
    else
      theta = pi;
  else if (x_P->value.re == 0)
    if (x_P->value.im >= 0)
      theta = pi / 2;
    else
      theta = 3 * pi / 2;
  else if (x_P->value.re > 0)
    theta = atan(x_P->value.im / x_P->value.re);
  else
    theta = atan(x_P->value.im / x_P->value.re) + pi;

  m = hypot(x_P->value.re, x_P->value.im);

  theta = theta * 180.0 / PI;

  varStrName = Tcl_GetStringFromObj(objv[2], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  Tcl_SetDoubleObj(objVar_P, m);

  Tcl_ObjSetVar2(interp, objv[2], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);
  objVar2_P = Tcl_NewStringObj(varStrName, varLength);
  Tcl_SetDoubleObj(objVar2_P, theta);

  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar2_P, TCL_NAMESPACE_ONLY);

  printf("m=%f angle=%f \n", m, theta);

  return TCL_OK;
}

/*
 * syntax: cxget $x ?re? ?im?
 */

int Trc_ScalarComplexExtractObjCmd(ClientData clientData, Tcl_Interp *interp,
                                   int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  Tcl_Obj *objX_P;

  double realPart;
  double imagPart;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *objVar2_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax cxget $x ?re? ?im?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxget variable  is not complex scalar\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);

  realPart = x_P->value.re;
  imagPart = x_P->value.im;

  varStrName = Tcl_GetStringFromObj(objv[2], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);

  Tcl_SetDoubleObj(objVar_P, realPart);

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar2_P = Tcl_NewStringObj(varStrName, varLength);

  Tcl_SetDoubleObj(objVar2_P, imagPart);

  Tcl_ObjSetVar2(interp, objv[2], NULL, objVar_P, TCL_NAMESPACE_ONLY);
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar2_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax: cxconj $x
 */

int Trc_ScalarComplexConjugateObjCmd(ClientData clientData, Tcl_Interp *interp,
                                     int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  Tcl_Obj *objX_P;

  double realPart;
  double imagPart;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 2 || objc > 2) {
    fprintf(stderr, "syntax cxconj $x");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  // printf("OBJECT_TYPE_NAME=%s\n",objX_P->typePtr->name);

  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxconj variable  is not complex scalar\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);

  x_P->value.im = -x_P->value.im;

  return TCL_OK;
}

/*
 * syntax cxcopy $x  ?z?
 */

int Trc_ScalarComplexDuplicateObjCmd(ClientData clientData, Tcl_Interp *interp,
                                     int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  Tcl_Obj *objX_P;

  double realPart;
  double imagPart;
  double tmp;

  cxScalar_t *x_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 3 || objc > 3) {
    fprintf(stderr, "syntax cxcopy $x  ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxcopy variable is not complex scalar\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value = x_P->value;

  varStrName = Tcl_GetStringFromObj(objv[2], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[2], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * Set an elemnt in a complex vector by complex scalar
 * syntax: vcxset $Vector $value $index
 */

int Trc_SetVectorElementComplex(ClientData clientData, Tcl_Interp *interp,
                                int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;

  cxVector_t *vec_P;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  short_t *typeX_P;
  Tcl_Obj *objX_P;
  cxScalar_t *x_P;

  int len;
  int i, j, k;
  int K;
  int status;
  double value;

  if (objc < 2)
    return TCL_ERROR;

  obj_P = objv[1];
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetCxVector(obj_P);
  if (vec_P == NULL) {
    fprintf(stderr, "Not a complex vector\n");
    return TCL_ERROR;
  }
  if (vec_P->type != TRC_TCL_TYPE_COMPLEX_VECTOR) {
    fprintf(stderr, "Not a complex vector\n");
    return TCL_ERROR;
  }

  switch (objc) {

  case 2:
  case 3:
    fprintf(stderr, "vcxset $Vector $value $index \n");
    return TCL_ERROR;
    break;
  case 4:
    objX_P = objv[2];
    typeX_P = Trc_GetPtr(objX_P);

    if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
      fprintf(stderr, "vcxset variable is not complex scalar\n");
      return TCL_ERROR;
    }

    x_P = Trc_GetCxScalar(objX_P);

    Tcl_GetIntFromObj(interp, objv[3], &i);
    if (i >= vec_P->length || i < 0) {
      fprintf(stderr, "Index out of range\n");
      return TCL_ERROR;
    }

    vec_P->vector_P[i] = x_P->value;

    break;

  default:
    fprintf(stderr, "Too many  variables\n");
    return TCL_ERROR;
    break;
  }

  return TCL_OK;
}

/*
 * Get a vector element as complex scalar from complex vector
 * syntax: vcxget $Vector  $i ?value?
 */

int Trc_GetVectorElementComplex(ClientData clientData, Tcl_Interp *interp,
                                int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  cxVector_t *vec_P;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  int len;
  int i, j, k;
  int status;
  cx_t value;
  cxScalar_t *z_P;

  if (objc < 2)
    return TCL_ERROR;

  obj_P = objv[1];
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetVector(obj_P);
  if (vec_P == NULL) {
    fprintf(stderr, "Not a complex vector\n");
    return TCL_ERROR;
  }
  if (vec_P->type != TRC_TCL_TYPE_COMPLEX_VECTOR) {
    fprintf(stderr, "Not a complex vector\n");
    return TCL_ERROR;
  }

  switch (objc) {

  case 2:
  case 3:

    fprintf(stderr, "vcxget $Vector  $i  ?value?\n");
    return TCL_ERROR;
    break;
  case 4:

    Tcl_GetIntFromObj(interp, objv[2], &i);
    if (i >= vec_P->length || i < 0) {
      fprintf(stderr, "Index out of range\n");
      return TCL_ERROR;
    }

    value = vec_P->vector_P[i];

    z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    z_P->value = value;

    varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = z_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    fprintf(stderr, "Too many  variables\n");
    return TCL_ERROR;
    break;
  }

  return TCL_OK;
}

/*
 * FFT of vector
 * syntax: vfft $Vector   ?complexVector?
 */

int Trc_VectorFFT(ClientData clientData, Tcl_Interp *interp, int objc,
                  Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  doubleVector_t *vec_P;
  cxVector_t *fft_P;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  int len;
  int i, j, k;
  int status;
  cx_t value;
  cxScalar_t *z_P;

  if (objc < 3)
    return TCL_ERROR;

  obj_P = objv[1];
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetVector(obj_P);
  if (vec_P == NULL) {
    fprintf(stderr, "Not a  vector\n");
    return TCL_ERROR;
  }
  if (vec_P->type != TRC_TCL_TYPE_REAL_VECTOR) {
    fprintf(stderr, "Not a real vector\n");
    return TCL_ERROR;
  }

  switch (objc) {

  case 2:

    fprintf(stderr, "vfft $Vector  ?resultComplex?\n");
    return TCL_ERROR;
    break;
  case 3:

    fft_P = Dsp_VectorFFT(vec_P);

    varStrName = Tcl_GetStringFromObj(objv[2], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = fft_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[2], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    fprintf(stderr, "Too many  variables\n");
    return TCL_ERROR;
    break;
  }

  return TCL_OK;
}

/*
 * inverse FFT of vector
 * syntax vifft $Vector   ?complexVector?
 */

int Trc_VectorInverseFFT(ClientData clientData, Tcl_Interp *interp, int objc,
                         Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  cxVector_t *vec_P;
  doubleVector_t *ifft_P;
  Tcl_ObjType *objType_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;
  int len;
  int i, j, k;
  int status;
  cx_t value;
  cxScalar_t *z_P;

  if (objc < 3)
    return TCL_ERROR;

  obj_P = objv[1];
  if (Trc_CheckObjType(obj_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  vec_P = Trc_GetVector(obj_P);
  if (vec_P == NULL) {
    fprintf(stderr, "Not a  vector\n");
    return TCL_ERROR;
  }
  if (vec_P->type != TRC_TCL_TYPE_COMPLEX_VECTOR) {
    fprintf(stderr, "Not a complex vector\n");
    return TCL_ERROR;
  }

  switch (objc) {

  case 2:

    fprintf(stderr, "vifft $Vector  ?resultReal?\n");
    return TCL_ERROR;
    break;
  case 3:

    ifft_P = Dsp_VectorInverseFFT(vec_P);

    varStrName = Tcl_GetStringFromObj(objv[2], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = ifft_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[2], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    fprintf(stderr, "Too many  variables\n");
    return TCL_ERROR;
    break;
  }

  return TCL_OK;
}

//.....

/*
 * syntax: saxpy $y $x $alpha
 * note x and y are real or complex vectors alpha double or complex scalar
 * scalar alpha x plus y
 */

int Trc_VectorObjSAXPYtCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                           Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;
  Tcl_Obj *objAlpha_P;
  short_t *typeAlpha_P;

  doubleVector_t *vx_P;
  doubleVector_t *vy_P;

  cxVector_t *vxx_P;
  cxVector_t *vyx_P;

  cxScalar_t *alphax;
  double alpha;

  double dotProductReal;
  cxScalar_t *dotProductComplex_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax vdot $x $y z where C=xTy");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[2];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  if (typeX_P->type == TRC_TCL_TYPE_REAL_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_REAL_VECTOR) {

    vx_P = Trc_GetVector(objX_P);
    vy_P = Trc_GetVector(objY_P);

    if (vx_P->length != vy_P->length || vx_P->transpose) {
      fprintf(
          stderr,
          "x vector and y vector dimensions not compatible for dot product\n");
      return TCL_ERROR;
    }
    Tcl_GetDoubleFromObj(interp, objv[3], &alpha);
    Dsp_SAXPYReal(vx_P, vy_P, alpha);

    return TCL_OK;
  }
  if (typeX_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR) {
    objAlpha_P = objv[3];
    if (Trc_CheckObjType(objAlpha_P)) {
      fprintf(stderr, "Alpha not a complex scalar\n");
      return (TCL_ERROR);
    }
    typeAlpha_P = Trc_GetPtr(objAlpha_P);
    if (typeAlpha_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
      fprintf(stderr, "Alpha not a complex scalar\n");
      return (TCL_ERROR);
    }

    alphax = Trc_GetCxScalar(objAlpha_P);
    vxx_P = Trc_GetCxVector(objX_P);
    vyx_P = Trc_GetCxVector(objY_P);

    // z=xTy A
    if (vxx_P->length != vyx_P->length) {
      fprintf(stderr, "x vector and y vector dimensions not compatible\n");
      return TCL_ERROR;
    }
    Dsp_SAXPYComplex(vxx_P, vyx_P, alphax);

    return TCL_OK;
  }
  if (typeX_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_REAL_VECTOR) {
  }
  if (typeX_P->type == TRC_TCL_TYPE_REAL_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR) {
  }

  fprintf(stderr,
          "NOT SUPPORTED X and Y must both be real or complex vectors \n");
  return TCL_ERROR;
}

/*
 * syntax: vconv $x $f $y
 * note x, y and z are real or complex vectors
 */

int Trc_VectorObjConvolveCmd(ClientData clientData, Tcl_Interp *interp,
                             int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;
  Tcl_Obj *objF_P;
  short_t *typeF_P;

  doubleVector_t *vx_P;
  doubleVector_t *vy_P;
  doubleVector_t *vf_P;

  cxVector_t *vxx_P;
  cxVector_t *vyx_P;
  cxVector_t *vfx_P;

  cxScalar_t *alphax;
  double alpha;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax vconv $x $f $y ");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[3];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  objF_P = objv[2];
  if (Trc_CheckObjType(objF_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeF_P = Trc_GetPtr(objF_P);

  if (typeX_P->type == TRC_TCL_TYPE_REAL_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_REAL_VECTOR &&
      typeF_P->type == TRC_TCL_TYPE_REAL_VECTOR) {

    vx_P = Trc_GetVector(objX_P);
    vy_P = Trc_GetVector(objY_P);
    vf_P = Trc_GetVector(objF_P);

    if (vx_P->length < vf_P->length) {
      fprintf(stderr,
              "Dimensions of vector x should be larger than vector f\n");
      return (TCL_ERROR);
    }
    if (vx_P->length != vy_P->length) {
      fprintf(stderr, "Dimensions of x and vector y should equal.\n");
      return (TCL_ERROR);
    }

    Dsp_ConvolveReal(vx_P, vf_P, vy_P);

    return TCL_OK;
  }
  if (typeX_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR &&
      typeF_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR) {

    vxx_P = Trc_GetCxVector(objX_P);
    vyx_P = Trc_GetCxVector(objY_P);
    vfx_P = Trc_GetCxVector(objF_P);

    if (vxx_P->length < vfx_P->length) {
      fprintf(stderr,
              "Dimensions of vector x should be larger than vector f\n");
      return (TCL_ERROR);
    }
    if (vxx_P->length != vyx_P->length) {
      fprintf(stderr, "Dimensions of x and vector y should equal.\n");
      return (TCL_ERROR);
    }

    Dsp_ConvolveComplex(vxx_P, vfx_P, vyx_P);

    return TCL_OK;
  }
  if (typeX_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_REAL_VECTOR) {
  }
  if (typeX_P->type == TRC_TCL_TYPE_REAL_VECTOR &&
      typeY_P->type == TRC_TCL_TYPE_COMPLEX_VECTOR) {
  }

  fprintf(stderr,
          "NOT SUPPORTED X and Y must both be real or complex vectors \n");
  return TCL_ERROR;
}

/*
 * syntax tload $fileName ?top?
 */
int Trc_LoadTopologyCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                        Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  char *topologyFileName;
  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  int action, initialChoice, scact;
  FILE *fnet, *frec;

  if (objc < 3 || objc > 3) {
    fprintf(stderr, "syntax tload $fileName ?top?");
    return TCL_ERROR;
  }

  topologyFileName = Tcl_GetStringFromObj(objv[1], &varLength);

#if 000
  if (trc_root_P != (struct node *)NULL) {
    Trc_RecoverMem(trc_root_P);
    trc_root_P = (struct node *)NULL;
  }
#endif

  if ((frec = fopen(topologyFileName, "r")) == NULL) {
    Trc_Message("Cannot open file !");
    return TCL_ERROR;
  }
  root_P = Trc_RecallTree(frec);
  fclose(frec);
  Trc_StoreTree(root_P, stdout);

  varStrName = Tcl_GetStringFromObj(objv[2], &varLength);
  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = root_P;
  objVar_P->typePtr = trc_nodeObjType_P;
  Tcl_ObjSetVar2(interp, objv[2], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax: tcalc $root $frequency $gen $genImp   [?totalImpedance?
 * ?totalCurrent?
 */
int Trc_CalcTopologyCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                        Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  double realPart;
  double imagPart;

  cxScalar_t *gen_P;
  cxScalar_t *genImp_P;
  cxScalar_t *totalCurrent_P;
  cxScalar_t *totalImpedance_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;

  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  struct node *current_P; /* current_P node accessed */
  float freq;             /* frequency */
  int i;
  complex eg,   /* voltage source */
      zs,       /* source impedence */
      v_in,     /* voltage of parent node */
      totalImp, /* impedence at top of tree */
      totalCur; /* current_P at top of tree */

  if (objc < 5 || objc > 7) {
    fprintf(stderr, "syntax tcalc $root $frequency $gen $genImp   "
                    "[?totalImpedance? ?totalCurrent?");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  Tcl_GetDoubleFromObj(interp, objv[2], &frequency);

  objGen_P = objv[3];
  if (Trc_CheckObjType(objGen_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeGen_P = Trc_GetPtr(objGen_P);

  objGenImp_P = objv[4];
  if (Trc_CheckObjType(objGenImp_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeGenImp_P = Trc_GetPtr(objGenImp_P);

  if (typeGen_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR ||
      typeGenImp_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "tcalc gen and/or genImp  are not complex scalars\n");
    return TCL_ERROR;
  }

  gen_P = Trc_GetCxScalar(objGen_P);
  genImp_P = Trc_GetCxScalar(objGenImp_P);

  eg = gen_P->value;
  zs = genImp_P->value;

  /*
   * Calculate characteristic impedance and propogation constant
   * for all nodes
   */
  Trc_TreeCharic(root_P, frequency);
  /*
   * calculate impedences
   */
  totalImp = Trc_CalcImp(root_P, root_P->zo, root_P->gamma);

  /*
   * calculate currents and voltages
   */
  totalCur = Trc_InitCur(root_P, root_P->gamma, root_P->zo, eg, zs,
                         root_P->zLoad, root_P->length, totalImp);
  v_in.re = 0.0;
  v_in.im = 0.0;
  Trc_CalcCurrent(root_P, totalCur, v_in, root_P->zo, root_P->gamma);
  printf("Total impedance = %e %e \n", totalImp.re, totalImp.im);
  printf("Total input current = %e %e \n", totalCur.re, totalCur.im);

  switch (objc) {

  case 6:

    totalImpedance_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    totalImpedance_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    totalImpedance_P->value.re = totalImp.re;
    totalImpedance_P->value.im = totalImp.im;

    varStrName = Tcl_GetStringFromObj(objv[5], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = totalImpedance_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[5], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  case 7:
    totalImpedance_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    totalImpedance_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    totalImpedance_P->value.re = totalImp.re;
    totalImpedance_P->value.im = totalImp.im;

    varStrName = Tcl_GetStringFromObj(objv[5], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = totalImpedance_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[5], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    totalCurrent_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    totalCurrent_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    totalCurrent_P->value.re = totalCur.re;
    totalCurrent_P->value.im = totalCur.im;

    varStrName = Tcl_GetStringFromObj(objv[6], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = totalCurrent_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[6], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    return TCL_OK;
    break;
  }
  return TCL_OK;
}

/*
 * syntax tpr $root [$fileName]
 */
int Trc_PrintTopologyCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                         Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  double realPart;
  double imagPart;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;

  int varLength;
  int status;

  char *topologyFileName;
  FILE *fnet;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  if (objc < 2 || objc > 3) {
    fprintf(stderr, "syntax tpr $root [$fileName] \n");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }
  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  if (objc == 3) {

    topologyFileName = Tcl_GetStringFromObj(objv[2], &varLength);

    if ((fnet = fopen(topologyFileName, "w")) == NULL) {
      Trc_Message("Cannot open file!");
      return TCL_ERROR;
    }
  } else {
    fnet = stdout;
  }
  Trc_StoreTree(root_P, fnet);
  if (objc == 3)
    fclose(fnet);

  return TCL_OK;
}

/*
 * syntax ninfo $root $nodeName
 */

int Trc_NodeInfoCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                    Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;

  int varLength;
  int status;

  struct node *root_P;

  char *nodeName;

  struct node *current_P; /* current_P node accessed */

  if (objc < 3 || objc > 3) {
    fprintf(stderr, "syntax ninfo $root $nodeName");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  current_P = Trc_SearchTreeName(root_P, nodeName);
  if (current_P == NULL) {
    fprintf(stderr, "Could not find node \n");
    return TCL_ERROR;
  }
  Trc_PrintNode(current_P, stdout);

  return TCL_OK;
}

/*
 * syntax nz0gamma $root $nodeName ?z0? ?gamma?
 */
int Trc_NodeZ0GammaCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                       Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  double realPart;
  double imagPart;

  cxScalar_t *z0_P;
  cxScalar_t *gamma_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  char *nodeName;

  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  struct node *current_P; /* current_P node accessed */
  float freq;             /* frequency */
  int i;
  complex eg,   /* voltage source */
      zs,       /* source impedence */
      v_in,     /* voltage of parent node */
      totalImp, /* impedence at top of tree */
      totalCur; /* current_P at top of tree */

  if (objc < 5 || objc > 5) {
    fprintf(stderr, "syntax nz0gamma $root $nodeName ?z0? ?gamma?");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  current_P = Trc_SearchTreeName(root_P, nodeName);
  if (current_P == NULL) {
    fprintf(stderr, "Could not find node \n");
    return TCL_ERROR;
  }

  switch (objc) {

  case 5:
    z0_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    z0_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    z0_P->value.re = current_P->zo.re;
    z0_P->value.im = current_P->zo.im;

    varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = z0_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    gamma_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    gamma_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    gamma_P->value.re = current_P->gamma.re;
    gamma_P->value.im = current_P->gamma.im;

    varStrName = Tcl_GetStringFromObj(objv[4], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = gamma_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[4], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    return TCL_ERROR;
    break;
  }
  return TCL_OK;
}

/*
 * syntax nvalues $root $nodeName /variables automatically generated/
 */

int Trc_NodeCalcValuesCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                          Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  double realPart;
  double imagPart;

  cxScalar_t *i_P;
  cxScalar_t *il_P;
  cxScalar_t *ir_P;
  cxScalar_t *z_P;
  cxScalar_t *zl_P;
  cxScalar_t *zr_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *objVarStr_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;

  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;
  char buffer[1024];
  char *nodeName;

  struct node *current_P; /* current_P node accessed */
  float freq;             /* frequency */
  int i;
  complex eg,   /* voltage source */
      zs,       /* source impedence */
      v_in,     /* voltage of parent node */
      totalImp, /* impedence at top of tree */
      totalCur; /* current_P at top of tree */

  if (objc < 3 || objc > 3) {
    fprintf(
        stderr,
        "syntax nvalues $root $nodeName /variables automatically generated/");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  current_P = Trc_SearchTreeName(root_P, nodeName);
  if (current_P == NULL) {
    fprintf(stderr, "Could not find node \n");
    return TCL_ERROR;
  }

  switch (current_P->type) {
  case NODE_TYPE_LEAF:
    if (current_P->infinite)
      printf("Impedence: infinite\n");
    else {

      zl_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
      zl_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
      zl_P->value.re = current_P->zLoad.re;
      zl_P->value.im = current_P->zLoad.im;

      strcpy(buffer, nodeName);
      strcat(buffer, "_zl");

      objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
      objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
      objVar_P->internalRep.otherValuePtr = zl_P;
      objVar_P->typePtr = trc_vecObjType_P;
      Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);

      i_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
      i_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
      i_P->value.re = current_P->inCurrent.re;
      i_P->value.im = current_P->inCurrent.im;

      strcpy(buffer, nodeName);
      strcat(buffer, "_i");

      objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
      objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
      objVar_P->internalRep.otherValuePtr = i_P;
      objVar_P->typePtr = trc_vecObjType_P;
      Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
      printf("Created complex scalar variable:%s\n", buffer);
    }

    break;
  case NODE_TYPE_UNARY:

    zl_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    zl_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    zl_P->value.re = current_P->leftImp.re;
    zl_P->value.im = current_P->leftImp.im;

    strcpy(buffer, nodeName);
    strcat(buffer, "_zl");

    objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVar_P->internalRep.otherValuePtr = zl_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
    printf("Created complex scalar variable:%s\n", buffer);

    i_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    i_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    i_P->value.re = current_P->inCurrent.re;
    i_P->value.im = current_P->inCurrent.im;

    strcpy(buffer, nodeName);
    strcat(buffer, "_i");

    objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVar_P->internalRep.otherValuePtr = i_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
    printf("Created complex scalar variable:%s\n", buffer);

    il_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    il_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    il_P->value.re = current_P->leftCurrent.re;
    il_P->value.im = current_P->leftCurrent.im;

    strcpy(buffer, nodeName);
    strcat(buffer, "_il");

    objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVar_P->internalRep.otherValuePtr = il_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
    printf("Created complex scalar variable:%s\n", buffer);

    break;
  case NODE_TYPE_BINARY:
    zl_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    zl_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    zl_P->value.re = current_P->leftImp.re;
    zl_P->value.im = current_P->leftImp.im;

    strcpy(buffer, nodeName);
    strcat(buffer, "_zl");

    objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVar_P->internalRep.otherValuePtr = zl_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
    printf("Created complex scalar variable:%s\n", buffer);

    zr_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    zr_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    zr_P->value.re = current_P->rightImp.re;
    zr_P->value.im = current_P->rightImp.im;

    strcpy(buffer, nodeName);
    strcat(buffer, "_zr");

    objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVar_P->internalRep.otherValuePtr = zr_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
    printf("Created complex scalar variable:%s\n", buffer);

    i_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    i_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    i_P->value.re = current_P->inCurrent.re;
    i_P->value.im = current_P->inCurrent.im;

    strcpy(buffer, nodeName);
    strcat(buffer, "_i");

    objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVar_P->internalRep.otherValuePtr = i_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
    printf("Created complex scalar variable:%s\n", buffer);
    il_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    il_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    il_P->value.re = current_P->leftCurrent.re;
    il_P->value.im = current_P->leftCurrent.im;

    strcpy(buffer, nodeName);
    strcat(buffer, "_il");

    objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVar_P->internalRep.otherValuePtr = il_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
    printf("Created complex scalar variable:%s\n", buffer);

    ir_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
    ir_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
    ir_P->value.re = current_P->rightCurrent.re;
    ir_P->value.im = current_P->rightCurrent.im;

    strcpy(buffer, nodeName);
    strcat(buffer, "_ir");

    objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
    objVar_P->internalRep.otherValuePtr = ir_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
    printf("Created complex scalar variable:%s\n", buffer);

    break;
  }

  strcpy(buffer, nodeName);
  strcat(buffer, "_type");
  objVar_P = Tcl_NewStringObj(buffer, strlen(buffer));
  objVarStr_P = Tcl_NewStringObj(buffer, strlen(buffer));
  Tcl_SetIntObj(objVar_P, current_P->type);
  Tcl_ObjSetVar2(interp, objVarStr_P, NULL, objVar_P, TCL_NAMESPACE_ONLY);
  printf("Created integer variable:%s\n", buffer);

  return TCL_OK;
}

/*
 * syntax zpar $x $y ?z?
 */

int Trc_ScalarComplexZParallelObjCmd(ClientData clientData, Tcl_Interp *interp,
                                     int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double realPart;
  double imagPart;

  complex x1, x2;
  complex zp;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax zpar $x $y ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[2];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR ||
      typeY_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxmult variables are not complex scalars\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);
  y_P = Trc_GetCxScalar(objY_P);

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value = Trc_ZParallel(x_P->value, y_P->value);

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax cxinv $x  ?z?
 */

int Trc_ScalarComplexInverseObjCmd(ClientData clientData, Tcl_Interp *interp,
                                   int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double realPart;
  double imagPart;
  double tmp;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 3 || objc > 3) {
    fprintf(stderr, "syntax cxinv $x  ?z?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxdiv variables are not complex scalars\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);

  tmp = x_P->value.re * x_P->value.re + x_P->value.im * x_P->value.im;
  if (tmp == 0.0) {
    printf("error, divide by 0.0 in cxdiv setting to 1.0e10\n");
    realPart = 1.0e10;
    imagPart = 1.0e10;

  } else {
    realPart = (x_P->value.re) / tmp;
    imagPart = (-x_P->value.im) / tmp;
  }

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;
  z_P->value.re = realPart;
  z_P->value.im = imagPart;

  varStrName = Tcl_GetStringFromObj(objv[2], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[2], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * syntax nimp  $topologyFileName  $nodeName $zs $fmin $fmax $points ?imResp? [
 * ?zinput? ?fresp? ]
 */

int Trc_NetImpulseResponseCmd(ClientData clientData, Tcl_Interp *interp,
                              int objc, Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  double realPart;
  double imagPart;

  doubleVector_t *imp_P;
  cxVector_t *zInput_P;
  cxVector_t *fresp_P;

  cxScalar_t *z0_P;
  cxScalar_t *gamma_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  char *nodeName;
  char *impText;
  char *netFileName;

  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  double zs;
  double fmin;
  double fmax;
  double frdel;
  int npts;
  int fftexp;
  int fftl;

  struct node *current_P; /* current_P node accessed */
  float freq;             /* frequency */
  int i;
  int length;

  if (objc < 8 || objc > 10) {
    fprintf(stderr, "syntax nimp  $topologyFileName  $nodeName $zs $fmin $fmax "
                    "$points ?imResp? [ ?zinput? ?fresp? ]");
    return TCL_ERROR;
  }
#if 0000
  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }
#endif
  obj_P = objv[1];
  netFileName = Tcl_GetStringFromObj(obj_P, &varLength);

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  // current_P=Trc_SearchTreeName(root_P,nodeName);
  // if(current_P==NULL) {
  //          fprintf(stderr,"Could not find node \n");
  //	 return TCL_ERROR;
  // }
  Tcl_GetDoubleFromObj(interp, objv[3], &zs);
  Tcl_GetDoubleFromObj(interp, objv[4], &fmin);
  Tcl_GetDoubleFromObj(interp, objv[5], &fmax);

  Tcl_GetIntFromObj(interp, objv[6], &npts);

  /*
   * Find closest power of two number
   */
  fftexp = (int)(log((float)npts) / log(2.0) + 0.5);
  fftl = 1 << fftexp;
  if (fftl > npts) {
    fftl = fftl / 2;
    fftexp -= 1;
  }
  frdel = (fmax - fmin) / fftl;

  switch (objc) {

  case 8:
    imp_P = Dsp_AllocateDoubleVector(fftl);
    zInput_P = NULL;
    fresp_P = NULL;

    break;

  case 9:
    impText = Tcl_GetStringFromObj(objv[7], &length);
    if (strcmp(impText, "none"))
      imp_P = NULL;
    else {
      imp_P = Dsp_AllocateDoubleVector(fftl);
    }
    zInput_P = Dsp_AllocateCxVector(fftl / 2);
    fresp_P = NULL;

    break;

  case 10:
    impText = Tcl_GetStringFromObj(objv[7], &length);
    if (strcmp(impText, "none"))
      imp_P = NULL;
    else {
      imp_P = Dsp_AllocateDoubleVector(fftl);
    }
    zInput_P = Dsp_AllocateCxVector(fftl / 2);
    fresp_P = Dsp_AllocateCxVector(fftl / 2);
    break;

  default:
    return TCL_ERROR;
    break;
  }

  Trc_NetImpVec(netFileName, nodeName, fmin, frdel, fmax, fftexp, 1.0, zs,
                imp_P, zInput_P, fresp_P);

  if (imp_P) {
    varStrName = Tcl_GetStringFromObj(objv[7], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = imp_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[7], NULL, objVar_P, TCL_NAMESPACE_ONLY);
  }
  if (zInput_P) {
    varStrName = Tcl_GetStringFromObj(objv[8], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = zInput_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[8], NULL, objVar_P, TCL_NAMESPACE_ONLY);
  }
  if (fresp_P) {
    varStrName = Tcl_GetStringFromObj(objv[9], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = fresp_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[9], NULL, objVar_P, TCL_NAMESPACE_ONLY);
  }
  return TCL_OK;
}

/*
 * syntax nvoltlength $topology $nodeName $points ?voltage?
 */

int Trc_NetLineLengthCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                         Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  doubleVector_t *y_P;
  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  char *nodeName;
  char *impText;
  char *netFileName;

  int varLength;
  int points;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  double zs;
  double fmin;
  double fmax;
  double frdel;
  int npts;

  struct node *current_P; /* current_P node accessed */
  int i;
  int length;

  if (objc < 5 || objc > 5) {
    fprintf(stderr,
            "syntax ncurrlength $topology $nodeName $points ?voltage? ");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  current_P = Trc_SearchTreeName(root_P, nodeName);
  if (current_P == NULL) {
    fprintf(stderr, "Could not find node \n");
    return TCL_ERROR;
  }

  Tcl_GetIntFromObj(interp, objv[3], &points);

  y_P = Dsp_AllocateDoubleVector(points);

  Trc_CalcCurrentLengthNode(current_P, points, y_P);

  if (!y_P)
    return TCL_ERROR;

  if (y_P) {
    varStrName = Tcl_GetStringFromObj(objv[4], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    objVar_P->internalRep.otherValuePtr = y_P;
    objVar_P->typePtr = trc_vecObjType_P;
    Tcl_ObjSetVar2(interp, objv[4], NULL, objVar_P, TCL_NAMESPACE_ONLY);
  }

  return TCL_OK;
}

/*
 * syntax ngetlength $root $nodeName ?length?
 */

int Trc_NodeGetLengthCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                         Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  char *nodeName;

  double length;

  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  struct node *current_P; /* current_P node accessed */
  float freq;             /* frequency */
  int i;

  if (objc < 3 || objc > 4) {
    fprintf(stderr, "syntax ngetlength $root $nodeName ?length?");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  current_P = Trc_SearchTreeName(root_P, nodeName);
  if (current_P == NULL) {
    fprintf(stderr, "Could not find node \n");
    return TCL_ERROR;
  }

  switch (objc) {
  case 3:
    printf("Length=%e\n", current_P->length);
    break;

  case 4:
    length = current_P->length;

    varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    Tcl_SetDoubleObj(objVar_P, length);
    Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    return TCL_ERROR;
    break;
  }
  return TCL_OK;
}

/*
 * syntax ngetrlcg $root $nodeName ?r? ?l? ?c? ?g?"
 */

int Trc_NodeGetRLCGCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                       Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  char *nodeName;

  double r, l, c, g;

  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  struct node *current_P; /* current_P node accessed */

  if (objc < 3 || objc > 7) {
    fprintf(stderr, "syntax ngetrlcg $root $nodeName ?r? ?l? ?c? ?g?");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  current_P = Trc_SearchTreeName(root_P, nodeName);
  if (current_P == NULL) {
    fprintf(stderr, "Could not find node \n");
    return TCL_ERROR;
  }

  switch (objc) {

  case 3:
    printf("r=%e l=%e c=%e g=%e\n", current_P->r, current_P->l, current_P->c,
           current_P->g);
    break;
  case 7:
    r = current_P->r;
    varStrName = Tcl_GetStringFromObj(objv[3], &varLength);
    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    Tcl_SetDoubleObj(objVar_P, r);
    Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    l = current_P->l;
    varStrName = Tcl_GetStringFromObj(objv[4], &varLength);
    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    Tcl_SetDoubleObj(objVar_P, l);
    Tcl_ObjSetVar2(interp, objv[4], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    c = current_P->c;
    varStrName = Tcl_GetStringFromObj(objv[5], &varLength);
    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    Tcl_SetDoubleObj(objVar_P, c);
    Tcl_ObjSetVar2(interp, objv[5], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    g = current_P->g;
    varStrName = Tcl_GetStringFromObj(objv[6], &varLength);
    objVar_P = Tcl_NewStringObj(varStrName, varLength);
    Tcl_SetDoubleObj(objVar_P, g);
    Tcl_ObjSetVar2(interp, objv[6], NULL, objVar_P, TCL_NAMESPACE_ONLY);

    break;

  default:
    return TCL_ERROR;
    break;
  }
  return TCL_OK;
}

/*
 * syntax nsetlength $root $nodeName $length
 */

int Trc_NodeSetLengthCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                         Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  char *nodeName;

  double length;

  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  struct node *current_P; /* current_P node accessed */

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax nsetlength $root $nodeName $length");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  current_P = Trc_SearchTreeName(root_P, nodeName);
  if (current_P == NULL) {
    fprintf(stderr, "Could not find node \n");
    return TCL_ERROR;
  }

  switch (objc) {

  case 4:
    Tcl_GetDoubleFromObj(interp, objv[3], &length);
    current_P->length = length;

    break;

  default:
    return TCL_ERROR;
    break;
  }
  return TCL_OK;
}

/*
 * syntax nsetrlcg $root $nodeName $r $l $c $g
 */

int Trc_NodeSetRLCGCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                       Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeGen_P;
  short_t *typeGenImp_P;
  Tcl_Obj *objGen_P;
  Tcl_Obj *objGenImp_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  char *nodeName;

  double r, l, c, g;

  int varLength;
  int status;

  struct node *root_P;
  float xin, yin, xn, yn;
  // char 	topologyFileName[256];
  char *messg1, *messg2;
  double frequency;

  struct node *current_P; /* current_P node accessed */

  if (objc < 7 || objc > 7) {
    fprintf(stderr, "syntax nsetrlcg $root $nodeName $r $l $c $g");
    return TCL_ERROR;
  }

  obj_P = objv[1];
  if (Trc_CheckObjNodeType(obj_P)) {
    fprintf(stderr, "Variable not a node\n");
    return (TCL_ERROR);
  }

  root_P = (struct node *)obj_P->internalRep.otherValuePtr;
  if (root_P == NULL) {
    fprintf(stderr, "Root is NUll\n");
    return TCL_ERROR;
  }

  nodeName = Tcl_GetStringFromObj(objv[2], &varLength);

  current_P = Trc_SearchTreeName(root_P, nodeName);
  if (current_P == NULL) {
    fprintf(stderr, "Could not find node \n");
    return TCL_ERROR;
  }

  switch (objc) {

  case 7:
    Tcl_GetDoubleFromObj(interp, objv[3], &r);
    current_P->r = r;
    Tcl_GetDoubleFromObj(interp, objv[4], &l);
    current_P->l = l;
    Tcl_GetDoubleFromObj(interp, objv[5], &c);
    current_P->c = c;
    Tcl_GetDoubleFromObj(interp, objv[6], &g);
    current_P->g = g;
    printf("Set to r=%e l=%e c=%e g=%e\n", current_P->r, current_P->l,
           current_P->c, current_P->g);

    break;

  default:
    return TCL_ERROR;
    break;
  }
  return TCL_OK;
}

/*
 * syntax rflc $z $z0 ?rc?
 */

int Trc_ScalarComplexReflectionCoefficientObjCmd(ClientData clientData,
                                                 Tcl_Interp *interp, int objc,
                                                 Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  short_t *typeY_P;
  Tcl_Obj *objX_P;
  Tcl_Obj *objY_P;

  double realPart;
  double imagPart;

  complex x1, x2;
  complex zp;

  cxScalar_t *x_P;
  cxScalar_t *y_P;
  cxScalar_t *z_P;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  int status;

  if (objc < 4 || objc > 4) {
    fprintf(stderr, "syntax rflc $z $z0 ?rc?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  objY_P = objv[2];
  if (Trc_CheckObjType(objY_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeY_P = Trc_GetPtr(objY_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR ||
      typeY_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "cxmult variables are not complex scalars\n");
    return TCL_ERROR;
  }

  x_P = Trc_GetCxScalar(objX_P);
  y_P = Trc_GetCxScalar(objY_P);

  z_P = (cxScalar_t *)calloc(1, sizeof(cxScalar_t));
  z_P->type = TRC_TCL_TYPE_COMPLEX_SCALAR;

  z_P->value =
      Cx_Div(Cx_Sub(x_P->value, y_P->value), Cx_Add(x_P->value, y_P->value));

  varStrName = Tcl_GetStringFromObj(objv[3], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  objVar_P->internalRep.otherValuePtr = z_P;
  objVar_P->typePtr = trc_vecObjType_P;
  Tcl_ObjSetVar2(interp, objv[3], NULL, objVar_P, TCL_NAMESPACE_ONLY);

  return TCL_OK;
}

/*
 * Calculate VSWR from complex Reflection Coefficient
 * syntax: vswr $rflc  ?vswr?
 */

int Trc_VSWRObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                   Tcl_Obj *CONST objv[]) {
  Tcl_Obj *resultPtr;
  short_t *typeX_P;
  Tcl_Obj *objX_P;

  double realPart;
  double imagPart;
  double tmp;

  cxScalar_t *rflc_P;

  double a, b;
  double vswr;
  double rflcMag;

  Tcl_Obj *obj_P;
  Tcl_Obj *objVar_P;
  Tcl_Obj *objVar2_P;
  Tcl_Obj *varNameObj_P;
  char *varStrName;
  int varLength;

  register double m, theta;
  static double pi = 3.14159265358979323;

  int status;

  if (objc < 3 || objc > 3) {
    fprintf(stderr, "syntax vswr $rflc  ?vswr?");
    return TCL_ERROR;
  }

  objX_P = objv[1];
  if (Trc_CheckObjType(objX_P)) {
    fprintf(stderr, "Variable not a vector/matrix/complex scalar\n");
    return (TCL_ERROR);
  }
  typeX_P = Trc_GetPtr(objX_P);

  if (typeX_P->type != TRC_TCL_TYPE_COMPLEX_SCALAR) {
    fprintf(stderr, "vswr variable is not complex scalar\n");
    return TCL_ERROR;
  }

  rflc_P = Trc_GetCxScalar(objX_P);

  a = rflc_P->value.re;
  b = rflc_P->value.im;

  rflcMag = sqrt(a * a + b * b);

  vswr = (1 + rflcMag) / (1 - rflcMag);

  varStrName = Tcl_GetStringFromObj(objv[2], &varLength);

  objVar_P = Tcl_NewStringObj(varStrName, varLength);
  Tcl_SetDoubleObj(objVar_P, vswr);
  Tcl_ObjSetVar2(interp, objv[2], NULL, objVar_P, TCL_NAMESPACE_ONLY);
  return TCL_OK;
}

/*
 *
 * syntax: help
 */
int Trc_HelpObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                   Tcl_Obj *CONST objv[]) {

  fprintf(stdout, "vhelp  for help on vector commands\n");
  fprintf(stdout, "cxhelp for help on complex scalar commands \n");
  fprintf(stdout, "thelp for help on transmission line network topologies\n");

  return TCL_OK;
}

/*
 *
 * syntax: vhelp
 */

int Trc_VHelpObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                    Tcl_Obj *CONST objv[]) {

  fprintf(stdout, "vec ?vector? [list ...]\n");
  fprintf(stdout, "cxvec ?vector? [list a+bi c+di ...]\n");
  fprintf(stdout, "vrd ?vector? fileName\n");
  fprintf(stdout, "vcxrd ?vector? fileName\n");
  fprintf(stdout, "vset $Vector $value $index\n");
  fprintf(stdout, "vget $Vector  $index ?value?\n");
  fprintf(stdout, "vcr ?vector? $length  [$std $seed] \n");
  fprintf(stdout, "vcxcr ?vector? $length  [$std $seed]\n");
  fprintf(stdout, "vdot $x $y ?z?    # can be both complex or both real\n");
  fprintf(stdout, "vcxset $Vector $value $index\n");
  fprintf(stdout, "vcxget $Vector  $i ?value?\n");
  fprintf(stdout, "vfft $Vector ?complexVector?\n");
  fprintf(stdout, "vifft $Vector  ?resultReal?\n");
  fprintf(stdout, "saxpy $y $x $alpha  # y= alpha x + y , alpha scalar\n");
  fprintf(stdout,
          "vconv $x $f $y        # vectors either all complex or all real\n");

  return TCL_OK;
}

/*
 * syntax: cxhelp
 */

int Trc_CXHelpObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                     Tcl_Obj *CONST objv[]) {

  fprintf(stdout, "cx $re $im ?z? \n");
  fprintf(stdout, "cxpolar $m $angle ?z?\n");
  fprintf(stdout, "cxmult $x $y ?z?\n");
  fprintf(stdout, "cxadd $x $y ?z?\n");
  fprintf(stdout, "cxsub $x $y ?z?\n");
  fprintf(stdout, "cxdiv $x $y ?z?  # z=x/y\n");
  fprintf(stdout, "cxexp $x  ?z?\n");
  fprintf(stdout, "cxtanh $x  ?z? #(exp(x) - exp(-x))/(exp(x) + exp(-x))\n");
  fprintf(stdout,
          "cxsqrt $x  ?z?   # convert to polar, take the square root of the "
          "magnitude and halve theta,then convert back to rectangular\n");
  fprintf(stdout, "cxgetpolar $x  ?mag? ?angle?\n");
  fprintf(stdout, "cxget $x ?re? ?im?\n");
  fprintf(stdout, "cxconj $x\n");
  fprintf(stdout, "cxcopy $x  ?z?\n");
  fprintf(stdout, "cxinv $x  ?z?\n");
  return TCL_OK;
}

/*
 * syntax: thelp
 */

int Trc_THelpObjCmd(ClientData clientData, Tcl_Interp *interp, int objc,
                    Tcl_Obj *CONST objv[]) {

  fprintf(stdout, "tload $fileName ?top?\n");
  fprintf(stdout, "tcalc $root $frequency $gen $genImp   [?totalImpedance? "
                  "?totalCurrent?]\n");
  fprintf(stdout, "tpr $root [$fileName]\n");
  fprintf(stdout, "ninfo $root $nodeName\n");
  fprintf(stdout, "nz0gamma $root $nodeName ?z0? ?gamma?\n");
  fprintf(stdout,
          "nvalues $root $nodeName /variables automatically generated/\n");
  fprintf(stdout, "ngetlength $root $nodeName ?length?\n");
  fprintf(stdout, "ngetrlcg $root $nodeName ?r? ?l? ?c? ?g?\n");
  fprintf(stdout, "ngettypeprm $root $nodeName ?p1? ?p2? ?p3? ?p4?\n");
  fprintf(stdout, "nsetlength $root $nodeName $length\n");
  fprintf(stdout, "nsetrlcg $root $nodeName $r $l $c $g\n");
  fprintf(stdout, "nsettypeprm $root $nodeName $p1 $p2 $p3 $p4\n");
  fprintf(stdout, "rflc $z $z0 ?rc?\n");
  fprintf(stdout, "vswr $rflc  ?vswr?\n");
  fprintf(stdout, "zpar $x $y ?z?\n");
  fprintf(stdout, "nimp  $topologyFileName  $nodeName $zs $fmin $fmax $points "
                  "?imResp? [ ?zinput? ?fresp? ]\n");
  fprintf(stdout, "ncurrlength $topology $nodeName $points ?voltage?\n");

  return TCL_OK;
}
