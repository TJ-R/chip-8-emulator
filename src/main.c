#include "chip8.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdint.h>
#include <stdio.h>

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    fprintf(stderr, "SDL Init error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *win = SDL_CreateWindow("Chip_8_Emulator", 64, 32, 0);
  if (!win)
  {
    fprintf(stderr, "Window error: %s\n", SDL_GetError());
  }

  SDL_ShowWindow(win);
  SDL_MaximizeWindow(win);

  SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);
  if (!renderer)
  {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  // Drawing the initial frame so that wayland will display the
  // the window
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  int running = 1;
  SDL_Event e;

  // Start loading the chip-8 stuff

  Chip8 chip8;
  printf("Just before init.\n");
  init(&chip8);
  if (load_rom(&chip8, "../programs/IBM Logo.ch8"))
  {
    printf("ROM loaded successfully\n");
    printf("Waiting to quit...\n");
  }
  else
  {
    running = false;
  }

  while (running)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_EVENT_QUIT)
        running = 0;
    }

    SDL_Delay(16);
  }

  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
