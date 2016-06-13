#include <sstream>
#include <iostream>

#include "utils.h"
#include "pattern.h"

using namespace std;

// public methods

Pattern::Pattern(const string & str) : _pattern(str) {
    _pattern = build_pattern(_pattern);
    compile_pcre();
}

Pattern::~Pattern() {
    pcre_free(re);
}

string Pattern::pattern() const {
    return _pattern;
}

// method to match any string against the input pattern
bool Pattern::match(const string & str) const {
    if (re == NULL) {
        return false;
    }
    int ovector[OVECCOUNT];
    int subject_length = str.size();
    const char* subject = str.c_str();
    int rc = pcre_exec(re, NULL, subject, subject_length, 0, 0, ovector, OVECCOUNT);

    if (rc < 0) {
#ifdef DEBUG
        cerr << "Match error: " << str << endl;
#endif
        return false;
    }
    return true;
}

// end of public methods

// private methods

// helper method to initialize and to compile PCRE attributes
bool Pattern::compile_pcre() {
    int  erroroffset;
    const char* error;
    const char* pcre_pattern = _pattern.c_str();
    re = pcre_compile(pcre_pattern, 0, &error, &erroroffset, NULL);
    if (re == NULL) {
#ifdef DEBUG
        cerr << "Pattern couldn't be compiled!" << endl;
#endif
        return false;
    }
    return true;
}

// default pattern representation
string Pattern::normal_pattern() {
    return "([\\w ]+)";
}

// greedy pattern representation
string Pattern::greedy_pattern() {
    return "(.+)";
}

// spaced pattern representation
string Pattern::spaced_pattern(int num) {
    stringstream ss;
    ss << "((?:\\w+[ ]{" << num << "}\\w+)+)";
    return ss.str();
}

// helper method to generate spaced pattern due to its complexity
string Pattern::process_spaced_pattern(const string & str, int & i) {
    int start = i+1;
    int end = utils::find_int(str, start);
    i = end+1;
    return spaced_pattern(utils::to_int(str, start, end+1));
}

// Parser method to transform input pattern into
// a regular expression accepted by PCRE2 library
string Pattern::build_pattern(const string & str) {
    stringstream ss;
    int length = str.size();
    ss << '^';
    for (int i = 0; i < length; i++) {
        if (str[i] != '%') {
            ss << str[i];
        } else {
            if (str[i+1] != '{') {
                ss << str[i];
            } else {
                i = utils::find_int(str, i+2) + 1; // skip group number
                if (str[i] == '}') {
                    ss << normal_pattern();
                } else {
                    if (str[i] == 'G') {
                        i++;
                        ss << greedy_pattern();
                    } else {
                        ss << process_spaced_pattern(str, i);
                    }
                }
            }
        }
    }
    ss << '$';
    return ss.str();
}
// end of private methods
