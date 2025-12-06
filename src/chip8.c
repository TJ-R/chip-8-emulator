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
  memset(chip8->memory, 0, CHIP8_MEMORY_SIZE - 1);
}

int load_rom(Chip8 *chip8, const char *filename)
{
  FILE *rom = fopen(filename, "rb");

  if (!rom)
  {
    fprintf(stderr, "There was an issue loading rom.");
    return 0;
  }

  // taking rom file ptr to end of file to get size
  fseek(rom, 0, SEEK_END);
  long size = ftell(rom);

  // take file pointer back to beginning
  rewind(rom);

  if (size > CHIP8_MEMORY_SIZE - CHIP8_START_ADDRESS)
  {
    fprintf(stderr, "ROM file size is too large to run.");
    fclose(rom);
    return 0;
  }

  // Read the rom into memory starting a chip8 start address of 0x200 (512
  // bytes)
  fread(&chip8->memory[CHIP8_START_ADDRESS], size, 1, rom);
  fclose(rom);

  return 1;
}
