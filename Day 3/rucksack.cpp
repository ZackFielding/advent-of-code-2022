#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

void streamBagItems(std::ifstream& file, std::vector<std::string>* bag_items){

    for (std::string& bag : *bag_items){
        if (! (file >> bag)) std::clog << "File read failed.\n";
    }
}

unsigned getBagTypes(std::vector<std::string>* bag_items){

    unsigned priority_sum {0};
    std::map<char, bool> char_map; // map of chars
    std::map<char, bool>::iterator iter;
    
    auto l_alpha_as_int = [](char c)->int {
            if (c >= 97 && c <=122) // lower case
                return (unsigned)c - 96;
            else // upper case (comes before lower in ascii)
                return (unsigned)c - 38;
        };

    for (const std::string& str : *bag_items){
        size_t idx {0};
        for (; idx < str.length()/2; ++idx){
           char_map.insert({str.at(idx), true}); // insert current char in string into map
        }
        for (; idx < str.length(); ++idx){
            iter = char_map.find(str.at(idx)); // get iterator to map item (==iterator_end if not found)
            if (iter != char_map.end()) break; // if character is contained within map -> MATCH FOUND
        }

         // determine matching characters int value and sum with current sum
        priority_sum += l_alpha_as_int(iter->first);
        //std::clog << priority_sum << '\n';
          // erase current map contents for next loop
        char_map.erase(char_map.begin(), char_map.end()); 
    }
    return priority_sum;
}

unsigned threeElfBagType(const std::vector<std::string>* bag_items){

    unsigned priority_sum {0};
    std::map<char, bool> char_map; // map of chars
    std::map<char, bool>::iterator iter;
    
    auto l_alpha_as_int = [](char c)->int {
            if (c >= 97 && c <=122) // lower case
                return (unsigned)c - 96;
            else // upper case (comes before lower in ascii)
                return (unsigned)c - 38;
        };

    for (size_t idx {0}; idx < bag_items->size(); idx += 3){
         // insert entire line into map
        std::for_each(bag_items->at(idx).cbegin(), bag_items->at(idx).cend(), 
                            [&char_map](char c) { char_map.insert({c, true}); });
        
         // now loop through 2nd line until you find a match
        char match_line2_char {};
        bool match_found {false};
        for (const char line2_char : bag_items->at(idx+1)){
            if ((iter = char_map.find(line2_char)) != char_map.end()){
                match_line2_char = iter->first; // hold matching char
                for (const char line3_char : bag_items->at(idx+2)){
                    if (line3_char == match_line2_char){ // if found match on line 2 && 3
                        priority_sum += l_alpha_as_int(match_line2_char); 
                        match_found = true;
                        break;
                    }
                }
                if (match_found) break;
            }
        }
    }
    return priority_sum;
}

int main(){
    std::ifstream itemsf;
    itemsf.open("items.txt");

    if (!!itemsf){
        std::vector<std::string> bag_items (300);

         // stream file into vector of strings 
        streamBagItems(itemsf, &bag_items);

         // determine each bags type
        std::cout << "Sum of priority items: " << getBagTypes(&bag_items) << '\n';

         // find 3-elf bag type
        std::cout << "3-Elf priority item sum: " << threeElfBagType(&bag_items) << '\n';
    }

    itemsf.close();
    return 0;
}
