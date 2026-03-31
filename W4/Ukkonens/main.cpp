#include <iostream>
#include "ukkonen.h"// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

void test(std::string input) {
    Ukkonen_Suffix_Tree suffix_tree{input, false};
    auto suffixes = suffix_tree.get_suffixes(input);

    // check every suffix of input exists in the array
    for (int i = 0; i < input.size(); i++) {
        std::string expected = input.substr(i);
        bool found = std::find(suffixes->begin(), suffixes->end(), expected) != suffixes->end();
        if (!found) {
            std::cout << "FAIL [" << input << "]: missing suffix '" << expected << "'\n";
            return;
        }
    }
    // check no extra suffixes
    if (suffixes->size() != input.size()) {
        std::cout << "FAIL [" << input << "]: expected " << input.size()
                  << " suffixes, got " << suffixes->size() << "\n";
        return;
    }
    std::cout << "PASS [" << input << "]\n";
}

inline void run_all_tests() {
    // Basic
    test("a$");
    test("ab$");
    test("abc$");
    test("aab$");
    test("abb$");

    // Repeating characters
    test("aaaa$");
    test("aaaaaa$");
    test("aaabaa$");
    test("ababab$");
    test("abababab$");

    // Classic Ukkonen test strings
    test("abcabxabcd$");
    test("abcdeabck&");
    test("mississippi$");
    test("banana$");
    test("abcdef$");

    // Palindromes
    test("abacaba$");
    test("racecar$");
    test("amanaplanacanalpanama$");

    // All same character
    test("aaaaaaaa$");
    test("bbbbbbbb$");

    // Two character alternating
    test("ababababababab$");
    test("abbaabba$");
    test("aabbaabb$");

    // Fibonacci strings (stress test for suffix links)
    test("abaababaabaab$");
    test("abaababaabaababaab$");

    // Long non-repeating
    test("abcdefghijklmnopqrstuvwxyz$");
    test("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuv&");

    // Repeating then unique
    test("aaaaaab$");
    test("aaaaabc$");
    test("abcabcabcx$");
    test("abcabcabcy$");
    test("xyzxyzxyz$");

    // Overlap heavy
    test("aababcabcd$");
    test("abcdabcaba$");
    test("aabaacaabaa$");
    test("aabaabaab$");

    // Single unique terminator stress
    test("abcbcbcbc$");
    test("bcbcbcbca$");
    test("cbcbcbcba$");

    // Mixed patterns
    test("abacabadabacaba$");
    test("abcdeabcdabce$");
    test("aaabbbccc$");
    test("abcccbba$");

    // Longer strings
    test("thequickbrownfoxjumpsoverthelazydog$");
    test("aaabaaabaaab$");
    test("abababcababc$");
    test("aabcaabxaaaz$");
    test("abcdeabcdeabcde$");
    test("zzzzabcdzzzzabcd$");
}

int main() {
    std::string input = "aaabaa$";
    try {
        Ukkonen_Suffix_Tree suffix_tree(input, true);
        auto suffixes = suffix_tree.get_suffixes(input);
        for (auto &suffix: *suffixes) {
            std:: cout << suffix << '\n';
        }

        suffix_tree.print_suffix_tree(input);
        std::cout << suffixes->size() << '\n';
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }

//    for (auto& [c, edge] : suffix_tree.root.children) {
//        std::cout << "root -> '" << c << "' : "
//                  << input.substr(edge->suffix_start, edge->get_end() - edge->suffix_start + 1)
//                  << '\n';
//    }
    run_all_tests();







    return 0;
}

