#ifndef PATTERN_H
#define PATTERN_H

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include <string>

/**
 * Class to represent the pattern behaviour.
 */

class Pattern {
    std::string _pattern;
    // PCRE2 attributes
    pcre2_code *re;
    pcre2_match_data *match_data;
public:
    Pattern(const std::string & str);
    virtual ~Pattern();
    std::string pattern() const;
    bool match(const std::string & str) const;
private:
    bool compile_pcre();
    std::string normal_pattern();
    std::string greedy_pattern();
    std::string spaced_pattern(int num);
    std::string process_spaced_pattern(const std::string & str, int & i);
    std::string build_pattern(const std::string & str);
};

#endif
