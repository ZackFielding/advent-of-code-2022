#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "no_space_funcs.hpp"

// Recrusive file tree solution using custom list

int main(){
    std::ifstream terminal_output_file;
    terminal_output_file.open("terminal_output.txt");

    if (!!terminal_output_file){
        std::vector<DIRECTORY*> heap_mem; // free heap alloc when done
        DIRECTORY outtermost {}; 
        heap_mem.push_back(&outtermost);
         // create directory tree
        createDirectory(terminal_output_file, outtermost, heap_mem);
        terminal_output_file.close();

         // part 1 solution: recruse through directory, returning current dir size and summing those which meet the threshold
        unsigned long total_sum {0}, threshold_sum {0};
        std::clog << "Total file size of device: " << (total_sum = getSize(&outtermost, threshold_sum)) << '\n';
        std::clog << "Sum of directories with sizes <= 100000: " << threshold_sum << '\n';

         // dir to del must be >= to this value - select SMALLEST of these values (as close to this value)
        unsigned long min_to_remove {30000000 - (70000000 - total_sum)};

         // part 2 solution: loop through vec containing all directories
        std::clog << "Size of best directory to remove: " << 
                ' ' << sumOfDir(heap_mem, min_to_remove) << '\n';

        // free heap alloc
        for (auto& free_me : heap_mem) delete free_me;
    } else std::cerr << "File failed to open.\n";

return 0;
}