#ifndef ROPE_BRIDGE_FUNCS_HPP
#define ROPE_BRIDGE_FUNCS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <cmath>
#include <cstring>
#include <algorithm>

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

double findResultant(double* head, double* tail, std::pair<double,double> offset = {0,0}){
    return std::sqrt(
                    std::pow((head[0]-(tail[0]+offset.first)), 2) 
                    + std::pow((head[1]-(tail[1]+offset.second)), 2)); 
}

std::pair<size_t, size_t> trackDisplacement(std::vector<COORD>& path){
    std::map<std::pair<double,double>, bool> knot1_visited_map {{{0.0,0.0}, true}}; // vistied first coord (0,0)
    std::map<std::pair<double,double>, bool> knot9_visited_map {{{0.0,0.0}, true}}; // vistied first coord (0,0)
    std::vector<std::pair<double, double>> xy_displacements {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    double *h_ptr {nullptr}, resultant {0}, x_dif {0}, y_dif {0};

     // idx == 0 represents head
    std::vector<double[2]> knots_vec (10); 
     // set all double* to 0
    std::for_each(knots_vec.begin(), knots_vec.end(), [](double* xy) 
            { xy[0] = 0.0;
              xy[1] = 0.0; });

    for (const auto& move : path){
        h_ptr = (move.first == 'U' || move.first == 'D') ? &(knots_vec.at(0)[1]) : &(knots_vec.at(0)[0]);
        //std::clog << "\n==== " << move.first << ' ' << move.second << " ====\n";
        for (int disp {0}; disp < std::abs(move.second); ++disp){
            for (size_t knot {0}; knot < knots_vec.size()-1; ++knot){
            // direct changes to head ptr only occurs to HEAD .. so only apply this once for each iteration of this loop
                if (knot == 0){
                // up & right are positive polarities (increment) - down & left are negative
                    if (move.first == 'U' || move.first == 'R') ++(*h_ptr);
                    else --(*h_ptr);
                }

                // find new vec between head&tail
                resultant = findResultant(knots_vec.at(knot), knots_vec.at(knot+1));

                // if vec between head & tail > 1 .. need to change tail position
                if (resultant > 1.5){
                    x_dif = knots_vec.at(knot)[0] - knots_vec.at(knot+1)[0]; // x&y vec between points 
                    y_dif = knots_vec.at(knot)[1] - knots_vec.at(knot+1)[1];
                
                // if diagonal -> search for move to get it within 1 diagonal adjacent block from head
                if (std::abs(x_dif) >= 1 && std::abs(y_dif) >= 1){
                    //if (knot == 5) std::clog << "found match new diagonal.\n";
                    for (const auto& xyd : xy_displacements){
                        if (findResultant(knots_vec.at(knot), knots_vec.at(knot+1), xyd) <= 1.5){
                            knots_vec.at(knot+1)[0] += xyd.first;
                            knots_vec.at(knot+1)[1] += xyd.second;
                            break;
                        }
                    }
                    // x OR y vec == 0 -> whatever != 0, apply same move as applied to head
                } else {
                    if (std::abs(x_dif) > 0){
                        knots_vec.at(knot+1)[0] += (x_dif > 0) ? 1 : -1;
                    } else{
                        knots_vec.at(knot+1)[1] += (y_dif > 0) ? 1 : -1;
                    }
                }
                
                // after changing tail, try to insert new tail xy coord into map
                // currently only registers 1 move for tail 9 which is incorrect
                if (knot == 0) knot1_visited_map.insert({{knots_vec.at(knot+1)[0],knots_vec.at(knot+1)[1]},true}); 
                if (knot == 8){
                    knot9_visited_map.insert({{knots_vec.at(knot+1)[0],knots_vec.at(knot+1)[1]}, true});
                    /*
                    std::clog << "Head coord: {" << knots_vec.at(knot)[0] << ',' << knots_vec.at(knot)[1] 
                        << "}\n";
                    std::clog << "Tail coord: {" << knots_vec.at(knot+1)[0] << ',' << knots_vec.at(knot+1)[1] 
                        << "}\n";
                    */
                }
                }
            }
        }
    }
    return std::pair<size_t, size_t> {knot1_visited_map.size(), knot9_visited_map.size()};
}

#endif
