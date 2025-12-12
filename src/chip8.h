#include "stack.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>
#include <stdint.h>

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_START_ADDRESS 0x200
#define CHIP8_REGISTER_COUNT 16
#define FONTSET_START_ADDRESS 0x050
#define FONTSET_SIZE 80
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

typedef struct Chip8
{
  bool draw_flag;
  uint16_t pc;
  uint16_t indexRegister;
  uint8_t delayTimer;
  uint8_t soundTimer;
  uint8_t registers[CHIP8_REGISTER_COUNT];
  Stack stack;
  uint8_t memory[CHIP8_MEMORY_SIZE];
  uint32_t gfx[64 * 32]; // Will hold all of the display info not a part of
                         // interpreter RGBA -> 32 bits 1 byte per piece
  SDL_Window *win;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  bool cpu_paused; // only true when waiting for input
  uint8_t paused_register;
  uint8_t pressedKey;
  uint8_t keys[16];
  uint8_t key_was_down[16];
} Chip8;

void init(Chip8 *chip8);
int setup_graphics(Chip8 *chip8);
int load_rom(Chip8 *chip8, const char *filename);
int cpu_cycle(Chip8 *chip8);
int draw_display(Chip8 *chip8);
uint16_t fetch_opcode(Chip8 *chip8);
uint16_t current_opcode(Chip8 *chip8);
void scan_keys(Chip8 *chip8);
