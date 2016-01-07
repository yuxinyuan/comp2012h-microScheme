#include "parse.h"
#include <cctype>
#include <iostream>

using namespace std;

typedef shared_ptr<Cell> cell_ptr;

static bool is_int(const string& elem) {
    auto it = elem.begin();
    auto it_end = elem.end();
    if (elem.length() == 1 && isdigit(*it) == 0) {
        return false;
    }
    else if (!(*it == '-' || *it == '+' || isdigit(*it) != 0)) {
        return false;
    }
    else {
        ++it;
        while (it != it_end) {
            if (isdigit(*it) == 0) {
                return false;
            }
            else {
                ++it;
            }
        }
        return true;
    }
}

static bool is_double(const string& elem) {
    auto it = elem.begin();
    auto it_end = elem.end();
    if (elem.length() == 1) {
        // length of double must be greater than one.
        return false;
    }
    bool dot = false;
    while (it != it_end) {
        if (isdigit(*it) == 0 && *it != '.') {
            // invalid character.
            return false;
        }
        else if (*it == '.') {
            // dot in a double
            if (!dot) {
                dot = true;
                ++it;
            }
            else {
                return false;
            }
        }
        else {
            ++it;
        }
    }
    return true;
}

static cell_ptr _parse(const vector<string>& tokens, vector<string>::const_iterator& it);

/** 
 * parse the tail of a cons pair
 * \param it, iterator points to the first unparsed token in tokens
 */
static cell_ptr _parse_tail(const vector<string>& tokens, vector<string>::const_iterator& it) {
    // first token is car
    if (*it == ")") {
        ++it;
        return nil;
    }
    cell_ptr my_car = _parse(tokens, it);
    if (*it == ")") {
        // if next token is ")", then cdr is nil
        ++it;
        return cons(my_car, nil);
    }
    else if (*it == ".") {
        cell_ptr my_cdr = _parse(tokens, ++it);
        if (*it != ")") {
            throw runtime_error("invalid s-expression");
        }
        ++it;
        return cons(my_car, my_cdr);
    }
    else {
        // else the cdr is another tail of cons pair
        cell_ptr my_cdr = _parse_tail(tokens, it);
        return cons(my_car, my_cdr);
    }
}

/**
 * parse tokens into one s-expression
 * param it, iterator points to the first unparsed token in tokens
 */
static cell_ptr _parse(const vector<string>& tokens, vector<string>::const_iterator& it) {
    if (*it == "(") {
        return _parse_tail(tokens, ++it);
    }
    else if (is_int(*it)) {
        cell_ptr ret = make_int(stoi(*it));
        ++it;
        return ret;
    }
    else if (is_double(*it)) {
        cell_ptr ret = make_double(stod(*it));
        ++it;
        return ret;
    }
    else if (*it == ")") {
        throw runtime_error("invalid token ')'");
    }
    else {
        cell_ptr ret = make_symbol(*it);
        ++it;
        return ret;
    }
}

cell_ptr parse(const vector<string>& tokens) {
    auto it = tokens.begin();
    cell_ptr ret = _parse(tokens, it);
    if (it != tokens.end()) {
        throw runtime_error("invalid s-expression");
    }
    else {
        return ret;
    }
}

/**
 * tokenize sexpr, push the parsed tokens to the tokens vector
 * \param count, count the unmatch parentheses
 */
static int expr_to_vector(const string& sexpr, vector<string>& tokens, int count) {
    auto it = sexpr.begin();
    auto extract_elem = [&]() ->string {
                            if (*it == '(') {
                                ++it;
                                return "(";
                            }
                            else if (*it == ')') {
                                ++it;
                                return ")";
                            }
                            auto start = it;
                            while (it != sexpr.end() && isspace(*it) == 0) {
                                if (*it == '(') {
                                    return string(start, it);
                                }
                                else if (*it == ')') {
                                    return string(start, it);
                                }
                                else {
                                    ++it;
                                }
                            }
                            return string(start, it);
                        };

    string elem;
    while (it != sexpr.end()) {
        if (isspace(*it) == 0) {
            elem = extract_elem();
            if (elem == "(") {
                ++count;
            }
            else if (elem == ")") {
                --count;
            }
            tokens.push_back(elem);
        }
        else {
            ++it;
        }
    }
    return count;
}

vector<string> read_expr() {
    int count = 0;
    string sexpr;
    vector<string> ret;
    do {
        getline(cin, sexpr);
        if (cin.eof()) {
            cout << endl;
            throw logic_error("keyboard interrupt");
        }
        count = expr_to_vector(sexpr, ret, count);
    } while (count != 0);
    return ret;
}