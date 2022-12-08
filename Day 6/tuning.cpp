#include <iostream>
#include <fstream>
#include <string>
#include "tuning_funcs.hpp"

int main(){
    std::ifstream buffer_file;
    buffer_file.open("buffer.txt");

    if (!!buffer_file){
        std::string str{}; 
        readFile(buffer_file, str); 
        buffer_file.close();

         // part 1 solution
        std::cout << "Start-of-packet marker position: " << findUniqueCharPattern<4>(&str) << '\n'; 
         // part 2 solution
        std::cout << "Start-of-message marker position: " << findUniqueCharPattern<14>(&str) << '\n';
    }
    return 0;
}