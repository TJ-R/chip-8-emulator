#include "chip8.h"
#include <SDL3/SDL_scancode.h>
#include <stdint.h>
#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>

int main()
{
  Chip8 chip8;
  printf("Just before init.\n");
  init(&chip8);
  setup_graphics(&chip8);

  if (load_rom(&chip8, "../programs/Tetris [Fran Dachille, 1991].ch8"))
  {
    printf("ROM loaded successfully\n");
    printf("Waiting to quit...\n");
  }

  // This will be the endless cycle for the program
  // Need to move some of the code from setup_graphics to here
  // and then break that out into some other functions
  for (;;)
  {
    // Scan inputs
    scan_keys(&chip8);

    if (chip8.cpu_paused)
    {
      // Waiting for release frame
      if (chip8.pressedKey != UINT8_MAX)
      {
        if (!chip8.keys[chip8.pressedKey])
        {
          chip8.registers[chip8.paused_register] = chip8.pressedKey;
          chip8.cpu_paused = false;
          chip8.pc = chip8.pc + 2;
          chip8.pressedKey = UINT8_MAX;
        }
      }
      // Waiting for clean input not previously held
      else
      {
        for (int i = 0; i < 16; i++)
        {
          if (chip8.keys[i] && !chip8.key_was_down[i])
          {
            chip8.pressedKey = i;
          }
        }
      }
    }
    else
    {
      // Perform one operation
      int cycles_per_frame = 10; // or 20, depending on CPU speed
      for (int i = 0; i < cycles_per_frame; i++)
      {
        if (!chip8.cpu_paused)
          cpu_cycle(&chip8);
      }
    }

    if (chip8.draw_flag)
    {
      draw_display(&chip8);
    }

    if (chip8.delayTimer > 0)
    {
      chip8.delayTimer -= 1;
    }

    if (chip8.soundTimer > 0)
    {
      chip8.soundTimer -= 1;
    }

    for (int i = 0; i < 16; i++)
    {
      chip8.key_was_down[i] = chip8.keys[i];
    }

    SDL_Delay(16);
  }

  return 0;
}
