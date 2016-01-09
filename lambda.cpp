#include "lambda.h"

using namespace std;

typedef     std::shared_ptr<Cell>               cell_ptr;
typedef     std::shared_ptr<frame>              frame_ptr;
typedef     decltype(cell_ptr (*)(cell_ptr))    func_t;

ProcedureCell::ProcedureCell(cell_ptr formals, cell_ptr body, frame_ptr env):
    formals_m(formals), body_m(body), env_m(env)
{}

string ProcedureCell::type() const {
    return "procedure";
}

string ProcedureCell::to_str() const {
    return "#<procedure>";
}

unsigned int ProcedureCell::length() const {
    return 0;
}

cell_ptr ProcedureCell::get_formals() const {
    return formals_m;
}

cell_ptr ProcedureCell::get_body() const {
    return body_m;
}

frame_ptr ProcedureCell::get_env() const {
    return env_m;
}

string PrimitiveCell::type() const {
    return "primitive";
}

string PrimitiveCell::to_str() const {
    return "#<primitive function>";
}

unsigned int PrimitiveCell::length() const {
    return 0;
}

func_t PrimitiveCell::get_func() const {
    return func_m;
}