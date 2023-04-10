#include "funcs.hpp"

/*
Node class -> there are going to be two cases when instantiating a node 
(1) the current node being created has not been created yet (1st ctor)
(2) the current node has already been created as it was an adjacent of earlier node(s) (2nd ctor)
*/

/*
TODO => DOES NOT CURRENTLY COMPILE
[] need to change graph to reference node* alloc on heap
*/

node::node(std::string name) : valve_name {name}, flow_rate {}, adj_node_map {}{}

void graph::add_node (std::string vstr, const int32_t fr){
    // try_emplace the current valve
    auto v = vstr.substr(0, 2);
    const auto [iter, _] = this -> graph_map.try_emplace(v, node{v});
    // set nodes flow rate
    iter -> second.flow_rate = fr;

    // set nodes adjacents
    for (szt i {2}; i < vstr.size(); i += 2){
        // adjacent valve string
        std::string adj_str { vstr.substr(i, 2) };
        // try_emplace adjacent valve into map (no need to check if exists as that will take atleast >=2 calls opposed to 1 here)
        const auto [adj_iter, _] = this -> graph_map.try_emplace( adj_str, node{adj_str} );
        // emplace current valves map with adjacent
        iter -> second -> adj_node_map.try_emplace(adj_iter->first, adj_iter->second);
    }
}

void graph::print_nodes(){

    using mciter = sn_map::const_iterator;
    void r_print_adj_nodes (mciter citer, mciter eiter){
        if (iter == eiter) return;
        std::clog << iter -> valve_name << ' ';
        r_print_adj_nodes(++iter, eiter);
    };

    for (const auto n : graph_map){
        std::cout << n.first << "'s adjacent nodes: ";
        r_print_adj_nodes(n.second.adj_node_map.cbegin(), n.second.adj_node_map.cend());
    }
}

void read_file_create_graphs (std::ifstream& file, graph* g){
    std::string str;
    while ( file >> str ){
        // rec get ALL valve names in current string
        std::string vstr {};
        vstr = r_ss(vstr, ++str.cbegin(), str.cend(), 65, 90, [](char){return false; });
        // rec get rate flow of current valve in string
        std::string rfstr {};
        rfstr = r_ss(rfstr, (str.cbegin()+23), str.cend(), 48, 57, [](char c){return (c == ';') ? true : false; });
        // add current node && adjacents to graph
        g -> add_node(vstr, std::stoi(rfstr));
    }
    file.close();
}

// recrusive substring generator which can work on the entire or parts of a string
std::string r_ss (std::string acc, strciter citer, const strciter eiter,
                    const int asciilb, const int asciiub,
                    std::function<bool(char c)> dpred){

     // if past the end of string
    if (citer == eiter || dpred(*citer)) return acc;

     // index if capital letter (capital letter ascii = 65-90)
    if ((int)(*citer) >= asciilb && (int)(*citer) <= asciiub) acc += *citer;

     // return the result of subsequent iteration
    return r_ss(acc, ++citer, eiter, asciilb, asciiub, dpred);
}
