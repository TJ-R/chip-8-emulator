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
  printf("This is the main function.\n");
  printf("This is something extra to double check if it recompiled.\n");
  return 0;
}
