#include "chip8.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init(Chip8 *chip8)
{
  chip8->draw_flag = false;

  // Sets the starting locating
  // to 0x200 since 0x000 to 0x1FF
  // was generally reserved for the interpreter
  chip8->pc = CHIP8_START_ADDRESS;
  chip8->indexRegister = 0;
  chip8->soundTimer = 0;
  chip8->delayTimer = 0;

  // Sets block of memeory to all 0s
  memset(chip8->memory, 0, CHIP8_MEMORY_SIZE);
  memset(chip8->registers, 0, 16);
  memset(chip8->gfx, 0xFF000000, DISPLAY_HEIGHT * DISPLAY_WIDTH);

  for (int i = 0; i < DISPLAY_HEIGHT * DISPLAY_WIDTH; i++)
  {
    chip8->gfx[i] = 0xFF000000;
  }

  // Loading the fonts into memory
  // If need explination of how these are fonts
  // ex. F
  // 0xF0 -> 11110000 without 0s -> 1111
  // 0x80 -> 10000000 without 0s -> 1
  // 0xF0 -> 11110000 without 0s -> 1111
  // 0x80 -> 10000000 without 0s -> 1
  // 0x80 -> 10000000 without 0s -> 1
  //
  // Makes an F in using the on bit i.e. turing that light on
  uint8_t fontset[FONTSET_SIZE] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  printf("Chip init success.\n");
}

int load_rom(Chip8 *chip8, const char *filename)
{
  FILE *rom = fopen(filename, "rb");

  if (!rom)
  {
    fprintf(stderr, "There was an issue loading rom.\n");
    return 0;
  }

  // taking rom file ptr to end of file to get size
  fseek(rom, 0, SEEK_END);
  long size = ftell(rom);

  printf("Size found successfully.\n");

  // take file pointer back to beginning
  rewind(rom);

  if (size > CHIP8_MEMORY_SIZE - CHIP8_START_ADDRESS)
  {
    fprintf(stderr, "ROM file size is too large to run.\n");
    fclose(rom);
    return 0;
  }

  // Read the rom into memory starting a chip8 start address of 0x200 (512
  // bytes)
  printf("Starting read...\n");
  fread(&chip8->memory[CHIP8_START_ADDRESS], size, 1, rom);
  fclose(rom);

  return 1;
}

uint16_t fetch_opcode(Chip8 *chip8)
{
  uint8_t firstHalf = chip8->memory[chip8->pc];
  uint8_t secondHalf = chip8->memory[chip8->pc + 1];

  // Should move pc from 0x200 to 0x216 on first pass?
  chip8->pc = chip8->pc + 2;

  // What this is doing is shifting over 8 bits to make room for second part
  // of instruction
  // 00001111 << 8 -> 11110000
  // 11110000 | 00001010 -> 11111010 (full opcode)
  uint16_t opcode = (uint16_t)firstHalf << 8 | secondHalf;

  return opcode;
}

int setup_graphics(Chip8 *chip8)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    fprintf(stderr, "SDL Init error: %s\n", SDL_GetError());
    return 1;
  }

  chip8->win = SDL_CreateWindow("Chip_8_Emulator", DISPLAY_WIDTH * 10,
                                DISPLAY_HEIGHT * 10, 0);
  if (!chip8->win)
  {
    fprintf(stderr, "Window error: %s\n", SDL_GetError());
  }

  SDL_ShowWindow(chip8->win);
  SDL_MaximizeWindow(chip8->win);

  chip8->renderer = SDL_CreateRenderer(chip8->win, NULL);
  if (!chip8->renderer)
  {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(chip8->win);
    SDL_Quit();
    return 1;
  }

  // Drawing the initial frame so that wayland will display the
  // the window
  SDL_SetRenderDrawColor(chip8->renderer, 0, 0, 0, 255);
  SDL_RenderClear(chip8->renderer);
  SDL_RenderPresent(chip8->renderer);

  chip8->texture = SDL_CreateTexture(chip8->renderer, SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH,
                                     DISPLAY_HEIGHT);

  SDL_SetTextureScaleMode(chip8->texture, SDL_SCALEMODE_NEAREST);

  return 0;
}

// int destroy_graphics() {}
int cpu_cycle(Chip8 *chip8)
{
  uint16_t opcode = fetch_opcode(chip8);
  uint8_t opfamily = (opcode & 0xF000) >> 12;
  uint8_t X = (opcode & 0x0F00) >> 8;
  uint8_t Y = (opcode & 0x00F0) >> 4;
  uint8_t N = (opcode & 0x000F);
  uint8_t NN = (opcode & 0x00FF);
  uint16_t NNN = (opcode & 0x0FFF);

  printf("Opcode: %X\n", opcode);

  switch (opfamily)
  {
  case 0x0:
    switch (N)
    {

    case 0x0:
      // Clear the display
      for (int i = 0; i < DISPLAY_HEIGHT * DISPLAY_WIDTH; i++)
      {
        chip8->gfx[i] = 0xFF000000;
      }
      SDL_RenderClear(chip8->renderer);
      SDL_RenderTexture(chip8->renderer, chip8->texture, NULL, NULL);
      SDL_RenderPresent(chip8->renderer);

      break;

    // This opcode 0x00EE is returning from a subroutine
    case 0xE:;
      // Reason for ; is so that I can put declartion after label without
      // warning
      uint16_t addr = pop(&chip8->stack);
      chip8->pc = addr;
      break;
    }

    break;

  case 0x1:
    chip8->pc = NNN;
    break;

  case 0x2:
    push(&chip8->stack, NNN);
    break;

  case 0x3:
    if (chip8->registers[X] == NN)
    {
      chip8->pc = chip8->pc + 4;
    }
    break;

  case 0x4:
    if (chip8->registers[X] != NN)
    {
      chip8->pc = chip8->pc + 4;
    }
    break;

  case 0x5:
    if (chip8->registers[X] == chip8->registers[Y])
    {
      chip8->pc = chip8->pc + 4;
    }
    break;

  case 0x6:
    chip8->registers[X] = NN;
    break;

  case 0x7:
    chip8->registers[X] = NN + chip8->registers[X];
    break;

  case 0x8:
    switch (N)
    {
    case 0x0:
      chip8->registers[X] = chip8->registers[Y];
      break;
    case 0x1:
      chip8->registers[X] = chip8->registers[X] | chip8->registers[Y];
      break;
    case 0x2:
      chip8->registers[X] = chip8->registers[X] & chip8->registers[Y];
      break;
    case 0x3:
      chip8->registers[X] = chip8->registers[X] ^ chip8->registers[Y];
      break;
    case 0x4:
      // Would overflow since it means Vx + Vy would > UINT8_MAX
      if (chip8->registers[X] > UINT8_MAX - chip8->registers[Y])
      {
        chip8->registers[0xF] = 1;
      }
      else
      {
        chip8->registers[0xF] = 0;
      }
      chip8->registers[X] = chip8->registers[X] + chip8->registers[Y];
      break;
    case 0x5:
      if (chip8->registers[X] > chip8->registers[Y])
      {
        chip8->registers[0xF] = 1;
      }
      else
      {
        chip8->registers[0xF] = 0;
      }

      chip8->registers[X] = chip8->registers[X] - chip8->registers[Y];
      break;
    case 0x6:
      chip8->registers[0xF] = chip8->registers[X] & 0x00000001;
      chip8->registers[X] = chip8->registers[X] >> 1;
      break;
    case 0x7:
      if (chip8->registers[Y] > chip8->registers[X])
      {
        chip8->registers[0xF] = 1;
      }
      else
      {
        chip8->registers[0xF] = 0;
      }

      chip8->registers[X] = chip8->registers[Y] - chip8->registers[X];
      break;
    case 0xE:
      chip8->registers[0xF] = chip8->registers[X] & 0x10000000;
      chip8->registers[0xF] = chip8->registers[X] << 1;
      break;
    }

    break;

  case 0x9:
    if (chip8->registers[X] != chip8->registers[Y])
    {
      chip8->pc = chip8->pc + 4;
    }
    break;

  case 0xA:
    chip8->indexRegister = NNN;
    printf("Index Register set to %x\n", NNN);
    break;

  case 0xB:
    chip8->pc = chip8->registers[0] + NNN;
    break;

  case 0xC:
    chip8->registers[X] = ((rand() % 255) + 1) & NN;
    break;

  case 0xD:;
    printf("Draw screen opcode...\n");
    uint8_t x_cor =
        chip8->registers[X] & 63; // Should keep all indexs from 0 -> 63
    uint8_t y_cor =
        chip8->registers[Y] & 31; // Should keep all indexs from 1 -> 31
    chip8->registers[0xF] = 0;

    for (int i = 0; i < N; i++)
    {
      // This gets the sprite data that we are going to load into
      // the gfx array before it gets put into the texture and stretched
      uint8_t sprite_data = chip8->memory[chip8->indexRegister + i];

      // Checking each bit from most to least (left to right) siginicant
      for (int j = 0; j < 8; j++)
      {
        if (y_cor + j > 63)
        {
          break;
        }
        uint32_t pixel =
            chip8
                ->gfx[(y_cor + i) * 64 + (x_cor + j)]; // info for current pixel

        // Starting the XOR
        // 0 0 => 0
        // 1 0 => 1
        // 0 1 => 1
        // 1 1 => 0
        bool sprite_on =
            (sprite_data >> (7 - j) & 0x01) == 1; // Should be either 1 or 0
        //
        bool pixel_on = (pixel == 0xFFFF) ? true : false;

        if (sprite_on && pixel_on)
        {
          chip8->gfx[(y_cor + i) * 64 + (x_cor + j)] = 0xFF000000;
          chip8->registers[0xF] = 1;
          chip8->draw_flag = true;
        }
        else if (sprite_on && !pixel_on)
        {
          chip8->gfx[(y_cor + i) * 64 + (x_cor + j)] = 0xFFFFFFFF;
          chip8->draw_flag = true;
        }
      }
    }
    break;

  case 0xE:
    switch (NN)
    {
    case 0x9E:; // Might need to fix this
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        if (SDL_EVENT_KEY_DOWN)
        {
          SDL_Scancode scancode = event.key.scancode;
          if (scancode == (chip8->registers[X] & 0x0F))
          {
            chip8->pc = chip8->pc + 4;
          }
        }
        break;
      }
      break;

    case 0xA1:; // Might need to fix this
      SDL_Event e;
      while (SDL_PollEvent(&e))
      {
        if (SDL_EVENT_KEY_DOWN)
        {
          SDL_Scancode scancode = event.key.scancode;
          if (scancode == (chip8->registers[X] & 0x0F))
          {
            break;
          }
        }

        chip8->pc = chip8->pc + 4;
        break;
      }
      break;
    }
    break;

  case 0xF:
    switch (NN)
    {
    case 0x07:
      chip8->registers[X] = chip8->delayTimer;
      break;
    case 0x0A:;
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        if (SDL_EVENT_KEY_DOWN)
        {
          SDL_Scancode scancode = event.key.scancode;
          chip8->registers[X] = scancode;
          break;
        }
      }

      break;
    case 0x15:
      chip8->delayTimer = chip8->registers[X];
      break;
    case 0x18:
      chip8->soundTimer = chip8->registers[X];
      break;
    case 0x1E:
      chip8->indexRegister = chip8->indexRegister + chip8->registers[X];
      break;
    case 0x29:
      chip8->indexRegister = chip8->memory[chip8->registers[X]];
      break;
    case 0x33:;
      int dividedNumber = (int)chip8->registers[X];
      chip8->memory[chip8->indexRegister + 2] = dividedNumber % 10;

      dividedNumber = dividedNumber / 10;
      chip8->memory[chip8->indexRegister + 1] = dividedNumber % 10;

      dividedNumber = dividedNumber / 10;
      chip8->memory[chip8->indexRegister] = dividedNumber % 10;
      break;
    case 0x55:
      for (int i = 0; i < 16; i++)
      {
        chip8->memory[chip8->indexRegister + i] = chip8->registers[i];
      }
      break;
    case 0x65:
      for (int i = 0; i < 16; i++)
      {
        chip8->registers[i] = chip8->memory[chip8->indexRegister + i];
      }
      break;
    }
    break;
  }

  return 1;
}

int draw_display(Chip8 *chip8)
{
  printf("Drawing...\n");
  SDL_UpdateTexture(chip8->texture, NULL, chip8->gfx, 64 * 4);
  SDL_RenderClear(chip8->renderer);
  SDL_RenderTexture(chip8->renderer, chip8->texture, NULL, NULL);
  SDL_RenderPresent(chip8->renderer);
  chip8->draw_flag = false;

  return 0;
}
