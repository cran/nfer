test_that("handle size is checked", {
  handle <- list(as.integer(1))
  input <- data.frame(c("x"),c(1))
  expect_error(nfer::apply(handle, input), "length")
})

test_that("log level type is checked", {
  handle <- list(1,2)
  input <- data.frame(c("x"),c(1))
  expect_error(nfer::apply(handle, input), "== \"integer\"")
})

test_that("event name type is checked", {
  handle <- list(as.integer(1),2)
  input <- data.frame(c(2),c(1))
  expect_error(nfer::apply(handle, input), "== \"character\"")
})

test_that("timestamp is type checked", {
  handle <- list(as.integer(1),2)
  input <- data.frame(c("x"),c(TRUE))
  expect_error(nfer::apply(handle, input), "== \"integer\"")
})

