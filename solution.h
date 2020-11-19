#ifndef FORMALS_SOLUTION_H
#define FORMALS_SOLUTION_H

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <stdexcept>

const int kBigInt = std::numeric_limits<int>::max() / 2;

class Stack {
    struct State {
        std::vector<int> lengths;
        explicit State(int k) : lengths(k + 1, kBigInt) { }
    };

    bool is_bad_regexp = false;
    std::stack<State> stack;
    const int state_size;
    const char required_symbol;

    static void ShowState(State& state, char c);
    State GetState();
public:
    Stack(char x, int k) : required_symbol(x), state_size(k) {}

    void PushAlpha(char c);
    void PushEpsilon();
    void Concatenate();
    void Addition();
    void StarOperation();
    std::string ShowResult();
    void ParseRegexp(const std::string& regexp);
};

#endif //FORMALS_SOLUTION_H
