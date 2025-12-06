#include "stack.h"
#include <stdbool.h>
#include <stdio.h>

void initialize(Stack *stack) { stack->top = -1; }

bool isEmpty(Stack *stack)
{
  if (stack->top == -1)
    return true;

  return false;
}

bool isFull(Stack *stack)
{
  if (stack->top == MAX_STACK_SIZE - 1)
    return true;

  return false;
}

uint16_t peek(Stack *stack)
{
  printf("This is a formatter test");
  return stack->arr[stack->top];
}

void push(Stack *stack, uint16_t val)
{
  if (isFull(stack))
  {
    fprintf(stderr, "Cannot push %d stack is full.\n", val);
    return;
  }

  stack->top = stack->top + 1;
  stack->arr[stack->top] = val;
  return;
}

uint16_t pop(Stack *stack)
{
  if (isEmpty(stack))
  {
    fprintf(stderr, "Cannot pop stack is empty.\n");
    return ERROR_VALUE;
  }

  uint16_t ret = stack->arr[stack->top];
  stack->top = stack->top - 1;

  return ret;
}
