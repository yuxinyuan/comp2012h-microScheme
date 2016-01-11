#ifdef DEBUG

#include <iostream>
#define __output(c) cout << "evaluating: " << c->to_str() << endl
#define __debug(c) __output(c)

#else

#define __debug(c) 

#endif