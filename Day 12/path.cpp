#include <iostream>
#include <fstream>
#include <vector>
#include "path_funcs.hpp"

/* 
- Data is given as a text file with an compile-time unknown row && column number
- Data will be read into a 1D array to simplify interpretation
*/

int main(){
    std::ifstream file {"path_ad.txt"};

    if (file){
         // read character data into 1D vector
        std::vector<char> vec;
        vec.reserve(1000); // reserve kb of mem to prevent constant reallocation
        readFile(file, &vec); // [PASS] 
        int line_length { getLineLength(file) };
        file.close();
        vec.shrink_to_fit(); 

        // create graph
        graph g {};
        g.createGraph(&vec, line_length);
        //g.test();
        //  Part 1 solution
        std::printf("The shortest number of steps to reach the end position is %d.\n", g.shortestNumberOfSteps());

    }
    return 0;
}