#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cstdint>
#include "funcs.hpp" // revised -> only used for file parsing
#include "deq_graph.cpp" // all graph/node data/functions

/* TO DO:
 [x]	working deque graph
 [x]	implement sand simulation in deque graph
 [x]	working p1 solution using deque graph
 [x]	working p2 solution using deque graph
 [.]	create a header for graph cpp file
*/

int main(int argc, char** argv){

	if (argc <= 1){
		std::cerr << "Failed to enter text file.\n";
		return -1;
	}

	std::ifstream file {argv[1]};

	if (file){

		unsigned min_x {}, max_x {}, max_y {};
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

		// create graph
		const int gsize = (max_x - min_x + 1) * (max_y+1);
		const int rsize = max_x - min_x + 1;
	
		std::printf("gsize: %d .. rsize: %d .. csize: %d\n", gsize, rsize, max_y+1);
		std::printf("min x: %d .. max x: %d .. max y: %d.\n", min_x, max_x, max_y);

		std::clog << "Graph prior to simulation:\n";
		degraph dg {min_x, max_x, max_y};
		dg.create_rock_formations(&rf);
		dg.print_graph();

		unsigned ret = dg.run_sand_simulation(500u, 0u);
		std::printf("[P1 solution] %u sand dropped before infinite loop.\n", ret);

		degraph dg2 {min_x, max_x, (max_y+2)};
		dg2.create_rock_formations(&rf, true); // add infinite bottom
		dg2.print_graph();
		unsigned ret2 = dg2.run_sand_simulation(500u, 0u, true);
		std::printf("[P2 solution] %u sand dropped before filling entry point.\n", ret2);
	}

	return 0;
}
