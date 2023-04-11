#include "funcs.hpp"

node::node(std::string name) : valve_name {name}, flow_rate {}, adj_node_map {}{}

// handles dirty work of creating new nodes if they don't already exist
node* graph::__add_single_node__(std::string str){
    auto check = this -> graph_map.find(str);
    node* node_to_ret = nullptr;
    if ( check == graph_map.end() ){
       node_to_ret = new node {str}; 
       graph_map.emplace(str, node_to_ret);
    } else {
        node_to_ret = check -> second;
    }
    return node_to_ret;
}

void graph::add_node (std::string vstr, const int32_t fr){

    auto v = vstr.substr(0, 2);
    node* cnode = this -> __add_single_node__(v);
    cnode -> flow_rate = fr;

    // set nodes adjacents
    node* anode = nullptr;
    for (szt i {2}; i < vstr.size(); i += 2){
        // adjacent valve string
        std::string adj_str { vstr.substr(i, 2) };
        // try_emplace adjacent valve into map (no need to check if exists as that will take atleast >=2 calls opposed to 1 here)
        anode = this -> __add_single_node__(adj_str);
        // emplace current valves map with adjacent
        cnode -> adj_node_map.emplace(adj_str, anode);
    }
}

/*
- currently ONLY print_nodes() utilize __iter_nodes which recursively calls __rec_inner 
    but set up with function args for future transform member functions
*/
/* ------------------------------------------------*/
void graph::__rec_inner(miter citer, const miter eiter, std::function<void(miter)> iter_func){
    if (citer == eiter) return;
    iter_func(citer);
    this -> __rec_inner(++citer, eiter, iter_func);
}

void graph::__iter_nodes__(std::function<void(miter)> iter_func, std::function<void(node*)> node_func){
    for (const auto& n : graph_map){
        this -> __rec_inner(n.second->adj_node_map.begin(), n.second->adj_node_map.end(), iter_func);
        node_func(n.second);
    }
}

void graph::print_nodes(){
    auto _l1 = [](miter iter){ std::cout << iter -> first << ' '; };
    auto _l2 = [](node* cnode) { std::cout << "... are adj to " << cnode -> valve_name << '\n'; };
    this -> __iter_nodes__(_l1, _l2);
}

graph::~graph(){
    std::for_each(this -> graph_map.begin(), this -> graph_map.end(), [](auto& p) { delete p.second; });
}
/* --------------------------------------------- */
std::string get_line_as_string (std::ifstream& file){
    char c {};
    std::string str_to_ret {};
    while (file.get(c) && c != '\n') str_to_ret += c;
    return str_to_ret;
}

void read_file_create_graph(std::ifstream& file, graph* g){
    std::string str;
    while (1){
        // get line as std string =>  if nothing extracted, break
        str = get_line_as_string(file);
        if (str == "") break;

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
