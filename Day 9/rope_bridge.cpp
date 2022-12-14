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

        // part 1 & 2 solutions
        std::pair<size_t, size_t> tail_visits {trackDisplacement(path)};
        std::cout << "Number of new blocks visited by rope tail 1: " 
            << tail_visits.first 
            << "\nNumber of new blocks visited by rope tail 9: " 
            << tail_visits.second << '\n';
    }

    return 0;
}