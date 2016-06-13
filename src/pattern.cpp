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
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
}

string Pattern::pattern() const {
    return _pattern;
}

// method to match any string against the input pattern
bool Pattern::match(const string & str) const {
    if (re == NULL) {
        return false;
    }
    int subject_length = str.size();
    PCRE2_SPTR subject = (PCRE2_SPTR)str.c_str();
    int rc = pcre2_match(re, subject, subject_length, 0, 0, match_data, NULL);

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
    int errornumber;
    PCRE2_SIZE erroroffset;
    PCRE2_SPTR pcre_pattern = (PCRE2_SPTR)_pattern.c_str();
    re = pcre2_compile(pcre_pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
    if (re == NULL) {
#ifdef DEBUG
        cerr << "Pattern couldn't be compiled!" << endl;
#endif
        return false;
    }
    match_data = pcre2_match_data_create_from_pattern(re, NULL);
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
    int end = Utils::find_int(str, start);
    i = end+1;
    return spaced_pattern(Utils::to_int(str, start, end+1));
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
                i = Utils::find_int(str, i+2) + 1; // skip group number
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
