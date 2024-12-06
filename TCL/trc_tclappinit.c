

#include "cx.h"
#include "trantopcalc_tcl.h"
#include <tcl.h>
#include <trantopcalc.h>

int Trc_TranTopCalcInit(Tcl_Interp *interp);

/*
 * Tcl_AppInit is called from Tcl_Main
 * after the Tcl interpreter has been created,
 * and before the script file
 * or interactive command loop is entered.
 */
int Trc_TclAppInit(Tcl_Interp *interp) {
  /*
   * Tcl_Init reads init.tcl from the TcTrc_TclAppInitl script library.
   */
  if (Tcl_Init(interp) == TCL_ERROR) {
    return TCL_ERROR;
  }

  Trc_TranTopCalcInit(interp);

  /*
   * Register application-specific commands.
   */
  /*
   * Define the start-up filename. This file is read in
   * case the program is run interactively.
   */
  Tcl_SetVar(interp, "tcl_rcFileName", "~/.mytcl", TCL_GLOBAL_ONLY);
  /*
   * Test of Tcl_Invoke, which is defined on page 691.
   */
  Tcl_Invoke(interp, "set", "foo", "$xyz [foo] {", NULL);
  return TCL_OK;
}
