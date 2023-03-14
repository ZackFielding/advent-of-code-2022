#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cstdint>
#include "funcs.hpp"

int main(int argc, char** argv){

	if (argc <= 1){
		std::cerr << "Failed to enter text file.\n";
		return -1;
	}

	std::ifstream file {argv[1]};

	if (file){

		int min_x {}, max_x {}, max_y {};
		ROCK_FORM rf {};

		{
			// PASSED
			const std::tuple<int, int, int, ROCK_FORM> tup = getRockFormations(file);
			min_x = std::get<0>(tup);
			max_x = std::get<1>(tup);
			max_y = std::get<2>(tup);
			rf = std::get<3>(tup);
		}

		std::printf("MIN X : %d, MAX X : %d, MAX Y : %d.\n", min_x, max_x, max_y);

		// test
		printRockForm(&rf);

		// create graph
		const int gsize = (max_x - min_x + 1) * (max_y+1);
		const int rsize = max_x - min_x + 1;
		node** graph = new node*[gsize];
	
		std::printf("gsize: %d .. rsize: %d .. csize: %d\n", gsize, rsize, max_y+1);
		std::printf("min x: %d .. max x: %d .. max y: %d.\n", min_x, max_x, max_y);

		// untested
		initializeGraph(graph, gsize, rsize);

		// untested
		createRockFormations(&rf, graph, min_x, rsize);

		// for testing
		visualizeRockFormations(graph, gsize, rsize);


		// run sand simulation
		node *start_node = graph[500-min_x];
		int p1_ans = runSandSimulation(start_node, graph, gsize, rsize);	
		visualizeRockFormations(graph, gsize, rsize);
		std::printf("%d pieces of sand fall before infinite loop.\n", p1_ans);

		for (int d {0}; d < gsize; ++d) delete graph[d];
		delete [] graph;
	}

	return 0;
}
