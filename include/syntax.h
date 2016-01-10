#ifndef _SYNTAX_H_
#define _SYNTAX_H_

typedef     std::shared_ptr<Cell>   cell_ptr;
typedef     std::shared_ptr<frame>  frame_ptr;

static cell_ptr define_syntax(cell_ptr c, frame_ptr env);
static cell_ptr lambda_syntax(cell_ptr c, frame_ptr env);
static cell_ptr set_syntax(cell_ptr c, frame_ptr env);
static cell_ptr if_syntax(cell_ptr c, frame_ptr env);
static cell_ptr and_syntax(cell_ptr c, frame_ptr env);
static cell_ptr or_syntax(cell_ptr c, frame_ptr env);
static cell_ptr cond_syntax(cell_ptr c, frame_ptr env);
static cell_ptr let_syntax(cell_ptr c, frame_ptr env);
static cell_ptr begin_syntax(cell_ptr c, frame_ptr env);
static cell_ptr quote_syntax(cell_ptr c);

#endif // _SYNTAX_H_