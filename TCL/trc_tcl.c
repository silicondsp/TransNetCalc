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
#include "trc_tclcommands.h"
#include <tcl.h>

/*
 * Trc_TranTopCalcInit is called when the package is loaded.
 */

int Trc_TranTopCalcInit(Tcl_Interp *interp) {
  /*
   * Initialize the stub table interface, which is
   * described in Chapter 46.
   */

  // trc_TCL_Interp=interp;

  if (Tcl_InitStubs(interp, "8.1", 0) == NULL) {
    return TCL_ERROR;
  }

  Trc_VectorRegisterTypes(interp);
  // setup compiled regular expressions for real and imaginary from text
  Trc_InitRegExp(interp);
  /*
   * Register two variations of random.
   * The orandom command uses the object interface.
   */

  Tcl_CreateObjCommand(interp, "vec", Trc_VectorObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);
  // printf("In Random the Init obj type this is the
  // name:%s\n",krn_vecObjType_P->name);
  Tcl_CreateObjCommand(interp, "vpr", Trc_VectorObjPrintCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxvec", Trc_VectorCxObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxvecpr", Trc_VectorCxObjPrintCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vrd", Trc_ReadVectorObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vcxrd", Trc_ReadCxVectorObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vset", Trc_SetVectorElementReal,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vget", Trc_GetVectorElementReal,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vcxcr", Trc_VectorComplexCreate,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vcr", Trc_VectorCreate, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vdot", Trc_VectorObjDotProductCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cx", Trc_ScalarComplexObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxmult", Trc_ScalarComplexMultObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxadd", Trc_ScalarComplexAddObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxsub", Trc_ScalarComplexSubObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxdiv", Trc_ScalarComplexDivObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxexp", Trc_ScalarComplexExpObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxtanh", Trc_ScalarComplexTanhObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxsqrt", Trc_ScalarComplexSqrtObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxget", Trc_ScalarComplexExtractObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxcopy", Trc_ScalarComplexDuplicateObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxconj", Trc_ScalarComplexConjugateObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vcxset", Trc_SetVectorElementComplex,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vcxget", Trc_GetVectorElementComplex,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vifft", Trc_VectorInverseFFT, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vfft", Trc_VectorFFT, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "saxpy", Trc_VectorObjSAXPYtCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vconv", Trc_VectorObjConvolveCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxgetpolar", Trc_ScalarComplexPolarObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxpolar", Trc_ScalarComplexFromPolarObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "tload", Trc_LoadTopologyCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateObjCommand(interp, "tpr", Trc_PrintTopologyCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "tcalc", Trc_CalcTopologyCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "ninfo", Trc_NodeInfoCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "nvalues", Trc_NodeCalcValuesCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "nz0gamma", Trc_NodeZ0GammaCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);
  Tcl_CreateObjCommand(interp, "cxinv", Trc_ScalarComplexInverseObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "zpar", Trc_ScalarComplexZParallelObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "nimp", Trc_NetImpulseResponseCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "nvlength", Trc_NetLineLengthCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "nsetrlcg", Trc_NodeSetRLCGCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "nsettypeprm", Trc_NodeSetRLCGCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "nsetlength", Trc_NodeSetLengthCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "ngetrlcg", Trc_NodeGetRLCGCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "ngettypeprm", Trc_NodeGetRLCGCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "ngetlength", Trc_NodeGetLengthCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vswr", Trc_VSWRObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "rflc",
                       Trc_ScalarComplexReflectionCoefficientObjCmd,
                       (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "vhelp", Trc_VHelpObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "cxhelp", Trc_CXHelpObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "thelp", Trc_THelpObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  Tcl_CreateObjCommand(interp, "help", Trc_HelpObjCmd, (ClientData)NULL,
                       (Tcl_CmdDeleteProc *)NULL);

  /*
   * Declare that we implement the random package
   * so scripts that do "package require random"
   * can load the library automatically.
   */
  Tcl_PkgProvide(interp, "display", "1.1");
  return TCL_OK;
}
