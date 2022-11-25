test_that("nfer correctly handles dupl_equals", {
  spec   <- nfer::load(system.file("extdata", "dupl_equals.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "dupl_equals.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "A")
  expect_equal(row$start, 10)
  expect_equal(row$end, 50)
  row <- result[2,]
  expect_equal(row$name, "A")
  expect_equal(row$start, 20)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles dupl_simple", {
  spec   <- nfer::load(system.file("extdata", "dupl_simple.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "dupl_simple.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "A")
  expect_equal(row$start, 20)
  expect_equal(row$end, 40)
  row <- result[2,]
  expect_equal(row$name, "A")
  expect_equal(row$start, 40)
  expect_equal(row$end, 60)
})

