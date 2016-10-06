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

  return(out_df)
}
