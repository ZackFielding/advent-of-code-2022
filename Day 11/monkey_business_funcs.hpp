#ifndef MONKEY_BUSINESS_FUNCS_HPP
#define MONKEY_BUESINESS_FUNCS_HPP
#include <iostream>
#include <fstream>
#include <deque>
#include <functional>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

struct monkey {
     // list of items represented by worrry levels
    std::deque<double> items;
     // lambda for manipulating worry level before test
    std::function<double(double)> op; 
     // what to divide item worry for test
    double test;
     // what monkey to throw item to based on result of test
    std::pair<int, int> tf_monkey;
    int inspection_count {0};
};

void stringOperation(std::ifstream& file, monkey *m){

     // strings to hold file reads
    std::string hold, value;
     // consume file contents until math operator is encountered
    while (file >> hold && (hold == "new" || hold == "old" || hold == "=")){}
     // read operator and subsequent number
    file >> value; // read in operation && number to apply operation

    if (value != "old"){
         // if value is a number
        double vali {};
        // convert value to int
        try {
            vali = std::stod(value);
        }
        catch (std::invalid_argument&){
            std::cerr << "Failure to convert string to double in stringOperation().\n";
            return;
        }

        // assign proper lambda to monkey based on operator
        if (hold == "+") m->op = [vali](double passed)->double { return passed + vali; };
        else m->op = [vali](double passed)->double { return passed * vali; };
    
    }else {
         // if old is the value (old + old OR old * old)
        if (hold == "+") m->op = [](double passed)->double { return passed + passed; };
        else m->op = [](double passed)->double { return passed * passed; };
    }
}

void stringTest(std::ifstream& file, monkey *m){

    std::string str;
    while (file >> str && (str == "divisible" || str == "by")) {}
    m->test = std::stod(str); 
}

void stringIf(std::ifstream& file, monkey *m){

    static bool monkey_bool {true};
    std::string str;

    file >> str >> str >> str >> str >> str; // consume all && final value
    int vali {};
    try {
        vali = std::stoi(str);
    }
    catch (std::invalid_argument&){
        std::cerr << "Failure to convert string to integer in stringIf().\n";
        return;
    }

    if (monkey_bool){
        m->tf_monkey.first = vali; // write to TRUE
        monkey_bool = false;
    } else{
        m->tf_monkey.second = vali; // write to FALSE
        monkey_bool = true;
    }
}

void stringStarting(std::ifstream& file, monkey *m){
     // consume "items"
    {
        std::string str;
        file >> str;
    }
     
    std::stringstream ss;
    double reti {};
    char hold;

    while (file.get(hold)){
         // if char is number
        if (hold >= 48 && hold <= 57) ss << hold;
         // else if white space or ignore character (i.e., ',')
        else {
            // ensure ss is not empty => if not, convert to int and push into obj list
            if (ss.str() != ""){
                ss >> std::dec >> reti; // read as int
                m->items.push_back(reti); // add to back of list
                ss.clear();
                ss.str("");
            }
            if (hold == 13 || hold == 10) return;
        }
    }
}

template <int NUM>
void getMonkeyAttributes(std::ifstream& file, std::array<monkey, NUM>& monkey_array){

    int midx {-1}; // offset by 1 as first monkey will be array index 0
    std::string str;

    while (file >> str){
        if (str == "Monkey") ++midx;
        else if (str == "Starting") stringStarting(file, &monkey_array[midx]);
        else if (str == "Operation:") stringOperation(file, &monkey_array[midx]);
        else if (str == "Test:") stringTest(file, &monkey_array[midx]);
        else if (str == "If") stringIf(file, &monkey_array[midx]);
    }
}

template <int NUM>
std::pair<int, int> passItem(std::array<monkey, NUM>& monkey_array, int nr){

    double ti {0};
     // for N rounds
    for (; nr >0 ; --nr){
         // iterate through each monkey
        for (auto miter {monkey_array.begin()}; miter != monkey_array.end(); ++miter){
             // while list is not empty (i.e., monkey has not inspected all items)
            while (!(miter->items.empty())){
                ti = miter->items.front(); // get front item of list
                miter->items.pop_front(); // pop got item from list
                ++(miter->inspection_count); // ++ inspection count for current monkey
                 // calc new item worry based on operation && dividing value by 3
                ti = std::floor((miter->op(ti) / 3.0)); 
                 // test item based on new worry score
                if (static_cast<int>(ti) % static_cast<int>(miter->test) == 0){
                     // if mod == 0 => push current item worry into correct monkey's list
                    monkey_array.at(miter->tf_monkey.first).items.push_back(ti);
                    //printf("Value is: %.1f, [TRUE] thrown to monkey %d\n", ti, miter->tf_monkey.first); 
                } else {
                     monkey_array.at(miter->tf_monkey.second).items.push_back(ti);
                     //printf("Value is: %.1f, [FALSE] thrown to monkey %d\n", ti, miter->tf_monkey.second);
                }
            }
        }
    }

     // loop through monkeys to determine what two have the highest activity scores
    std::pair<int, int> highest_scores {0,0};
    int temp {0};
    for (auto miter {monkey_array.begin()}; miter != monkey_array.end(); ++miter){
        if (miter->inspection_count > highest_scores.second){
            if (miter->inspection_count > highest_scores.first){
                temp = highest_scores.first;
                highest_scores.first = miter->inspection_count;
                highest_scores.second = temp;
            } else {
                highest_scores.second = miter->inspection_count;
            }
        }
    }
    return highest_scores;
}
#endif