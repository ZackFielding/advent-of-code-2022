#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <map>
#include <utility>
#include "crate_sort_funcs.hpp"

int main(){

    std::ifstream cratesf;
    cratesf.open("crates.txt");

    if (!!cratesf){

         // read initial crate set up
        std::vector<std::vector<char>> starting_crates (9, std::vector<char>(8, 63));
        readCrates(cratesf, starting_crates);
        //print2DVec<char>(starting_crates); 
         // read remainder of file as moves 
        std::vector<std::vector<int>> move_vec (503, std::vector<int> (3,0));
        readMoves(cratesf, move_vec); 
        //print2DVec<int>(move_vec);
        cratesf.close();

         // generate stacks for crates to allow LIFO
        std::vector<std::stack<char>> stack_vec (9);
        createStacks(starting_crates, stack_vec);

         // part 1 solution
        std::clog << "Crates at the top of the stack after the shuffle: " 
            << shuffleCrates(move_vec, stack_vec) << '\n';
    }

    return 0;
}