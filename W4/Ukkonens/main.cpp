#include <iostream>
#include "ukkonen.h"// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
#include <random>
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
    std::string input2 = "thequickbrownfoxjumpsoverthelazydog$";
    Ukkonen_Suffix_Tree suffix_tree(input2, true);

//    run_all_tests();

    int N = 100000;
    std::string massive_input = "";
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::mt19937 engine(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<int> dist(0, 25);

    for(int i = 0; i < N; i++) {
        massive_input += alphabet[dist(engine)];
    }

    auto start = std::chrono::high_resolution_clock::now();

    Ukkonen_Suffix_Tree tree(massive_input, false);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "DONE!" << std::endl;
    std::cout << "Construction time: " << diff.count() << " seconds" << std::endl;


    start = std::chrono::high_resolution_clock::now();


    end = std::chrono::high_resolution_clock::now();
    diff = end - start;

    std::cout << "DONE!" << std::endl;
    std::cout << "Construction time: " << diff.count() << " seconds" << std::endl;







    return 0;
}

