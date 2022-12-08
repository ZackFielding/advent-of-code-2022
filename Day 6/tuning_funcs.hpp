#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

void readFile(std::ifstream& buffer_file, std::string& str){

     // read entire file into string stream
     // NOTE: need to read into char buffer first or else reads as large int
    std::stringstream ss;
    char hold {};
    while (!!buffer_file) if (buffer_file.get(hold)) ss << hold;
    ss >> str;
}

// tempated to allow use for both solutions
// moving window (size based on template) that uses map to find unique sequences 
template <unsigned PATTERN_SIZE>
unsigned findUniqueCharPattern(const std::string* str_v){

    std::map<char, bool> c_map;
    for (unsigned start_idx {0}; start_idx < str_v->size()-PATTERN_SIZE; ++start_idx){
       for (unsigned current_idx {0}; current_idx < PATTERN_SIZE; ++current_idx){
         // will emplace new pair in map IF provided key is not already within map
            c_map.try_emplace(str_v->at(start_idx+current_idx), true);
        // once size reaches desired pattern size -> return current position in string
            if (c_map.size() == PATTERN_SIZE) return (start_idx + 1 + current_idx);
       } 
        c_map.clear(); // if pattern not found in window -> clear map to reuse next iteration
    }
    return 0; // error
}