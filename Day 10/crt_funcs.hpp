#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <set>

void readSignalFile(std::ifstream& file, std::vector<int*>& signal_vec){

    std::string str;
    int val;
    while (file >> str){
        if (str.at(0) == 'a'){
            file >> val;
            signal_vec.push_back(new int [2] {2, val});
        }else{
            signal_vec.push_back(new int [2] {1, 0});
        }
    }
}

unsigned signalStrengths(std::vector<int*>& signal_vec){

    int cycle_check {20};
    int total_cycle_count {0};
    unsigned signal_sum {1}, signal_strength {0};

    for (const auto& cycle : signal_vec){
        total_cycle_count += cycle[0];

        if (total_cycle_count >= cycle_check){
            signal_strength += cycle_check * signal_sum;
            cycle_check += 40;
        }

        signal_sum += cycle[1]; // after cycle -> add values
        //std::clog << "Signal sum: " << signal_sum << '\n';
    }
    return signal_strength;
}

void readScreen(std::vector<int*>& signal_vec){

    int tick {0}, c_px {0}, ms {1};
    std::set<int> sprite_set {ms, ms-1, ms+1};

    for (const auto& cycle : signal_vec){

        tick = cycle[0]; // set loop var to current cycle count
    // let CPU cycle
        for (; tick > 0; --tick){
        // if current pixel being drawn overlaps with current sprite position -> draw '#'
            if (sprite_set.find(c_px) != sprite_set.end()) std::cout << '#';
            else std::cout << '.';
            ++c_px; // ++ current pixel position with each cycle
            if (c_px >= 40){
                std::cout << '\n';
                c_px = 0;
            }
        }

    // change sprite middle position && clear set && insert new sprite positions
        ms += cycle[1];
        sprite_set.clear();
        sprite_set.insert(ms);
        sprite_set.insert(ms-1);
        sprite_set.insert(ms+1);
    }
}