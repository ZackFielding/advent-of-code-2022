#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include "rope_bridge_funcs.hpp"

int main(){
    std::ifstream file;
    file.open("path.txt");

    if (!!file){
        std::vector<COORD> path;
        readFile(file, path);

        // part 1 solution
        std::cout << "Number of new blocks visited by rope tail: " 
            << trackDisplacement(path) << '\n';
    }

    return 0;
}