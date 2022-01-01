// SPDX-License-Identifier: zlib-acknowledgement

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <limits.h>
#include <cmocka.h>

#include <stdio.h>

extern void qemu_arm_exit(void);

extern int testable_main(void);

void
test_tem(void **state)
{
  // LED_Init_Expect(); something to do with mocking?
  //expect_function_call();
  //function_called();
  // this won't be flexible to function name changes......
  //
  // checking if led is on is really just checking register states
  // this is not useful if using a hal, as we are just testing their framework

  // with testing, unit tests for 'edge modules' and test their state (a whole lot of jargon for the same thing!)
  // naming with 'should' terminology?
  //
  // integration tests for interworking parts and test their interaction 
  // naming with 'expect' terminology? (these expectations are collection of asserts) 
  // in test set-up you configure mocks to return particular values
  // we are concerned with function calls, e.g. when x() is called and returns y I expect z() to be called
  // mocks are required to maintain isolation with integration tests?
  //
  // gcc --Wl,--wrap=func; now have __wrap_func() and __real_func()

  assert_int_equal(testable_main(), 0);
}

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
