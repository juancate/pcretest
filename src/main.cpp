#include <string>
#include <iostream>

#include "pattern.h"

using namespace std;

int main(int argc, char* argv[]) {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);

    if (argc != 2) {
        cerr << "Invalid number of parameters. Usage: patt <pattern>" << '\n';
        return -1;
    }

#ifdef DEBUG
    cerr << "Running on DEBUG mode using: \"" << argv[1] << "\" as pattern.\n";
#endif

    string str;
    const Pattern p(argv[1]);

    while(getline(cin, str)) {
        bool result = p.match(str);
        if (result) {
            cout << str << '\n';
        }
    }
    return 0;
}
