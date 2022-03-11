// SPDX-License-Identifier: zlib-acknowledgement

// IMPORTANT(Ryan): Assuming both binaries have vendor linker.ld and startup.s

#define MAJOR 0 
#define MINOR 1 

u8 version[2] = { MAJOR, MINOR };
int
main(void)
{
  uart_printf("Bootloader loading");
  toggle_board_led();
  
  goto_application();
}

void
goto_application(void)
{
  uart_printf("Jumping to application");

  void (*application_reset_handler)(void) = *(volatile u32 *)(0x80000 + 4);

  __set_MSP(0x80000);

  application_reset_handler();
}

// bootloader and application are separate binaries

/* FLASH LAYOUT
 * slot 2 - previous version
 * slot 1 - intermediate copy area
 * application - current application
 * bootloader
 */

// ===================================================================================================================================
// Application
// ===================================================================================================================================

// IMPORTANT(Ryan): Inside of system_.c, look for something called vector table offset, e.g. USER_VECT_TAB_ADDRESS, VECT_TAB_OFFSET
// and modify it appropriately

u8 version[2] = { MAJOR, MINOR };

int
main(void)
{
  uart_printf("Application version (%d).(%d)", version[0], version[1]);
}
