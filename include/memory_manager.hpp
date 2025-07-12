#ifndef OSEMU_MEMORY_MANAGER_H_
#define OSEMU_MEMORY_MANAGER_H_

#include <cstdint>
#include <list>
#include <memory>
#include <mutex>
#include <string>

namespace osemu {

struct MemoryBlock {
    uint32_t start_address;
    uint32_t size;
    bool is_free;
    uint32_t pcb_id;
};

class MemoryManager {
public:
    explicit MemoryManager(uint32_t total_size);
    bool allocate(uint32_t pcb_id, uint32_t size);
    void free(uint32_t pcb_id);
    void generate_memory_report(const std::string& filename) const;

    // NEW: Method to check if a process is already in memory.
    // This is a const method because it only reads the memory state.
    bool is_allocated(uint32_t pcb_id) const;

private:
    void coalesce_free_blocks(std::list<MemoryBlock>::iterator newly_freed_block);
    std::list<MemoryBlock> memory_map_;
    uint32_t total_memory_size_;
    mutable std::mutex memory_mutex_;
};

}
#endif
