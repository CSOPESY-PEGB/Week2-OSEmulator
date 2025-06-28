#include "config.hpp"

#include <algorithm>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

namespace osemu {

Config::Config(uint32_t cpu, SchedulingAlgorithm sched, uint32_t quantum,
               uint32_t freq, uint32_t minIns, uint32_t maxIns, uint32_t delay)
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
      delayCyclesPerInstruction{delay} {
  if (scheduler != SchedulingAlgorithm::RoundRobin) {
    quantumCycles = 1;
  }
}

Config Config::fromFile(const std::filesystem::path& file) {
  std::ifstream in(file);
  if (!in) {
    // Usar configuración por defecto si no se encuentra el archivo
    return Config(
        1,                              // num-cpu
        SchedulingAlgorithm::FCFS,      // scheduler
        10,                             // quantum-cycles
        10,                             // batch-process-freq
        10,                             // min-ins
        20,                             // max-ins
        0                               // delay-per-exec
    );
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
    }
  }
  return cfg;
}

}  // namespace osemu