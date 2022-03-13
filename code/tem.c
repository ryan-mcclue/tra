// SPDX-License-Identifier: zlib-acknowledgement

#include <stdbool.h>

// the interrupt handler will increment this by the set resolution
int timestamp = 0; // elapsed time
void
timer_init(void)
{
  int clocks_per_tick = __CORE_CLK / 100; // this is our resolution; calculate with 1/f
  // load = clocks_per_tick; assign tick rate
  // fire interrupt when hit
  timestamp = 0;
}

// NOTE(Ryan): Often there is a VTOR register which we can use to add an interrupt handler
// outside of the startup file, isr_vector = (* long)SCB->VTOR; isr_vector[15] = handler;



#include "stm32f4xx_hal_gpio.c"

typedef float V2 __attribute__((vector_size(8)));

// __cleanup__

#if defined(TRA_TESTING)
  int testable_main(void)
#else
  int main(void)
#endif
{
  // TODO(Ryan): Benefits of CISC over RISC?
  // TODO(Ryan): What are cases when you have to write assembly in embedded?
  // TODO(Ryan): Reasons for differences
  // Cortex M4 harvard -> ibus (rom) + dbus (ram, io) (determinable from datasheet block diagram)
  // Cortex M0 von-neumann
  // TODO(Ryan): How to properly calculate Vdd? Currently measuring from pwr jumper
  // TODO(Ryan): What is boot0 in relation to Vᵢₕ and Vᵢₗ?
  // Vcc is to circuit; 5V. Vdd is device; 3.3V
  // Rpu 30kΩ 
  // Vil: 0.43V, Vih: 1.26V
  // Voh: only know min, Vol: only know max (assume Vdd?)
  
  // First determine forward voltage of led. 
  // Then see how much currrent a particular colour sinks
  // Don't want to connect led as input to MCU, as high current can damage circuitry

  // If writing directly to registers, need to take into account that some operations may
  // require a few cycles, e.g. initialising clock. HAL takes care of this.
  // We could inspect instruction cycle count in cortex-m4 manual
  // current sunk/source?


  // hi-z/floating/tri-stated. useful to share io lines?
  // input (hi-z, pull-down (driven high), pull-up (driven low))
  // output (push-pull, open-drain (ground or disconnected))

  // With switch, require resistor to prevent floating and/or short

  // TODO(Ryan): Investigate changing clock values for optimal operation

  // Raw sources of clock signal:
  //   1. MCU internal clock, say 6MHz. Known as HSI
  //   2. Board external crystal, say 8MHz. Known as HSE (often chosen as more accurate; less thermal)
  //   3. PLL + HSI/HSE
  //   4. Clock input pin (MCO), i.e. feed clock signal from another MCU into this MCU
  // The SYSCLK is the combination of the previous. It's an intermediarey clock that feeds:
  //   1. HCLK (AHB bus, core, memory, DMA)
  //   2. PCLK (APB bus)
  //   3. FCLK (free-running; listen to interrupts when in sleep mode)
  // PLL_M + PLL_N + PLL_P can be fed HSI or HSE before SYSCLK
  
  // Clock may be configured automatically in system.c
  // Inspect rcc.c for startup/default information.

  // systick is timer specific to cortex-m4

  // __disable_irq() intrinsic. use in error handlers?

#if 0
  // NOTE(Ryan): AHB1 speed currently 16MHz
  __HAL_RCC_GPIOG_CLK_ENABLE();

  GPIO_InitTypeDef gpio_button = {0};
  gpio_button.Pin  = GPIO_PIN_0;
  gpio_button.Mode = GPIO_MODE_INPUT;
  gpio_button.Pull = GPIO_PULLDOWN;
  //gpio_button.Speed = ;
  //gpio_button.Alternate = ;
  HAL_GPIO_Init(GPIOG, &gpio_button);

  GPIO_InitTypeDef gpio_led = {0};
  gpio_led.Pin  = GPIO_PIN_1;
  // TODO(Ryan): When would you want open drain? 
  gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_led.Pull = GPIO_NOPULL;
  gpio_led.Speed = GPIO_SPEED_FREQ_LOW;
  //gpio_led.Alternate = ;
  HAL_GPIO_Init(GPIOG, &gpio_led);

  bool gpio_button_was_down = false;

  // Interrupts are generally more efficient. Response time-critical. Allow for power saving.
  // Not hitting bus as hard which might interfere with other bus masters, e.g. DMA
  
  // Polling when duration of operation specific, e.g. sending out 50us pulse
  // If high throughput, e.g 30000 times a second, context switch will be too much (i-cache, d-cache?)

  while (1)
  {
    bool gpio_button_is_down = (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_0) == GPIO_PIN_SET);
    if (gpio_button_is_down) 
    {
      if (!gpio_button_was_down)
      {
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_1);
      }
      gpio_button_was_down = true;
    }
    else
    {
      gpio_button_was_down = false;
    }

  }
#endif

  return 0;
}
