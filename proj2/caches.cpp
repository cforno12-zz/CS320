#include "caches.hpp"

void direct_mapped(std::string instruct, unsigned int  address, int cache_size, unsigned int cache[], int* counter){
    //cache size /32
    int total_cache_size = cache_size * 1024;
    int cache_line_size = 32;
    int byte_offset = 5; //2^5 == 32

    unsigned int num_slots = (total_cache_size)/(cache_line_size);
    unsigned int index = (address >> byte_offset) % (num_slots); //shifting by 5 since the cache line is 32 bytes
    unsigned int tag = address >> (unsigned int)(log2(num_slots)+byte_offset);

    if(cache[index] == tag){
        (*counter)++;
    } else {
        cache[index] = tag;
    }
}

void set_associative(std::string instruct, unsigned int address, int* counter, unsigned int** cache, int set, deque<unsigned int> lru_queue){

    int byte_offset = 5;
    int cache_line_size = 32;
    int cache_size = 16 * 1024;

    int num_slots = ((cache_size)/(cache_line_size))/set;
    unsigned int index = (address >> byte_offset)%(num_slots);
    unsigned int tag = address >> (unsigned int) (log2(num_slots)+byte_offset);

}
    //this queue holds the tags
    // std::deque<unsigned int> lru_queue;

//     //access

//     //check to make sure it isn't in the queue already
//     bool update = false;
//     for(int x : lru_queue){
//         if (lru_queue.at(x) == tag){
//             update = true;
//             lru_queue.erase(x);
//             lru_queue.push_front(tag);
//             break;
//         }
//     }
//     //replace if queue is full
//     int replace_this_element_in_cache = -1;
//     if(lru_queue.size() == cache_size && !update){
//         replace_this_element_in_cache = lru_queue.at(cache_size-1);
//         lru_queue.pop_back();
//     } else if(!update){
//         lru_queue.push_front(tag);
//     }

// }

// void lru_helper(std::deque<unsigned int> lru_queue){
//     //check to make sure it isn't in the queue already
//     bool update = false;
//     for(int x : lru_queue){
//         if (lru_queue.at(x) == tag){
//             update = true;
//             lru_queue.erase(x);
//             lru_queue.push_front(tag);
//             break;
//         }
//     }
//     //replace if queue is full
//     int replace_this_element_in_cache = 0;
//     if(lru_queue.size() == cache_size && !update){
//         replace_this_element_in_cache = lru_queue.at(cache_size-1);
//         lru_queue.pop_back();
//     } else if(!update){
//         lru_queue.push_front(tag);
//     }
// }
