#include "caches.hpp"



void print_vector(std::vector<int> path){
    for (std::vector<int>::const_iterator i = path.begin(); i != path.end(); ++i)
        std::cout << *i << " ";
}

void print_deque(std::deque<Index_tuple*> path){
    for (std::deque<Index_tuple*>::const_iterator i = path.begin(); i != path.end(); ++i)
        std::cout << (*i)->set << "," << (*i)->way << " ";
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

void set_associative(std::string instruct, unsigned int address, int* counter, std::vector< std::vector<int> > &cache, int way, std::deque<Index_tuple*> &lru_queue, int num_slots){

    int byte_offset = 5;
    unsigned int index = (address >> byte_offset) % (num_slots);
    unsigned int tag = address >> (unsigned int) (log2(num_slots) + byte_offset);

    bool hit = false;
    int hit_way = 0;
    for(int i = 0; i < way; i++){
        if(cache.at(index).at(i) == tag){
            (*counter)++;
            hit = true;
            hit_way = i;
            break;
        }
    }
    //if(!hit) way--;
    //if( way >= set) std::cout << "whoops" << way << std::endl;
    Index_tuple* store_me = new Index_tuple();
    store_me->set = index;
    store_me->way = 0;
    Index_tuple* victim = new Index_tuple();
    victim->set = 0;
    victim->way = 0;
    bool cold = false;
    if(hit){
        delete victim;
        store_me->way = hit_way;
        bool update = false;
        for(int x = 0; x < (int)lru_queue.size(); x++){
            if (lru_queue.at(x)->set == store_me->set && lru_queue.at(x)->way == store_me->way){
                update = true;
                lru_queue.erase(lru_queue.begin()+x);
                lru_queue.push_front(store_me);
                break;
            }
        }
        if(!update){
            lru_queue.push_front(store_me);
        }
    } else if(lru_queue.size() == (num_slots * way)){
        //this is a miss and the queue is full
        for (std::deque<Index_tuple*>::const_iterator i = lru_queue.end(); i != lru_queue.begin(); --i){
            if(store_me->set == (*i)->set){
                victim = (*i);
                lru_queue.erase(i);
                break;
            }
        }
        delete store_me;
        lru_queue.push_front(victim);
        cache[victim->set][victim->way] = tag;
    } else {
        //this is a miss and the queue is not full
        cold = true;
        delete victim;
        for(int i = 0; i < way; i++){
            if(cache[index][i] == -1){
                cache[index][i] = tag;
                store_me->way = i;
                break;
            }
        }
        store_me->set = index;
        lru_queue.push_front(store_me);
    }
}
