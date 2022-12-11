#ifndef TREEHOUSE_FUNCS_HPP
#define TREEHOUSE_FUNCS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

int getGridLength(std::ifstream& file){
    char hold;
    int length {0};
    while (file.get(hold)){
        if (hold != '\n') ++length;
        else break;
    }
    file.seekg(0L, std::ios_base::beg);
    return length;
}

void readGrid(std::ifstream& file, std::vector<int>& flat_grid_vec, size_t& grid_depth){

    std::string str;
    while (file >> str){
        ++grid_depth; // track num rows
        for (char& val : str){
            flat_grid_vec.push_back((int)val - 48);
        }
    }
}

std::pair<int,int> findVisibleTrees(std::vector<int>& grid_vec, const int grid_length){

    // loop through grid, avoiding edges as these are always visible (account for them at end)
    int visible_trees {grid_length}, row_pos {0}, col_pos {0}, val {0};
    std::vector<int> visible_vec (4, 0);
    int greatest_visible {0}, current_visible {1};
    int not_seen {0}; // if == 4 @ end of check, not seen from any side

    for (int idx {grid_length}; idx < ((int)grid_vec.size() - grid_length); ++idx){
       val = grid_vec.at(idx); // store current val 
       row_pos = idx / grid_length; // current row position
       col_pos = idx - (row_pos*grid_length); // current column position

    // if current position is first row or last row -> it's visible
       if (col_pos == 0 || col_pos == grid_length - 1) ++visible_trees; 

    // if not, run it through!
       else{
            not_seen = 0; // reset not_seen for current col&row check
            // check columns first
            for (int up {idx-grid_length}; up >= 0; up -= grid_length){
                ++visible_vec.at(0);
                if (grid_vec.at(up) >= val){
                    ++not_seen;
                    break;
                }
            }
            // check down column next
            for (int down {idx+grid_length}; down < (int)grid_vec.size(); down += grid_length){
                ++visible_vec.at(1);
                if (grid_vec.at(down) >= val){
                    ++not_seen;
                    break;
                }
            }
            // check to right
            for (int right {idx+1}; right < ((row_pos+1)*grid_length); ++right){
                ++visible_vec.at(2);
                if (grid_vec.at(right) >= val){
                    ++not_seen;
                    break;
                }
            }
            // check to left
            for (int left {idx-1}; left >= (row_pos*grid_length); --left){
                ++visible_vec.at(3);
                if (grid_vec.at(left) >= val){
                    ++not_seen;
                    break;
                }
            }
            if (not_seen <4) ++visible_trees; // not visisble from all sides == 4
            for (const auto& score : visible_vec) current_visible *= score;
            if (current_visible > greatest_visible) greatest_visible = current_visible; 
             // reset visible_vec values to 0
            std::for_each(visible_vec.begin(), visible_vec.end(), [](int& set){ set = 0; });
             // reset current visible to 1
            current_visible = 1;
       }
    }
    visible_trees += grid_length;
    return std::pair<int,int> ({visible_trees, greatest_visible});
}

#endif
