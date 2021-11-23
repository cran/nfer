test_that("nfer correctly handles simple_coincide", {
  spec   <- nfer::load(system.file("extdata", "simple_coincide.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_coincide.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  row <- result[3,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 40)
  expect_equal(row$end, 50)
  row <- result[4,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 40)
  expect_equal(row$end, 50)
  row <- result[5,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 40)
  expect_equal(row$end, 50)
})

test_that("nfer correctly handles where_after_lhs", {
  spec   <- nfer::load(system.file("extdata", "where_after_lhs.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_after_lhs.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 20)
  expect_equal(row$end, 20)
})

test_that("nfer correctly handles simple_contain", {
  spec   <- nfer::load(system.file("extdata", "simple_contain.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_contain.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  row <- result[3,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 40)
  row <- result[4,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 60)
  expect_equal(row$end, 70)
  row <- result[5,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 50)
  expect_equal(row$end, 80)
})

test_that("nfer correctly handles nested_time", {
  spec   <- nfer::load(system.file("extdata", "nested_time.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_time.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "BOOT")
  expect_equal(row$start, 42)
  expect_equal(row$end, 160)
  expect_equal(row$count, "3")
  row <- result[2,]
  expect_equal(row$name, "RISK")
  expect_equal(row$start, 160)
  expect_equal(row$end, 255)
  row <- result[3,]
  expect_equal(row$name, "BOOT")
  expect_equal(row$start, 255)
  expect_equal(row$end, 312)
  expect_equal(row$count, "4")
})

# fixed by hand because the order changes from online monitoring
test_that("nfer correctly handles simple_finish", {
  spec   <- nfer::load(system.file("extdata", "simple_finish.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_finish.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[3,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  row <- result[1,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[6,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 50)
  expect_equal(row$end, 60)
  row <- result[4,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 40)
  expect_equal(row$end, 60)
  row <- result[5,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 40)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles nested_contain_map", {
  spec   <- nfer::load(system.file("extdata", "nested_contain_map.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_contain_map.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 20)
  expect_equal(row$end, 50)
})

test_that("nfer correctly handles where_atomic", {
  spec   <- nfer::load(system.file("extdata", "where_atomic.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_atomic.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 10)
  expect_equal(row$end, 10)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 50)
  expect_equal(row$end, 50)
})

test_that("nfer correctly handles where_after_both_eq", {
  spec   <- nfer::load(system.file("extdata", "where_after_both_eq.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_after_both_eq.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 40)
  expect_equal(row$end, 40)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 60)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles nested_map_or", {
  spec   <- nfer::load(system.file("extdata", "nested_map_or.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_map_or.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 20)
  expect_equal(row$end, 50)
  row <- result[2,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 100)
  expect_equal(row$end, 130)
})

test_that("nfer correctly handles exponential", {
  # skip exponential as we have to set --full and this isn't supported yet from R
  skip("--full option not yet supported in R")
  spec   <- nfer::load(system.file("extdata", "exponential.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "exponential.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "0")
  row <- result[2,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "3")
  row <- result[3,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "1")
  row <- result[4,]
  expect_equal(row$name, "B")
  expect_equal(row$start, 0)
  expect_equal(row$end, 0)
  expect_equal(row$v, "2")
})

test_that("nfer correctly handles nested_after_where", {
  spec   <- nfer::load(system.file("extdata", "nested_after_where.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_after_where.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 20)
  expect_equal(row$end, 50)
  row <- result[2,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 40)
  expect_equal(row$end, 70)
})

test_that("nfer correctly handles docs_also", {
  spec   <- nfer::load(system.file("extdata", "docs_also.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_also.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
})

test_that("nfer correctly handles docs_comment", {
  spec   <- nfer::load(system.file("extdata", "docs_comment.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_comment.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  expect_equal(row$proc, "1")
  expect_equal(row$comment, "running")
  row <- result[2,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
  expect_equal(row$proc, "2")
  expect_equal(row$comment, "running")
})

test_that("nfer correctly handles simple_after", {
  spec   <- nfer::load(system.file("extdata", "simple_after.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_after.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 20)
  expect_equal(row$end, 20)
})

test_that("nfer correctly handles simple_meet", {
  spec   <- nfer::load(system.file("extdata", "simple_meet.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_meet.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 40)
  row <- result[3,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 40)
  expect_equal(row$end, 50)
  row <- result[4,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 40)
  expect_equal(row$end, 70)
  row <- result[5,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 50)
  expect_equal(row$end, 70)
  row <- result[6,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 60)
  expect_equal(row$end, 70)
})

test_that("nfer correctly handles simple_utf8", {
  l10n <- l10n_info()
  if (l10n$`UTF-8`) {
    spec   <- nfer::load(system.file("extdata", "simple_utf8.nfer", package = "nfer"))
    input  <- nfer::read(system.file("extdata", "simple_utf8.events", package = "nfer"))
    result <- nfer::apply(spec, input)
    row <- result[1,]
    expect_equal(row$name, "身体")
    expect_equal(row$start, 30)
    expect_equal(row$end, 40)
    row <- result[2,]
    expect_equal(row$name, "身体")
    expect_equal(row$start, 50)
    expect_equal(row$end, 60)
  } else {
    skip("No UTF-8 support in this R build!")
  }
})

test_that("nfer correctly handles where_after_both", {
  spec   <- nfer::load(system.file("extdata", "where_after_both.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_after_both.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 30)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 40)
  expect_equal(row$end, 40)
  row <- result[3,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 60)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles where_after_map", {
  spec   <- nfer::load(system.file("extdata", "where_after_map.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_after_map.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 20)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 60)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles fmsd_risk_nested", {
  spec   <- nfer::load(system.file("extdata", "fmsd_risk_nested.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "fmsd_risk_nested.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "BOOT")
  expect_equal(row$start, 42)
  expect_equal(row$end, 160)
  expect_equal(row$count, "3")
  row <- result[2,]
  expect_equal(row$name, "RISK")
  expect_equal(row$start, 42)
  expect_equal(row$end, 312)
  expect_equal(row$count, "3")
  row <- result[3,]
  expect_equal(row$name, "BOOT")
  expect_equal(row$start, 255)
  expect_equal(row$end, 312)
  expect_equal(row$count, "4")
})

test_that("nfer correctly handles where_after", {
  spec   <- nfer::load(system.file("extdata", "where_after.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_after.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 20)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 40)
  expect_equal(row$end, 40)
})

test_that("nfer correctly handles nested_map", {
  spec   <- nfer::load(system.file("extdata", "nested_map.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_map.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 50)
  expect_equal(row$end, 80)
})

test_that("nfer correctly handles docs_import", {
  spec   <- nfer::load(system.file("extdata", "docs_import.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_import.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "TESTING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "TWOTESTS")
  expect_equal(row$start, 10)
  expect_equal(row$end, 65)
  row <- result[3,]
  expect_equal(row$name, "TESTING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
})

test_that("nfer correctly handles simple_start", {
  spec   <- nfer::load(system.file("extdata", "simple_start.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_start.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[3,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[4,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 40)
  expect_equal(row$end, 50)
  row <- result[5,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 40)
  expect_equal(row$end, 60)
  row <- result[6,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 40)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles docs_testing", {
  spec   <- nfer::load(system.file("extdata", "docs_testing.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_testing.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  expect_equal(row$proc, "1")
  row <- result[2,]
  expect_equal(row$name, "TESTING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[3,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
  expect_equal(row$proc, "2")
  row <- result[4,]
  expect_equal(row$name, "TESTING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
})

test_that("nfer correctly handles simple_overlap", {
  spec   <- nfer::load(system.file("extdata", "simple_overlap.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_overlap.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  row <- result[3,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 30)
  expect_equal(row$end, 50)
  row <- result[4,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 30)
  expect_equal(row$end, 60)
  row <- result[5,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 40)
  expect_equal(row$end, 60)
  row <- result[6,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 40)
  expect_equal(row$end, 70)
  row <- result[7,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 50)
  expect_equal(row$end, 70)
})

test_that("nfer correctly handles docs_nested_testing", {
  spec   <- nfer::load(system.file("extdata", "docs_nested_testing.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_nested_testing.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "TESTING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "TESTING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
})

test_that("nfer correctly handles simple_follow", {
  spec   <- nfer::load(system.file("extdata", "simple_follow.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_follow.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 40)
  row <- result[3,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 20)
  expect_equal(row$end, 40)
  row <- result[4,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 40)
  expect_equal(row$end, 50)
  row <- result[5,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 50)
  expect_equal(row$end, 70)
  row <- result[6,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 60)
  expect_equal(row$end, 70)
})

test_that("nfer correctly handles simple_during", {
  spec   <- nfer::load(system.file("extdata", "simple_during.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_during.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 40)
  row <- result[3,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 60)
  expect_equal(row$end, 70)
  row <- result[4,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 50)
  expect_equal(row$end, 80)
  row <- result[5,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 50)
  expect_equal(row$end, 80)
})

test_that("nfer correctly handles where_real_equals", {
  spec   <- nfer::load(system.file("extdata", "where_real_equals.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_real_equals.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 10)
  expect_equal(row$end, 40)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 30)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles nested_bool", {
  spec   <- nfer::load(system.file("extdata", "nested_bool.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_bool.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 70)
  expect_equal(row$end, 90)
})

test_that("nfer correctly handles where_bool", {
  spec   <- nfer::load(system.file("extdata", "where_bool.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_bool.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 50)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles nested_coincide", {
  spec   <- nfer::load(system.file("extdata", "nested_coincide.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_coincide.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 40)
  expect_equal(row$end, 50)
})

test_that("nfer correctly handles docs_atomic", {
  spec   <- nfer::load(system.file("extdata", "docs_atomic.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_atomic.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "STARTING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 10)
  expect_equal(row$pid, "1")
  row <- result[2,]
  expect_equal(row$name, "SUCCESS")
  expect_equal(row$start, 15)
  expect_equal(row$end, 15)
  row <- result[3,]
  expect_equal(row$name, "ENDING")
  expect_equal(row$start, 20)
  expect_equal(row$end, 20)
  expect_equal(row$pid, "1")
  row <- result[4,]
  expect_equal(row$name, "STARTING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 50)
  expect_equal(row$pid, "2")
  row <- result[5,]
  expect_equal(row$name, "ENDING")
  expect_equal(row$start, 65)
  expect_equal(row$end, 65)
  expect_equal(row$pid, "2")
})

test_that("nfer correctly handles fmsd_risk", {
  spec   <- nfer::load(system.file("extdata", "fmsd_risk.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "fmsd_risk.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "BOOT")
  expect_equal(row$start, 42)
  expect_equal(row$end, 160)
  expect_equal(row$count, "3")
  row <- result[2,]
  expect_equal(row$name, "DBOOT")
  expect_equal(row$start, 42)
  expect_equal(row$end, 312)
  expect_equal(row$count, "3")
  row <- result[3,]
  expect_equal(row$name, "RISK")
  expect_equal(row$start, 42)
  expect_equal(row$end, 312)
  expect_equal(row$count, "3")
  row <- result[4,]
  expect_equal(row$name, "BOOT")
  expect_equal(row$start, 255)
  expect_equal(row$end, 312)
  expect_equal(row$count, "4")
})

test_that("nfer correctly handles where_bool_equals", {
  spec   <- nfer::load(system.file("extdata", "where_bool_equals.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_bool_equals.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 10)
  expect_equal(row$end, 40)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 30)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles nested_during", {
  spec   <- nfer::load(system.file("extdata", "nested_during.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_during.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 50)
  expect_equal(row$end, 80)
})

test_that("nfer correctly handles simple_slice", {
  spec   <- nfer::load(system.file("extdata", "simple_slice.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_slice.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 20)
  expect_equal(row$end, 30)
  row <- result[3,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 30)
  expect_equal(row$end, 50)
  row <- result[4,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 40)
  expect_equal(row$end, 50)
  row <- result[5,]
  expect_equal(row$name, "F")
  expect_equal(row$start, 40)
  expect_equal(row$end, 60)
  row <- result[6,]
  expect_equal(row$name, "G")
  expect_equal(row$start, 50)
  expect_equal(row$end, 60)
  row <- result[7,]
  expect_equal(row$name, "E")
  expect_equal(row$start, 50)
  expect_equal(row$end, 70)
})

test_that("nfer correctly handles where_string_equals", {
  spec   <- nfer::load(system.file("extdata", "where_string_equals.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_string_equals.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 10)
  expect_equal(row$end, 40)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 30)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles docs_operating", {
  spec   <- nfer::load(system.file("extdata", "docs_operating.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_operating.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
})

test_that("nfer correctly handles simple_before", {
  spec   <- nfer::load(system.file("extdata", "simple_before.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "simple_before.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 30)
  expect_equal(row$end, 40)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 50)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles docs_two_close_ops", {
  spec   <- nfer::load(system.file("extdata", "docs_two_close_ops.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_two_close_ops.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "TWO_CLOSE_OPS")
  expect_equal(row$start, 10)
  expect_equal(row$end, 65)
  row <- result[3,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
})

test_that("nfer correctly handles docs_short_op", {
  spec   <- nfer::load(system.file("extdata", "docs_short_op.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_short_op.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "SHORT_OP")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
})

test_that("nfer correctly handles docs_pid_equals", {
  spec   <- nfer::load(system.file("extdata", "docs_pid_equals.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_pid_equals.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[2,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
})

test_that("nfer correctly handles where_int_equals", {
  spec   <- nfer::load(system.file("extdata", "where_int_equals.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "where_int_equals.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 10)
  expect_equal(row$end, 40)
  row <- result[2,]
  expect_equal(row$name, "C")
  expect_equal(row$start, 30)
  expect_equal(row$end, 60)
})

test_that("nfer correctly handles docs_failure", {
  spec   <- nfer::load(system.file("extdata", "docs_failure.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "docs_failure.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "SUCCESS")
  expect_equal(row$start, 15)
  expect_equal(row$end, 15)
  row <- result[2,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 10)
  expect_equal(row$end, 20)
  row <- result[3,]
  expect_equal(row$name, "OPERATING")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
  row <- result[4,]
  expect_equal(row$name, "FAILURE")
  expect_equal(row$start, 50)
  expect_equal(row$end, 65)
})

test_that("nfer correctly handles nested_bool_equals", {
  spec   <- nfer::load(system.file("extdata", "nested_bool_equals.nfer", package = "nfer"))
  input  <- nfer::read(system.file("extdata", "nested_bool_equals.events", package = "nfer"))
  result <- nfer::apply(spec, input)
  row <- result[1,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 10)
  expect_equal(row$end, 30)
  row <- result[2,]
  expect_equal(row$name, "D")
  expect_equal(row$start, 40)
  expect_equal(row$end, 90)
})

