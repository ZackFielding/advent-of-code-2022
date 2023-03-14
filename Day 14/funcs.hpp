#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

std::string _getline(std::ifstream& f); 

using ROCK_FORM = std::vector<std::vector<std::pair<int,int>>>;

std::tuple<int, int, int, ROCK_FORM> getRockFormations(std::ifstream& file);

void printRockForm(const ROCK_FORM* rf);

enum class FILLED : int {NOT_FILLED = 0, ROCK = 1, SAND = 2};

struct node {
	std::pair<int, int> xy;
	FILLED filled_with;
	node* direct_below;
	node* below_left;
	node* below_right;
	// ctor
	node() : xy {-1,-1}, filled_with {FILLED::NOT_FILLED},
		direct_below {nullptr}, below_left {nullptr}, below_right {nullptr} {}
};

void initializeGraph(node **graph, const int gsize, const int row_size);

void createRockFormations(ROCK_FORM* rf, node** graph, const int min_x, const int row_size);

void visualizeRockFormations(node** graph, const int gsize, const int row_size);

#endif
