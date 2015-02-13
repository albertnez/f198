#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "command.h"

#include <queue>


class CommandQueue {
public:
  void push(const Command& command);
  Command pop();
  bool empty() const;
  
private:
  std::queue<Command> m_queue;
};

#endif // COMMAND_QUEUE_H
