#include "boardstate_stack.h"

bool
Boardstate_Stack::is_stack_empty (void)
{
  return stack_size == 0;
}

// ganz schlimm und falsch
void
Boardstate_Stack::push_back_stack (const Boardstate &new_boardstate)
{
  stack[stack_size++] = new_boardstate;
}
void
Boardstate_Stack::pop_back_stack (void)
{
  --stack_size;
}
void
Boardstate_Stack::clear_stack (void)
{
  stack_size = 0;
}
size_t
Boardstate_Stack::give_stack_size (void)
{
  return stack_size;
}
Boardstate &
Boardstate_Stack::back_stack (void)
{
  return stack[stack_size - 1];
}

const Boardstate *
Boardstate_Stack::data_stack (void) const
{
  return stack;
}

Boardstate_Stack::Boardstate_Stack (size_t init_stack_size)
{
  stack = new Boardstate[init_stack_size]();
  stack_size = init_stack_size;
}
