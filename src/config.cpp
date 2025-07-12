#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

namespace osemu {

Config::Config(uint32_t cpu, SchedulingAlgorithm sched, uint32_t quantum,
               uint32_t freq, uint32_t minIns, uint32_t maxIns, uint32_t delay,
               uint32_t maxMem, uint32_t memFrame, uint32_t memProc)
    : cpuCount{std::clamp(cpu, 1u, 128u)},
      scheduler{sched},
      quantumCycles{
          std::clamp(quantum, 1u, std::numeric_limits<uint32_t>::max())},
      processGenFrequency{
          std::clamp(freq, 1u, std::numeric_limits<uint32_t>::max())},
      minInstructions{
          std::clamp(minIns, 1u, std::numeric_limits<uint32_t>::max())},
      maxInstructions{std::clamp(maxIns, minInstructions,
                                 std::numeric_limits<uint32_t>::max())},
      delayCyclesPerInstruction{delay},
      // Initialize new memory members
      maxOverallMemory{maxMem},
      memoryPerFrame{memFrame},
      memoryPerProcess{memProc} {
  if (scheduler != SchedulingAlgorithm::RoundRobin) {
    quantumCycles = 1;
  }
}

Config Config::fromFile(const std::filesystem::path& file) {
  std::ifstream in(file);
  if (!in) {
    throw std::runtime_error("Cannot open file: " + file.string());
  }

  Config cfg;
  std::string key, value;
  while (in >> key >> value) {
    if (key == "num-cpu") {
      cfg.cpuCount = std::stoul(value);
    } else if (key == "scheduler") {
      cfg.scheduler = (value == "fcfs") ? SchedulingAlgorithm::FCFS
                                        : SchedulingAlgorithm::RoundRobin;
    } else if (key == "quantum-cycles") {
      cfg.quantumCycles = std::stoul(value);
    } else if (key == "batch-process-freq") {
      cfg.processGenFrequency = std::stoul(value);
    } else if (key == "min-ins") {
      cfg.minInstructions = std::stoul(value);
    } else if (key == "max-ins") {
      cfg.maxInstructions = std::stoul(value);
    } else if (key == "delay-per-exec") {
      cfg.delayCyclesPerInstruction = std::stoul(value);
    } else if (key == "max-overall-mem") { // New
      cfg.maxOverallMemory = std::stoul(value);
    } else if (key == "mem-per-frame") { // New
      cfg.memoryPerFrame = std::stoul(value);
    } else if (key == "mem-per-proc") { // New
      cfg.memoryPerProcess = std::stoul(value);
    }
  }
  return cfg;
}

}  // namespace osemu
