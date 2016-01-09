#ifndef _LAMBDA_H_
#define _LAMBDA_H_

#include "abstractCell.h"
#include "frame.h"

typedef     std::shared_ptr<Cell>   cell_ptr;
typedef     std::shared_ptr<frame>  frame_ptr;

class ProcedureCell: public Cell {
public:
    ProcedureCell() = delete;
    ProcedureCell(cell_ptr formals, cell_ptr body, frame_ptr env=global_frame);
    ProcedureCell(const ProcedureCell& c) = delete;
    virtual ~ProcedureCell() {}
    ProcedureCell& operator=(const ProcedureCell& c) = delete;

    virtual std::string type() const;
    virtual std::string to_str() const;
    virtual unsigned int length() const;

    cell_ptr get_formals() const;
    cell_ptr get_body() const;
    frame_ptr get_env() const;
private:
    cell_ptr formals_m;
    cell_ptr body_m;
    frame_ptr env_m;
};

inline cell_ptr lambda(cell_ptr formals, cell_ptr body, frame_ptr env) {
    return cell_ptr(new ProcedureCell(formals, body, env));
}

inline bool procedurep(cell_ptr c) {
    return c->type() == "procedure";
}

inline cell_ptr get_formals(cell_ptr c) {
    if (!procedurep(c)) {
        throw std::runtime_error(c->to_str() + " is not of type procedure");
    }
    return (std::static_pointer_cast<ProcedureCell>(c))->get_formals();
}

inline cell_ptr get_body(cell_ptr c) {
    if (!procedurep(c)) {
        throw std::runtime_error(c->to_str() + " is not of type procedure");
    }
    return (std::static_pointer_cast<ProcedureCell>(c))->get_body();
}

inline frame_ptr get_env(cell_ptr c) {
    if (!procedurep(c)) {
        throw std::runtime_error(c->to_str() + " is not of type procedure");
    }
    return (std::static_pointer_cast<ProcedureCell>(c))->get_env();
}

#endif // _LAMBDA_H_