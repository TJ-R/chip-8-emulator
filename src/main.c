#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdint.h>
#include <stdio.h>

#define MEMORY_SIZE 4096
#define DEFAULT_STACK_SIZE 100

typedef struct {
  uint16_t top;
  uint16_t arr[DEFAULT_STACK_SIZE];
} Stack;

// This is the representation of the chip-8's memory
uint8_t memory[MEMORY_SIZE];

// This is the program counter which points to the next instruction in the stack
// will be ignoring the top 4 bits and acting like this is a 12-bit integer
uint16_t pc = 0;

// This is the representation of the index register again like the program
// counter we will be ignoring the top 4 bits and acting like it is a 12-bit
// integer
uint16_t indexRegister = 0;

// TODO implement a stack so that we have a call stack for 16-bit addresses;
Stack stack;

uint8_t delayTimer = 0;
uint8_t soundTimer = 0;

// These are the representation of the chip-8's variable registers
uint8_t register0 = 0;
uint8_t register1 = 0;
uint8_t register2 = 0;
uint8_t register3 = 0;
uint8_t register4 = 0;
uint8_t register5 = 0;
uint8_t register6 = 0;
uint8_t register7 = 0;
uint8_t register8 = 0;
uint8_t register9 = 0;
uint8_t registerA = 0;
uint8_t registerB = 0;
uint8_t registerC = 0;
uint8_t registerD = 0;
uint8_t registerE = 0;

// This is a flag register i.e. generally it is only set to 0 or 1
uint8_t registerF = 0;

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "SDL Init error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *win = SDL_CreateWindow("Chip_8_Emulator", 800, 600, 0);
  if (!win) {
    fprintf(stderr, "Window error: %s\n", SDL_GetError());
  }

  SDL_ShowWindow(win);
  SDL_MaximizeWindow(win);

  SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);
  if (!renderer) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  int running = 1;
  SDL_Event e;
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT)
        running = 0;
    }

    SDL_Delay(16);
  }

  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
