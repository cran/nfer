test_that("nfer correctly handles nested_ends", {
  spec   <- nfer::load(system.file("extdata", "nested_ends.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_ends.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[3,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[4,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 80)
  expect_equal(row$end, 80)
  row <- result[5,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 70)
  expect_equal(row$end, 90)
  row <- result[6,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 70)
  expect_equal(row$end, 90)
})

