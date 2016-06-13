#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils {
public:
    static int find_int(const std::string & str, int start);
    static int to_int(const std::string & str, int start, int end);
};

#endif
