#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <tuple>
#include "funcs.hpp"

/* 
TODO
[x] p1 solution
*/

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

        [[maybe_unused]] int32_t max_bs_dist = std::max_element(coord.begin(), coord.end(), _max) -> at(4);

        /*  COMPUTE GRID DIMENSIONS
        Need to determine max & min x and y values for creating the grid
        Need to compute this for every beacon in each direction (x4)
        */

        i32_4tup d = compute_grid_dimensions(coord);

        std::printf("x min: %d, x max: %d.\n", std::get<0>(d), std::get<1>(d));
        std::printf("y min: %d, y max: %d.\n", std::get<2>(d), std::get<3>(d));

        // get horizontal vectors that lay on y line of interest
        int32_t yLOI = 2000000;
        i32vp vec_p = y_LOI_vectors(coord, yLOI);

        // sort vectors in ascending order based on first x pos
        std::sort(vec_p.begin(), vec_p.end(), [] (auto j, auto k) {return j.first < k.first; });

        // compute number of gaps on y LOI (does not consider beacons)
        int32_t  visible_on_yLOI = count_vector_overlap(vec_p);
        std::clog << "Number of visible points on line: " << visible_on_yLOI << '\n';

        // count number of beacons on yLOI
        const int32_t beacon_count = count_unique_pairs(coord, yLOI);

        std::clog << "Number of beacons on line: " << beacon_count << '\n';
        visible_on_yLOI -= beacon_count;

        // p1 solution
        std::printf("%d spaces where a beacon can't be on line %d.\n", visible_on_yLOI, yLOI);
    }

    return 0;
}