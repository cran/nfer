test_that("argument numbers are checked", {
  input <- data.frame(c(1,2,3))
  expect_error(nfer::learn(input), "length")
})

test_that("event name type is checked", {
  input <- data.frame(c(2),c(1))
  expect_error(nfer::learn(input), "== \"character\"")
})

test_that("timestamp is type checked", {
  input <- data.frame(c("x"),c(TRUE))
  expect_error(nfer::learn(input), "== \"integer\"")
})

test_that("log level is type checked", {
  input <- data.frame(c("x"),c(1))
  expect_error(nfer::learn(input, "3"), "== \"numeric\"")
  expect_error(nfer::learn(input, TRUE), "== \"numeric\"")
  expect_error(nfer::learn(input, as.factor("3")), "== \"numeric\"")
})
