test_that("filename is type checked", {
  expect_error(nfer::load(3), "== \"character\"")
  expect_error(nfer::load(TRUE), "== \"character\"")
  expect_error(nfer::load(as.factor("x")), "== \"character\"")
})

test_that("log level is type checked", {
  expect_error(nfer::load(system.file("extdata", "ops.nfer", package = "nfer"), "3"), "== \"numeric\"")
  expect_error(nfer::load(system.file("extdata", "ops.nfer", package = "nfer"), TRUE), "== \"numeric\"")
  expect_error(nfer::load(system.file("extdata", "ops.nfer", package = "nfer"), as.factor("3")), "== \"numeric\"")
})

test_that("non-extant spec files throw an error", {
  expect_error(nfer::load("foo"), "Unable to load specification")
})

test_that("log level is set", {
  skip("there doesn't seem to be a way to check REprintf output")
  spec   <- nfer::load(system.file("extdata", "ops.nfer", package = "nfer"), 1)
  input  <- nfer::read(system.file("extdata", "ops.events", package = "nfer"))
  expect_output(nfer::apply(spec, input), "Creating initial pool")
})