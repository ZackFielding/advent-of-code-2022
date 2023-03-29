#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <tuple>
#include "funcs.hpp"

int main(int argc, char** argv){

    if (argc <= 1){
        std::cerr << "Failed to provide file.\n";
        return -1;
    }

    std::ifstream file {argv[1]};

    if (file){

        // read file into vector of arrays
        COORD coord = get_file_coord(file);
        file.close();

        // Manhatten distance - NOT resultant vector
        // MODIFYING == BAD???
        auto _vec_mag = [] (auto& vec){
                vec[4] = static_cast<int32_t>(std::abs(vec[2]-vec[0]) + std::abs(vec[3]-vec[1]));
        };

        // compute distance between beacon and nearest sensor
        std::for_each(coord.begin(), coord.end(), _vec_mag);

        //print_coord(coord);

        std::tuple<int32_t, int32_t, int32_t, int32_t> xy_min_max;

        // get max distance from any beacon to sensor
        auto _max = [] (auto& ar1, auto& ar2){
                return ar1[4] < ar2[4];
        };

        int32_t max_bs_dist = std::max_element(coord.begin(), coord.end(), _max) -> at(4);

        // compute grid dimensions 

    }

    return 0;
}