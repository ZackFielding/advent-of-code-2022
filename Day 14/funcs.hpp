#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

std::string _getline(std::ifstream& f); 

using ROCK_FORM = std::vector<std::vector<std::pair<int,int>>>;

std::tuple<int, int, int, ROCK_FORM> getRockFormations(std::ifstream& file);

enum class FILLED : int {NOT_FILLED = 0, ROCK = 1, SAND = 2, INF_BOTTOM = 3};

#endif
