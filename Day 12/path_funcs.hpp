#ifndef PATH_FUNCS_HPP
#define PATH_FUNCS_HPP 
#include <iostream> 
#include <fstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

void readFile(std::ifstream& file, std::vector<char> *vec){
    char chold {};
    while (file >> chold){
        if (chold != 13 && chold != 10) vec->push_back(chold);
    }
    return;
}

int getLineLength(std::ifstream& file){
    // reset get pointer
    file.clear();
    file.seekg(0L, std::ios_base::beg);
    char buffer [1000]; // assumes there will be <1000 char in line
    file.getline(buffer, sizeof buffer);
    return (int)(file.gcount()-1);
}

class graph; // fwd decl for friend decl
class vertex {
    int id; // unique id
    std::unordered_set<vertex*> adjacent_set;
    vertex(int a_id) : id {a_id}, adjacent_set {} {};
    friend class ::graph;
};

class graph {
    std::unordered_map<int, vertex*> all_vertex_map;
    vertex *start;
    vertex *end;
public:
    graph() : all_vertex_map {}, start {nullptr}, end {nullptr} {}
    ~graph() { for (std::pair<int, vertex*> v : all_vertex_map) delete v.second; }
    vertex* addVertex(int ident, const char letter){
        // check to see if vertex has already been created
         // if ident does not exist => create
        auto check_map = all_vertex_map.find(ident);
        vertex* vptr {nullptr};
        if (check_map == all_vertex_map.end()){
            vptr = new vertex{ident};
            all_vertex_map.insert({ident, vptr});
             // handle Start && End cases
            if (letter == 'S') start = vptr;
            else if (letter == 'E') end = vptr;
        } else vptr = check_map->second;
        return vptr;
    }

    void addAdjacent(vertex* current, int ident, const char a_letter){
         // get vertex ptr to adajcent
        auto check_map = all_vertex_map.find(ident);
        vertex* to_add {nullptr};
        if (check_map == all_vertex_map.end()) to_add = this->addVertex(ident, a_letter);
        else to_add = check_map->second;
         // add to current vertex's adjacent list
        if (current == nullptr) std::clog << "NULL";
        current->adjacent_set.insert(to_add); 
        return;
    }

    void createGraph(std::vector<char> *, const int);
    void check_add(std::vector<char> *, const int, const int, vertex* current);
    void test();
};

void graph::check_add(std::vector<char> *vec, const int id, const int a_id, vertex* current){
    // handle Start && End cases
    int current_val {0}, adjacent_val {0};

    if (vec->at(id) == 'S' || vec->at(id) == 'E'){
        current_val = (vec->at(id) == 'S') ? 97 : 122; 
    } else current_val = vec->at(id);
    
    if (vec->at(a_id) == 'S' || vec->at(a_id) == 'E'){
        adjacent_val = (vec->at(a_id) == 'S') ? 97 : 122; 
    } else adjacent_val = vec->at(a_id);

    int step_check = current_val - adjacent_val;
    if (step_check == -1 || step_check >= 0){
        this->addAdjacent(current, a_id, vec->at(a_id));
    }
}

void graph::createGraph(std::vector<char> *vec, const int line_length){

    vertex *current {nullptr};
    int above_id {line_length * -1}, below_id {line_length}, right_id {1}, left_id {-1};
    int cc {0};

    for (int id {0}; id < (int)vec->size(); ++id){

        current = this->addVertex(id, vec->at(id)); 

         // above
        if (above_id >= 0) this->check_add(vec, id, above_id, current);
        
         // below
        if (below_id < (int)vec->size()) this->check_add(vec, id, below_id, current);

         // left [CRASH]
        if (left_id >= 0 && (cc % line_length != 0)) this->check_add(vec, id, left_id, current);
        else if (cc % line_length == 0) cc = 0; // reset counter

         // right
        if (cc < (line_length-1)) this->check_add(vec, id, right_id, current);
        ++cc;
        ++above_id; ++below_id; ++right_id; ++left_id;
    }
    return;
}

void graph::test(){
    for (auto p : this->all_vertex_map){
        std::printf("%d adjacents are: ", p.second->id);
        for (auto i : p.second->adjacent_set){
            std::cout << i->id << ' ';
        }
        std::cout << '\n';
    }
    return;
}

#endif