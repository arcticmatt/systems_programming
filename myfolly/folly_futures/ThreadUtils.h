#pragma once

#include <sstream>
#include <string>
#include <thread>

std::string tidLabelStr(std::thread::id threadId, std::string label) {
  std::stringstream ss;
  ss << "[" << threadId << " " << label << "] ";
  return ss.str();
}
