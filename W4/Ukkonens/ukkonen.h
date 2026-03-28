//
// Created by Yahya Muayyiduddin on 26/03/26.
//

#ifndef UKKONENS_UKKONEN_H
#define UKKONENS_UKKONEN_H
#include <vector>
#include <tuple>
#include <unordered_map>
#include <iostream>

struct Node;

struct Edge {
    std::unique_ptr<Node> child;
    int suffix_start;
    int suffix_end;

    Edge() = default;

    Edge(int start, int end, std::unique_ptr<Node> new_child) {
        this->suffix_start = start;
        this->suffix_end = end;
        this->child = std::move(new_child);
    }
};

struct Node {
    std::unordered_map<char, std::unique_ptr<Edge>> children;
    Node* suffix_link;

    void add_edge(int suffix_start, int suffix_end, char first_character) {
        children[first_character] = std::make_unique<Edge>(
                suffix_start, suffix_end, std::make_unique<Node>());
    }

    void add_edge(int suffix_start, int suffix_end, char first_character, std::unique_ptr<Node> new_child) {
        children[first_character] = std::make_unique<Edge>(
                suffix_start, suffix_end, std::move(new_child));
    }

    bool has_edge(char key) {
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
};


class Ukkonen_Suffix_Tree {
    Node root;

    Ukkonen_Suffix_Tree(std::string& input) {
        root = Node{};
        root.suffix_link = &root;

        for (int phase = 0; phase < input.size(); phase++) {
            Node* active_node = &root;
            int remainder_starting_index = 0;

            for (int j = 0; j <= phase; j++) {
                auto [last_traversed_edge,
                        last_traversed_node,
                        edge_start,
                        edge_end,
                        new_remainder_index_start] =
                        traverse_suffix_tree(input, phase, active_node, remainder_starting_index, j);

                // TODO: apply Ukkonen rules based on traversal result
                if (last_traversed_edge == nullptr) {
                    if (!active_node->has_edge(input[j])){
                        active_node->add_edge(j, phase, input[j]);

                    }
                else if (){

                }
                }
            }
        }
    }


    Traversal_Result traverse_suffix_tree(std::string& input, int phase, Node* active_node, int suffix_start_index, int j) {
        // TODO: implement traversal logic
        int target = phase - 1;
        int remainder_length = target - suffix_start_index + 1;
        // j == phase
        if (j == phase) {
            return {nullptr, active_node, -1, -1, -1};
        }
        Edge* current_edge = active_node->get_edge(input[suffix_start_index]);
        Node* current_node = active_node;

         while (remainder_length > 0){
             int edge_length = (current_edge->suffix_end - current_edge->suffix_start + 1);
             int new_remainder = remainder_length - edge_length;
             if (new_remainder < 0) {
                 // Suffix fully traversed, no remainder
                 return {current_edge, current_node, current_edge->suffix_start, current_edge->suffix_start + remainder_length - 1, target - remainder_length + 1 };
             }
             if (new_remainder == 0){
                 return {current_edge, current_node, current_edge->suffix_start, current_edge->suffix_end , -1 };

             } else {
                 current_node = current_edge->child.get();
                 current_edge = current_node->get_edge(input[suffix_start_index + edge_length]);
                 remainder_length = new_remainder;
             }
         }

    }
};

#endif //UKKONENS_UKKONEN_H
