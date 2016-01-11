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
    else if (*it == '-' || *it == '+') {
        if (elem.length() == 2) {
            return false;
        }
        else {
            ++it;
        }
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

static bool is_bool(const string& elem) {
    return elem == "#t" || elem == "#f" || elem == "#T" || elem == "#F";
}

/**
 * convert boolean str to c++ boolean value
 * no error check
 */
static bool stob(const string& elem) {
    if (elem == "#t" || elem == "#T") { return true; }
    else { return false; }
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
    if (it == tokens.end()) {
        return nil;
    }
    else if (*it == "(") {
        return _parse_tail(tokens, ++it);
    }
    else if (*it == "'") {
        cell_ptr ret = _parse(tokens, ++it);
        return cons(make_symbol("quote"), cons(ret, nil));            
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
    else if (is_bool(*it)) {
        cell_ptr ret = make_bool(stob(*it));
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
        throw runtime_error("invalid s-expression or more than one s-expression");
    }
    else {
        return ret;
    }
}

static int read_console(string& sexpr) {
    getline(cin, sexpr);
    if (cin.eof()) {
        cout << endl;
        throw logic_error("keyboard interrupt");
    }
    else if (sexpr.empty()) {
        return -1;
    }
    else {
        return 0;
    }
}

/**
 * tokenize sexpr, push the parsed tokens to the tokens vector
 * \param count, count the unmatch parentheses
 */
static int expr_to_vector(string& sexpr, vector<string>& tokens, int count) {
    auto it = sexpr.begin();
    auto extract_string = [&]() ->string {
        auto start = it;
        string ret;
        do {
            ++it;
            if (it == sexpr.end()) {
                ret += string(start, it);
                read_console(sexpr);
                start = it = sexpr.begin();
            }
        } while (*it != '"');
        // add tail to resutl
        ++it;
        ret += string(start, it);
        return ret;
    };
    auto extract_token = [&]() ->string {
        auto start = it;
        if (*it == '(' || *it == ')' || *it == '\'') {
            ++it;
            return string(start, it);
        }
        else {
            while (it != sexpr.end() && isspace(*it) == 0) {
                if (*it == '(' || *it == ')') {
                    return string(start, it);
                }
                else {
                    ++it;
                }
            }
            return string(start, it);
        }
    };
    string elem;
    while (it != sexpr.end()) {
        if (isspace(*it) == 0) {
            if (*it == '"') {
                elem = extract_string();
            }
            else {
                elem = extract_token();
            }
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
        if (read_console(sexpr) != -1 && *(sexpr.begin()) != ';') {
            // if expression on console is not null and not comment
            // tokenize the s-expression
            count = expr_to_vector(sexpr, ret, count);
        }
    } while (count != 0);
    return ret;
}