//
// Created by Yahya Muayyiduddin on 26/03/26.
//

#ifndef UKKONENS_UKKONEN_H
#define UKKONENS_UKKONEN_H
#include <vector>
#include <tuple>
#include <unordered_map>
#include <iostream>


class Node;

struct Edge {
public:
    Node* child;
    int start;
    int end;
    Edge (int start, int end, Node* child) {
        this->start = start;
        this->end = end;
        this->child = child;
    }
    Edge(){}

};


class Node {
public:
    std::unordered_map<char, Edge*> edges;
    int suffix_index;
    Node (int suffix_index) {
        this->suffix_index = suffix_index;
    }

    Node (){}

    void add_edge(int start, int end, std::string& string, int leaf_index){
        edges[string.at(start)] = new Edge(start, end, new Node(leaf_index));
    }
    void add_edge(int start, int end, std::string& string, Node* new_child){
        edges[string.at(start)] = new Edge(start, end, new_child);
    }
};

struct Traverse_Result{
    Node* last_node;
    Edge* last_edge;
    int last_match_index;
    bool full_suffix_match;
};



class Ukkonen {
public:
    Node root;

    explicit Ukkonen (std::string input) {
        construct_suffix_tree(input);

    }

    void construct_suffix_tree(std::string& input){


        for (int phase = 0; phase < input.length();phase++){
            Node* current_node = &this->root;
            std::cout << "Phase " << phase <<'\n';

            for (int j = 0; j <= phase; j++) {
                std::cout << "J " << j <<'\n';

                auto [last_node, last_edge, last_match_index, full_match] = traverse_tree(current_node, j, phase, input);
                if (last_edge == nullptr){
                    if (!last_node->edges.contains(input[phase])){
                        last_node->add_edge(j, phase, input, j);
                    } else {
                        // Rule 3, do nothing
                    }
                }
                // Rule 3
                else if (last_edge->end == last_match_index && last_edge->child->edges.contains(input[phase])){
//                    do nothing
                    std::cout << "Rule 3" << '\n';
                    break;
                }
                // Rule 1
                else if (last_edge->end == last_match_index && last_edge->child->edges.empty()){
                    last_edge->end = phase;
                    std::cout << "Rule 1" << '\n';


                }

                // Rule 2 new child
                else if (last_edge->end == last_match_index && !last_edge->child->edges.contains(input[phase])){
                    last_edge->child->add_edge(phase, phase, input, j);
                    std::cout << "Rule 2 new child" << '\n';
//
                }
                //Rule 2 split in edge
                else if (last_edge->end > last_match_index && input[phase] != input[last_match_index+1]){
                    std::cout << "Rule 2 split" << '\n';
                    std::cout << input[last_match_index] << '\n';
                    int k = last_match_index;
                    int old_end = last_edge->end;
                    Node* old_edge_child = last_edge->child;
                    Edge* old_edge = last_edge;
                    Node* new_internal_node = new Node(j);
                    old_edge->child = new_internal_node;
                    old_edge->end = k;
                    new_internal_node->add_edge(k + 1, old_end, input, old_edge_child);
                    new_internal_node->add_edge(phase, phase, input, j);
                } else {
                    std::cout << "Rule 3" << '\n';
                    break;

                }


            }

        }

    }

    /**
     * Traverses the suffix tree until it reaches a point when a rule must be applied. Returns the last active node
     * and last index before rule was applied.
     * @param node
     * @param suffix_index
     * @param end
     * @param string
     * @return Traverse_Result struct with the results of the traversal
     */
    Traverse_Result traverse_tree(Node* node, int suffix_index , int end, std::string& string){
        int current = suffix_index;
        int tree_index;
        Node* current_node = node;
        Edge* current_edge;
        bool mid_edge = false;

        if (!current_node->edges.contains(string.at(suffix_index))){
            std::cout << "no path from root" << '\n';

            return {current_node, nullptr, -1, false};
        }
        if (end == suffix_index){
            std::cout << "start" << '\n';
            return {current_node, nullptr, -1, false};
        }
        current_edge = current_node->edges.at(string.at(current));
        tree_index = current_edge -> start;
        while ( current < end ){
            if (current_edge->end == tree_index){
                if (current == end-1){
                    return {current_node, current_edge, tree_index, false};
                }

                current_node = current_edge->child;
                current_edge = current_node->edges.at(string[current + 1]);
                tree_index = current_edge->start;
                mid_edge = false;

            } else {
                tree_index++;
                mid_edge = true;
            }
            current++;

        }
        return {current_node, current_edge, tree_index-1 , false};

    }


};

#endif //UKKONENS_UKKONEN_H
