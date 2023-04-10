#include <iostream>
#include <fstream>
#include <string>
#include "funcs.hpp"


int main(int argc, char** argv){

    if ( argc < 2 ){
        std::cerr << "[ERROR] Failed to enter file.\n";
        return -1;
    }

    std::ifstream file {argv[1]};

    if ( file ){
        graph g {};
        read_file_create_graph(file, &g);
        g.print_nodes();
    }

    return 0;
}