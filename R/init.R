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

#' nfer: A package for inferring interval abstractions of event traces.
#'
#' The nfer package provides an R interface to the nfer language for 
#' abstracting event traces into a hierarchy of intervals with data.
#' 
#' @section nfer functions:
#' load, learn, read, apply
#'
#' @docType package
#' @name nfer
NULL


#' @useDynLib nfer, .registration = TRUE
.onUnload <- function (libpath) {
  library.dynam.unload("nfer", libpath)
}