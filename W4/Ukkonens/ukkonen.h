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
    Node* suffix_link;
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
        root = Node{};
        root.suffix_link = &root;
        construct_suffix_tree(input);

    }

    void construct_suffix_tree(std::string& input){

        bool internal_node_created = false;
        int remainder_index;

        for (int phase = 0; phase < input.length();phase++){
            Node* current_node = &this->root;
            std::cout << "Phase " << phase <<'\n';

            internal_node_created = false;

            for (int j = 0; j <= phase; j++) {
                std::cout << "J " << j <<'\n';

                auto [last_node, last_edge, last_match_index, full_match] = traverse_tree(current_node, j, phase, input);
                if (last_edge == nullptr){
                    if (!last_node->edges.contains(input[phase])){
                        last_node->add_edge(j, phase, input, j);
                        last_node->edges.at(input[phase])->child->suffix_link = &root;
                    } else {
                        // Rule 3, do nothing
                        break;
                    }
                    internal_node_created = false;
                }
                // Rule 3 at the end of an edge
                else if (last_edge->end == last_match_index && last_edge->child->edges.contains(input[phase])){
//                    do nothing
                    std::cout << "Rule 3" << '\n';
                    break;
                    if (phase > 0) {

                    }
                }
                // Rule 1
                else if (last_edge->end == last_match_index && last_edge->child->edges.empty()){
                    last_edge->end = phase;
                    std::cout << "Rule 1" << '\n';


                }

                // Rule 2 new child
                else if (last_edge->end == last_match_index && !last_edge->child->edges.contains(input[phase])){
                    last_edge->child->add_edge(phase, phase, input, j);
                    last_edge->child->edges.at(input[phase])->child->suffix_link = &root;
                    std::cout << "Rule 2 new child" << '\n';

                    if (internal_node_created) {
                        current_node->suffix_link = last_node;;
                    }
                    current_node = last_node->suffix_link;



                    internal_node_created = true;


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
                    new_internal_node->edges.at(input[phase])->child->suffix_link = &root;

                    if (internal_node_created) {
                        current_node->suffix_link = new_internal_node;
                    }
                    current_node = last_node->suffix_link;

                    internal_node_created = true;

                } else {
                    std::cout << "fuck you" << '\n';
                    break;
                    internal_node_created = false;


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
        Node* current_node = node;
        Edge* current_edge;
        int remainder = (end-1) - suffix_index + 1;


        if (!current_node->edges.contains(string.at(suffix_index))){
            std::cout << "no path from root" << '\n';

            return {current_node, nullptr, -1};
        }
        if (end == suffix_index){
            std::cout << "start" << '\n';
            return {current_node, nullptr, -1};
        }
        current_edge = current_node->edges.at(string.at(current));

        while (remainder > 0){
            int skip_count = current_edge-> end - current_edge->start + 1;

            // We can cross the child
            if (remainder > skip_count){
                current_node = current_edge->child;
                current_edge = current_node->edges.at(string[current + skip_count]);
                remainder -= skip_count;
                current += skip_count;

            }
            // We stop mid-edge
            else if (remainder < skip_count) {
                return {current_node, current_edge, current_edge->start + remainder - 1};

            } else {
//              Stop at the end of an edge
                return {current_node, current_edge, current_edge->end};
            }



        }


    }


};

#endif //UKKONENS_UKKONEN_H
