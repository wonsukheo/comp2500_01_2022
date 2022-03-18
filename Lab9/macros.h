#ifndef MACROS_H
#define MACROS_H

#define MAX(x, y) ((x) < (y) ? y : x)

#define MIN(x, y) ((x) < (y) ? x : y)

#define ABS(x) ((x) < 0 ? -(x) : x)

#define RANGE(curr, min, max) if ((min) <= (max)) \
                              for (curr = min; curr <= (max); ++curr) \

#define RANGE_DESC(curr, max, min) if ((max) >= (min)) \
                                   for (curr = max; curr >= (min); --curr) \

#define SET(ary, start, count, value) do { size_t i = start;    \
                                      while (i < ((start) + (count))) \
                                      { *(ary + i++) = value; } }   \
                                      while (0)                 \

#endif /* MACROS_H */
