
test_that("correct types are created", {
  expect_true(inherits(create_array_in_c(), 'array'))
  expect_true(inherits(create_list_in_c(), 'list'))
  expect_true(inherits(create_data_frame_in_c(), 'data.frame'))
})
