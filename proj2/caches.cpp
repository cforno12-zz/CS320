#include "caches.hpp"

void print_vector(std::vector<int> path){
    for (std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i)
        std::cout << *i << " ";
}


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

void set_associative(std::string instruct, unsigned int address, int* counter, std::vector< std::vector<int> > &cache, int way, std::vector< std::deque<int> > &lru_queue, int num_slots){


    //each row has to have its own lru_queue
    //the lru_queue holds ways in the row
    int byte_offset = 5;
    unsigned int index = (address >> byte_offset) % (num_slots);
    unsigned int tag = address >> (unsigned int) (log2(num_slots) + byte_offset);

    bool hit = false;
    int hit_way = 0;
    for(int i = 0; i < way; i++){
        if(cache.at(index).at(i) == (int) tag){
            (*counter)++;
            hit = true;
            hit_way = i;
            break;
        }
    }
    if(hit){
        bool update = false;
        for(int x = 0; x < way; x++){
            if (lru_queue[index][x] == hit_way){
                update = true;
                lru_queue[index].erase(lru_queue[index].begin()+x);
                lru_queue[index].push_front(hit_way);
                break;
            }
        }
        if(!update){
            lru_queue[index].push_front(hit_way);
        }
    } else if(lru_queue.size() == way){
        //this is a miss and the queue is full
        int victim = lru_queue[index].back();
        lru_queue[index].pop_back();
        lru_queue[index].push_front(victim);

        cache[index][victim] = tag;
    } else {
        //this is a miss and the queue is not full
        int store_me = 0;
        for(int i = 0; i < way; i++){
            if(cache[index][i] == -1){
                cache[index][i] = tag;
                store_me = i;
                break;
            }
        }
        lru_queue[index].push_front(store_me);
    }
}
