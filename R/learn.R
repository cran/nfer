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

#' Learn an nfer specification from an event trace.
#'
#' This function applies the nfer mining algorithm to a trace to attempt to
#' learn a specification.  It will only learn before rules and will only find
#' those when there is periodic behavior in the trace.  This works best for
#' system traces of real-time systems where events represent scheduled, 
#' periodic behavior.  At this time, only the event names will be considered
#' while any data is ignored.
#'
#' Event traces are passed as dataframes with at least two columns.
#' The first two columns contain event names and timestamps, respectively.
#' Names should be strings.  Timestamps should be integers or strings, ideally,
#' but may be numeric.
#' Subsequent columns will be ignored by this function, for now.
#' 
#' Example dataframe events: \preformatted{
#' | name | timestamp | x |    y |
#' -------------------------------
#' | foo  | 123       | 2 | NA   |
#' | bar  | 987       | 3 | TRUE |
#' }
#' 
#' @param events The dataframe containing events.
#' @param loglevel (Optional) The logging level to set (0-4), where 0 is only 
#'                 warnings and errors and 3 is debug.  Default is 0.
#' @return A handle to a learned nfer specification, loaded into R.
#' @examples
#' events <- nfer::read(system.file("extdata", "qnx.events", package = "nfer"))
#' learned <- nfer::learn(events)
#' learned <- nfer::learn(events, loglevel=1)
#' @export
learn <- function(events, loglevel=0) {
  # transform to char from factor
  f <- sapply(events, is.factor)
  events[f] <- lapply(events[f], as.character)

  # input validation
  stopifnot(length(events) >= 2)
  stopifnot(sapply(events, class)[1] == "character")
  # this is weird, but we don't want to lose precision by converting to floating point numbers
  stopifnot(sapply(events, class)[2] == "integer" || sapply(events, class)[2] == "character" || sapply(events, class)[2] == "numeric")
  stopifnot(class(loglevel) == "numeric" || class(loglevel) == "integer")
  .Call(learn_, events, loglevel)
}
