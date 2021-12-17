// SPDX-License-Identifier: zlib-acknowledgement

/*
#!/usr/bin/env python

import serial
import io

# Connect to a device matching what we want
port = list(list_ports.grep(r"/dev/cu.usbmodem*"))[0]
ser = serial.Serial(port.device, 115200, timeout=1)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

# Write our command and send immediately
sio.write("hello\n")
sio.flush()

# Our serial implementation prints the input to the shell
command = sio.readline()
# Read the response on the next line
response = sio.readline()
# output to user
print(response)
 */

/*
BYPASSING INTERMEDIATE LAYERS (LIKE NETWORKS):
$ device_name set "Kitchen Timer"
> OK

$ wifi set MyHotSpot WPA2 mypassword
> OK

INFORMATION:
$ bluetooth_address get
> F0:5C:12:E3:02:BC

$ battery get
> 35% 

ORCHESTRATING TESTS:
$ factory_reset
> OK

$ factory_reset
> FAIL,6
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct
{
  char *name;
  int (*proc)(int argc, char *argv[]);
  char *help_msg;
} CommandInfo;

CommandInfo commands[];

void
init_commands(void)
{
  add_command("ls", command_ls);
}

void
command_output(char *str)
{
  // uart_send_str(str);
}

void
command_ls(int argc, char *argv[])
{
  command_output("We called ls");
  //if (argc > 1)
  //{
  //  command_error("Command takes 0 or 1 arguments\n");
  //}
}

char *
eat_spaces(char *str)
{
  char *result = str;

  while (*result != '\0')
  {
    if (*result != ' ') break;
    result++;
  }

  return result;
}

char *
find_character_from_left(char *str, char ch)
{
  char *result = str;

  while (*result != '\0')
  {
    if (*result == ch) break;
    result++;
  }

  return result;
}

#define MAX_NUM_ARGS 8
#define MAX_ARG_LENGTH 32
void
run_command(char *line)
{
  char args[MAX_NUM_ARGS][MAX_ARG_LENGTH] = {0};
  int arg_count = 0;

  char *remaining = line;
  while (*remaining != '\0')
  {
    remaining = eat_spaces(remaining);
    if (*remaining == '\0') break;
    
    // TODO(Ryan): Remark on general command parsing algorithm
    char *until_space = find_character_from_left(remaining, ' ');
    if (*until_space != '\0')
    {
      int arg_length = (until_space - remaining);
      assert(arg_length > 0);

      snprintf(args[arg_count++], MAX_ARG_LENGTH, "%.*s", arg_length, remaining);  
      remaining += arg_length;
    }
    else
    {
      strcpy(args[arg_count++], remaining);
      break;
    }
  }

  if (arg_count == 0) return;

  bool found = false;
  char *command_name = args[0];
  for (int i = 0; i < MAX_NUM_COMMANDS; ++i)
  {
    if (strcmp(commands[i].name, command_name) == 0)
    {
      // NOTE(Ryan): Reset this every time we issue a command
      issued_error = false;
      commands[i].proc(arg_count - 1, &args[1]);
      found = true;
    }
  }

  if (!found) uart_send_str("Command %s, not found\n", command_name);
}

int
get_integer(char *arg)
{
  if (arg == NULL)
  {
    issued_error = true;
    return 0;
  }
  
  for (int i = 0; i < strlen(arg); ++i)
  {
    char ch = arg[i];
    if (arg[i] >= '0' && arg[i] <= '9')
    {
      
    }
    else
    {
      issued_error = 0;
      return 0;
    }
  }

  return atoi(arg);
}

void
command_add(int argc, char *argv[])
{
  if (argc != 2) 
  {
    uart_send_str("Command requires exactly 2 arguments\n");
    return;
  }

  i0 = get_integer(argv[0]);
  i1 = get_integer(argv[1]);

  if (issued_error) return;
  
  int result = i0 + i1;
  command_output("%d + %d = %d", i0, i1, result);
}

int
main(void)
{
  run_command("   example command here ");

  return 0;
}

#if 0
int
main(void)
{
  while (1)
  {
    uart_send_str("> ");
    // Interrupt will fill in buffer and set global value
    if (command_has_been_received)
    {
      command_response = run_command(command_received);
      uart_send_str(command_response);
      uart_send_str("> ");
    }
  }

  return 0;
}


char
uart_block_get_ch(void)
{
  // IMPORTANT(Ryan): Assume console uses \n instead of \r
}

void
uart_send_ch(char ch)
{

}
#endif
