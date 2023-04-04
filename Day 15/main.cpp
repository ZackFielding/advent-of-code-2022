#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <span>
#include <cstring>
#include <cstdio>
#include <chrono>
#include "funcs.hpp"

/* 
TODO
[x] p1 solution - case duration = 0ms
[x] p2 solution - case duration = ~4000ms
*/

int main(int argc, char** argv){

    if (argc <= 1){
        std::cerr << "Failed to provide file.\n";
        return -1;
    }

    std::ifstream file {argv[1]};

    if (file){

        const auto start_time = steady_clock::now();
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

        // determine which y is LOI based on data set
        int32_t yLOI {0}, p2_max_bound {0};
        {
            char testc [] {"test_data"};
            if (std::strncmp(argv[1], testc, 9) == 0){
                yLOI = 10;
                p2_max_bound = 20;
            } else{
                yLOI = 2000000;
                p2_max_bound = 4000000;
            }
        }

        // get horizontal vectors that lay on y line of interest
        i32vp vec_p = y_LOI_vectors(coord, yLOI);

        // sort vectors in ascending order based on first x pos
        std::sort(vec_p.begin(), vec_p.end(), [] (auto j, auto k) {return j.first < k.first; });

        // create a spand of sorted pairs
        i32p_span sp { vec_p.begin(), vec_p.end() };

        // count vector overlap using span
        int32_t  visible_on_yLOI = count_vector_overlap(sp);

        std::clog << "Number of visible points on line: " << visible_on_yLOI << '\n';

        // count number of beacons on yLOI
        const int32_t beacon_count = count_unique_pairs(coord, yLOI);

        std::clog << "Number of beacons on line: " << beacon_count << '\n';
        visible_on_yLOI -= beacon_count;
        auto p1_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - start_time);

        // p1 solution
        std::printf("[p1] %d spaces where a beacon can't be on line %d.\n", visible_on_yLOI, yLOI);
        std::clog << "p1 duration (ms): " << p1_duration_ms << '\n';

        /*
            P2 - need to limit x && y ranges from 0 - 400,000
            - solution: find which line has a >1 break in the vectors
            - this requires computing the horizontal `vision` vectors
                for each row from 0 - 400,000
        */

       const auto p2_start_time = steady_clock::now();
       int64_t x {0}, y {0};
       for (; y <= p2_max_bound; ++y){
            
            // compute vectors along current line of interest
            i32vp vp { y_LOI_vectors(coord, y) };

            // sort pairs in ascending order
            std::sort(vp.begin(), vp.end(), [] (auto j, auto k) {return j.first < k.first; });

            // need to ensure vectors with equal origins are in ascending order according
            // to their second end position
            adjacent_find_and_swap(vp);

            // determine if a >1 break exists between horizontal vectors
            i32p_span sp { vp.begin(), vp.end() };

            x = constrained_count_vector_overlap(sp, p2_max_bound);

            // constrained algo will return -1 if no >1 gap found (i.e., continue loop)
            if (x != -1) break;
       }

        const uint64_t tuning_freq = x * 4000000 + y;
        const auto p2_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(steady_clock::now() - p2_start_time);
        std::printf("[p2] x: %ld, y: %ld, x*4,000,000+y: %lu.\n", x, y, tuning_freq);
        std::clog << "p2 duration (ms): " << p2_duration_ms << '\n';
    }

    return 0;
}