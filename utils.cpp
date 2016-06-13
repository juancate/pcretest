#include "utils.h"

using namespace std;

int Utils::find_int(const string & str, int start) {
    int end = start;
    int length = str.size();
    while (isdigit(str[end])) {
        end++;
    }
    return end-1;
}

int Utils::to_int(const string & str, int start, int end) {
    int n = 0;
    for (int i = start; i < end; i++) {
        n *= 10;
        n += int(str[i] - '0');
    }
    return n;
}
