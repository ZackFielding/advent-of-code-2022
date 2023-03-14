#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <tuple>
#include <cstdint>
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
				epos = sv.size() - 1;
			}

			y = sv.substr(spos, epos-spos);
			ss << y;
			ss >> std::dec >> y_int;
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
			cnode->direct_below = nullptr;
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
			cnode -> direct_below = graph[n + row_size];
		}

		// BELOW_LEFT NODE
		if ( n % row_size == 0 || (n + row_size >= gsize) ){
			cnode -> below_left = nullptr;
		} else {
			// bottom left node will already exist
			// [created as direct_below for previous node]
			cnode -> below_left = graph[n+row_size-1];
		}

		// BELOW_RIGHT NODE
		if ( ( (n+1) % row_size == 0 ) || (n + row_size >= gsize) ){
			cnode -> below_right = nullptr;
		} else {
			temp = new node {};	
			++track_new;
			cnode -> below_right = temp;
			graph[n+row_size+1] = temp;
		}
		//std::printf("%d. allocs => %d.\n", n, track_new);
	}

}

void createRockFormations(ROCK_FORM* rf, node** graph, const int min_x, const int row_size){

	// for translating {x,y} to [n]
	auto _ni = [min_x, row_size](const int x_pos, const int y_pos)
		{ return (x_pos - min_x) + (y_pos * row_size); };

	// for each vector of pairs
	for (const auto& v : *rf){
		// iterate across pairs
		// (can't use range as need to stop at last-1 pair)
		for (std::size_t p {0}; p < v.size()-1; ++p){

			const int *_greater_x {nullptr}, *_lesser_x {nullptr};
			const int *_greater_y {nullptr}, *_lesser_y {nullptr};

			// if same x or y values found => have a cont. rock
			if (v.at(p).first == v.at(p+1).first || v.at(p).second == v.at(p+1).second){

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
			}
		}
	}
}

void visualizeRockFormations(node** graph, const int gsize, const int row_size){
	
	for (int n {0}; n < gsize; ++n){
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
			default:{
				std::cout << "? ";
			}
		}
		
		if ( (n+1) % row_size == 0) std::cout << '\n';
	}
}
