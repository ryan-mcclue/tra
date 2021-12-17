// SPDX-License-Identifier: zlib-acknowledgement

#if defined(EMULATOR_TESTING)
#define MAIN int test_main(void)
#else
#define MAIN int main(void)
#endif

typedef reg volatile u32;

typedef volatile struct
{
  reg something1;
  reg something2;
} GPIOReg;
#if defined(EMULATOR_TESTING)
GPIOReg PORTA;
#endif

void
led_init(void)
{
  return;
}

MAIN
{
  led_init();

  return 0;
}
