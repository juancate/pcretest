#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * Utils namespace with helpers functions.
 */

namespace utils {
    int find_int(const std::string & str, int start);
    int to_int(const std::string & str, int start, int end);
};

#endif
