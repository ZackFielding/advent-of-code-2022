#ifndef ROPE_BRIDGE_FUNCS_HPP
#define ROPE_BRIDGE_FUNCS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <cmath>
#include <cstring>

 // double to allow cmath funcs
using COORD = std::pair<char, double>;

void readFile(std::ifstream& file, std::vector<COORD>& path){
    char chold;
    double dhold;

    while (file >> chold >> dhold){
        if (chold == 'L' || chold == 'D') dhold *= -1.0;
        path.push_back({chold, dhold}); // push new pair into vec 
    }
}

size_t trackDisplacement(std::vector<COORD>& path){

    std::map<std::pair<double,double>, bool> visited_map {{{0.0,0.0}, true}}; // vistied first coord (0,0)
    double head[] {0,0}, tail[] {0,0}, *h_ptr {nullptr};
    double prev_head[] {0,0};
    double resultant {0};
    double x_dif {0}, y_dif {0};

    for (const auto& move : path){
        h_ptr = (move.first == 'U' || move.first == 'D') ? &head[1] : &head[0];
        //std::clog << "\n==== " << move.first << ' ' << move.second << " ====\n";
        for (int disp {0}; disp < std::abs(move.second); ++disp){
            std::memcpy(prev_head, head, sizeof head); // cache prev head before incrementing
        
        // up & right are positive polarities (increment) - down & left are negative
            if (move.first == 'U' || move.first == 'R') ++(*h_ptr);
            else --(*h_ptr);

        // find new vec between head&tail
            resultant = 
                std::sqrt(
                    std::pow((head[0]-tail[0]), 2) 
                    + std::pow((head[1]-tail[1]), 2)); 
        // if vec between head & tail > 1 .. need to change tail position
            if (resultant > 1.5){
               x_dif = std::abs(head[0] - tail[0]); // x&y vec between points 
               y_dif = std::abs(head[1] - tail[1]);
            
            // if diagonal -> set tail to cached head && try insert
               if (x_dif >= 1 && y_dif >= 1){
                std::memcpy(tail, prev_head, sizeof prev_head);
            
            // x OR y vec == 0 -> whatever != 0, apply same move as applied to head
               } else {
                    (x_dif > 0) ? 
                            tail[0] += (move.second / std::abs(move.second)) 
                            : tail[1] += (move.second / std::abs(move.second));
               }
            
             // after changing tail, try to insert new tail xy coord into map
            visited_map.insert({{tail[0],tail[1]},true}); 
            }
            //std::clog << "Tail coord: {" << tail[0] << ',' << tail[1] << "}" << '\n';
        }
    }
    return visited_map.size();
}

#endif
