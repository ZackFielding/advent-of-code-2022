#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <sstream>
#include <tuple>
#include <cstdint>

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
	int max_x {0}, min_x {INT32_MAX}, max_y {INT32_MAX};

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


int main(int argc, char** argv){

	if (argc <= 1){
		std::cerr << "Failed to enter text file.\n";
		return -1;
	}

	std::ifstream file {argv[1]};

	if (file){

		// PASSED
		// change return type and isolate variables
		const ROCK_FORM rf = getRockFormations(file);

		// test
		printRockForm(&rf);

		// create graph


		// all nodes of graph will be held in carray
		// as exact size will be known after file read
		// despite requiring more memory, the carray reduces the
		// headache associated with establishing adjacent nodes that have
		// not been initialized yet


		// all nodes will also be held in map to allow easy access
		// while establishing the rock formation

		// single nested for loop of with x as outter and y as inner loop
		// to establish each rock formation (calling on map to get the correct nodes)
	}

	return 0;
}
