#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

void readFile(std::ifstream& buffer_file, std::string& str){

    std::stringstream ss;
    char hold {};
     // read entire file into string stream
    while (!!buffer_file){
        if (buffer_file.get(hold)) ss << hold;
    }
    ss >> str;
}

unsigned findUnique4Char(const std::string* str_v){

    std::map<char, bool> c_map;
    for (unsigned start_idx {0}; start_idx < str_v->size()-4; ++start_idx){
       for (unsigned current_idx {0}; current_idx < 4; ++current_idx){
            c_map.try_emplace(str_v->at(start_idx+current_idx), true);
            if (c_map.size() == 4) return (start_idx + 1 + current_idx);
       } 
        c_map.clear();
    }
    return 0; // error
}