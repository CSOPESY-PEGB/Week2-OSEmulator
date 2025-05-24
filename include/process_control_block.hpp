// process_control_block.hpp
#pragma once

#include <chrono>
#include <string>
#include <functional>   // for std::hash
#include <vector>

struct HistoryEntry {
    std::string command;
    std::string output;
    
    HistoryEntry(std::string cmd, std::string out) 
        : command(std::move(cmd)), output(std::move(out)) {}
};

struct PCB {
  PCB(std::string procName, size_t totalLines);

  void step();
  bool isComplete() const;
  std::string status() const;

  void addToHistory(const std::string& command, const std::string& output);
  void printHistory() const;
  void clearHistory();

  std::string processName;
  size_t      currentInstruction{0};
  size_t      totalInstructions;
  std::chrono::system_clock::time_point creationTime;
  std::vector<HistoryEntry> sessionHistory; // History of commands executed by the process
};

inline bool operator==(PCB const& a, PCB const& b) noexcept {
  return a.processName == b.processName;
}

namespace std {
template<>
struct hash<PCB> {
  size_t operator()(PCB const& pcb) const noexcept {
    return std::hash<std::string>()(pcb.processName);
  }
};
}
