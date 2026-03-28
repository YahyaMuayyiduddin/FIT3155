//
// Created by Yahya Muayyiduddin on 26/03/26.
//

#ifndef UKKONENS_UKKONEN_H
#define UKKONENS_UKKONEN_H
#include <vector>
#include <tuple>
#include <unordered_map>
#include <iostream>
#include <print>

struct Node;



struct Edge {
    std::unique_ptr<Node> child;
    int suffix_start;
    int suffix_end;
    int* global_suffix_end;

    Edge() = default;

    Edge(int start, int end, std::unique_ptr<Node> new_child, int* global_end)  {
        this->suffix_start = start;
        this->suffix_end = end;
        this->child = std::move(new_child);
        this->global_suffix_end = global_end;
    }

    void set_child(std::unique_ptr<Node> new_child) {
        this->child = std::move(new_child);
    }
};

struct Node {
    std::unordered_map<char, std::unique_ptr<Edge>> children;
    Node* suffix_link;


    void add_edge(int suffix_start, int suffix_end, char first_character, int* global_end) {
        children[first_character] = std::make_unique<Edge>(
                suffix_start, suffix_end, std::make_unique<Node>(), global_end);
    }

    void add_edge(int suffix_start, int suffix_end, char first_character, std::unique_ptr<Node> new_child, int* global_end) {
        children[first_character] = std::make_unique<Edge>(
                suffix_start, suffix_end, std::move(new_child), global_end);
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
public:
    Node root;
    int GLOBAL_END = -1;


    Ukkonen_Suffix_Tree(std::string& input) {
        root = Node{};
        root.suffix_link = &root;
        int last_leaf_index = -1;

        for (int phase = 0; phase < input.size(); phase++) {
            GLOBAL_END++;
            Node* active_node = &root;
            int remainder_starting_index = 0;

            for (int j = last_leaf_index + 1; j <= phase; j++) {
                auto [last_traversed_edge,
                        last_traversed_node,
                        edge_start,
                        edge_end,
                        new_remainder_index_start] =
                        traverse_suffix_tree(input, phase, active_node, j, j);

                // TODO: apply Ukkonen rules based on traversal result
                // Root
                if (last_traversed_edge == nullptr) {
                    // Rule 2 alternate
                    if (!active_node->has_edge(input[j])){
                        active_node->add_edge(j, phase, input[j], &GLOBAL_END);
                        last_leaf_index = j;

                    }
                    // Rule 3
                    else {
                        break;
                    }

                }

                    // Rule 1
                else if (edge_end == last_traversed_edge->suffix_end && last_traversed_edge->child->children.empty()){
                    last_traversed_edge->suffix_end = phase;
                    last_leaf_index = j;
                    std::cout << "Rule 1" << '\n';

                }

                // Rule 2 regular
                else if (edge_end < last_traversed_edge->suffix_end && input[edge_end+1] != input[phase]){

                    auto old_child = std::move(last_traversed_edge->child);
                    Edge* old_edge = last_traversed_edge;
                    int old_start = last_traversed_edge->suffix_start;
                    int old_end =  last_traversed_edge->suffix_end;

                    // Create new internal node
                    std::unique_ptr<Node> new_internal_node = std::make_unique<Node>();
                    Node* new_internal_node_ptr = new_internal_node.get();
                    old_edge->set_child(std::move(new_internal_node));
                    old_edge->suffix_end = edge_end;

                    // Add new edges to the new_internal_node
                    new_internal_node_ptr->add_edge(edge_end+1, old_end, input[edge_end+1], std::move(old_child), &GLOBAL_END);
                    new_internal_node_ptr->add_edge(phase, phase, input[phase], &GLOBAL_END);

                    last_leaf_index = j;




                }
                // Rule 2 alternate
                else if (edge_end == last_traversed_edge->suffix_end && !last_traversed_edge->child->has_edge(input[phase])){
                    last_traversed_edge->child->add_edge(phase, phase, input[phase], &GLOBAL_END);
                    last_leaf_index = j;

                }

                // Rule 3 end of edge
                else if (edge_end == last_traversed_edge->suffix_end && last_traversed_edge->child->has_edge(input[phase])){
                    std::cout << "Rule 3" << '\n';
                    break;
                }

                // Rule 3 mid edge
                else if (edge_end < last_traversed_edge->suffix_end && input[edge_end+1] == input[phase]){
                    std::cout << "Rule 3" << '\n';

                    break;
                }

            }
        }
    }


    Traversal_Result traverse_suffix_tree(std::string& input, int phase, Node* active_node, int suffix_start_index, int j) {

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
        return {nullptr, active_node, -1, -1, -1};
    }

    void print_tree_bfs(std::string& input) {
        std::queue<std::pair<Node*, int>> q;
        q.push({&root, 0});

        while (!q.empty()) {
            auto [node, depth] = q.front();
            q.pop();

            for (auto& [c, edge] : node->children) {
                std::string label = input.substr(edge->suffix_start,
                                                 edge->suffix_end - edge->suffix_start + 1);
                std::cout << std::string(depth * 2, ' ') << label << "\n";
                q.push({edge->child.get(), depth + 1});
            }
        }
    }
};

#endif //UKKONENS_UKKONEN_H
