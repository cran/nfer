test_that("name and timestamp columns have correct names", {
  input <- nfer::read(system.file("extdata", "simple_before.events", package = "nfer"))
  expect_equal(names(input)[1], "Name")
  expect_equal(names(input)[2], "Time")
})

test_that("boolean data columns are set correctly", {
  input <- nfer::read(system.file("extdata", "where_bool.events", package = "nfer"))
  expect_equal(names(input)[3], "foo")
  expect_equal(class(input$foo), "logical")
  expect_equal(input[1,]$foo, TRUE)
  expect_equal(input[2,]$foo, NA)
  expect_equal(input[3,]$foo, FALSE)
  expect_equal(input[4,]$foo, NA)
  expect_equal(input[5,]$foo, TRUE)
  expect_equal(input[6,]$foo, NA)
})

test_that("int data columns are set correctly", {
  input <- nfer::read(system.file("extdata", "where_int_equals.events", package = "nfer"))
  expect_equal(names(input)[3], "foo")
  expect_equal(names(input)[4], "bar")
  expect_equal(class(input$foo), "integer")
  expect_equal(class(input$bar), "integer")
  expect_equal(input[1,]$foo, 1)
  expect_equal(input[2,]$foo, NA_integer_)
  expect_equal(input[3,]$foo, 3)
  expect_equal(input[4,]$foo, NA_integer_)
  expect_equal(input[5,]$foo, 2)
  expect_equal(input[6,]$foo, NA_integer_)
  expect_equal(input[1,]$bar, NA_integer_)
  expect_equal(input[2,]$bar, 2)
  expect_equal(input[3,]$bar, NA_integer_)
  expect_equal(input[4,]$bar, 1)
  expect_equal(input[5,]$bar, NA_integer_)
  expect_equal(input[6,]$bar, 3)
})

test_that("real data columns are set correctly", {
  input <- nfer::read(system.file("extdata", "where_real_equals.events", package = "nfer"))
  expect_equal(names(input)[3], "foo")
  expect_equal(names(input)[4], "bar")
  expect_equal(class(input$foo), "numeric")
  expect_equal(class(input$bar), "numeric")
  expect_equal(input[1,]$foo, 1.1)
  expect_equal(input[2,]$foo, NA_real_)
  expect_equal(input[3,]$foo, 3.3)
  expect_equal(input[4,]$foo, NA_real_)
  expect_equal(input[5,]$foo, 2.2)
  expect_equal(input[6,]$foo, NA_real_)
  expect_equal(input[1,]$bar, NA_real_)
  expect_equal(input[2,]$bar, 2.2)
  expect_equal(input[3,]$bar, NA_real_)
  expect_equal(input[4,]$bar, 1.1)
  expect_equal(input[5,]$bar, NA_real_)
  expect_equal(input[6,]$bar, 3.3)
})

test_that("string data columns are set correctly", {
  input <- nfer::read(system.file("extdata", "where_string_equals.events", package = "nfer"))
  expect_equal(names(input)[3], "foo")
  expect_equal(names(input)[4], "bar")
  expect_equal(class(input$foo), "character")
  expect_equal(class(input$bar), "character")
  expect_equal(input[1,]$foo, "one")
  # this is something we have to live with, I think: strings are converted to empty not NA
  expect_equal(input[2,]$foo, "")
  expect_equal(input[3,]$foo, "three")
  expect_equal(input[4,]$foo, "")
  expect_equal(input[5,]$foo, "two")
  expect_equal(input[6,]$foo, "")
  expect_equal(input[1,]$bar, "")
  expect_equal(input[2,]$bar, "two")
  expect_equal(input[3,]$bar, "")
  expect_equal(input[4,]$bar, "one")
  expect_equal(input[5,]$bar, "")
  expect_equal(input[6,]$bar, "three")
})