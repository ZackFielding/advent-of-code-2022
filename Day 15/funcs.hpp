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
using i32_4tup = std::tuple < int32_t, int32_t, int32_t, int32_t >;
using i32vp = std::vector<std::pair<int32_t, int32_t>>;

std::string get_string(std::ifstream&);
i32_z get_int(std::string_view, const char, const std::size_t, const char);
COORD get_file_coord(std::ifstream&);
void print_coord(const COORD&);
i32_4tup compute_grid_dimensions(const COORD&);
i32vp y_LOI_vectors(const COORD&, const int32_t);
int32_t count_vector_overlap(const i32vp&);
int32_t count_unique_pairs(const COORD& coord, const int32_t yLOI);

#endif