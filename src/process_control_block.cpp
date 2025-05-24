#include "process_control_block.hpp"

#include <sstream>
#include <format>
#include <iostream>

PCB::PCB(std::string procName, size_t totalLines)
    : processName(std::move(procName)),
      currentInstruction(0),
      totalInstructions(totalLines),
      creationTime(std::chrono::system_clock::now()) {}

void PCB::step() {
  if (currentInstruction < totalInstructions) {
    currentInstruction++;
  }
}

bool PCB::isComplete() const { return currentInstruction >= totalInstructions; }

std::string PCB::status() const {
  std::ostringstream oss;
  oss << processName << "  " << currentInstruction << "/" << totalInstructions
      << " Created at: "
      << std::format("{:%m/%d/%Y, %I:%M:%S %p}", creationTime);
  return oss.str();
}

void PCB::addToHistory(const std::string& command, const std::string& output) {
    if (!command.empty()) {
        sessionHistory.emplace_back(command, output);
    }
}

void PCB::printHistory() const {
    for (const auto& entry : sessionHistory) {
        std::cout << "~ " << entry.command << "\n";
        if (!entry.output.empty()) {
            std::cout << entry.output << "\n";
        }
    }
}

void PCB::clearHistory() {
    sessionHistory.clear();
}