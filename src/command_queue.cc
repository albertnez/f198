#include "command_queue.h"
#include "scene_node.h"


void CommandQueue::push(const Command& command) {
  m_queue.push(command);
}

Command CommandQueue::pop() {
  Command command = m_queue.front();
  m_queue.pop();
  return command;
}

bool CommandQueue::empty() const {
  return m_queue.empty();
}
