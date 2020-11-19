#include <iostream>
#include "solution.h"

int main() {
    std::string regexp;
    char x;
    int k;
    std::cin >> regexp >> x >> k;

    Stack stack(x, k);
    stack.ParseRegexp(regexp);
    std::cout << stack.ShowResult() << '\n';
    return 0;
}
