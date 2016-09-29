# rGBAT16AB
An approach to using [NYC DCP's geocoder](http://www1.nyc.gov/site/planning/data-maps/open-data/dwn-gde-home.page) from within an R package.

This package is currently in development.  At present, the package only works on Windows 64-bit machines. Requires devtools and RCPP to build from within RStudio.

        ```R
        devtools::install_github("gmculp/rGBAT16AB")
        ```
Functions:

* `GC_df(x,id_col,addr_col,boro_col)` accepts an input data frame and column names for unique id, address and borough code fields.
