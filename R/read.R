#    nfer - a system for inferring abstractions of event streams
#    Copyright (C) 2021  Sean Kauffman
#
#    This file is part of nfer.
#    nfer is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#' Reads an nfer event file into an R dataframe.
#'
#' The nfer event format is not read easily into R.  This function
#' essentially simplifies sharing event traces between the nfer
#' command-line tool and the R package.
#'
#' Nfer event files are formatted as follows:
#' Each row is a pipe-separated list of either 2 or 4 values represents one event.
#' The first column is the event name.  The second is the event timestamp (integer).
#' The third and fourth columns are used when the event carries data.
#' The third column is a semicolon separated list of data keys.
#' The fourth column is a semicolon separate list of data values in the same order 
#' as the keys.
#'
#' Example event file contents: \preformatted{
#' foo|123|x|2
#' bar|987|x;y|3;true
#' }
#' 
#' The result of reading such a file with this function is a dataframe where the 
#' first column contains event names, the second contains event timestamps, and 
#' subsequent columns contain data.  The data is formatted with one column per name.
#' Events that don't carry some data key will have an NA value for that column.
#' 
#' Resulting dataframe of event: \preformatted{
#' | name | timestamp | x |    y |
#' -------------------------------
#' | foo  | 123       | 2 | NA   |
#' | bar  | 987       | 3 | TRUE |
#' }
#' 
#' @param event_file The name of the file containing nfer formatted events.
#' @return A dataframe containing the events.
#' @examples
#' events <- nfer::read(system.file("extdata", "ops.events", package = "nfer"))
#' @export
read <- function(event_file) {
  df <- utils::read.table(event_file, sep="|", header=F, 
        colClasses = "character", fill=T, encoding = "UTF-8")
  columns <- c("Name","Time")
  if (length(df) == 2) {
    names(df) <- columns
    df
  } else if (length(df) == 4) {
    # figure out keys...
    key_strings <- unique(df[,3])
    keys <- character(0)
    key_map <- list()
    for (key_string in key_strings) {
      new_keys <- unlist(strsplit(key_string, ";"))
      keys <- c(keys, new_keys)
      key_map[[key_string]] <- new_keys
    }
    columns <- c(columns, unique(keys))

    values <- data.frame(matrix(ncol = length(columns), nrow = 0))
    colnames(values) <- columns

    # loop over all the rows
    for (row in 1:nrow(df)) {
      # where to put the data
      new_row <- character(length(columns))
      new_row[1] <- df[row,1]
      new_row[2] <- df[row,2]

      # get the values
      new_vals <- unlist(strsplit(df[row,4], ";"))
      # replace T/F values since R needs uppercase, but nfer expects lower
      new_vals <- gsub("^true$", "TRUE", gsub("^false$", "FALSE", new_vals))
      # get the keys for the values
      row_keys <- key_map[[df[row,3]]]
      
      for (i in 1:length(row_keys)) {
        key <- row_keys[i]
        colnum <- match(key, names(values))

        new_row[colnum] <- new_vals[i]
      }
      values[row,] <- new_row
    }

    # attempt to retype things
    # as.is=T says don't convert chars to factors
    # numerals="no.loss" seems like it should prefer int over numeric, but it doesn't, so we aren't passing it
    converted <- utils::type.convert(values, as.is=T)
  }
}