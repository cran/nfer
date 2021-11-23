## ---- include = FALSE---------------------------------------------------------
knitr::opts_chunk$set(
  collapse = TRUE,
  comment = "#>"
)

## ----setup--------------------------------------------------------------------
library(nfer)

## -----------------------------------------------------------------------------
ssps <- nfer::load(system.file("extdata", "ssps.nfer", package = "nfer"))

## -----------------------------------------------------------------------------
ssps <- nfer::load(system.file("extdata", "ssps.nfer", package = "nfer"))
df <- read.table(system.file("extdata", "ssps.events", package = "nfer"), sep="|", header=FALSE, colClasses = "character")
intervals <- nfer::apply(ssps, df)
summary(intervals) 

## -----------------------------------------------------------------------------
test <- nfer::load(system.file("extdata", "ops.nfer", package = "nfer"))
ops <- nfer::read(system.file("extdata", "ops.events", package = "nfer"))
str(ops)
intervals <- nfer::apply(test, ops)
summary(intervals)

## -----------------------------------------------------------------------------
df <- read.table(system.file("extdata", "ssps.events", package = "nfer"), sep="|", header=FALSE)
learned <- nfer::learn(df)
intervals <- nfer::apply(learned, df)
summary(intervals)

