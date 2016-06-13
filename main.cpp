#include <bits/stdc++.h>

#include "pattern.h"

using namespace std;

int main() {
    cin.tie(NULL);
    ios_base::sync_with_stdio(false);
    string str;
    while(getline(cin, str)) {
        const Pattern p(str);
        cout << "/" << p.pattern() << "/g\n";
    }
    return 0;
}
