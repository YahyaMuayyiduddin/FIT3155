#include <iostream>
#include "ukkonen.h"// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    std::string input = "abcabca";
    Ukkonen suffix_tree{input};

    std::cout << suffix_tree.root.edges['a']->start<< suffix_tree.root.edges['a']->end << std::endl;
    std::cout << suffix_tree.root.edges['b']->start<< suffix_tree.root.edges['b']->end << std::endl;

//    std::cout << suffix_tree.root.edges['c']->start << suffix_tree.root.edges['c']->end << std::endl;
//    std::cout << suffix_tree.root.edges['d']->start << suffix_tree.root.edges['d']->end << std::endl;


    return 0;
}