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

  if (load_rom(&chip8, "../programs/6-keypad.ch8"))
  {
    printf("ROM loaded successfully\n");
    printf("Waiting to quit...\n");
  }

  // This will be the endless cycle for the program
  // Need to move some of the code from setup_graphics to here
  // and then break that out into some other functions
  SDL_Event e;
  for (;;)
  {

    while (SDL_PollEvent(&e) != 0)
    {
      SDL_Scancode scancode = e.key.scancode;

      if (e.type == SDL_EVENT_QUIT)
      {
        printf("Should be exiting...\n");
        SDL_DestroyWindow(chip8.win);
        SDL_Quit();

        return 0;
      }

      if (e.type == SDL_EVENT_KEY_DOWN)
      {
        if (scancode == SDL_SCANCODE_ESCAPE)
        {
          printf("ESC key press...\n");
          printf("Should be exiting...\n");
          SDL_DestroyWindow(chip8.win);
          SDL_Quit();
          return 0;
        }
        else if (scancode == SDL_SCANCODE_1)
        {
          chip8.key_pressed = true;
          chip8.keys[0x01] = 1;
        }
        else if (scancode == SDL_SCANCODE_2)
        {
          chip8.key_pressed = true;
          chip8.keys[0x02] = 1;
        }
        else if (scancode == SDL_SCANCODE_3)
        {
          chip8.key_pressed = true;
          chip8.keys[0x03] = 1;
        }
        else if (scancode == SDL_SCANCODE_4)
        {
          chip8.key_pressed = true;
          chip8.keys[0x0C] = 1;
        }
        else if (scancode == SDL_SCANCODE_Q)
        {
          chip8.key_pressed = true;
          chip8.keys[0x04] = 1;
        }
        else if (scancode == SDL_SCANCODE_W)
        {
          chip8.key_pressed = true;
          chip8.keys[0x05] = 1;
        }
        else if (scancode == SDL_SCANCODE_E)
        {
          chip8.key_pressed = true;
          chip8.keys[0x06] = 1;
        }
        else if (scancode == SDL_SCANCODE_R)
        {
          chip8.key_pressed = true;
          chip8.keys[0x0D] = 1;
        }
        else if (scancode == SDL_SCANCODE_A)
        {
          chip8.key_pressed = true;
          chip8.keys[0x07] = 1;
        }
        else if (scancode == SDL_SCANCODE_S)
        {
          chip8.key_pressed = true;
          chip8.keys[0x08] = 1;
        }
        else if (scancode == SDL_SCANCODE_D)
        {
          chip8.key_pressed = true;
          chip8.keys[0x09] = 1;
        }
        else if (scancode == SDL_SCANCODE_F)
        {
          chip8.key_pressed = true;
          chip8.keys[0x0E] = 1;
        }
        else if (scancode == SDL_SCANCODE_Z)
        {
          chip8.key_pressed = true;
          chip8.keys[0x0A] = 1;
        }
        else if (scancode == SDL_SCANCODE_X)
        {
          chip8.key_pressed = true;
          chip8.keys[0x00] = 1;
        }
        else if (scancode == SDL_SCANCODE_C)
        {
          chip8.key_pressed = true;
          chip8.keys[0x0B] = 1;
        }
        else if (scancode == SDL_SCANCODE_V)
        {
          chip8.key_pressed = true;
          chip8.keys[0x0F] = 1;
        }
      }
      else if (e.type == SDL_EVENT_KEY_UP)
      {
        if (scancode == SDL_SCANCODE_1)
        {
          chip8.key_pressed = false;
          chip8.keys[0x01] = 0;
        }
        else if (scancode == SDL_SCANCODE_2)
        {
          chip8.key_pressed = false;
          chip8.keys[0x02] = 0;
        }
        else if (scancode == SDL_SCANCODE_3)
        {
          chip8.key_pressed = false;
          chip8.keys[0x03] = 0;
        }
        else if (scancode == SDL_SCANCODE_4)
        {
          chip8.key_pressed = false;
          chip8.keys[0x0C] = 0;
        }
        else if (scancode == SDL_SCANCODE_Q)
        {
          chip8.key_pressed = false;
          chip8.keys[0x04] = 0;
        }
        else if (scancode == SDL_SCANCODE_W)
        {
          chip8.key_pressed = false;
          chip8.keys[0x05] = 0;
        }
        else if (scancode == SDL_SCANCODE_E)
        {
          chip8.key_pressed = false;
          chip8.keys[0x06] = 0;
        }
        else if (scancode == SDL_SCANCODE_R)
        {
          chip8.key_pressed = false;
          chip8.keys[0x0D] = 0;
        }
        else if (scancode == SDL_SCANCODE_A)
        {
          chip8.key_pressed = false;
          chip8.keys[0x07] = 0;
        }
        else if (scancode == SDL_SCANCODE_S)
        {
          chip8.key_pressed = false;
          chip8.keys[0x08] = 0;
        }
        else if (scancode == SDL_SCANCODE_D)
        {
          chip8.key_pressed = false;
          chip8.keys[0x09] = 0;
        }
        else if (scancode == SDL_SCANCODE_F)
        {
          chip8.key_pressed = false;
          chip8.keys[0x0E] = 0;
        }
        else if (scancode == SDL_SCANCODE_Z)
        {
          chip8.key_pressed = false;
          chip8.keys[0x0A] = 0;
        }
        else if (scancode == SDL_SCANCODE_X)
        {
          chip8.key_pressed = false;
          chip8.keys[0x00] = 0;
        }
        else if (scancode == SDL_SCANCODE_C)
        {
          chip8.key_pressed = false;
          chip8.keys[0x0B] = 0;
        }
        else if (scancode == SDL_SCANCODE_V)
        {
          chip8.key_pressed = false;
          chip8.keys[0x0F] = 0;
        }
      }
    }

    // Perform one operation
    cpu_cycle(&chip8);

    if (chip8.draw_flag)
    {
      draw_display(&chip8);
    }

    chip8.delayTimer -= 1;
    chip8.soundTimer -= 1;
    SDL_Delay(16);
  }

  return 0;
}
