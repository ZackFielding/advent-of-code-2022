#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include "treehouse_funcs.hpp"

int main(){
    std::ifstream file;
    file.open("grid.txt");

    if (!!file){
         // grid length to allow 1D vec of 2D plane
        const int grid_length {getGridLength(file)};
        std::vector<int> grid_vec;

    // read 2D grid into 1D vector
        size_t grid_depth {0};
        readGrid(file, grid_vec, grid_depth);
        file.close();

    // part 1 (.first) and 2 (.second) solutions
        std::pair<int,int> results {findVisibleTrees(grid_vec, grid_length)};
        std::cout << "Number of visible trees: " << results.first
            << "\nGreatest scenic score of any tree: " << results.second << '\n';
    }
    return 0;
}