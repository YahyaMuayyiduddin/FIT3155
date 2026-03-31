//
// Created by Yahya Muayyiduddin on 26/03/26.
//

#ifndef UKKONENS_UKKONEN_H
#define UKKONENS_UKKONEN_H
#include <vector>
#include <tuple>
#include <unordered_map>
#include <iostream>
#include <algorithm>

struct Node;

enum class Traversal_Endpoint {
    NO_REMAINDER,
    MID_EDGE,
    LAST_EXTENSION,
    END_EDGE
};

struct Edge {
    std::unique_ptr<Node> child;
    int suffix_start;
    int suffix_end;
    int* global_suffix_end;
    bool is_leaf;

    Edge() = default;

    Edge(int start, int end, std::unique_ptr<Node> new_child, int* global_end, bool leaf)  {
        this->suffix_start = start;
        this->suffix_end = end;
        this->child = std::move(new_child);
        this->global_suffix_end = global_end;
        this->is_leaf = leaf;
    }

    void set_child(std::unique_ptr<Node> new_child) {
        this->child = std::move(new_child);
    }

    int get_end(){
        return is_leaf ? *global_suffix_end : suffix_end;
    }

    void set_leaf(){
        this->is_leaf = true;
    }
    void set_non_leaf(){
        this->is_leaf = false;
    }
};

struct Node {
    std::unordered_map<char, std::unique_ptr<Edge>> children;
    Node* suffix_link = nullptr;
    int depth;
    int no;


    void add_edge(int suffix_start, int suffix_end, char first_character, int* global_end, bool is_leaf) {
        children[first_character] = std::make_unique<Edge>(
                suffix_start, suffix_end, std::make_unique<Node>(), global_end, is_leaf);
    }

    void add_edge(int suffix_start, int suffix_end, char first_character, std::unique_ptr<Node> new_child, int* global_end, bool is_leaf) {
        children[first_character] = std::make_unique<Edge>(
                suffix_start, suffix_end, std::move(new_child), global_end, is_leaf);
    }

    bool has_edge(char key) const {
        return this->children.contains(key);
    }

    Edge* get_edge(char key) {
        return this->children.at(key).get();
    }
};

struct Traversal_Result {
    Edge* last_traversed_edge;
    Node* last_traversed_node;
    int edge_start;
    int edge_end;
    int remainder_index_start;
    Traversal_Endpoint endpoint;
};


class Ukkonen_Suffix_Tree {
public:
    Node root{};
    int GLOBAL_END = -1;
    bool print_rules = false;
    std::string string{};


    Ukkonen_Suffix_Tree(std::string& input, bool print) {
        string = input;
        print_rules = print;
        root = Node{};
        root.suffix_link = &root;
        int last_leaf_index = -1;
        Node* last_created_node = nullptr;
        Node* active_node = &root;
        int remainder_starting_index = 0;
        for (int phase = 0; phase < input.size(); phase++) {
            GLOBAL_END++;

            last_created_node = nullptr;
            for (int j = last_leaf_index + 1; j <= phase; j++) {
                auto [last_traversed_edge,
                        last_traversed_node,
                        edge_start,
                        edge_end,
                        new_remainder_index_start,
                        traversal_end] =
                        find_endpoint(input, phase, active_node, remainder_starting_index, j);
                // Case J == Phase
                if (traversal_end == Traversal_Endpoint::LAST_EXTENSION) {
                    // Rule 3
                    if (last_traversed_node->has_edge(input[phase])) {
                        resolve_suffix_links(last_created_node, &root);
                        last_created_node = nullptr;
                        active_node = last_traversed_node;
                        remainder_starting_index = j;
                        break;
                    }
                    // Rule 2 alternate
                    else {
                        last_traversed_node->add_edge(phase, phase, input[phase], &GLOBAL_END, true);
                        resolve_suffix_links(last_created_node, &root);
                        last_created_node = nullptr;
                        remainder_starting_index = j + 1;
                        last_leaf_index = j;
                        active_node = last_traversed_node;
                    }
                }

                // Case where we have 0 remainder, Just need to immediately apply rule
                else if(traversal_end == Traversal_Endpoint::NO_REMAINDER) {


                    // Rule 3 end of edge
                    if (last_traversed_node->has_edge(input[phase])) {
                        resolve_suffix_links(last_created_node, last_traversed_node);
                        last_created_node = nullptr;
                        remainder_starting_index = phase;
                        active_node = last_traversed_node;

                        break;
                    }
                    // Rule 2 alternate
                    else if (!last_traversed_node->has_edge(input[phase])){
                        last_traversed_node->add_edge(phase, phase, input[phase], &GLOBAL_END, true);
                        resolve_suffix_links(last_created_node, last_traversed_node);
                        last_created_node = nullptr;
                        remainder_starting_index = phase;
                        last_leaf_index = j;
                        active_node = last_traversed_node->suffix_link;


                    }
                }
                // Traverse through an edge, and end up at its end before applying rule
                else if (traversal_end == Traversal_Endpoint::END_EDGE) {

                    // Rule 3
                    if (last_traversed_edge->child->has_edge(input[phase])) {
                        resolve_suffix_links(last_created_node, last_traversed_edge->child.get());
                        last_created_node = nullptr;
                        remainder_starting_index = phase;
                        active_node = last_traversed_edge->child.get();
                        break;
                    }
                    // Rule 2 alternate
                    else {
                        last_traversed_edge->child->add_edge(phase, phase, input[phase], &GLOBAL_END, true);
                        resolve_suffix_links(last_created_node, last_traversed_edge->child.get());
                        last_created_node = nullptr;
                        remainder_starting_index = phase;
                        last_leaf_index = j;
                        active_node = last_traversed_edge->child->suffix_link;


                    }
                }
                else if (traversal_end == Traversal_Endpoint::MID_EDGE) {
                    // Rule 2 regular
                    if (input[edge_end+1] != input[phase]){

                        auto old_child = std::move(last_traversed_edge->child);
                        Edge* old_edge = last_traversed_edge;
                        int old_start = last_traversed_edge->suffix_start;
                        int old_end =  last_traversed_edge->get_end();
                        bool old_edge_is_leaf = old_edge->is_leaf;

                        // Create new internal node
                        std::unique_ptr<Node> new_internal_node = std::make_unique<Node>();
                        Node* new_internal_node_ptr = new_internal_node.get();
                        old_edge->set_child(std::move(new_internal_node));
                        old_edge->suffix_end = edge_end;
                        old_edge->set_non_leaf();

                        // Add new edges to the new_internal_node
                        new_internal_node_ptr->add_edge(edge_end+1, old_end, input[edge_end+1], std::move(old_child), &GLOBAL_END, old_edge_is_leaf);
                        new_internal_node_ptr->add_edge(phase, phase, input[phase], &GLOBAL_END, true);

                        last_leaf_index = j;

                        resolve_suffix_links(last_created_node, new_internal_node_ptr);
                        last_created_node = new_internal_node_ptr;

                        // Setup next extension
                        active_node = last_traversed_node->suffix_link;
                        remainder_starting_index = new_remainder_index_start;

                        // Special case when active node is the root. The root's suffix link is itself, so we have to manually shorten the remainder by 1
                        if (last_traversed_node == &root) {
                            remainder_starting_index = j + 1;
                        }


                    } else {
                        // Rule 3
                        resolve_suffix_links(last_created_node, last_traversed_node);
                        last_created_node = nullptr;
                        remainder_starting_index = new_remainder_index_start;
                        active_node = last_traversed_node;
                        break;
                    }
                }

            }
        }
    }

    void resolve_suffix_links(Node* origin_node, Node* target_node){
        if (origin_node != nullptr) {
            origin_node->suffix_link = target_node;
            return;
        }
    }

    Traversal_Result find_endpoint(std::string& input, int phase, Node* active_node, int suffix_start_index, int j){
        int target = phase - 1;
        int remainder_length = target - suffix_start_index + 1;
        // j == phase
        if (j == phase) {
            return {nullptr, active_node, -1, -1, -1, Traversal_Endpoint::LAST_EXTENSION};
        }
        // No remainder
        if (remainder_length == 0) {
            return {nullptr, active_node, -1, -1, phase, Traversal_Endpoint::NO_REMAINDER};
        }

        int current_pos = suffix_start_index;
        Edge* current_edge = active_node->get_edge(input[current_pos]);
        Node* current_node = active_node;

        int edge_length = (current_edge->get_end() - current_edge->suffix_start + 1);
        int new_remainder = remainder_length - edge_length;

        if (new_remainder == 0 ){
            return {current_edge, current_node, current_edge->suffix_start, current_edge->get_end(), target - remainder_length + 1 , Traversal_Endpoint::END_EDGE};
        }
        if (new_remainder < 0) {
            return {current_edge, current_node, current_edge->suffix_start, current_edge->suffix_start + remainder_length - 1, target - remainder_length + 1 , Traversal_Endpoint::MID_EDGE};
        }
        else {
            return traverse_suffix_tree(input, phase, active_node, suffix_start_index, j);
        }
        return traverse_suffix_tree(input, phase, active_node, suffix_start_index, j);


    }

    Traversal_Result traverse_suffix_tree(std::string& input, int phase, Node* active_node, int suffix_start_index, int j) {
        int target = phase - 1;
        int remainder_length = target - suffix_start_index + 1;
        // j == phase
        if (j == phase) {
            // Mid edge stop
            std:: cout << "shouldn't happen" << '\n';
            return {nullptr, active_node, -1, -1, -1};
        }
        int current_pos = suffix_start_index;
        Edge* current_edge = active_node->get_edge(input[current_pos]);
        Node* current_node = active_node;

         while (remainder_length > 0){
             int edge_length = (current_edge->get_end() - current_edge->suffix_start + 1);
             int new_remainder = remainder_length - edge_length;
             if (new_remainder < 0) {

                 return {current_edge, current_node, current_edge->suffix_start, current_edge->suffix_start + remainder_length - 1, target - remainder_length + 1, Traversal_Endpoint::MID_EDGE};
             }
             if (new_remainder == 0){
                 return {current_edge, current_node, current_edge->suffix_start, current_edge->get_end() , phase , Traversal_Endpoint::END_EDGE};

             } else {
                 current_node = current_edge->child.get();
                 current_pos += edge_length;
                 current_edge = current_node->get_edge(input[current_pos]);
                 remainder_length = new_remainder;
             }
         }

    }

    template<typename F>
    void level_order_traverse(const  F&function, Node* node){
        if (node->children.empty()){
            function(node);
            return;
        } else {
            function(node);
            for (auto & child: node->children){
                level_order_traverse(function, child.second->child.get());
            }
        }

    }

    void print_suffix_tree(std::string &input){
        int index = 0;
        assign_numbers(&root, index, 0);
        const auto print = [input](Node* node){
            std::cout << "Number: "<< node->no << '\n';
            std::cout << "Depth: "<< node->depth << '\n';
            if (node->suffix_link != nullptr) {
                std::cout << "Suffix Link: " << node->suffix_link->no << '\n';
            } else {
                std::cout << "Leaf" << '\n';
            }
            for (auto &child: node->children){
                std::cout << "Edge to: " << child.second->child->no << " Edge: " << input.substr(child.second->suffix_start, child.second->get_end() - child.second->suffix_start + 1)<< '\n';
            }
        };
        level_order_traverse(print, &root);
    }



    void assign_numbers(Node* node, int& number, int depth){
        if (node->children.empty()){
            node->no = number;
            node->depth = depth;
            number ++ ;
            return;
        }
        node->no = number;
        node->depth = depth;

        number ++;
        for(auto &child :node->children){
            assign_numbers(child.second->child.get(), number, depth+1);
        }
    }



    auto get_suffixes(std::string& input) {
        auto array = std::make_unique<std::vector<std::string>>();
        get_suffixes_aux(&root, input, "", array.get());
        return array;
    }


    void get_suffixes_aux(Node* node, std::string& input, const std::string& current, std::vector<std::string>* suffixes) {
        if (node->children.empty()) {
            suffixes->push_back(current);
            return;
        }
        for (auto& [c, edge] : node->children) {
            std::string label = input.substr(edge->suffix_start,
                                             edge->get_end() - edge->suffix_start + 1);
            get_suffixes_aux(edge->child.get(), input, current + label, suffixes);
        }
    }
    void print_suffixes() {
        auto suffixes = get_suffixes(string);
        const auto sort_func = [](std::string string1, std::string string2) {
            return string1.size() < string2.size();
        };
        std::sort(suffixes->begin(), suffixes->end(), sort_func);
        for (auto &suffix: *suffixes) {
            std::cout << suffix << '\n';
        }

    }
};

#endif //UKKONENS_UKKONEN_H




