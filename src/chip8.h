#include "stack.h"
#include <stdint.h>

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_START_ADDRESS 0x200
#define CHIP8_REGISTER_COUNT 16
#define FONTSET_START_ADDRESS 0x050
#define FONTSET_SIZE 80

typedef struct Chip8
{
  uint16_t pc;
  uint16_t indexRegister;
  uint8_t delayTimer;
  uint8_t soundTimer;
  uint8_t registers[CHIP8_REGISTER_COUNT];
  Stack stack;
  uint8_t memory[CHIP8_MEMORY_SIZE];
  uint8_t gfx[64 * 32];
} Chip8;

void init(Chip8 *chip8);
int load_rom(Chip8 *chip8, const char *filename);
int cpu_loop(Chip8 *chip8);
uint16_t fetch_opcode(Chip8 *chip8);
uint16_t decode_opcode(Chip8 *chip8, uint16_t opcode);
