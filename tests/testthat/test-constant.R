test_that("nfer correctly handles constant", {
  spec   <- nfer::load(system.file("extdata", "constant.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "constant.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 60)
  expect_equal(row$end, 70)
})

