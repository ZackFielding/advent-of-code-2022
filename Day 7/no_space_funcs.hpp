#ifndef NO_SPACE_FUNCS_HPP
#define NO_SPACE_FUNCS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <vector>
#include <string>
#include <sstream>

class DIRECTORY{
public:
    unsigned long file_size;
    DIRECTORY* containing_directory;
    std::map<std::string, DIRECTORY*> subdirectories;
    DIRECTORY() : file_size {0}, containing_directory {nullptr} {}
};

void createDirectory(std::ifstream& file, DIRECTORY& outtermost, std::vector<DIRECTORY*>& heap_mem){

    // for reading file and creating sub strings to interpret file
    char line[50]; // for getline()
    std::string line_as_str, sub_line_str, cd_str;
    
    // obj* && iterators for handling list creation 
    DIRECTORY* current_directory = &outtermost;
    DIRECTORY *temp_directory {nullptr}, *previous_directory {nullptr};
    std::map<std::string, DIRECTORY*>::iterator iter;
    std::pair<std::map<std::string, DIRECTORY*>::iterator, bool> iter_ret;

     // stream and temp int for getting file sizes in current directory
    std::stringstream ss;
    unsigned long found_file_size {0};

    file.getline(line, sizeof line); // comsume outter most directory command

    while (file.getline(line, sizeof line)){
        line_as_str.append(line, file.gcount()); // read into string
        //std::cout << "LINE:   " << line_as_str << '\n';
        sub_line_str = line_as_str.substr(0, 4);

    // if subdir found -> create new directory obj && insert as pair into current obj's map
        if (sub_line_str == "dir "){
            temp_directory = new DIRECTORY; // create new obj
              // insert into current obj's map
            
            iter_ret = current_directory->subdirectories.insert
                ({line_as_str.substr(4, sizeof line), temp_directory});
            heap_mem.push_back(temp_directory); // add to heap alloc tracking vector
            //if (!(iter_ret.second)) std::clog << "Failure to insert new DIRECTORY into map.\n";
            //std::clog << "Heap mem vector size is now " << heap_mem.size() << '\n';

    // if changing directory -> cache current directory as previous
        }else if (sub_line_str == "$ cd"){
        // if not going backwards but forwards ...
            if (line_as_str.substr(5,1) != "."){
                previous_directory = current_directory;
                cd_str = line_as_str.substr(5, sizeof line); // get directory name to be changed to
                iter = current_directory->subdirectories.find(cd_str); // get iter to directory being changed to

            // check to see if find returned an actual pair
                if (iter != current_directory->subdirectories.end()){
                    current_directory = iter->second; // assign current directory to correct subdir obj
                    current_directory->containing_directory = previous_directory; // assign ptr to previous obj
                }else std::clog << "Can't find directory in map.\n";
        // if going backwards -> current directory is now the containing directory
            } else if (line_as_str.substr(5,1) == ".") {
                current_directory = current_directory->containing_directory;
            }

    // if list command -> ignore
        } else if (sub_line_str == "$ ls"){
            // do nothing

    // if not a command or directory -> its a file size -> add number too current dir size
        } else {
            ss << line;
            ss >> std::dec >> found_file_size;
            current_directory->file_size += found_file_size;
            ss.str("");
            ss.clear();
        }
        line_as_str.clear();
        sub_line_str.clear();
    }
}

unsigned long getSize(DIRECTORY* directory_p, unsigned long& size_threshold){

 // if no subdirectories -> return sum of all files in current directory 
if (directory_p->subdirectories.empty()){
    if (directory_p->file_size <= 100000) size_threshold += directory_p->file_size;
    return directory_p->file_size;
// if there are subdirectories -> find their sizes first
} else {
    // iterate through map to find size of subdirectories
    unsigned long sum {0};
    for (auto map_iter {directory_p->subdirectories.begin()}; 
            map_iter != directory_p->subdirectories.end(); ++map_iter){
          // recursion
        sum += getSize(map_iter->second, size_threshold);
        }
    directory_p->file_size += sum; // set obj file size
    if (directory_p->file_size <= 100000) size_threshold += directory_p->file_size;
    return directory_p->file_size; // return file size for next dir
    }
}

unsigned long sumOfDir(std::vector<DIRECTORY*>& heap_mem, unsigned long min_to_remove){

     // start with size of device
    unsigned long smallest_to_remove {70000000};
    for (size_t idx {0}; idx < heap_mem.size(); ++idx){
       if (heap_mem.at(idx)->file_size >= min_to_remove){ // if current dir is >= minimum required
    // if this is the smallest value so far -> save
        if (heap_mem.at(idx)->file_size < smallest_to_remove) smallest_to_remove = heap_mem.at(idx)->file_size;
       } 
    }
    return smallest_to_remove;
}

#endif