#include "caches.hpp"

void print_vector(std::vector<int> path){
    for (std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i)
        std::cout << *i << " ";
}

void print_deque(std::deque<int> path){
    for (std::deque<int>::const_iterator i = path.begin(); i != path.end(); ++i)
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

void set_associative(std::string instruct, unsigned int address, int* counter, std::vector< std::vector<int> > &cache, int way, std::vector< std::deque<int> > &lru_queue, int num_slots, bool no_alloc){


    //each row has to have its own lru_queue
    //the lru_queue holds ways in the row
    int byte_offset = 5;
    unsigned int index = (address >> byte_offset) % (num_slots);
    unsigned int tag = address >> (unsigned int) (log2(num_slots) + byte_offset);


    //we are looking for a hit in the cache
    bool hit = false;
    int hit_way = 0;
    hit_helper(hit, hit_way, tag, (*counter), index, way, cache);
    if(no_alloc){
        if(instruct == "S" && !hit){
            return;
        }
    }
    lru(index, tag, way, cache, lru_queue, hit, hit_way);
}

//int fully_associative(std::string instruct, unsigned int address, int &counter);


void next_line_prefetch(std::string instruct, unsigned int address, int &counter, std::vector< std::vector<int> > &cache, int way, std::vector< std::deque<int> > &lru_queue, int num_slots, bool miss){
    int cache_line = 32;
    int next_line = address + cache_line;

    int byte_offset = 5;
    unsigned int index = (address >> byte_offset) % (num_slots);
    unsigned int tag = address >> (unsigned int) (log2(num_slots) + byte_offset);

    unsigned int index_next = (next_line >> byte_offset) % (num_slots);
    unsigned int tag_next = next_line >> (unsigned int) (log2(num_slots) + byte_offset);

    //look for a hit
    bool hit = false;
    int hit_way = 0;
    hit_helper(hit, hit_way, tag, counter, index, way, cache);
    lru(index, tag, way, cache, lru_queue, hit, hit_way);

    if((miss && !hit) || (!miss)){
        //prefetch
        hit = false;
        hit_way = 0;
        int dummy_counter = 0;
        hit_helper(hit, hit_way, tag_next, dummy_counter, index_next, way, cache);
        lru(index_next, tag_next, way, cache, lru_queue, hit, hit_way);
    }
}

void fully_associative(std::string instruct, unsigned int address, int& counter, std::vector< std::vector<int> > &cache, std::vector< std::deque<int> > &lru_queue, int ways){

    int byte_offset = 5;
    int num_slots = 1;
    unsigned int index = (address >> byte_offset) % (num_slots);
    unsigned int tag = address >> (unsigned int) (log2(num_slots) + byte_offset);

    int hit_way = 0;
    bool hit = false;
    hit_helper(hit, hit_way, tag, counter, index, ways, cache);
    lru(index, tag, ways, cache, lru_queue, hit, hit_way);
}



void hit_helper(bool &hit, int &hit_way, int tag, int &counter, int index, int way, std::vector< std::vector<int> > &cache){
    // std::cout << cache[index][0] << std::endl;
    // std::cout << tag << std::endl;
    for(int i = 0; i < way; i++){
        if(cache[index][i] == (int) tag){
            counter++;
            hit = true;
            hit_way = i;
            break;
        }
    }
}

void lru(int index, int tag, int way, std::vector < std::vector<int> > &cache, std::vector< std::deque<int> > &lru_queue, bool hit, int hit_way){
    //is the set full?
    bool empty = false;
    for (int i = 0; i < way; i++){
        if(cache[index][i] == -1){
            empty = true;
            break;
        }
    }
    if(hit){
        //condition where there is a cache hit and we don't need to select a victim
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
    } else if(!empty){
        //this is a miss and the queue is full
        int victim = lru_queue[index].back();
        lru_queue[index].pop_back();
        lru_queue[index].push_front(victim);
        cache[index][victim] = tag;
    } else {
        //this is a miss and the set is not full
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
