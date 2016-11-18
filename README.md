# rGBAT16AB
An approach to using [NYC DCP's geocoder](http://www1.nyc.gov/site/planning/data-maps/open-data/dwn-gde-home.page) from within an R package.  Geosupport Desktop must be installed on your machine prior to building this package.  If you are using a 64-bit machine, you must install the 64-bit version of Geosupport Desktop. If you are using a 32-bit machine, you must install the 32-bit version of Geosupport Desktop. 

This package is currently in development.  At present, the package only works on Windows 32-bit and 64-bit machines. Requires devtools and Rcpp to build from within RStudio.

Installing the package:

        ```R
        #requires devtools
        library(devtools)
        #make sure Rtools is TRUE
        devtools::find_rtools()
        #install package
        devtools::install_github("gmculp/rGBAT16AB")
        #load package
        library(rGBAT16AB)
        ```

If you are behind a web proxy (NYC.gov employees):

        ```R
        #load libraries
        library(RCurl)
        library(httr)
        library(devtools)
	library(withr)
	library(Rcpp)
	
	#does Rtools work with Rcpp
	Rcpp::evalCpp("2+2")
	
        set_config( config( ssl_verifypeer = 0L ) )
        #check web browser options for your proxy
        my_proxy <- "myproxy.nyc"
        set_config(
                use_proxy(url=my_proxy, port=8080, username=Sys.getenv("USERNAME"),password=.rs.askForPassword("Enter password:"))
        )
        options(unzip = 'internal')
        pack_loc <- "gmculp/rGBAT16AB"
        #install package hosted on GitHub
        #check if default library path is local
        #if not, check if R install is local
        #if not, specify local drive
	if (devtools::find_rtools()) {
        	if (grepl("^[[:alpha:]]{1}:/",.libPaths()[1])){
	        	devtools::install_github(pack_loc)
        	} else if (grepl("^[[:alpha:]]{1}:/",Sys.getenv("R_HOME"))) {
	        	install_lib <- paste0(Sys.getenv("R_HOME"),"/library")
	        	withr::with_libpaths(new=install_lib, devtools::install_github(pack_loc), action="replace")
        	} else {
	        	#specify local drive
	        	install_lib <- 'J:/R_libraries'
	        	withr::with_libpaths(new=install_lib, devtools::install_github(pack_loc), action="replace")
        	}
	} else {
		cat("INSTALL RTOOLS!")
	}
        #load package
        library(rGBAT16AB)
        ```
