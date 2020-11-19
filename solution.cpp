#include "solution.h"

//#define DEBUG
#ifdef DEBUG
    #define $DBG if (1)
#else
    #define $DBG if (0)
#endif


void Stack::ShowState(Stack::State &state, char c) {
    std::cout << c << ": ";
    for (auto x : state.lengths) {
        if (x == kBigInt) {
            std::cout << -1 << " ";
        } else {
            std::cout << x << " ";
        }
    }
    std::cout << std::endl;
}

Stack::State Stack::GetState() {
    if (stack.empty()) {
        is_bad_regexp = true;
        throw std::invalid_argument{""};
    }
    State state = stack.top();
    stack.pop();
    return state;
}

void Stack::PushAlpha(char c) {
    State state(state_size);
    state.lengths[0] = 1;
    if (state_size >= 1 && c == required_symbol) {
        state.lengths[1] = 0;
    }
    $DBG ShowState(state, c);
    stack.push(std::move(state));
}

void Stack::PushEpsilon() {
    State state(state_size);
    state.lengths[0] = 0;
    $DBG ShowState(state, '1');
    stack.push(std::move(state));
}

void Stack::Concatenate() {
    State rhs_state = GetState();
    State lhs_state = GetState();

    State result_state(state_size);
    for (int i = 0; i <= state_size; ++i) {
        for (int j = 0; j <= i; ++j) {
            if (lhs_state.lengths[j] == 0) {
                if (result_state.lengths[i] > rhs_state.lengths[i - j]) {
                    result_state.lengths[i] = rhs_state.lengths[i - j];
                    for (int l = i - 1; l >= 0; --l) {
                        result_state.lengths[l] = std::min(result_state.lengths[l], result_state.lengths[i] + (i - l));
                    }
                }
            } else {
                if (result_state.lengths[j] > lhs_state.lengths[j] + (i - j) + rhs_state.lengths[i - j]) {
                    result_state.lengths[j] = lhs_state.lengths[j] + (i - j) + rhs_state.lengths[i - j];
                    for (int l = i - 1; l >= 0; --l) {
                        result_state.lengths[l] = std::min(result_state.lengths[l], result_state.lengths[i] + (i - l));
                    }
                }
            }
        }
    }
    $DBG ShowState(result_state, '.');
    stack.push(std::move(result_state));
}

void Stack::Addition() {
    State rhs_state = GetState();
    State lhs_state = GetState();

    State result_state(state_size);
    for (int i = 0; i <= state_size; ++i) {
        result_state.lengths[i] = std::min(lhs_state.lengths[i], rhs_state.lengths[i]);
    }
    $DBG ShowState(result_state, '+');
    stack.push(std::move(result_state));
}

void Stack::StarOperation() {
    State current_state = GetState();

    State result_state(state_size);
    result_state.lengths[0] = 0;
    for (int i = 1; i <= state_size; ++i) {
        if (current_state.lengths[i] == 0) {
            result_state.lengths[i] = 0;
        } else {
            for (int j = 0; j <= i / 2; ++j) {
                if (result_state.lengths[j] == 0 && result_state.lengths[i - j] == 0) {
                    result_state.lengths[i] = 0;
                    for (int l = i - 1; l >= 0; --l) {
                        result_state.lengths[l] = std::min(result_state.lengths[l], result_state.lengths[i] + (i - l));
                    }
                }
            }
        }
    }
    for (int i = 0; i <= state_size; ++i) {
        for (int j = 0; j <= i; ++j) {
            if (result_state.lengths[j] == 0) {
                if (result_state.lengths[i] > current_state.lengths[i - j]) {
                    result_state.lengths[i] = current_state.lengths[i - j];
                    for (int l = i - 1; l >= 0; --l) {
                        result_state.lengths[l] = std::min(result_state.lengths[l], result_state.lengths[i] + (i - l));
                    }
                }
            }
        }
    }
    $DBG ShowState(result_state, '*');
    stack.push(std::move(result_state));
}

std::string Stack::ShowResult() {
    if (is_bad_regexp) {
        return "ERROR";
    }
    try {
        State result = GetState();
        $DBG std::cout << "________" << std::endl;
        $DBG ShowState(result, ' ');
        if (result.lengths[state_size] == kBigInt) {
            return "INF";
        } else {
            return std::to_string(state_size + result.lengths[state_size]);
        }
    } catch (...) {
        return "ERROR";
    }
}

void Stack::ParseRegexp(const std::string &regexp) {
    for (auto c : regexp) {
        try {
            if (isalpha(c)) {
                PushAlpha(c);
            } else if (isdigit(c)) {
                PushEpsilon();
            } else if (c == '.') {
                Concatenate();
            } else if (c == '+') {
                Addition();
            } else if (c == '*') {
                StarOperation();
            }
        } catch (...) { }
    }
}
