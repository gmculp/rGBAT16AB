#' The \code{GBAT.zip_code} function geocodes a data frame of NYC addresses using house number, street name, and borough code.
#'
#' The New York Department of City Planning's
#' \href{https://www1.nyc.gov/site/planning/data-maps/open-data/dwn-gde-home.page}{Geosupport Desktop} geocoding software
#' must be installed prior to installing the \code{rGBAT16AB} package.
#'
#' @title Geocode NYC addresses with borough code
#' @name GBAT.borough_code
#' @aliases GBAT.borough_code rGBAT16AB
#' @export GBAT.borough_code
#' @param input_df a data frame containing NYC address
#' @param id_colname the column name of an arbitrary unique identifier
#' @param address_colname the column name of addresses where addresses are in the format of house number and street name (e.g., 123 Smith Street)
#' @param borough_code_colname the column name of the borough code where Manhattan = 1, Bronx = 2, Brooklyn = 3, Queens = 4, and Staten Island = 5
#' @return The \code{GBAT.borough_code} function returns almost 200 columns which are described in the \href{https://www1.nyc.gov/assets/planning/download/pdf/data-maps/open-data/upg.pdf?r=16c}{Geosupport Desktop User Guide}
#' @examples # create a data frame of addresses
#' ADDR <- c("448 Lafayette Ave","727 Manhattan Ave","31-00 47th Ave","308 E 78th S","10 Morton St")
#' ZIP_CODE <- c("11205","11222","11101","10075","10014")
#' BORO_CODE <- c(3,3,4,1,1)
#' u_id <- 1:length(ADDR)
#' df = data.frame(u_id, ADDR, ZIP_CODE, BORO_CODE)
#'
#' #geocode data frame
#' df_gc <- GBAT.borough_code(df,'u_id','ADDR','BORO_CODE')

###wrapper function for geocoding with borough code
GBAT.borough_code <- function(input_df, id_colname, address_colname, borough_code_colname) {
  gc_df <- GBAT_df(input_df, id_colname, address_colname, borough_code_colname, "boro_code")
  return(gc_df)
}
