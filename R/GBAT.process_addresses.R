#' The \code{GBAT.process_addresses} function is a more efficient way to geocode a large data frame of NYC addresses where there are duplicates.  It aggregates, geocodes and then merges the geocoded results back to the original data frame.

#function to aggregate addresses, geocode them 
#and merge the results back to the original dataset
#this is a more efficient way to handle large datasets with a lot
#of duplicate addresses
GBAT.process_addresses <- function(in_df, add_col_name, third_col_name, source_cols, geocode_cols, add_type="boro_code", return_type="all") {

	ret_cols <- c(source_cols, geocode_cols)
	
	#aggregate duplicate addresses
	df_unq <- unique(in_df[,c(add_col_name,third_col_name)])
	
	#add unique id
	df_unq$u_id <- 1:nrow(df_unq)
	
	#record process start time
	p1 <- proc.time()
	
	#call geocoder based on address type
	if(grepl("^Z.*$",add_type, ignore.case = TRUE)){
		GC_df <- GBAT.zip_code(df_unq,"u_id",add_col_name,third_col_name)
	} else {
		GC_df <- GBAT.borough_code(df_unq,"u_id",add_col_name,third_col_name)
	}
	
	#record process end time
	p2 <- proc.time() - p1
	
	#processing statistics (records per second)
	cat(paste0(round((nrow(GC_df)/as.numeric(p2[3])), digits = 0)," records per second", "\n"))
	
	#in order to lighten load on merge, discard extraneous columns
	keep_col <- c(c(add_col_name,third_col_name),gc_cols)
	GC_df <- GC_df[,keep_col]
	
	#join back to original
	output_df <- merge(in_df, GC_df, by=c(add_col_name,third_col_name))
	
	#extracted successfully geocoded records
	success_df <- unique(subset(output_df, F1E_GRC=="00" | F1E_GRC=="01", select=ret_cols))
	
	#percentage of successfully geocoded records
	cat(paste0(round((nrow(success_df)/nrow(in_df))*100, digits = 0),"% records successfully geocoded"))
	
	#option to return only successfully geocoded records
	if (return_type != "all") {
		return(success_df[,ret_cols])
	} else {
		return(output_df[,ret_cols])
	}
}