#include <iostream>
#include <fstream>
#include <vector>
#include "path_funcs.hpp"

int main(){
    std::ifstream file("path_td.txt");
    
    if (!!file){
        // use gcount() to determine number of elements in each row
        {
            char t [500];
            file.getline(t, 500);
        }
         // flat multi dimensional array
        fmdarray<int> vertices;

        // gcount() includes delim char in gcount() so adjust
        vertices.dim_size = (int)file.gcount() - 1;
        file.seekg(0L, std::ios_base::beg);

        readFile<int> (file, &vertices);
        file.close();

        graph g;
        // create graph && get start-end-pair
        std::pair <int, int> sep {createGraph<int> (&g, &vertices)};

        // test
        printf("%d\n", g.find(0)->second.size());
        //printf("Shortest distance to end is %d\n",findShortestPath(&g, &sep));

    } else {
        std::cerr << "Error in file open.\n";
    }
    return 0;
}