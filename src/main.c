#include "chip8.h"
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

  if (load_rom(&chip8, "../programs/Pong (alt).ch8"))
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

      if (e.type == SDL_EVENT_QUIT)
      {
        printf("Should be exiting...\n");
        SDL_DestroyWindow(chip8.win);
        SDL_Quit();

        return 0;
      }
      else if (e.type == SDL_EVENT_KEY_DOWN)
      {
        if (e.key.key == SDLK_Q)
        {
          printf("Q key press...\n");
          printf("Should be exiting...\n");
          SDL_DestroyWindow(chip8.win);
          SDL_Quit();

          return 0;
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
