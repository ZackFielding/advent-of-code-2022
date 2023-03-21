#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <tuple>
#include <cstdint>
#include <functional>
#include "funcs.hpp"

std::string _getline(std::ifstream& f){
	// custom get line for std::string
	std::string str {};
	char c {};
	while (f.get(c) && c != '\n'){
		str += c;
	}
	return str;
}

using ROCK_FORM = std::vector<std::vector<std::pair<int,int>>>;
std::tuple<int, int, int, ROCK_FORM> getRockFormations(std::ifstream& file){

	ROCK_FORM rf {};
	std::string line {};
	std::stringstream ss {};
	int x_int {0}, y_int {0};
	int min_x {INT32_MAX}, max_x {0}, max_y {0};

	while ( (line = _getline(file)) != ""){

		rf.push_back({});
		
		// create string view
		std::string_view sv (&line[0], line.size());
		std::string_view x {}, y {};
		std::size_t spos {0}, epos {0};
		
		while (spos != sv.npos){

			// get end pos for x val
			epos = sv.find_first_of(',', spos);

			// get substring for x
			// count == number of elements, NOT END POS of substr
			x = sv.substr(spos, epos-spos);
			ss << x;
			ss >> std::dec >> x_int;
			ss.clear();

			// checking to see if max or min
			if (x_int > max_x) max_x = x_int;
			if (x_int < min_x) min_x = x_int;

			// new start pos +1 after comma
			spos = epos+1;	
			epos = sv.find_first_of(' ', spos);

			// if last value => will result in npos
			if (epos == sv.npos){
				epos = sv.size();
			}

			y = sv.substr(spos, epos-spos);
			ss << y;
			if (!(ss >> std::dec >> y_int)){
				std::cout << spos << " ... " << epos-spos << '\n';
			}
			ss.clear();

			// check to see if max
			if (y_int > max_y) max_y = y_int;
			
			// insert pair into current vec of pairs
			rf.back().push_back({x_int, y_int});

			// get next spos before next loop
			// will return npos IF the final pair of line
			spos = sv.find_first_of(' ', epos+1);
		}
	}

	return std::tuple <int, int, int, ROCK_FORM> {min_x, max_x, max_y, rf};
}

void printRockForm(const ROCK_FORM* rf){
	for (const auto& v : *rf){
		for (const auto& p : v){
			std::printf("{%d,%d} ", p.first, p.second);
		}
		std::cout << '\n';
	}
}

void initializeGraph(node **graph, const int gsize, const int row_size){

	int track_new {0};
	node *cnode {nullptr}, *temp {nullptr};
	for (int n {0}; n < gsize; ++n){

		// if first row => will need to init current node	
		if ( (n - row_size) < 0 ){
			cnode = new node {};
			++track_new;
			graph[n] = cnode;
		} else {
			cnode = graph[n];
		}

		// DIRECT_BELOW NODE
		// IF last row => will be null
		if ( (n + row_size) >= gsize ){
			cnode->adj_node_array[0] = nullptr;
		} else {
			// create new node => index into graph => set to below
			// THIS ONLY NEEDS TO HAPPEN FOR FIRST NODE
			// below_right acts as a leading init for `direct_below`
			if (n % row_size == 0){
				temp = new node {};
				++track_new;
				graph[n + row_size] = temp;
			}

			// set
			cnode -> adj_node_array[0] = graph[n + row_size];
		}

		// BELOW_LEFT NODE
		if ( n % row_size == 0 || (n + row_size >= gsize) ){
			cnode -> adj_node_array[1] = nullptr;
		} else {
			// bottom left node will already exist
			// [created as direct_below for previous node]
			cnode -> adj_node_array[1] = graph[n+row_size-1];
		}

		// BELOW_RIGHT NODE
		if ( ( (n+1) % row_size == 0 ) || (n + row_size >= gsize) ){
			cnode -> adj_node_array[2] = nullptr;
		} else {
			temp = new node {};	
			++track_new;
			cnode -> adj_node_array[2] = temp;
			graph[n+row_size+1] = temp;
		}
		//std::printf("%d. allocs => %d.\n", n, track_new);
	}

}

void createRockFormations(ROCK_FORM* rf, node** graph, const int min_x, const int row_size, 
			  bool create_inf_botom, const int gsize2){

	// for translating {x,y} to [n]
	auto _ni = [min_x, row_size](const int x_pos, const int y_pos)
		{ return (x_pos - min_x) + (y_pos * row_size); };

	// for each vector of pairs
	for (const auto& v : *rf){
		// iterate across pairs
		// (can't use range as need to stop at last-1 pair)
		for (std::size_t p {0}; p < v.size(); ++p){

			const int *_greater_x {nullptr}, *_lesser_x {nullptr};
			const int *_greater_y {nullptr}, *_lesser_y {nullptr};

			// if same x or y values found => have a cont. rock
			if ( ( (p + 1) < v.size() ) && (v.at(p).first == v.at(p+1).first || v.at(p).second == v.at(p+1).second) ){

				// it's not garunteed the leading pair will be less than
				// following pair => to make looping easier, do this

				if (v.at(p).first >= v.at(p+1).first){
					_greater_x = &v.at(p).first;
					_lesser_x = &v.at(p+1).first;
				} else {
					_greater_x = &v.at(p+1).first;
					_lesser_x = &v.at(p).first;
				}

				if (v.at(p).second >= v.at(p+1).second){
					_greater_y = &v.at(p).second;
					_lesser_y = &v.at(p+1).second;
				} else {
					_greater_y = &v.at(p+1).second;
					_lesser_y = &v.at(p).second;
				}

				for (int _x {*_lesser_x}; _x <= *_greater_x; ++_x){
					for (int _y {*_lesser_y}; _y <= *_greater_y; ++_y){
						graph[_ni(_x, _y)] -> filled_with = FILLED::ROCK;
					}
				}
			} else {
				// no matching pair => just draw individual rock	
				graph[_ni(v.at(p).first, v.at(p).second)] -> filled_with = FILLED::ROCK;
			}
		}
	}

	// p2
	if (create_inf_botom){
		for (int b {gsize2 - row_size}; b < gsize2; ++b){
			graph[b] -> filled_with = FILLED::INF_BOTTOM;
		}
	}
}

void visualizeRockFormations(node** graph, const int gsize, const int row_size){
	
	for (int i {0}; i < row_size; ++i){
		if (i % 10 == 0) std::cout << "0 ";
		else std::cout << "  ";
	}
	std::cout << '\n';

	for (int n {0}; n < gsize; ++n){
		if (n == 43){
			std::cout << "+ ";
			continue;
		}
		switch (graph[n] -> filled_with){
			case (FILLED::NOT_FILLED): {
				std::cout << ". ";
				break;
			}
			
			case (FILLED::ROCK):{
				std::cout << "# ";
				break;
			}

			case (FILLED::SAND):{
				std::cout << "o ";
				break;
			}

			case (FILLED::INF_BOTTOM):{
				std::cout << "= ";
				break;
			}
			default:{
				std::cout << "? ";
			}
		}
		
		if ( (n+1) % row_size == 0) std::cout << '\n';
	}
}

void adhocNodeAllocation(node*, const int, std::vector<node*>*);
std::pair<int, std::vector<node*>> runSandSimulation(node *start_node, const bool UPDATE_NULL){
	// all sand starts at row 500 position
	// sand falls until it can't move down, down-left, or down-right
	// once a peice of sand encounters a viable nullptr
	// stop simulation
	
	int sim_runs {0};
	node *cur_node {nullptr};
	bool INFNITE_FOUND {false};
	bool ADJ_EMPTY {false};
	[[maybe_unused]] std::vector<node*> adhoc_nodes {};
	bool sand_filled = false;

	while (!INFNITE_FOUND){
		cur_node = start_node;
		
		while (1){
			
			// will only be called if UPDATE_NULL == true
			if (cur_node -> filled_with == FILLED::SAND){
				INFNITE_FOUND = true;
				break;
			}

			int adj_idx {0};
			for (node* adjacents : cur_node -> adj_node_array){
				if (adjacents == nullptr){

					if (UPDATE_NULL){

						adhocNodeAllocation(cur_node, adj_idx, &adhoc_nodes);
						// sand has already been alloc
						// skip over sand filling
						sand_filled = true;
						break;

					} else {

						INFNITE_FOUND = true;
						break;
					}

				} else {
					if (adjacents -> filled_with == FILLED::NOT_FILLED){
						cur_node = adjacents;
						ADJ_EMPTY = true;
						break;
					}

					// fill current if at direct below is inf bottom 
					if (adjacents -> filled_with == FILLED::INF_BOTTOM){
						break;
					}
				}

				++adj_idx;
			}

			if (INFNITE_FOUND) break;
			if (ADJ_EMPTY){
				ADJ_EMPTY = false;
				continue;
			}

			// if sand can't got anywhere => break and run next simulation
			// ONLY if sand has already been filled by adhoc node alloc func
			if (!sand_filled) cur_node -> filled_with = FILLED::SAND;
			else sand_filled = false; // flip to false for next run
			
			++sim_runs;
			break;
		}

		//visualizeRockFormations(graph, gsize, row_size);
		//std::cout << "\n ====================================================================== \n";
	}
	return std::make_pair (sim_runs, adhoc_nodes);
}


// handle new nodes in graph
// need to keep track of the these new nodes for deletion
void adhocNodeAllocation(node* top_node, const int adj_idx, std::vector<node*>* adhoc_nodes){

	node *above_node {nullptr}, *cur_node {top_node};
	
	// iterate DOWN nodes, checking correct adjacent
	while (1){
		// if adjacent is null => alloc new empty-filled node
		// and add node to adhoc nodes
		if (cur_node -> adj_node_array[adj_idx] == nullptr){

			node *temp = nullptr;
			temp = new node {}; // alloc new node
			cur_node -> adj_node_array[adj_idx] = temp; // assign new node to adj of cur
			adhoc_nodes -> push_back(temp);

			if (cur_node -> adj_node_array[0] -> filled_with == FILLED::INF_BOTTOM){

				temp -> filled_with = FILLED::INF_BOTTOM;

				// if cur node is sitting ontop of inf bottom
				// above node will be null
				// fill cur node with sand and break
				// else fill above node with sand
				if (above_node == nullptr){
					cur_node -> filled_with = FILLED::SAND;
				} else {
					above_node -> filled_with = FILLED::SAND;
				}

				break;

			} else {

				temp -> filled_with = FILLED::NOT_FILLED;
				above_node = temp;
				cur_node = cur_node -> adj_node_array[0];
			}

		// else if adjacent is sand => change prev node filled to SAND
		} else if (cur_node -> adj_node_array[adj_idx] -> filled_with == FILLED::SAND){

			above_node -> filled_with = FILLED::SAND;
			break;

		} else if (cur_node -> adj_node_array[adj_idx] -> filled_with == FILLED::NOT_FILLED){

			above_node = cur_node -> adj_node_array[adj_idx];
			cur_node = cur_node -> adj_node_array[0];

		}
	}
}
