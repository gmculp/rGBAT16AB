# rGBAT16AB
An approach to using [NYC DCP's geocoder](http://www1.nyc.gov/site/planning/data-maps/open-data/dwn-gde-home.page) from within an R package.

This package is currently in development.  At present, the package only works on Windows 64-bit machines. Requires devtools and RCPP to build from within RStudio.

        ```R
        devtools::install_github("gmculp/rGBAT16AB")
        #load package
        library(rGBAT16AB)

        #create test data frame
        ADDR1 <- c("125 WORTH STREET","42-09 28 ST","250 BEDFORD PARK BLVD W","30 LAFAYETTE AVE")
        #1=MANHATTAN/NEW YORK; 2=BRONX; 3=BROOKLYN/KINGS, 4=QUEENS, 5=STATEN ISLAND/RICHMOND
        BORO <- c(1,4,2,3)
        u_id <- 1:length(ADDR1)
        df = data.frame(u_id, ADDR1, BORO) 

        #geocode test data frame with geocoding package
        #first use might take a few seconds
        df2 <- GC_df(df,'u_id','ADDR1','BORO')
        ```
Functions:

* `GC_df(x,id_col,addr_col,boro_col)` accepts an input data frame and column names for unique id, address and borough code fields.
