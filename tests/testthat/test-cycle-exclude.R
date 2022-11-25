test_that("nfer correctly handles cycle_exclude", {
  # skip as we have to set --full and this isn't supported yet from R
  skip("--full option not yet supported in R")
  spec   <- nfer::load(system.file("extdata", "cycle_exclude.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "cycle_exclude.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "A")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "3")
  row <- result[2,]
  expect_equal(row$name, "A")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "2")
  row <- result[3,]
  expect_equal(row$name, "A")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "1")
  row <- result[4,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "3")
  row <- result[5,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "2")
  row <- result[6,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "1")
  row <- result[7,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "3")
  row <- result[8,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "2")
  row <- result[9,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "1")
})

