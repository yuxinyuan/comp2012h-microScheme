#ifndef _FRAME_H_
#define _FRAME_H_

#include <unordered_map>
#include "cons.h"

class frame: public std::enable_shared_from_this<frame> {
public:
    typedef     std::shared_ptr<frame>  frame_ptr;
    typedef     std::shared_ptr<Cell>   cell_ptr;
public:
    frame();
    frame(frame_ptr f);
    frame(const frame& f) = delete;
    frame& operator=(const frame& f) = delete;
    ~frame() {}

    cell_ptr look_up(std::string name);
    void define(std::string name, cell_ptr value);

    frame_ptr make_new_frame(cell_ptr formals, cell_ptr args);

private:
    // parent frame of the current frame
    frame_ptr parent;
    // bindings of names to values
    std::unordered_map<std::string, cell_ptr> bindings;
private:
    /**
     * bind values in args list to variables in formals list
     * this method does not check content in formals and args
     */
    static void _bind_vars(cell_ptr formals, cell_ptr args, frame_ptr f);
};

extern std::shared_ptr<frame> global_frame;

#endif // _FRAME_H_