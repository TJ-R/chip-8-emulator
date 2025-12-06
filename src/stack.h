#include <stdbool.h>
#include <stdint.h>
#define MAX_STACK_SIZE 100

// Should be safe for 0xFFFF as error value for stack
// since the stack should really only be returning 12-bit
// addresses so an address should never overlap with the
// error value
#define ERROR_VALUE 0xFFFF

typedef struct Stack
{
  int8_t top;
  uint16_t arr[MAX_STACK_SIZE];
} Stack;

void initailize(Stack *stack);
bool isEmpty(Stack *stack);
bool isFull(Stack *stack);
uint16_t peek(Stack *stack);
void push(Stack *stack, uint16_t val);
uint16_t pop(Stack *stack);
