test_that("nfer correctly handles exclusive_order", {
  spec   <- nfer::load(system.file("extdata", "exclusive_order.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "exclusive_order.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  row <- result[2,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 1)
  expect_equal(row$end, 1)
})

