#ifndef FUNCS_HPP
#define FUNCS_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>

// fwd decs for using statements
class node;
struct graph;

using szt = std::size_t;
using strv_pos = std::pair < std::string, szt > ;
using sn_map = std::unordered_map<std::string, node*>;
using strciter = std::string::const_iterator;

class node {
  std::string valve_name;
  int32_t flow_rate;
  std::unordered_map<std::string, node*> adj_node_map;
  node (std::string);
  node(std::string, const int32_t);
  friend graph;
};

struct graph {
  sn_map graph_map;
  graph(): graph_map {} {}
  void add_node(std::string, const int32_t);
  void print_nodes();
};

void read_file_create_graph (std::ifstream&, graph*);
std::string r_ss (std::string, strciter, const strciter,
                    const int, const int, std::function<bool(char c)>);

#endif