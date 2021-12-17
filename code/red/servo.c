// SPDX-License-Identifier: zlib-acknowledgement

typedef struct
{
  r32 angle_max;
  r32 angle_min;
  r32 position;
  r32 target_angle;
} Servo;

void
set_target(Servo *servo, r32 angle)
{
  if (angle > servo->angle_max) 
  {
    servo->target_angle = servo->angle_max;
  }
  else if (angle < servo->angle_min) 
  {
    servo->target_angle = servo->angle_min;
  }
  else
  {
    servo->target_angle = angle;
  }
}

r32
get_duty_cycle_for_angle(Servo *servo, r32 angle)
{
  r32 result = 0.0f; 

  return result;
}

int
main(void)
{
  // 3DOF means 3 servos.
  // For PWM, want period and duty cycle. It can be useful to preserve power as something that is on for say 90% of time looks like 100%?
  // TODO(Ryan): Servo plastic gear more likely to strip?
  // USB is 5V, 0.5A, so operating amp is about 100mA, so fine?
  // From datasheet: 20ms, 50Hz period
  // duty cycle: 1.0ms -90°, 1.5ms 0°, 2ms 90° (5V)

  // pwm_init_period();
  // pwm_set_duty_cycle();
  // uart_init(rx_pin, tx_pin);
  
  /*
    TODO(Ryan): Host program will open COM port with UART configured
    To ensure not sending too fast for hardware,
    if (accumulated_time > 0.05f)
    {
      // We want to be able to say roughly every 50ms, send command
      accumulated_time -= 0.05f;
      send_bytes();
    }
  */

  // IMPORTANT(Ryan): Compress data into single byte if possible to avoid the need for checking
  // if byte out of order, e.g. < 10 must be joint number, > 10 must be angle, etc.
  while (1)
  {
    if (uart_data_available(&uart))
    {
        uart_get_byte(&uart);
    }

    for (int servo_i = 0; servo_i < NUM_SERVOS; ++servo_i)
    {
      update(&servos[servo_i]);
    }
  }

  return 0;
}
