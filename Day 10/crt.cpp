#include <iostream>
#include <fstream>
#include <vector>
#include "crt_funcs.hpp"

int main(){
    std::ifstream file {"signal.txt"};
    if (!!file){
        std::vector<int*> signal_vec;
        readSignalFile(file, signal_vec);
        file.close();

         // part 1 solution
        std::cout << "Signal streangth is " << signalStrengths(signal_vec) << " units.\n";

         // part 2 solution
        readScreen(signal_vec);

         // free mem
        for (int* free_me : signal_vec) delete [] free_me;            

    }

    return 0;
}