#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    auto check = [&](char delim, int type) {
        bool first = true;
        for (char c : s)
            if (c == delim) {
                if (first) return false;
                first = true;
            } else {
                if (!isalpha(c) ||
                   (!type && !islower(c)) ||
                   (type == 1 && !isupper(c)) ||
                   (type == 2 && first && !isupper(c)) ||
                   (type == 2 && !first && !islower(c))) return false;
                first = false;
            }
        return !first;
    };
    vector<string> styles;
    if (all_of(s.begin(), s.end(), [](char c) { return islower(c); })) styles.emplace_back("flatcase");
    if (all_of(s.begin(), s.end(), [](char c) { return isupper(c); })) styles.emplace_back("SCREAMINGCASE");
    if (all_of(s.begin(), s.end(), [](char c) { return isalpha(c); }) && islower(s[0])) styles.emplace_back("camelCase");
    if (all_of(s.begin(), s.end(), [](char c) { return isalpha(c); }) && isupper(s[0])) styles.emplace_back("PascalCase");
    if (all_of(s.begin(), s.end(), [i = 0](char c) mutable { return isalpha(c) && (i++ & 1 ? isupper(c) : islower(c)); })) styles.emplace_back("sPoNgEcAsE");
    if (check('_', 0)) styles.emplace_back("snake_case");
    if (check('_', 1)) styles.emplace_back("MACRO_CASE");
    if (check('-', 0)) styles.emplace_back("kebab-case");
    if (check('-', 1)) styles.emplace_back("COBOL-CASE");
    if (check('-', 2)) styles.emplace_back("Train-Case");

    sort(styles.begin(), styles.end());
    if (styles.empty()) cout << "O nei!";
    else
        for (auto name : styles) cout << name << "\n";
}
