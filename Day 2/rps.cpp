#include <iostream>
#include <fstream>
#include <map>

unsigned stratScoreAsIs(std::fstream& file){

    static const std::map<char, int> A_map {
        {'X', 3},
        {'Y', 6},
        {'Z', 0}
    };

    static const std::map<char, int> B_map {
        {'X', 0},
        {'Y', 3},
        {'Z', 6}
    };

    static const std::map<char, int> C_map{
        {'X', 6},
        {'Y', 0},
        {'Z', 3}
    };

    static const std::map<char, const std::map<char, int>*> map_of_map_ptrs {
        {'A', &A_map},
        {'B', &B_map},
        {'C', &C_map}
    };

    unsigned total_score {0};
    char my_play {'E'}, opp_play {'E'}; // E for error
    while (!!file){
        if (file >> opp_play >> my_play){
            total_score += map_of_map_ptrs.at(opp_play)->at(my_play); 
            total_score += static_cast<int>(my_play) - 87; // rock = 1, paper = 2, scissors = 3
        } 
    }
    return total_score;
}

unsigned requiredOutcomeScore(std::fstream& file){

    static const std::map<char, char> A_map {
        {'X', 'C'},
        {'Y', 'A'},
        {'Z', 'B'}
    };

    static const std::map<char, char> B_map {
        {'X', 'A'},
        {'Y', 'B'},
        {'Z', 'C'}
    };

    static const std::map<char, char> C_map{
        {'X', 'B'},
        {'Y', 'C'},
        {'Z', 'A'}
    };

    static const std::map<char, const std::map<char, char>*> map_of_map_ptrs {
        {'A', &A_map},
        {'B', &B_map},
        {'C', &C_map}
    };

    unsigned total_score {0};
    char my_play {'E'}, opp_play {'E'}; // E for error
    while (!!file){
        if (file >> opp_play >> my_play){
            total_score += static_cast<int>(map_of_map_ptrs.at(opp_play)->at(my_play)) - 64;
            switch (static_cast<int>(my_play)){
                case 89: { 
                    total_score += 3; 
                    break;
                }
                case 90: { 
                    total_score += 6;
                    break;
                }
            }
            //std::cout << score << '\n';
            //std::cout << total_score << '\n';
        } 
    }
    return total_score;
}

int main(){

    std::fstream strat_guide;
    strat_guide.open("strategy_guide.txt");

    if (!!strat_guide){
         // part 1 solution
        std::cout << "Strategy score as is: " << stratScoreAsIs(strat_guide) << '\n'; 

         // reset stream flags && seek getter back to start of file prior to next read
        strat_guide.clear(); 
        strat_guide.seekg(0L, std::ios_base::beg); 

         // part 2 solution
        std::cout << "Total score if second character is outcome of game: " << requiredOutcomeScore(strat_guide);
    }

    strat_guide.close();
    return 0;
}