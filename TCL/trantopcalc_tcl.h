/*  Transmission Line Topology Calculation and Analysis (TranTopCalc) Command Tool
    Copyright (C) 1985-2006  Sasan H Ardalan

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcl.h"


#define VECTOR_TYPE_NAME "mvrcx"
#define NODE_TYPE_NAME "node"

#define TRC_TCL_TYPE_SHORT_VECTOR 0
#define TRC_TCL_TYPE_BYTE_VECTOR 1
#define TRC_TCL_TYPE_REAL_VECTOR 2
#define TRC_TCL_TYPE_COMPLEX_VECTOR 3
#define TRC_TCL_TYPE_REAL_MATRIX  4
#define TRC_TCL_TYPE_COMPLEX_MATRIX  5
#define TRC_TCL_TYPE_COMPLEX_SCALAR  6
#define TRC_TCL_TYPE_CAP_CPX_MATRIX 7


#define INVERSE_FFT 1
#define FORWARD_FFT 1



typedef struct short_type {
        short   type;
        int     length;
        short   *vector_P;
} shortVector_t, *shortVector_Pt;

typedef struct short_vector {
        short   type;
} short_t, *short_Pt;

typedef struct byte_vector {
        short   type;
        int     length;
        unsigned char   *vector_P;
} byteVector_t, *byteVector_Pt;

typedef struct double_vector {
        short   type;
	short   transpose;
        int     length;
        double   *vector_P;
} doubleVector_t, *doubleVector_Pt;

typedef struct doublecx_vector {
        short   type;
	short   transpose;
        int     length;
        cx_t   *vector_P;
} cxVector_t, *cxVector_Pt;






typedef struct complex_scalar {
        short   type;
        cx_t   value;
} cxScalar_t, *cxScalar_Pt;




typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef short INT16;
typedef int boolean;
typedef long INT32;


cx_t Dsp_Cx_Complex(double a, double b);
cx_t Dsp_Cx_Add( cx_t x, cx_t y);
cx_t Dsp_Cx_Sub(cx_t x, cx_t y);
cx_t Dsp_Cx_Mult(cx_t x, cx_t y);
cx_t Dsp_Cx_Div(cx_t x,  cx_t y);
cx_t Dsp_Cx_Exp( cx_t x);
cx_t Dsp_Cx_Tanh(cx_t x);
cx_t Dsp_Cx_Sqrt(cx_t x);


extern Tcl_ObjType *trc_vecObjType_P;
extern Tcl_ObjType *trc_nodeObjType_P;


/*
 *
 * Declarations for application-specific command procedures
 */

int Trc_TclAppInit(Tcl_Interp *interp);
int Plus1ObjCmd(ClientData clientData, Tcl_Interp *interp,
             int objc, Tcl_Obj *CONST objv[]);


short_t *Trc_GetPtr(Tcl_Obj *obj_P);
void VectorRegisterTypes(Tcl_Interp *interp );
int Trc_VectorFromString(Tcl_Interp *interp, Tcl_Obj *obj_P);


int Trc_VectorFromFile(Tcl_Interp *interp,Tcl_Obj *obj_P,char *fileName);
int Trc_CxVectorFromFile(Tcl_Interp *interp,Tcl_Obj *obj_P,char *fileName);


void Trc_StringFromObject(Tcl_Obj *obj_P);
doubleVector_t *Trc_GetVector(Tcl_Obj *resultPtr);
cxScalar_t *Trc_GetCxScalar(Tcl_Obj *obj_P);


cxVector_t *Trc_GetCxVector(Tcl_Obj *obj_P);

																	
void Trc_PrintCxVector(cxVector_t *vec_P,char format[], FILE *fp, int twocol ) ;
void Trc_PrintVector(doubleVector_t *vec_P,char format[], FILE *fp,int  nolen) ;


								
void IIP_PlotVector(double *vec_P, int points);
int Trc_CxVectorFromString(Tcl_Interp *interp, Tcl_Obj *obj_P);

int Trc_PrintComplexScalarObj(Tcl_Obj *obj_P, char format[], FILE *fp);
void Trc_PrintComplexScalar(cxScalar_t *x_P,char format[], FILE *fp);

doubleVector_t* Dsp_AllocateVector(int length);
cxVector_t* Dsp_AllocateCxVector(int length);



int Dsp_GenGaussComplexVector(cxVector_t *vec_P,float std,float mean, long seedIn);
int Dsp_GenGaussVector(doubleVector_t *vec_P,float std,float mean, long seedIn);
int Dsp_DotComplexComplex(cxVector_t * x, cxVector_t *y, cxScalar_t *z);
int Dsp_DotRealReal(doubleVector_t * x, doubleVector_t *y, double *z);


int Trc_InitRegExp(Tcl_Interp *interp);


doubleVector_t  *Dsp_VectorInverseFFT(cxVector_t *vec_P);
cxVector_t  *Dsp_VectorFFT(doubleVector_t *vec_P);
int Dsp_SAXPYComplex(cxVector_t *vx_P,cxVector_t *vy_P,cxScalar_t *alpha);
int Dsp_SAXPYReal(doubleVector_t *vx_P,doubleVector_t *vy_P,double alpha);
int Dsp_ConvolveReal(doubleVector_t *vx_P,doubleVector_t *vf_P,doubleVector_t *vy_P);
int Dsp_ConvolveComplex(cxVector_t *vx_P,cxVector_t *vf_P,cxVector_t *vy_P);



int Trc_NetImpVec(char *netFileName,char *nodeName,
                 float frmin,float frdel,float frmax,int nfft,
		 float sourceE,float sourceImp,
		 doubleVector_t *imp_P, cxVector_t *zInput_P, cxVector_t *fresp_P );
		 
void Trc_CalcCurrentLengthNode(struct node	*node_P,int points, 
                  doubleVector_t *y_P);



