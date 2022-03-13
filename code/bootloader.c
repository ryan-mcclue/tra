// SPDX-License-Identifier: zlib-acknowledgement

/*
 * TODO(Ryan): Put inside of 'What I did today' musing 
ELECTRONICS:
MULTIMETER:
4000 count means can display 3.999. Therefore, left most digit can never be 4.
The decimal place will shift over when needing to display higher than 4V
Autoranging means this will happen automatically and we won't have to
explicitly select the voltage range we are in.
True RMS means can accurately read non-sinusoidal wave forms
 */



// TODO(Ryan): Investigate youtubers ali challenger and later bitluni (use cheap stm32 boards indicated by edwin fairchild youtuber)

// TODO(Ryan): Inside of jlink debugger, know how to inspect flash memory contents of the board

// TODO(Ryan): How is an OTA update any different to a normal wireless update? I think just general term not specific to protocol in use
// Typically will various packet formats for OTA:
// data, start, end, header, response

/*
without bootloader, monolithic firmware
 
Could also unpack ELF file, however usually just
a binary file.
    - flashing over over-the-air, e.g. phone connected to ear buds
    - security, e.g. verify firmware integrity
MSP and PSP refer to single stack pointer r13, however in different contexts? This way for scheduling?
https://bitbucket.org/csowter/redkernel/src/master/
https://electronics.stackexchange.com/questions/403967/main-stack-pointermsp-vs-process-stack-pointerpsp

On reset, MSP is value of 0x000. PC is 0x04 which points to reset handler.
The reset handler will init hardware and copy/init variables from rom to ram.

If a bootloader is present, the bootloader will have its own reset handler that when finished doing appropriate tasks,
will then call the applications reset handler and follow the steps outlined above.
So, bootloader and application will each have their own vector table.

Although arm says it starts at 0x00, the memory map of an mcu may be different.
So, typically have boot modes, (i.e. setting boot pin value) to control where the boot address is. 
e.g. setting to 0 will start from flash at 0x020000. This address will be a memory alias for 0x00.
Different memory starting regions will have different permissions that allow for read/write/execute  

Dual bank flash allows to erase in one bank while running code in another. Useful for updates.
 */



// IMPORTANT(Ryan): Assuming both binaries have vendor linker.ld and startup.s/system.c

#define MAJOR 0 
#define MINOR 1 

u8 version[2] = { MAJOR, MINOR };
int
main(void)
{
  uart_printf("Bootloader loading");
  toggle_board_led();

  while (tick < three_second_tick)
  {
    if (button_down)
    {
      // wait for an approriate signal (e.g. usart) to start copying new firmware
      // so, the data transfer of the new firmware is initiated from an external program 
      
      // TODO(Ryan): Investigate further bootloader functionality from https://www.youtube.com/watch?v=DMHwB4PLfW0  

      hal_nvic_systemreset(); // to load the new update
    }
  }
  
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


// To view the actual STM32 bootloader (stored in system ROM, as oppose to our own which takes us space in user flash)
// look at AN2606 Application Note pdf file, AN3155
// To switch to internal bootloader have to set boot pin; so how does this software coordinate flashing?
// hard reset is cutting power

// by removing the jumpers, typically shorting a particular pin to high or ground   
