/*  Transmission Line Topology Calculation and Analysis (TransTopCalc TTC) Library 
 *    Copyright (C) 1985-2017  Sasan H Ardalan
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




int Trc_VectorObjPrintCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
int Trc_VectorObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);




int Trc_VectorCxObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		

int Trc_VectorCxObjPrintCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

int Trc_ReadVectorObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
int Trc_ReadCxVectorObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

int Trc_GetVectorElementReal(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_SetVectorElementReal(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);		

int Trc_VectorCreate(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);		

int Trc_VectorComplexCreate(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);		

int Trc_VectorObjDotProductCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_ScalarComplexObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_ScalarComplexDuplicateObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_ScalarComplexConjugateObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);		
		
int Trc_ScalarComplexExtractObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

int Trc_ScalarComplexSqrtObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_ScalarComplexTanhObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);		

int Trc_ScalarComplexExpObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

int Trc_ScalarComplexDivObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);


int Trc_ScalarComplexSubObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

int Trc_ScalarComplexAddObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_ScalarComplexMultObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_SetVectorElementComplex(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);				


int Trc_GetVectorElementComplex(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);



int Trc_VectorInverseFFT(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_VectorFFT(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_VectorObjSAXPYtCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);


int Trc_VectorObjConvolveCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		

int Trc_ScalarComplexPolarObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

int Trc_ScalarComplexFromPolarObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

int Trc_LoadTopologyCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);		
				
int Trc_PrintTopologyCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

		
int Trc_CalcTopologyCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_NodeInfoCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

		
int Trc_NodeCalcValuesCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);		

int Trc_NodeZ0GammaCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);	
		
int Trc_ScalarComplexInverseObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);

int Trc_ScalarComplexZParallelObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		

int Trc_NetImpulseResponseCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
					
int Trc_NetLineLengthCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);	
		
int Trc_NodeSetRLCGCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_NodeSetLengthCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_NodeGetRLCGCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_NodeGetLengthCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
							
		
int Trc_VSWRObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		

int Trc_ScalarComplexReflectionCoefficientObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_THelpObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_VHelpObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);
		
int Trc_CXHelpObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);					

int Trc_HelpObjCmd(ClientData clientData, Tcl_Interp *interp,
		int objc, Tcl_Obj *CONST objv[]);					



