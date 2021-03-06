


#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Creating a list within C and returning it to R
//
//  1. Create individual integer/real/whatever vectors
//  2. allocate a VECSXP of the correct size
//  3. assign each member into the list
//  4. create names and assign them to the list
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP create_list_in_c_() {


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Each member of the list gets allocated separately
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int n = 10;

  SEXP idx_ = PROTECT(allocVector(INTSXP , n));
  SEXP x_   = PROTECT(allocVector(REALSXP, n));
  SEXP y_   = PROTECT(allocVector(REALSXP, n));


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Assign some dummy values into the members
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < n; i++) {
    INTEGER(idx_)[i] = i + 1;
    REAL(x_)[i] = i + 10;
    REAL(y_)[i] = i + 100;
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Allocate a list with 3 members
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP list_ = PROTECT(allocVector(VECSXP, 3));

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Add members to the list
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SET_VECTOR_ELT(list_,  0, idx_);
  SET_VECTOR_ELT(list_,  1, x_);
  SET_VECTOR_ELT(list_,  2, y_);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set the names on the list.
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP names = PROTECT(allocVector(STRSXP, 3));
  SET_STRING_ELT(names,  0, mkChar("idx"));
  SET_STRING_ELT(names,  1, mkChar("x"));
  SET_STRING_ELT(names,  2, mkChar("y"));
  setAttrib(list_, R_NamesSymbol, names);

  UNPROTECT(5);
  return list_;
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Creating a data.frame within C and returning it to R
//
//  1. Create individual integer/real/whatever vectors
//  2. allocate a VECSXP of the correct size
//  3. assign each member into the data.frame
//  4. create names and assign them to the data.frame
//  5. set the class to "data.frame"
//  6. set rownames on the data.frame
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP create_data_frame_in_c_() {


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Each member of the list gets allocated separately
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int n = 10;

  SEXP idx_ = PROTECT(allocVector(INTSXP , n));
  SEXP x_   = PROTECT(allocVector(REALSXP, n));
  SEXP y_   = PROTECT(allocVector(REALSXP, n));


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Assign some dummy values into the members
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < n; i++) {
    INTEGER(idx_)[i] = i + 1;
    REAL(x_)[i] = i + 10;
    REAL(y_)[i] = i + 100;
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Allocate a data.frame
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP df_ = PROTECT(allocVector(VECSXP, 3));

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Add columns to the data.frame
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SET_VECTOR_ELT(df_,  0, idx_);
  SET_VECTOR_ELT(df_,  1, x_);
  SET_VECTOR_ELT(df_,  2, y_);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Treat the VECSXP as a data.frame rather than a list
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SET_CLASS(df_, mkString("data.frame"));

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set the names on the list.
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP names = PROTECT(allocVector(STRSXP, 3));
  SET_STRING_ELT(names,  0, mkChar("idx"));
  SET_STRING_ELT(names,  1, mkChar("x"));
  SET_STRING_ELT(names,  2, mkChar("y"));
  setAttrib(df_, R_NamesSymbol, names);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Set the row.names on the list.
  // Use the shortcut as used in .set_row_names() in R
  // i.e. set rownames to c(NA_integer, -len) and it will
  // take care of the rest. This is equivalent to rownames(x) <- NULL
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP rownames = PROTECT(allocVector(INTSXP, 2));
  SET_INTEGER_ELT(rownames, 0, NA_INTEGER);
  SET_INTEGER_ELT(rownames, 1, -n);
  setAttrib(df_, R_RowNamesSymbol, rownames);

  UNPROTECT(6);
  return df_;
}
