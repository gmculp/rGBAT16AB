###wrapper function for parsing output
GBAT_df <- function(x, id_col, add_col, third_col, third_col_type) {
  out_df <- GBAT(x, id_col, add_col, third_col, third_col_type)

  #break output into fixed width columns
  sapply(1:nrow(GC_output), function(i)
    out_df[[paste0(as.character(GC_output$GC_function[i]),"_",as.character(GC_output$GC_colname[i]))]] <<- gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", substr(out_df[[paste0(as.character(GC_output$GC_function[i]),"_output")]], GC_output$GC_start[i], GC_output$GC_end[i]), perl=TRUE)
  )

  #add columns for census geographies formatted according to USCB
  out_df[['F1E_USCBStateCounty']] <- ifelse(out_df[['F1E_CensusBoro']]==1,'36061',
                                            ifelse(out_df[['F1E_CensusBoro']]==2,'36005',
                                                   ifelse(out_df[['F1E_CensusBoro']]==3,'36047',
                                                          ifelse(out_df[['F1E_CensusBoro']]==4,'36081',
                                                                 ifelse(out_df[['F1E_CensusBoro']]==5,'36085',"")))))





  out_df[['F1E_1990_USCBTract']] <- ifelse(grepl("^[[:digit:]]$",out_df[['F1E_USCBStateCounty']]) & grepl("^[[:digit:]]$",out_df[['F1E_1990CensusTract']]), paste0(out_df[['F1E_USCBStateCounty']], sprintf("%06d", as.numeric(out_df[['F1E_1990CensusTract']]))),"")
  out_df[['F1E_2000_USCBTract']] <- ifelse(grepl("^[[:digit:]]$",out_df[['F1E_USCBStateCounty']]) & grepl("^[[:digit:]]$",out_df[['F1E_2000CensusTract']]), paste0(out_df[['F1E_USCBStateCounty']], sprintf("%06d", as.numeric(out_df[['F1E_2000CensusTract']]))),"")
  out_df[['F1E_2010_USCBTract']] <- ifelse(grepl("^[[:digit:]]$",out_df[['F1E_USCBStateCounty']]) & grepl("^[[:digit:]]$",out_df[['F1E_2010CensusTract']]), paste0(out_df[['F1E_USCBStateCounty']], sprintf("%06d", as.numeric(out_df[['F1E_2010CensusTract']]))),"")

  out_df[['F1E_2000_USCBBlock']] <- ifelse(grepl("^[[:digit:]]$",out_df[['F1E_2000_USCBTract']]) & grepl("^[[:digit:]]$",out_df[['F1E_2000CensusBlock']]), paste0(out_df[['F1E_2000_USCBTract']], sprintf("%04d", as.numeric(out_df[['F1E_2000CensusBlock']])), out_df[['F1E_2000CensusBlockSuffix']]),"")
  out_df[['F1E_2010_USCBBlock']] <- ifelse(grepl("^[[:digit:]]$",out_df[['F1E_2010_USCBTract']]) & grepl("^[[:digit:]]$",out_df[['F1E_2010CensusBlock']]), paste0(out_df[['F1E_2010_USCBTract']], sprintf("%04d", as.numeric(out_df[['F1E_2010CensusBlock']])), out_df[['F1E_2010CensusBlockSuffix']]),"")

  #remove space deliminated output
  out_df$F1A_output <- NULL
  out_df$F1E_output <- NULL
  out_df$FAP_output <- NULL

  return(out_df)
}
