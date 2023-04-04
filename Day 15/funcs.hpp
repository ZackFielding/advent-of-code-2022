#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <array>
#include <cstdint>
#include <span>
#include <functional>
#include <chrono>

using COORD = std::vector < std::array < int32_t, 5 > >;
using i32_z = std::pair < int32_t, std::size_t >;
using i32_4tup = std::tuple < int32_t, int32_t, int32_t, int32_t >;
using i32vp = std::vector<std::pair<int32_t, int32_t>>;
using i32p = std::pair<int32_t, int32_t>;
using i32p_span = std::span<i32p>;
using steady_clock = std::chrono::steady_clock;

std::string get_string(std::ifstream&);
i32_z get_int(std::string_view, const char, const std::size_t, const char);
COORD get_file_coord(std::ifstream&);
void print_coord(const COORD&);
i32_4tup compute_grid_dimensions(const COORD&);
i32vp y_LOI_vectors(const COORD&, const int32_t);
int32_t count_vector_overlap(i32p_span);
int32_t count_unique_pairs(const COORD& coord, const int32_t yLOI);
int32_t constrained_count_vector_overlap(std::span<i32p>, const int32_t);
std::span<i32p> resize_span(std::span<i32p>, const int32_t);
std::size_t span_find_and_count(std::span<i32p>, std::function<bool (const i32p)>);
void adjacent_find_and_swap(i32vp&);
int32_t _modify_span(i32p_span, std::function<int32_t(i32p)>);

#endif