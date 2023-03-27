#include <deque>
#include <vector>
#include <iostream>
#include <algorithm>

using ROCK_FORM = std::vector<std::vector<std::pair<int,int>>>;

// indirect - only degraph can control nodes
struct _node {

    int x;
    int y;
    _node* b;
    _node* bl;
    _node* br;
    _node* r;
    char fill;

    //ctor
    _node() : x {-1}, y {-1},
            b {nullptr}, bl {nullptr},
            br {nullptr}, r {nullptr},
            fill {'.'}
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
        std::for_each(this -> graph.begin(), this -> graph.end(),
                        [this] (auto& vec_col) mutable
                            { this -> set_bottom_left_edges(++vec_col.begin(), vec_col.begin(), vec_col.size()); }
                    ); 

        // set bottom right edges
        std::for_each(this -> graph.begin(), this -> graph.end(),
                        [this] (auto& vec_col) mutable
                            { this -> set_bottom_left_edges(vec_col.begin(), ++vec_col.begin(), vec_col.size()); }
                    ); 
    }

    // constructing graph edges
    // each of these act on a SINGLE column of data,
    // allowing for building out the graph in single columns
    using VECIT = std::vector <_node>::iterator;

    // establish bottom node member pointers - SINGLE COLUMN
    void set_below_edges(VECIT start_it, const int n){
        // iterate through n-1 node(s), setting the current bottom memeber to the node below it
        VECIT below_it {start_it};
        std::for_each_n(start_it, (n-1), [&below_it] (_node& cnode) mutable { cnode.b = &*(++below_it); } );
    }

    // establish bottom left " " "
    // ASSUMES that both vectors are of the same size
    void set_bottom_left_edges(VECIT start_it_being_set, VECIT start_it_ref, const int n){

        // being_set is right adjacent column to ref
        std::for_each_n (start_it_being_set, (n-1),
                            [&start_it_ref] (_node& cnode) mutable { cnode.bl = &*(++start_it_ref); }
                        );
    }

    // establish bottom right  " " " -> identical to bottom left set except for member attribute
    void set_bottom_right_edges(VECIT start_it_being_set, VECIT start_it_ref, const int n){
        
        std::for_each_n (start_it_being_set, (n-1),
                            [&start_it_ref] (_node& cnode) mutable { cnode.br = &*(++start_it_ref); } );
    }

    void print_graph(){
        // iterate row x col
        for (unsigned row {0}; row < this -> number_rows; ++row){
            for (unsigned col {0}; col < this -> number_col; ++col){
                std::cout << this -> graph.at(col).at(row).fill << ' ';
            }
            std::cout << '\n';
        }
    }

     // generate rock formations (setting fill)
    void create_rock_formations(ROCK_FORM *rf){

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
                        this -> graph[ _x - this -> min_x ][ _y ].fill = '#';
                    }
                } 
            }
        }
    }
};