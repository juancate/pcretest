#include <sstream>
#include <iostream>

#include "utils.h"
#include "pattern.h"

using namespace std;

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

bool Pattern::match(const string & str) const {
    if (re == NULL) {
        return false;
    }
    int subject_length = str.size();
    PCRE2_SPTR subject = (PCRE2_SPTR)str.c_str();
    int rc = pcre2_match(re, subject, subject_length, 0, 0, match_data, NULL);

    if (rc < 0) {
        cerr << "Match error: " << str << endl;
        return false;
    }
    return true;
}

// private methods

bool Pattern::compile_pcre() {
    int errornumber;
    PCRE2_SIZE erroroffset;
    PCRE2_SPTR pcre_pattern = (PCRE2_SPTR)_pattern.c_str();
    re = pcre2_compile(pcre_pattern, PCRE2_ZERO_TERMINATED, 0, &errornumber, &erroroffset, NULL);
    if (re == NULL) {
        cerr << "Pattern couldn't be compiled!" << endl;
        return false;
    }
    match_data = pcre2_match_data_create_from_pattern(re, NULL);
    return true;
}

string Pattern::normal_pattern() {
    return "([\\w ]+)";
}

string Pattern::greedy_pattern() {
    return "(.+)";
}

string Pattern::spaced_pattern(int num) {
    stringstream ss;
    ss << "((?:\\w+[ ]{" << num << "}\\w+)+)";
    return ss.str();
}

string Pattern::process_spaced_pattern(const string & str, int & i) {
    int start = i+1;
    int end = Utils::find_int(str, start);
    i = end+1;
    return spaced_pattern(Utils::to_int(str, start, end+1));
}

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
