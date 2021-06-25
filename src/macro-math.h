#ifndef _MACRO_MATH_H_
#define _MACRO_MATH_H_

#define ROUNDUP(x, y) ((y) * (((x) + ((y) - 1)) / (y)))
#define ROUNDDOWN(x, y) ((y) * ((x) / (y)))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))



#endif
