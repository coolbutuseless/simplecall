
<!-- README.md is generated from README.Rmd. Please edit that file -->

# simplecall - demo package for calling C code with `.Call()`

<!-- badges: start -->

![](https://img.shields.io/badge/cool-useless-green.svg) [![R build
status](https://github.com/coolbutuseless/simplecall/workflows/R-CMD-check/badge.svg)](https://github.com/coolbutuseless/simplecall/actions)
<!-- badges: end -->

`simplecall` is a small demo package showing how C code could be
included in a package and called with `.Call()`.

This package now includes demo code for:

  - creating a list in C
  - creating a data.frame in C

You can install from
[GitHub](https://github.com/coolbutuseless/simplecall) with:

``` r
# install.package('remotes')
remotes::install_github('coolbutuseless/simplecall)
```

## What’s in the box?

Package contains example C functions and R wrappers to call them

| C function                       | R function                 |
| -------------------------------- | -------------------------- |
| `SEXP add_(SEXP x_, SEXP y_)`    | `add_Call(x, y)`           |
| `SEXP create_list_in_c_()`       | `create_list_in_c()`       |
| `SEXP create_data.frame_in_c_()` | `create_data_frame_in_c()` |

## What’s in the R functions?

  - A call using `.Call()`
  - First argument is the C function name e.g. `add_`
  - Other arguments to the R function can be passed through to C

<!-- end list -->

``` r
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Add two numbers
#'
#' @param x,y numbers to add
#'
#' @useDynLib simplecall add_
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
add_Call <- function(x, y) {
  .Call(add_, x, y)
}
```

## What’s in the C functions?

  - C function returns an `SEXP` pointer which is a real R object
  - all arguments passed as SEXP pointers

<!-- end list -->

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

## Creating an R List within C

Example code is included to create a list completely within C and return
it to R

<details>

<summary> Click to show/hide code </summary>

``` c
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
```

</details>

## Creating an R data.frame within C

Example code is included to create a data.frame completely within C and
return it to R

<details>

<summary> Click to show/hide code </summary>

``` c
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
```

</details>

## Creating an R array within C

Example code is included to create an array completely within C and
return it to R

<details>

<summary> Click to show/hide code </summary>

``` c
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
```

</details>

### Similar packages

This is one of a series of small demo packages for  
calling other languages from R:

  - [{simplec}](https://github.com/coolbutuseless/simplec) - calling C
    with `.C()`
  - [{simplecall}](https://github.com/coolbutuseless/simplecall) -
    calling C with `.Call()`
  - [{simplercpp}](https://github.com/coolbutuseless/simplercpp) -
    calling C++ with `{Rcpp}`
  - [{simplefortran}](https://github.com/coolbutuseless/simplefortran) -
    calling Fortran with `.Fortran()`

## Installation

## Resources

  - [David Vaughan’s post - Now you C
    me](https://blog.davisvaughan.com/2019/03/02/now-you-c-me/)
  - [Peter Dalgaard’s Keynote from
    UseR 2004](http://www.ci.tuwien.ac.at/Conferences/useR-2004/Keynotes/Dalgaard.pdf)
    discusses R’s language interfaces
  - [Using R — Calling C Code ‘Hello
    World’](http://mazamascience.com/WorkingWithData/?p=1067)
  - [Hadley’s Advanced R book chapter - ‘Rs C
    interface’](http://adv-r.had.co.nz/C-interface.html)
  - [Rcpp](https://cran.r-project.org/package=Rcpp)

## Acknowledgements

  - R Core for developing and maintaining such a wonderful language.
  - CRAN maintainers, for patiently shepherding packages onto CRAN and
    maintaining the repository
