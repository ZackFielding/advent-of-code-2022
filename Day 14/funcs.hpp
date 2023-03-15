#ifndef FUNCS_HPP
#define FUNCS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <array>

std::string _getline(std::ifstream& f); 

using ROCK_FORM = std::vector<std::vector<std::pair<int,int>>>;

std::tuple<int, int, int, ROCK_FORM> getRockFormations(std::ifstream& file);

void printRockForm(const ROCK_FORM* rf);

enum class FILLED : int {NOT_FILLED = 0, ROCK = 1, SAND = 2, INF_BOTTOM = 3};

struct node {
	std::pair<int, int> xy;
	FILLED filled_with;
	// [0] below, [1] below-left, [2] below-right
	std::array<node*, 3> adj_node_array;
	// ctor
	node() : xy {-1,-1}, filled_with {FILLED::NOT_FILLED},
		adj_node_array {nullptr, nullptr, nullptr} {}
};

void initializeGraph(node **graph, const int gsize, const int row_size);

void createRockFormations(ROCK_FORM* rf, node** graph, const int min_x, const int row_size);

void visualizeRockFormations(node** graph, const int gsize, const int row_size);

int runSandSimulation(node *start_node, node** graph, const int gsize, const int row_size);

#endif
