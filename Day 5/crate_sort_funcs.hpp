#ifndef CRATE_SORT_FUNCS_HPP
#define CRATE_SORT_FUNCS_HPP

#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <map>
#include <utility>
#include <sstream>

void readCrates(std::ifstream& file, std::vector<std::vector<char>>& crates){

    size_t line_size {0};
    while (file.get() != '\n'){
        ++line_size;
    }
    ++line_size; // increment to account for null terminator
     // seek back to start of file
    file.seekg(0L, std::ios_base::beg);
     // create new array to hold each line
    char* line_hold = new char[line_size];

     // vector of pairs (generate iterators for map insert)
    std::vector<std::pair<size_t, std::vector<char>*>> pair_vec (9); 
    size_t idx {0};
    for (auto iter {pair_vec.begin()}; iter != pair_vec.end(); ++iter, ++idx){
         // first = file position ptr, second = pointer to vector for that file position
         //std::clog << "pos: " << pos << '\n';
        *iter = std::make_pair(idx, &crates.at(idx));
    }

     // create map to index in newly created pairs    
    std::map<size_t, std::vector<char>*> vec_ptrs_map (pair_vec.begin(), pair_vec.end());

    size_t cc {7}; // current crate position == 8 (max num of crates per stack at start)
    std::map<size_t, std::vector<char>*>::iterator iter; // hold return from map
    size_t check_idx {1}, current_vec_idx {0};

    while (file.getline(line_hold, line_size) && line_hold[1] != '1'){ // read until crate numbers line
        //std::clog << line_hold << '\n';
        for (size_t idx {0}; idx < line_size; ++idx){
            //std::clog << idx << ' ';
            if (idx == check_idx){
                if (line_hold[idx] >= 65 && line_hold[idx] <= 90){
                    iter = vec_ptrs_map.find(current_vec_idx);
                    iter->second->at(cc) = line_hold[idx]; // index char (crate value) into correct vector
                    //std::clog << line_hold[idx] << ' ';
                }
                ++current_vec_idx;
                check_idx += 4;
            }
        }
        current_vec_idx = 0; // reset vector position for next line
        check_idx = 1; // reset position checker for next line
        --cc; // next row of vector
    }
    
    delete [] line_hold;
    return; 
}

template <typename OF_TYPE>
void print2DVec(std::vector<std::vector<OF_TYPE>>& starting_crates){
    for (const auto vec : starting_crates){
        for (const auto val : vec){
            std::cout << val << ' ';
        }
        std::cout << '\n';
    }
}

void createStacks(std::vector<std::vector<char>>& starting_crates,
        std::vector<std::stack<char>>& stack_vec){
    
    size_t stack_idx {0};
    for (const std::vector<char>& crates_vec : starting_crates){
        for (size_t crate_idx {0}; crate_idx < crates_vec.size(); ++crate_idx){
            // if current val of current vector == letter
           if (crates_vec.at(crate_idx) != '?'){
                 // push to top of stack
                stack_vec.at(stack_idx).push(crates_vec.at(crate_idx));
           }else break; // if '?' ... end of container
        }
        ++stack_idx; // read into next stack, next iteration
    }
}

void readMoves(std::ifstream& file, std::vector<std::vector<int>>& move_vec){

    file.get(); // consume white space
    std::string unused_str{};
     // simplest way I can think of that avoids managing new and stringstream
    while (!!file){
        for (auto& vec : move_vec){
            file >> unused_str;
            file >> vec.at(0);
            file >> unused_str;
            file >> vec.at(1); 
            file >> unused_str;
            file >> vec.at(2);
        }
    }
}

std::string shuffleCrates(std::vector<std::vector<int>>& move_vec,
    std::vector<std::stack<char>>& stacks){
    
    for (std::vector<int>& order : move_vec){
       for (int num_to_move {0}; num_to_move < order.at(0); ++num_to_move){
            // push top value from to-be-moved stack to accepting stack 
            stacks.at(order.at(2)-1).push(stacks.at(order.at(1)-1).top()); 
            // pop value from to-be-moved stack which was just added to accepting stack
            stacks.at(order.at(1)-1).pop();
            //std::clog << num_to_move << ' ';
       }
    }

    std::stringstream ss;
    for (std::stack<char>& get_top : stacks){
         // if stack is not empty -> stream value into stringstream
        if (!get_top.empty()) ss << get_top.top();
    }

    return ss.str();
}

#endif