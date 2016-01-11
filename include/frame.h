#ifndef _FRAME_H_
#define _FRAME_H_

#include <unordered_map>
#include "cons.h"

class frame {
public:
    typedef     std::shared_ptr<frame>                      frame_ptr;
    typedef     std::shared_ptr<Cell>                       cell_ptr;
    typedef     std::unordered_map<std::string, cell_ptr>   map_t;
    typedef     map_t::iterator                             map_iter;
public:
    frame();
    frame(frame_ptr f);
    frame(const frame& f) = delete;
    frame& operator=(const frame& f) = delete;
    ~frame();

    cell_ptr look_up(std::string name);
    /**
     * define a name in current frame, bind value to the name.
     */
    void define(std::string name, cell_ptr value);
    /** 
     * set value of name in current environment(chain of frames)
     * error if name doesn't exit in current env
     */
    void set(std::string name, cell_ptr value);

    static frame_ptr make_new_frame(cell_ptr formals, cell_ptr args, frame_ptr parent);

private:
    // parent frame of the current frame
    frame_ptr parent_m;
    // bindings of names to values
    std::unordered_map<std::string, cell_ptr> bindings;
private:
    /**
     * bind values in args list to variables in formals list
     * this method does not check content in formals and args
     */
    static void _bind_vars(cell_ptr formals, cell_ptr args, frame_ptr f);

    /**
     * look up the name in the current environment
     * return the iterator points to the <name, value> pair
     * on success, end() iterator on failure
     *
     * assumes that the end() iterator is all the same
     * for all map_t instances
     */
    map_iter _look_up(std::string name);
};

extern std::shared_ptr<frame> global_frame;

#endif // _FRAME_H_