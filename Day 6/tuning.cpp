#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "tuning_funcs.hpp"

int main(){

    std::ifstream buffer_file;
    buffer_file.open("buffer.txt");

    if (!!buffer_file){
        std::string str{}; 
        readFile(buffer_file, str); 
        buffer_file.close();

         // moving window to find first unique 4 char sequence position
         // part 1 solution
        std::cout << "Position: " << findUnique4Char(&str) << '\n'; 
    }
    return 0;
}