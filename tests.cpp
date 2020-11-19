#include <fstream>
#include <iostream>
#include "solution.h"

int main() {
    std::ifstream tests_file("../tests.txt");
    if (!tests_file.is_open()) {
        tests_file.open("tests.txt");
    }
    std::string regexp;
    char x;
    int k;
    std::string answer;

    while (tests_file >> regexp >> x >> k >> answer) {
        Stack stack(x, k);
        stack.ParseRegexp(regexp);
        auto result = stack.ShowResult();
        if (result != answer) {
            std::cout << "FAILED TEST:    " << regexp << " " << x << " " << k << '\n';
            std::cout << "RESULT:         " << result << '\n';
            std::cout << "CORRECT RESULT: " << answer << '\n' << '\n';
        } else {
            std::cout << "PASSED TEST: " << regexp << " " << x << " " << answer << '\n' << '\n';
        }
    }
    return 0;
}
