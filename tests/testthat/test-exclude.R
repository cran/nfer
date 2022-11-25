test_that("nfer correctly handles exclude_self", {
  spec   <- nfer::load(system.file("extdata", "exclude_self.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "exclude_self.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 1)
  expect_equal(row$x, "0")
  row <- result[2,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 1)
  expect_equal(row$x, "1")
  row <- result[3,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 0)
  expect_equal(row$end, 1)
  row <- result[4,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 0)
  expect_equal(row$end, 2)
  row <- result[6,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 1)
  expect_equal(row$end, 2)
  expect_equal(row$x, "0")
  row <- result[7,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 1)
  expect_equal(row$end, 2)
  expect_equal(row$x, "1")
  row <- result[8,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 1)
  expect_equal(row$end, 2)
  row <- result[5,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 0)
  expect_equal(row$end, 2)
})

