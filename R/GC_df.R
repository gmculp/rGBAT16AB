###a test function

GC_df <- function(x, id_col, add_col, boro_col) {
    #return("test")

  out_df <- GBAT(x, id_col, add_col, boro_col)


  #More output fields to come...
  out_df$GC_X_COOR <- gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", substr(out_df$out_all2, 127, 133), perl=TRUE)
  out_df$GC_Y_COOR <- gsub("(?<=[\\s])\\s*|^\\s+|\\s+$", "", substr(out_df$out_all2, 134, 140), perl=TRUE)

  #remove space deliminated output
  out_df$out_all1 <- NULL
  out_df$out_all2 <- NULL
  out_df$out_all2AP <- NULL

  return(out_df)
}
