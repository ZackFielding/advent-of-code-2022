#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <sstream>


void readFile(std::ifstream& data_file, std::vector<std::vector<int>>* vec_of_vec){

    std::stringstream ss;
    char chold {};
    int ihold {};
    size_t outter_vec_idx {0}, inner_vec_idx {0};

    while (!!data_file && outter_vec_idx < vec_of_vec->size()){
       if (data_file.peek() != '-' && data_file.peek() != ',' && data_file.peek() != '\n' && data_file.peek() != '\r'){
            data_file.get(chold);
            ss << chold;
       }else{
        if (ss.str() != ""){
            ss >> std::dec >> ihold;
            ss.clear();
            vec_of_vec->at(outter_vec_idx).push_back(ihold);
            ++inner_vec_idx;
            if (inner_vec_idx > 3){
                ++outter_vec_idx; // increment outter
                inner_vec_idx = 0; // reset inner
            }
        }
         // consume non-integer values
        data_file.get(); 
       } 
    }
}

void readVecVec(std::vector<std::vector<int>>& vec_of_vec){
    for (const std::vector<int>& vec : vec_of_vec){
        for (const int& val : vec) std::cout << val << ' ';
        std::cout << '\n';
    }
}

unsigned countCompleteOverlaps(std::vector<std::vector<int>>& vec_of_vec){

    unsigned overlap_counter {0};
    bool cond1_b {false};

    for (const std::vector<int>& vec : vec_of_vec){
        cond1_b = false; // assum first condition will fail
        if (vec.at(2) >= vec.at(0)){
            if (vec.at(3) <= vec.at(1)){
                 // if both conditions true -> ++counter and don't check the reverse 
                ++overlap_counter;
                cond1_b = true;
            } 
        }
        if (!cond1_b){ // if condition 1 was not satisified, check the reverse
            if (vec.at(0) >= vec.at(2)){
                if (vec.at(1) <= vec.at(3)) ++overlap_counter;
            }
        }
    }
    //std::cout << overlap_counter << '\n';
    return overlap_counter;
}

unsigned countAnyOverlaps(std::vector<std::vector<int>>& vec_of_vec){

    unsigned overlap_counter {0};
    bool overlap_found {false};

    for (const std::vector<int>& vec : vec_of_vec){
        overlap_found = false; // assume false first comparison
        if (vec.at(2) >= vec.at(0)){
            if (vec.at(2) <= vec.at(1)){
                ++overlap_counter;
                overlap_found = true;
            }
        }
        if (!overlap_found){
            if (vec.at(0) >= vec.at(2)){
                if (vec.at(0) <= vec.at(3)) ++overlap_counter;
            }
        }
    }
    return overlap_counter;
}

int main(){
    std::ifstream data_file;
    data_file.open("task_assignments.txt");

    if (!!data_file){
        std::vector<std::vector<int>> vec_of_vec (1000);

         // working file read to vec of vec 
        readFile(data_file, &vec_of_vec);
        data_file.close();

         // test
        //readVecVec(vec_of_vec);

        // part 1 solution
        std::cout << "Elves with complete overlapping assignments: " << countCompleteOverlaps(vec_of_vec) << '\n';

        std::cout << "Elves with any overlapping assignments: " << countAnyOverlaps(vec_of_vec) << '\n';
    }

    return 0;
}