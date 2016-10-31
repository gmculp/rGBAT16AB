###wrapper function for parsing output
GBAT_df <- function(x, id_col, add_col, third_col, third_col_type) {
  out_df <- GBAT(x, id_col, add_col, third_col, third_col_type)

  #break output into fixed width columns
  sapply(1:nrow(GC_output), function(i)
    out_df[[paste0(as.character(GC_output$GC_function[i]),"_",as.character(GC_output$GC_colname[i]))]] <<- gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", substr(out_df[[paste0(as.character(GC_output$GC_function[i]),"_output")]], GC_output$GC_start[i], GC_output$GC_end[i]), perl=TRUE)
  )

  #remove space deliminated output
  out_df$F1A_output <- NULL
  out_df$F1E_output <- NULL
  out_df$FAP_output <- NULL

  return(GBAT_census_format(out_df))
}

###wrapper function for census fields into USCB format
GBAT_census_format <- function(x) {

  #add columns for census geographies formatted according to USCB
  x[['F1E_USCBStateCounty']] <- ifelse(x[['F1E_CensusBoro']]==1,'36061',
                                            ifelse(x[['F1E_CensusBoro']]==2,'36005',
                                                   ifelse(x[['F1E_CensusBoro']]==3,'36047',
                                                          ifelse(x[['F1E_CensusBoro']]==4,'36081',
                                                                 ifelse(x[['F1E_CensusBoro']]==5,'36085','00000')))))



  x[['F1E_1990_USCBTract']] <- gsub("^0.*$","",paste0(as.character(x[['F1E_USCBStateCounty']]), sprintf("%06d", as.numeric(paste0('0', gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", x[['F1E_1990CensusTract']], perl=TRUE))))))
  x[['F1E_2000_USCBTract']] <- gsub("^0.*$","",paste0(as.character(x[['F1E_USCBStateCounty']]), sprintf("%06d", as.numeric(paste0('0', gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", x[['F1E_2000CensusTract']], perl=TRUE))))))
  x[['F1E_2010_USCBTract']] <- gsub("^0.*$","",paste0(as.character(x[['F1E_USCBStateCounty']]), sprintf("%06d", as.numeric(paste0('0', gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", x[['F1E_2010CensusTract']], perl=TRUE))))))

  x[['F1E_2000_USCBBlock']] <- gsub("^0.*$","",paste0(as.character(x[['F1E_1990_USCBTract']]), sprintf("%04d", as.numeric(paste0('0', gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", x[['F1E_2000CensusBlock']], perl=TRUE)))),x[['F1E_2000CensusBlockSuffix']]))
  x[['F1E_2010_USCBBlock']] <- gsub("^0.*$","",paste0(as.character(x[['F1E_2010_USCBTract']]), sprintf("%04d", as.numeric(paste0('0', gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", x[['F1E_2010CensusBlock']], perl=TRUE)))),x[['F1E_2010CensusBlockSuffix']]))
  return(x)
}

