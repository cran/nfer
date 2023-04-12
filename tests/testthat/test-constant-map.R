test_that("nfer correctly handles constant_map", {
  spec   <- nfer::load(system.file("extdata", "constant_map.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "constant_map.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  expect_equal(row$y, "3")
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 60)
  expect_equal(row$end, 70)
  expect_equal(row$y, "3")
})

