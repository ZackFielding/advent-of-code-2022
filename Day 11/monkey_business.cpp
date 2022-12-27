#include <iostream>
#include <fstream>
#include <deque>
#include <array>
#include "monkey_business_funcs.hpp"

int main(){
    std::ifstream file ("data.txt");
    if (!!file){
         // array of monkey structs 
        constexpr int nm {8}, nr {20};
        std::array<monkey, nm> monkey_array;
        getMonkeyAttributes<nm>(file, monkey_array);
        file.close(); 

        std::pair<int, int> active_pair {0,0};
         // part 1 solution
        active_pair = passItem<nm>(monkey_array, nr);
        printf("The higest inspection counts were %d and %d, resulting in a monkey business level of %d",
                active_pair.first, active_pair.second, active_pair.first * active_pair.second);

        /*
        for (auto& m : monkey_array){
            std::cout << "\nMonkey: ";
            for (auto& item : m.items){
                std::cout << item << ' ';
            }
            std::cout << '\n';
        }
        */
    }
    return 0;
}