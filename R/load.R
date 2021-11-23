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

#' Load an nfer specification from a file.
#' 
#' This function loads an existing nfer specification, stored in a file, 
#' into R in a format that can be used by the \code{nfer::apply} function.
#' See \url{http://nfer.io/} for information on how to write nfer specifications.
#' 
#' Nfer specifications consist of rules that describe a relationship between
#' pairs of intervals.  These relationships describe new intervals that can
#' then be used to describe more intervals.  The result is a hierarchy of 
#' intervals.  Although the algorithm takes intervals as its input, event
#' traces are actually the target, so events are converted to intervals with
#' zero duration.
#' 
#' @param specfile The name of file containing the nfer specification to load.
#' @param loglevel (Optional) The logging level to set (0-4), where 0 is only 
#'                 warnings and errors and 3 is debug.  Default is 0.
#' @return A handle to the nfer specification, loaded into R.
#' @examples
#' test <- nfer::load(system.file("extdata", "ops.nfer", package = "nfer"))
#' ssps <- nfer::load(system.file("extdata", "ssps.nfer", package = "nfer"), loglevel=1)
#' @export
load <- function(specfile, loglevel=0) {
  stopifnot(class(specfile) == "character")
  stopifnot(class(loglevel) == "numeric" || class(loglevel) == "integer")
  .Call(load_, specfile, loglevel)  
}
