#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// need to think of way to keep track of concurrent newlines to track when to index values for new elf

void read_calories (std::ifstream& cal_file, std::vector<std::vector<int>>* elf_food, 
                        const size_t elf_num){
    std::stringstream ss; // used to rebuild characters into integer value
    static char hold_char {'E'}; // 'E' for error .. acts as middle-man between file and string streams
    bool empty_line {false}; // if empty line occurs -> bool will never be flipped to false
    int reconstructed_to_int {-1}; // -1 for error checking

    while (!!cal_file && !empty_line){
        empty_line = true; // if it doesn't enter next loop -> will be be true and break out

        while (cal_file.peek() != '\n' && cal_file.peek() != '\r' && !!cal_file){
            cal_file.get(hold_char); // extract single char from file
            ss << hold_char; // stream into stringstream for later int-rebuild
            empty_line = false;
        }      
        cal_file.get();

        if (!empty_line){ // only handle string stream IF an characters were extracted
            if (! (ss >> std::dec >> reconstructed_to_int)) std::cout << "Error in string stream.\n"; // stream as int to temp int holder
            //std::cout << reconstructed_to_int << '\n';
            elf_food->at(elf_num).push_back(reconstructed_to_int); // push back into current elf outter vector position
            ss.clear(); // reset flags
            ss.str(""); // set string to empty
        }
    }
}

void findGreatestCal(std::vector<std::vector<int>>& elf_food){
     // keep track of top 3 cal counts & current elf total cal
    unsigned total_cal {0}, current_max_cal {0};

    for (const auto& elf_vec : elf_food){
        for (const auto& count_cal : elf_vec){
            total_cal += count_cal;
        }
        if (total_cal > current_max_cal) current_max_cal = total_cal;
        total_cal = 0; // set count back to 0 prior to next run
    }
    std::cout << "Max calories account between elfs: " << current_max_cal << '\n';
}

int main(){
    std::ifstream cal_file;
    cal_file.open("number_of_calories.txt");
    std::vector<std::vector<int>> elf_food (2000); //assume 1000 elfs max
    size_t current_elf {0}; 

    if (!!cal_file){
        while (!!cal_file){
            read_calories(cal_file, &elf_food, current_elf);            
            ++current_elf;
        }

        cal_file.close();
        std::cout << (current_elf + 1) << " number of elfs had their food counter.\n";
    } else return -1; // can't open file -> terminate program

    elf_food.resize(current_elf-1); // re-size vec to match true number of elfs
    //const std::vector<std::vector<int>>* elf_food_ptr {&elf_food}; // access by ptr prevents change to values

    // find elf with greatest total calorie count
    findGreatestCal(elf_food);

    return 0;
}