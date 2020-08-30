
#include <R.h>
#include <Rinternals.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Add 2 numbers
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP add_(SEXP x_, SEXP y_) {

  int xtype = TYPEOF(x_);
  int ytype = TYPEOF(y_);

  if (xtype != REALSXP | ytype != REALSXP) {
    Rf_error("add_() expects only floating point numbers i.e. doubles or reals");
  }

  int xlen = length(x_);
  int ylen = length(y_);

  if (xlen != 1 | ylen != 1) {
    error("add_() expects arguments to be length = 1");
  }

  SEXP result = PROTECT(allocVector(REALSXP, 1));
  REAL(result)[0] = asReal(x_) + asReal(y_);
  UNPROTECT(1);

  return result;
}

