#include "chip8.h"
#include <stdint.h>
#include <stdio.h>

int main()
{
  Chip8 chip8;
  printf("Just before init.\n");
  init(&chip8);
  setup_graphics(&chip8);

  if (load_rom(&chip8, "../programs/IBM Logo.ch8"))
  {
    printf("ROM loaded successfully\n");
    printf("Waiting to quit...\n");
  }

  // This will be the endless cycle for the program
  // Need to move some of the code from setup_graphics to here
  // and then break that out into some other functions
  for (;;)
  {
  }

  return 0;
}
