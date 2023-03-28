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