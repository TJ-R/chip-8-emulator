#include "chip8.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void init(Chip8 *chip8)
{
  // Sets the starting locating
  // to 0x200 since 0x000 to 0x1FF
  // was generally reserved for the interpreter
  chip8->pc = CHIP8_START_ADDRESS;

  // Sets block of memeory to all 0s
  memset(chip8->memory, 0, CHIP8_MEMORY_SIZE);

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

int cpu_loop(Chip8 *chip8)
{
  uint16_t opcode = fetch_opcode(chip8);
  uint8_t opfamily = (opcode & 0xF000) >> 12;
  uint8_t X = (opcode & 0x0F00) >> 8;
  uint8_t Y = (opcode & 0x00F0) >> 4;
  uint8_t N = (opcode & 0x000F);
  uint8_t NN = (opcode & 0x00FF);
  uint16_t NNN = (opcode & 0x0FFF);

  switch (opfamily)
  {
  case 0x0:
    switch (N)
    {
    case 0x0:
      // Clear the display
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
    break;
  case 0x2:
    break;
  case 0x3:
    break;
  case 0x4:
    break;
  case 0x5:
    break;
  case 0x6:
    break;
  case 0x7:
    break;
  case 0x8:
    break;
  case 0x9:
    break;
  case 0xA:
    break;
  case 0xB:
    break;
  case 0xC:
    break;
  case 0xD:
    break;
  case 0xE:
    break;
  case 0xF:
    break;
  }

  return 1;
}
