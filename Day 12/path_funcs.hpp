#ifndef PATH_FUNCS_HPP
#define PATH_FUNCS_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>

using graph = std::unordered_map<int, std::unordered_set<int>>;

template <typename VEC_T>
struct fmdarray{
    std::vector<VEC_T> vec;
    int dim_size;
};

template <typename VEC_T>
void readFile(std::ifstream& file, fmdarray<VEC_T>* v){
    char c;
    while (file.get(c)){
        if (c >= 65 && c <= 122){
            v->vec.push_back(c-97);
        }
    }
}

template <typename VEC_T>
std::pair<int, int> createGraph(graph *g, fmdarray<VEC_T> *v){
    // iterate through flat array
    const int dim {v->dim_size};
    std::pair<int, int> sep;

    // same check for all conditions
    const auto l_check_insert = [&g](int curv, int adjav, int curp, int adjap)->void{
        if ( (adjav == -14 || adjav == -28 || curv-adjav >= -1) ||
            (curv == -14 && (adjav == 0 || adjav == 1)) ){
            g->find(curp)->second.insert(adjap);
        }
    };

    for (int idx {0}; idx < (int)v->vec.size(); ++idx){
        // create new map key
        (*g)[idx];
        // check to see if start or end vertex
        if (v->vec.at(idx) == -14 || v->vec.at(idx) == -28){
            if (v->vec.at(idx) == -14) sep.first = idx;
            else sep.second = idx;
        }

        // check above 
        if (idx-dim >= 0){
            l_check_insert(v->vec.at(idx), v->vec.at(idx-dim), idx, idx-dim);
        }
        // check below
        if (idx+dim < (int)v->vec.size()){
            l_check_insert(v->vec.at(idx), v->vec.at(idx+dim), idx, idx+dim);
        }

        // check right -> mod stops those with no rightward edges (i.e., end of row)
        if (idx % (dim-1) != 0 && idx + 1 < (int)v->vec.size()){
            l_check_insert(v->vec.at(idx), v->vec.at(idx+1), idx, idx+1);
        }

        // check left -> mod stops those with no leftward edges (i.e., start of new row)
        if (idx % dim != 0 && idx - 1 >= 0){
            l_check_insert(v->vec.at(idx), v->vec.at(idx-1), idx, idx-1);
        }
    }
    return sep;
}

// DOES NOT WORK
// get stuck in infinite loop, stuck in {0,0} in pq
int findShortestPath(graph* g, std::pair<int, int>* sep){

    using INTP = std::pair<int, int>;
    // priority que to hold next best case node to visit
    // pq uses a cusomt lambda comparator to ensure the smallest pair.second values are at the top
    auto l_comp = [](const INTP& a, const INTP& b) { return b.second > a.second; };
    std::priority_queue<INTP, std::vector<INTP>, decltype(l_comp)> pq(l_comp);
    pq.push({sep->first,0});

    // array to hold distances betwen each start->all other vertices
    std::vector<int> dist (g->size(), (int)INFINITY);

    int vv {0}, cd {0};
    while (!pq.empty()){
        // access top of priority queue to determine what vertex to visit
        vv = pq.top().second; 
        ++cd; // all edges == 1

        // iterate through this vertices adjacent vertices && add to pq with updated distances
        for (const int& t : g->find(vv)->second){
            pq.push({t, cd});
            if (cd < dist.at(t)) dist.at(t) = cd;
        }
        pq.pop(); // remove node just visited
    }
    return dist.at(5);
}

#endif