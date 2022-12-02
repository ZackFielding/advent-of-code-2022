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

void shuffleTop3Cal(const unsigned to_add_cal, unsigned* top_3_cal){
    //    0    1   2
     // [1st, 2nd, 3rd]
    if (to_add_cal < top_3_cal[1]){ // if its <2nd -> new 3rd place value
        top_3_cal[2] = to_add_cal; // if new 3rd pos -> replace old 3rd value and return
        return; 
    }

    if (to_add_cal < top_3_cal[0]){ // if new 2nd place value
        unsigned temp_hold {top_3_cal[1]}; // store old 2nd (current 2nd place)
        top_3_cal[1] = to_add_cal; // read-in new 2nd
        top_3_cal[2] = temp_hold;
        return;
    } else{
        // new 1st place - shuffle all other values down
        unsigned hold_2nd {top_3_cal[0]}, hold_3rd {top_3_cal[1]};
        top_3_cal[0] = to_add_cal; // new 1st
        top_3_cal[1] = hold_2nd; // new 2nd (old 1st)
        top_3_cal[2] = hold_3rd; // new 3rd (old 2nd)
    }
}

void findTop3GreatestCal(std::vector<std::vector<int>>& elf_food){

     // main body is identical to find single greatest func
    unsigned total_cal {0}, current_max_cal {0};
    unsigned top_3_cal[3] {0,0,0};

    for (const auto& elf_vec : elf_food){
        for (const auto& count_cal : elf_vec){
            total_cal += count_cal;
        }
        if (total_cal > top_3_cal[2]) shuffleTop3Cal(total_cal, top_3_cal);  // if > current 3rd place, shuffle top 3 with new value
        total_cal = 0; // set count back to 0 prior to next run
    }

    std::cout << "Greatest calorie count: " << top_3_cal[0] << ".\nSecond greatest calorie count: " << top_3_cal[1]
        << ".\nThird greatest calorie count: " << top_3_cal[2] << '\n'; 

    std::cout << "Sum of top 3 calorie counts: " << (top_3_cal[0] + top_3_cal[1] + top_3_cal[2]) << '\n';
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

    // find product of top 3 calorie counts
    findTop3GreatestCal(elf_food);

    return 0;
}