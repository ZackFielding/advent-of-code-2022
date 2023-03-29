#ifndef FUNCS_CPP
#define FUNCS_CPP

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string_view>
#include <cstdint>
#include <algorithm>
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

#endif