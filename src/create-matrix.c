


#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Creating an array within C and returning it to R
//
//  1. Create individual vector
//  2. Create an integer vector to hold the dimensions.
//  3. Set the dimension attribute on the vector
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP create_array_in_c_() {

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Each member of the list gets allocated separately
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int rows = 10;
  int cols = 1;
  int n = rows * cols;

  SEXP arr_ = PROTECT(allocVector(INTSXP, n));
  SEXP dim_ = PROTECT(allocVector(INTSXP, 2));

  INTEGER(dim_)[0] = rows;
  INTEGER(dim_)[1] = cols;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Assign some dummy values into the members
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < n; i++) {
    INTEGER(arr_)[i] = i + 1;
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set the dimensions
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  setAttrib(arr_, R_DimSymbol, dim_);

  UNPROTECT(2);
  return arr_;
}
