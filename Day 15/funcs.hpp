#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <array>
#include <cstdint>

using COORD = std::vector < std::array < int32_t, 5 > >;
using i32_z = std::pair < int32_t, std::size_t >;

std::string get_string(std::ifstream&);
i32_z get_int(std::string_view, const char, const std::size_t, const char);
COORD get_file_coord(std::ifstream&);
void print_coord(const COORD&);

#endif