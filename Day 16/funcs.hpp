#ifndef FUNCS_HPP
#define FUNCS_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>

// fwd decs for using statements
class node;
struct graph;

using szt = std::size_t;
using strv_pos = std::pair < std::string, szt > ;
using sn_map = std::unordered_map<std::string, node*>;
using strciter = std::string::const_iterator;
using miter = sn_map::iterator;

class node {
  std::string valve_name;
  int32_t flow_rate;
  sn_map adj_node_map;
  node (std::string);
  node(std::string, const int32_t);
  friend graph;
};

struct graph {
  sn_map graph_map;
  graph(): graph_map {} {}
  void add_node(std::string, const int32_t);
  void print_nodes();
  ~graph();
private:
  node* __add_single_node__(std::string);
  void __iter_nodes__(std::function<void(miter)>, std::function<void(node*)> node_func);
  void __rec_inner(miter, const miter, std::function<void(miter)>);
};

std::string get_line_as_string (std::ifstream&);
void read_file_create_graph (std::ifstream&, graph*);
std::string r_ss (std::string, strciter, const strciter,
                    const int, const int, std::function<bool(char c)>);

#endif