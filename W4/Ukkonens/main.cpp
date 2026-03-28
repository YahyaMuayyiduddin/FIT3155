#include <iostream>
#include "ukkonen.h"// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
#include <vector>
#include <algorithm>






int main() {
    std::string input = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuv&";
    Ukkonen_Suffix_Tree suffix_tree{input};

//    suffix_tree.print_suffixes(&suffix_tree.root, input, "");
    auto suffixes = suffix_tree.get_suffixes(input);
    std::sort(suffixes->begin(), suffixes->end(), [](const std::string &a, const std::string &b){
        return a.length() < b.length();
    });
    for (auto &suffix: *suffixes){
        std::cout << suffix <<'\n';
    }
    std::cout << suffixes->size() <<'\n';
    std::cout << input.size() << '\n';

//    suffix_tree.print_tree_bfs(input);
//


//    std::cout << suffix_tree.root.edges['c']->start << suffix_tree.root.edges['c']->end << std::endl;
//    std::cout << suffix_tree.root.edges['d']->start << suffix_tree.root.edges['d']->end << std::endl;goo



    return 0;
}