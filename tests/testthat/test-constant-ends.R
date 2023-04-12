test_that("nfer correctly handles constant_ends", {
  spec   <- nfer::load(system.file("extdata", "constant_ends.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "constant_ends.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 21)
  expect_equal(row$end, 32)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 61)
  expect_equal(row$end, 72)
})

