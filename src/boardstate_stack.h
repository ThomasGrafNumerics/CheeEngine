#pragma once
#include "boardstate.h"

class Boardstate_Stack
{
  Boardstate *stack;
  size_t stack_size;

public:
  bool is_stack_empty (void);
  void push_back_stack (const Boardstate &new_boardstate);
  void pop_back_stack (void);
  void clear_stack (void);
  size_t give_stack_size (void);
  Boardstate &back_stack (void);
  const Boardstate *data_stack (void) const;

  Boardstate_Stack (size_t);
};
