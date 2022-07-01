// SPDX-License-Identifier: zlib-acknowledgement

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <limits.h>
#include <cmocka.h>

extern void qemu_arm_exit(void);

extern int testable_main(void);

// unit tests for data structures, math, state machines, i.e. edge of project

void
test_tem(void **state)
{
  assert_int_equal(testable_main(), 0);
}

// Is there any real point of testing main() as it will only be a few iterations anyway and simply testing a return code?
int 
main(void)
{
	const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_tem),
  };

  int cmocka_res = cmocka_run_group_tests(tests, NULL, NULL);

  qemu_arm_exit();

  return cmocka_res;
}

// if wanting to run on target, will require semihosting I/O functionality implemented by JLink
// https://mcuoneclipse.com/2015/08/23/tutorial-using-gnu-profiling-gprof-with-arm-cortex-m/

// mocks will utilise expect_ functions
// 1. Identify object to be mocked
// 2. What arguments and return values do you want to see for the mock object

int
__wrap_mock(char *path)
{
  check_expected_ptr(path);
  
  // mock_ptr_type(char *)
  int value = mock_type(int);

  return value;
}

void
test_with_mock(void **state)
{
  // this is what it will take
  expect_string(__wrap_mock, path, "hi");

  // this is what values are obtained from stack with mock_type(type) 
  will_return(__wrap_mock, 0);

  // so, seems we have two options in passing values to mock objects?

  // now call with arguments as we stated were expected
  int ret = some_func();
  assert_int_equal();
}

// a mock object will be setup in a caller function


// a mock is used when wanting to check if a function was called, e.g. expect_

// --wrap=open will redirect to __wrap_open

// may require specifying the weak attribute on source functions to be mocked 
// this allows redefinition of the symbol inside same translation unit due to unity build (perhaps good to put this in a macro for testing builds only?)

// using mocks will imply interaction based testing over state based testing

void
__wrap_led_init(void)
{
  function_called();   
}

void
test_main_should_initialise_led(void)
{
  // could I just pass in led_init()?
  expect_function_call(__wrap_led_init);
  assert_int_equal(0, testable_main());
}

/* 1. paramater checking: 
     expect_string() <-> check_expected_ptr()
     expect_value() <-> check_expected()
   2. variables:
     will_return() <-> mock_type()
   3. call ordering:
     expect_function_call() <-> function_called()
*/
