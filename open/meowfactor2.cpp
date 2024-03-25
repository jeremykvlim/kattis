#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;

    vector<string> patterns{"meow", "moew|meo|eow|me.?o?w|m.?ow", "me|eo|ow|m.?o|m.?.?w|e.?w", "m|e|o|w"};
    int factor = 4;

    for (int i = 0; i < patterns.size(); i++)
        if (regex_search(s, regex(patterns[i]))) {
            factor = min(factor, i);
            break;
        }

    cout << factor;
}
