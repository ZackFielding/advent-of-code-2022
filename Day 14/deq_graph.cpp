#include <deque>
#include <vector>
#include <iostream>
#include <algorithm>
#include "funcs.hpp"

// indirect - only degraph can control nodes
struct _node {

    int x;
    int y;
    // bottom, bottom-left, bottom-right
    std::array<_node*, 3> adj_nodes;
    FILLED fill;

    //ctor
    _node() : x {-1}, y {-1},
            adj_nodes {nullptr, nullptr, nullptr},
            fill {FILLED::NOT_FILLED}
            {}
};

// interactable
struct degraph{

    const unsigned min_x;
    const unsigned max_x;
    const unsigned max_y;
    unsigned number_rows;
    unsigned number_col;
    std::deque < std::vector< _node > > graph;   

    degraph(const unsigned amin_x, const unsigned amax_x, const unsigned amax_y) 
        : min_x {amin_x}, max_x {amax_x}, max_y {amax_y},
            number_rows { (max_y + 1 )}, number_col { (amax_x - amin_x + 1) }, graph { (amax_x - amin_x + 1) }
    {
        // construct columns of data -> resize && allocate from heap
        std::for_each(graph.begin(), graph.end(), [this](auto& vec) mutable { vec.resize(this -> number_rows); });

        // set bottom edges
        std::for_each(graph.begin(), graph.end(),
                        [this] (auto& vec_col) mutable { this -> set_below_edges(vec_col.begin(), vec_col.size()); } );

        // set bottom left edges
        for (auto lead {++graph.begin()}, trail {graph.begin()}; lead != graph.end(); ++lead, ++trail){
            this -> set_bottom_left_edges (lead -> begin(), trail -> begin(), trail->size());
        }

        // set bottom right edges
        for (auto lead {++graph.begin()}, trail {graph.begin()}; lead != graph.end(); ++lead, ++trail){
            this -> set_bottom_right_edges (trail -> begin(), lead -> begin(), trail->size());
        }
    }

    // constructing graph edges
    // each of these act on a SINGLE column of data,
    // allowing for building out the graph in single columns
    using VECIT = std::vector <_node>::iterator;

    // establish bottom node member pointers - SINGLE COLUMN
    void set_below_edges(VECIT start_it, const int n){
        // iterate through n-1 node(s), setting the current bottom memeber to the node below it
        VECIT below_it {start_it};
        std::for_each_n(start_it, (n-1), [&below_it] (_node& cnode) mutable { cnode.adj_nodes[0] = &*(++below_it); } );
    }

    // establish bottom left " " "
    // ASSUMES that both vectors are of the same size
    void set_bottom_left_edges(VECIT start_it_being_set, VECIT start_it_ref, const int n){

        // being_set is right adjacent column to ref
        std::for_each_n (start_it_being_set, (n-1),
                            [&start_it_ref] (_node& cnode) mutable { cnode.adj_nodes[1] = &*(++start_it_ref); }
                        );
    }

    // establish bottom right  " " " -> identical to bottom left set except for member attribute
    void set_bottom_right_edges(VECIT start_it_being_set, VECIT start_it_ref, const int n){
        
        std::for_each_n (start_it_being_set, (n-1),
                            [&start_it_ref] (_node& cnode) mutable { cnode.adj_nodes[2] = &*(++start_it_ref); } );
    }

    void print_graph(){
        // iterate row x col
        for (unsigned row {0}; row < this -> number_rows; ++row){
            for (unsigned col {0}; col < this -> number_col; ++col){
                switch ( this -> graph.at(col).at(row).fill ){
                    case (FILLED::NOT_FILLED):{
                        std::cout << ". ";
                        break;
                    }
                    case (FILLED::ROCK): {
                        std::cout << "# ";
                        break;
                    }
                    case (FILLED::SAND): {
                        std::cout << "o ";
                        break;
                    }
                    case (FILLED::INF_BOTTOM): {
                        std::cout << "= ";
                        break;
                    }
                    default: {
                        std::cout << "? ";
                    }
                }
            }
            std::cout << '\n';
        }
    }

     // generate rock formations (setting fill)
    void create_rock_formations(ROCK_FORM *rf, bool infinite_bottom_present = false){

        // iterate over rock form
        for (const auto& vec_pair : *rf){
            for (std::size_t p {0}; p < vec_pair.size() - 1; ++p){

                // get adjacent pairs
                std::vector<int> x_range { vec_pair.at(p).first, vec_pair.at(p+1).first };
                std::vector<int> y_range { vec_pair.at(p).second, vec_pair.at(p+1).second };

                // sort adjacenet pairs into asecending order as it's not garunteed they will be
                // the greater value needs to be known for nested looping to fill in
                // continuous rock formations
                std::sort(x_range.begin(), x_range.end());
                std::sort(y_range.begin(), y_range.end());

                for (int _x {x_range[0]}; _x <= x_range[1]; ++_x){
                    for (int _y {y_range[0]}; _y <= y_range[1]; ++_y){
                        this -> graph[ _x - this -> min_x ][ _y ].fill = FILLED::ROCK;
                    }
                } 
            }
        }

        if (infinite_bottom_present){
            std::for_each(graph.begin(), graph.end(),
                            [](auto& vec) { vec.back().fill = FILLED::INF_BOTTOM; } );
        }
    }

    unsigned run_sand_simulation(const unsigned sxpos, const unsigned sypos, bool consider_inf_bottom = false){

        _node* const start_node { & this -> graph[ (sxpos - this -> min_x) ][sypos] }; // assign starting node
        _node* cnode {start_node};
        [[maybe_unused]] bool inf_found {false}, adj_empty_found {false};
        unsigned sand_count {0};

        while (1){

            if (cnode -> fill == FILLED::SAND){
               // sand has reached starting node p2 solution break -> if it hit in p1, error
               return sand_count;
            }

            int adj_pos {0};

            // iterate over all adjacent nodes
            for (auto& adj : cnode -> adj_nodes){

                // if nullptr encountered -> return current sand count
                if (adj == nullptr){

                    // part 1
                    if (!consider_inf_bottom){
                        this -> print_graph();
                        return sand_count;

                    } else {
                    // part 2 -> append column and get adjacent node (which will not be null this time)
                        append_columns_to_graph(adj_pos);

                        // if the node is filled with inf bottom -> break to fill current node
                        if (cnode -> adj_nodes[adj_pos] -> fill == FILLED::INF_BOTTOM){
                            break;
                        }

                        // else, set current node to newly created node
                        cnode = cnode->adj_nodes[adj_pos];
                    }
                }

                // if empty adjacent found -> visit empty adjacent node
                if (adj -> fill == FILLED::NOT_FILLED){
                    cnode = adj;
                    adj_empty_found = true;
                    break;
                }
                
                ++adj_pos;
            }

            if (adj_empty_found){
                adj_empty_found = false;
                continue;
            }

            // if all adjacents are filled ...
            cnode -> fill = FILLED::SAND;
            cnode = start_node;
            ++sand_count;
        }
    }

    void append_columns_to_graph(const unsigned side){

        switch (side){
            case (1): {

                // append new vector to front of deque
                graph.push_front( std::vector<_node> (number_rows) );

                // set bottom
                set_below_edges(graph[0].begin(), number_rows);

                // set bottom right
                set_bottom_right_edges(graph[0].begin(), graph[1].begin(), number_rows );

                // UPDATE bottom left of graph[1]
                set_bottom_left_edges(graph[1].begin(), graph[0].begin(), number_rows);

                // set inf bottom
                graph[0].back().fill = FILLED::INF_BOTTOM;
                break;
            }
            case (2):{

                // append new vector to end of deque
                graph.push_back( std::vector<_node> (number_rows) );

                // set bottom
                set_below_edges( graph.back().begin(), number_rows );

                // set bottom left edges
                auto adj_iter = (++graph.rbegin()) -> begin();
                set_bottom_left_edges(graph.back().begin(), adj_iter, number_rows);

                // UPDATE bottom right edges
                set_bottom_right_edges(adj_iter, graph.back().begin(), number_rows);

                // set inf bottom filling
                graph.back().back().fill = FILLED::INF_BOTTOM;

                break;
            }

            default:{
                // this WILL result in a SEGFAULT if this case is met (in calling function)
                std::cerr << "ERROR unknown side value passed to graph append.\n";
                break;
            }
        }
    }
};