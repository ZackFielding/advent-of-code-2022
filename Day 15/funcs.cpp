#ifndef FUNCS_CPP
#define FUNCS_CPP

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string_view>
#include <cstdint>
#include <algorithm>
#include <set>
#include "funcs.hpp"

COORD get_file_coord(std::ifstream& file){

    // vector to hold xy coordinates
    COORD coord {};

    // allows simple iteration to extract integer values
    const std::array<const char, 4> xyxy {'x','y','x','y'};
    const std::array<const char, 4> delim {',',':',',',':'};

    // iterate across each line of file, processing 1 line ea. iteration
    while (file){

        // read line line from file into string => CONTAINS NO NEWLINES
        std::string s = get_string(file);

        // generate string_view for efficient substr ops
        std::string_view sv {&s[0], s.size()};

        // hold results (will be pushed into vector after all are extracted & casted)
        std::array<int32_t, 5> t_arr;

        // .first = casted i32, .second = offset for reducing substr search
        i32_z ret {static_cast<int32_t>(0), static_cast<std::size_t>(0)};

        // extract and cast each x&y val
        for (std::size_t idx {0}; idx < 4; ++idx){
            ret = get_int(sv, xyxy[idx], ret.second, delim[idx]); 
            t_arr[idx] = ret.first; 
        }

        // push array into vector [make final value inf for error checking]
        t_arr[4] = INT32_MAX;
        coord.push_back(t_arr);
    }

    return coord;
}

// extracts line as std string
std::string get_string(std::ifstream& file){
    std::string str {}; char c {};
    while (file.get(c) && c != '\n') str += c;
    return str;
}

// extracts and casts integer values from a string view
i32_z get_int(std::string_view sv, const char coi, const std::size_t offset, const char delim){

    // find index in string in which the number stops
    const auto spos = sv.find_first_of(coi, offset)+2;
    const auto epos = sv.find_first_of(delim, offset);

    // final y val will not have adjacent non-delim char
    // check for npos
    std::string_view svsub;
    if (epos != sv.npos) svsub = sv.substr( spos, (epos-spos) );
    else svsub = sv.substr(spos);

    std::string str;
    std::for_each(svsub.begin(), svsub.end(), [&str](const char c) { str += c; } );

    return { static_cast<int32_t> (std::stoi(str)), (epos+1) };
}

// print coordinate data struct
void print_coord(const COORD& coord){
    for (const auto& arr : coord){
        for (const auto& v : arr){
            std::cout << v << ' ';
        }
        std::cout << '\n';
    }

}

i32_4tup compute_grid_dimensions(const COORD& coord){
    
    // min x, max x, min y, max y
    int32_t min_x {INT32_MAX}, max_x {INT32_MIN}, min_y {INT32_MAX}, max_y {INT32_MIN};
    int32_t mag_cache {}, cache {};

    for (const auto& arr : coord){
        // cache vector magnitude
        mag_cache = arr[4];
        // x
        if ( (cache = arr[0] - mag_cache) < min_x) min_x = cache;
        if ( (cache = arr[0] + mag_cache) > max_x) max_x = cache;
        // y
        if ( (cache = arr[1] - mag_cache) < min_y) min_y = cache;
        if ( (cache = arr[1] + mag_cache) > max_y) max_y = cache;
    }

    return {min_x, max_x, min_y, max_y};
}

i32vp y_LOI_vectors(const COORD& coord, const int32_t y_LOI){

    std::vector<std::pair<int32_t, int32_t>> vec_i32p;

    for (const auto& arr : coord){

        // check to see if vector would ever intercept y LOI
        // skip calculations if not
        if (arr[1] <= y_LOI){
            if ( arr[1] + arr[4] < y_LOI) continue;
        } else {
            if ( arr[1] - arr[4] > y_LOI) continue;
        }

        // now that it's gartuneed to intercept the y LOI
        // determine the max horizontal line created
        // e.g., 8,7,9, y=10
        // i. find the absolute integer needed to get to y LOI (y beacon - LOI)
        int32_t gap = (int32_t) std::abs( (arr[1] - y_LOI) );

        // ii. compute the length of the x_vector that remains after accounting
        // for y vector needed to get to LOI
        int32_t x_mag = arr[4] - gap;


        // iii. x beacon pos +/- x_mag gives == the covered points on LOI
        // and push into tracking vector
        vec_i32p.push_back( { (arr[0] - x_mag), (arr[0] + x_mag) } );
        std::cout << "pair: " << vec_i32p.back().first << ", " << vec_i32p.back().second << '\n';
    }
    return vec_i32p;
}

int32_t count_vector_overlap(const i32vp& vp){
    
    int32_t number_overlap {0};
    int32_t min {vp[0].first}, max {vp[0].second};
    auto inc_overlap = [&]() { number_overlap += std::abs(max-min) + 1; };

    // first pair is used as baseline -> iterate starting at ++begin()
    for (auto iter {vp.begin()+1}; iter != vp.end(); ++iter){

        // if gap found -> computer number of gaps && set new min/max
        if (iter -> first > max){
            inc_overlap();
            min = iter -> first;
            max = iter -> second;

        } else if (iter -> second > max){
            // if no gap found (i.e., p.first resides within current range)
            // update max
            max = iter -> second;
        }
    }

    // need to account for final vector
    inc_overlap();

    return number_overlap;
}

int32_t count_unique_pairs(const COORD& coord, const int32_t yLOI){
    std::set<std::pair<int32_t, int32_t>> i32set;
    int32_t count {0};

    for (const auto& arr : coord){
        if (arr[3] == yLOI){
            // check to see if duplicate
            auto ret = i32set.emplace( std::make_pair(arr[2], arr[3]) ); 
            if ( ret.second ) ++count;
        }
    }
    return count;
}

#endif