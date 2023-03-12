#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include <string_view>
#include <algorithm>

// abstraction to represent each list
class node{
public:
	std::vector<std::pair<int, node*>> int_node_vec;
	node() : int_node_vec {} {}
};

std::pair<int, std::size_t> getInteger(std::string_view sub_str){
	// get offset counter of next comma
	//  need to check if ']' or ',' comes first
	std::size_t comma_epos = sub_str.find_first_of(',');
	std::size_t bracket_epos = sub_str.find_first_of(']');
	std::size_t epos = (bracket_epos < comma_epos || comma_epos == sub_str.npos) ? bracket_epos : comma_epos;

	// enter each value into ss to be converted to int and returned
	std::stringstream ss {};
	std::for_each_n(sub_str.begin(), epos, [&ss](char c){ ss << c; });
	int i {};
	ss >> std::dec >> i;
	--epos;
	return {i, epos};
}

void createNodes(std::ifstream& f, std::vector<node*>* root_vec){
	std::string line {};
	while (f){
		if (f >> line){
			// create view of string => include last `]`
			// for getInteger() to work
			std::string_view sv {&line[1], line.size()-1};

			// create base root node && add to vector
			node* cur_node = new node {};
			root_vec->push_back(cur_node);

			// create stack to allow traversal through nested nodes
			std::stack<node*> stack {};
			//stack.push(cur_node);

			// ptr to previous node to allow node* indexing
			node *prev_node {nullptr};

			// pair for getInteger return
			std::pair<int, std::size_t> p;

			// iterate through line
			for (std::size_t idx {0}; idx < sv.size(); ++idx){
				//std::cout << sv[idx] << ' ';
				if (sv[idx] == '['){
					// cache previous node
					prev_node = cur_node;

					// add to stack
					stack.push(cur_node);

					// create new node
					cur_node = new node {};

					// index new node into previous node array
					// -1 signals that it's an array
					prev_node -> int_node_vec.push_back({ -1, cur_node });

				} else if (sv[idx] == ']'){
					// traverse backwards
					if (stack.empty()){
						continue;
					}
					cur_node = stack.top();
					stack.pop();

				} else if (sv[idx] != ','){
					// get integer and offset
					p = getInteger(sv.substr(idx));			
					cur_node -> int_node_vec.push_back({ p.first, nullptr });

					// adjust loop cond. to ensure skipping over middle of integer
					idx += p.second;
				}
			}
		}
	}
}

void rPrintNodes(const std::vector<std::pair<int, node*>>* vec){
	std::cout << "   list: ";
	if (vec->size() == 0) std::cout << "empty ";
	for (const std::pair<int, node*> p : *vec){
			if (p.first != -1) std::cout << p.first << ' ';
			else {
				rPrintNodes(&p.second->int_node_vec);
			}
		}

	std::cout << " back ";
	return;
}

void printNodesWrapper(std::vector<node*>* root_vec){
	// wrapper that calls recursive print func
	for (const node* n : *root_vec){
		std::cout << "New root: ";
		rPrintNodes(&n->int_node_vec);
		std::cout << '\n';
	}
}

void rDeleteNodes(node* node){
	// given node, iterate through it's entire vector
	// recursively call each nested node
	// until end of array is reached
	// then delete current node and return
	for (auto p : node->int_node_vec){
		if (p.second != nullptr) rDeleteNodes(p.second);
	}
	delete node;
	return;
}

enum class RRESULT : int {EQUAL = -1, GREATER = 0, LESS = 1};

using VECPAIR = std::vector<std::pair<int, node*>>;
RRESULT _compare(VECPAIR* lhs, VECPAIR *rhs){
	// used smallest arrays size
	std::size_t limit = (lhs->size() < rhs->size()) ? lhs->size() : rhs->size();
	node* lhs_second {nullptr}, *rhs_second {nullptr};

	RRESULT iret {RRESULT::GREATER};
	for (int i {0}; i < limit; ++i){

		lhs_second = lhs->at(i).second;
		rhs_second = rhs->at(i).second;


		// if both are non-list values
		if (lhs_second == nullptr && rhs_second == nullptr){
			// invalid seq cond (left needs to be LESS THAN right)
			if (rhs->at(i).first > lhs->at(i).first){
				return RRESULT::LESS;
			} else if (rhs->at(i).first < lhs->at(i).first){
				return RRESULT::GREATER;
			}

		// both are lists
		} else if (lhs_second != nullptr && rhs_second != nullptr){
			iret = _compare(&lhs->at(i).second->int_node_vec,
					&rhs->at(i).second->int_node_vec);
			if (iret > RRESULT::EQUAL) return iret;

		// one must be int and the other a list
		} else {
			// if lhs is the single value
			if (lhs_second == nullptr){
				VECPAIR vp {{lhs->at(i).first, nullptr}};
				iret =  _compare(&vp, &rhs->at(i).second->int_node_vec);
				if (iret > RRESULT::EQUAL) return iret;
			// if rhs is single int
			} else {
				VECPAIR vp {{rhs->at(i).first, nullptr}};
				iret =  _compare(&lhs->at(i).second->int_node_vec, &vp);
				if (iret > RRESULT::EQUAL) return iret;
			}
		}
	}

	// if all values are good at end of list
	// && if both are same size => return true
	if (lhs->size() == rhs->size()){
		return RRESULT::EQUAL;
	}
	else if (lhs->size() < rhs->size()) return RRESULT::LESS;
	return RRESULT::GREATER;
}

int compareSequences(std::vector<node*>* root_vec){
	// compare adjacents each loop
	int comp {1}, idx_sum {0};
	RRESULT iret {0};
	for (int idx {0}; idx < root_vec -> size()-1; idx += 2, ++comp){
		iret =_compare(&root_vec->at(idx)->int_node_vec, &root_vec->at(idx+1)->int_node_vec) ;
		if (iret != RRESULT::GREATER){
			idx_sum += comp;	
			// if (iret == RRESULT::EQUAL) std::cout << "SAME";
		}
	}
	return idx_sum;
}

// FOOT GUN HAS GONE OFF
// ++ptr on a custom type DOES NOT WORK
void quicksortSequences(const std::size_t lb, const std::size_t ub,
			std::vector<node*>* root_vec,
			int* div2_idx, int* div6_idx){

	// going out of range for some reason when == used for base case
	// base case
	if (lb >= ub) return;

	std::size_t lhs {lb}, rhs {ub-1}, pivot {ub};
	node *temp {nullptr};
	
	while (1){
		// while left is less than pivot
		while (_compare(&root_vec->at(lhs)->int_node_vec, &root_vec->at(pivot)->int_node_vec) == RRESULT::LESS){
			++lhs;
		}

		// while right is greater than pivot
		while (_compare(&root_vec->at(rhs)->int_node_vec, &root_vec->at(pivot)->int_node_vec) == RRESULT::GREATER && rhs > 0){
			--rhs;
		}

		// terminal => swap lhs and pivot
		if (lhs >= rhs){
			temp = root_vec->at(lhs);
			root_vec->at(lhs) = root_vec->at(pivot);
			root_vec->at(pivot) = temp;
			break;

		} else {
			// if lptr & rptr are equal => don't swap (infinite loop)
			if (_compare(&root_vec->at(lhs)->int_node_vec, &root_vec->at(rhs)->int_node_vec) == RRESULT::EQUAL){
				++lhs;
				--rhs;
			// swap left and right nodes
			} else {
				temp = root_vec->at(lhs);
				root_vec->at(lhs) = root_vec->at(rhs);
				root_vec->at(rhs) = temp;
			}
		}
	}
	

	// check to see if lptr (prev. pivot) is == to divider packs
	// [[2]] || [[6]]
	// if root has only 1 element and is a list ...
	VECPAIR* t_lhs = &root_vec->at(lhs)->int_node_vec;
	if (t_lhs -> size() == 1 && t_lhs -> at(0).first == -1){
		// and that list value == 2 || 6
		VECPAIR *v = &t_lhs->at(0).second->int_node_vec;

		// if vector has exactly one value && it's not a list
		// pointer arithmatic == FOOT GUN DOT COM
		if (v->size() == 1 && v->at(0).first > -1){
			std::clog << "HIT -> " << lhs << '\n';
			if (v->at(0).first == 2) *div2_idx = lhs + 1;
			else if (v->at(0).first == 6) *div6_idx = lhs + 1;
		}
		
		// if both dividers have been placed in final spot
		// stop sorting
		if (*div2_idx > -1 && *div6_idx > -1) return;
	}

	// handle recusive calls
	if (lhs > 0){
		std::size_t new_ub= lhs - 1;
		quicksortSequences(lb, new_ub, root_vec, div2_idx, div6_idx);
	}


	if (lhs < root_vec->size()-1){
		std::size_t new_lb = lhs + 1;
		quicksortSequences(new_lb, ub, root_vec, div2_idx, div6_idx);
	}
	return;
}

std::size_t findDividers(std::vector<node*>* root_vec, const int val_to_search, int* div_idx){

	// linear search => the list is small && my case data found while sorting
	// already impl a quicksort on custom classess
	// could optimize later to be binary search
	std::cout << "Looking for " << val_to_search << " divider post-sort.\n";
	int pos {1}; // start at 1 as indexs in the current problem are 1-indexed
	for (node* node_ptr : *root_vec){
		VECPAIR *cur = &node_ptr->int_node_vec;
		if (cur -> size() == 1 && cur -> at(0).first == -1){

			VECPAIR *v = &cur -> at(0).second -> int_node_vec;

			// if vector has exactly one value && it's not a list
			if (v->size() == 1 && v->at(0).first > -1){
				if (v->at(0).first == val_to_search) *div_idx = pos;
				break;
			}
		}
		++pos;
	}
	return pos;
}

int main(){
	std::ifstream f {"c2data.txt"};	
	if (!!f){
		std::vector<node*> root_vec {};
		createNodes(f, &root_vec);
		f.close();
		
		// debugggin
		//printNodesWrapper(&root_vec);	

		// p1 solution
		std::cout << "[P1] Sum of indicies in correct order: "
			<< compareSequences(&root_vec) << '\n';


		// p2 => 1) sort entire vector of root nodes (ascending)
		// 2) find indexes of [[2]] and [[6]] values => get their product
		// quicksort => use _compare() to determine order
		// QUICKSORT => only sort until [[2]] and  [[6]] are pivots
		// once they've been pivots, we know they're in the correct position

		int div2_idx {-1}, div6_idx {-1};
		quicksortSequences(0,
					root_vec.size()-1,
					&root_vec, 
					&div2_idx,
					&div6_idx
					);

		//printNodesWrapper(&root_vec);

		if (div2_idx == -1) findDividers(&root_vec, 2, &div2_idx);
		if (div6_idx == -1) findDividers(&root_vec, 6, &div2_idx);

		std::cout << "div2 index: " << div2_idx << ", div6 index: " << div6_idx << '\n';
		std::cout << "Product of indices: " << div2_idx * div6_idx << '\n';

		// [PASSED sanitizer] dealloc heap used for nodes
		for (node* n : root_vec) rDeleteNodes(n);
	}

	return 0;
}
