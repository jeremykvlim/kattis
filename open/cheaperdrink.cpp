#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto turn = [](string &m) -> string {
        auto temp = m;
        reverse(temp.begin(), temp.end());
        string s;
        for (char c : temp) {
            if (c == '2' || c == '3' || c == '4' || c == '5' || c == '7') return m;
            if (c == '0' || c == '1' || c == '8') s += c;
            if (c == '6') s += '9';
            if (c == '9') s += '6';
        }

        return min(s, m);
    };

    vector<string> s;
    while (n--) {
        string m;
        cin >> m;

        s.emplace_back(turn(m));
    }

    sort(s.begin(), s.end(), [&](string& s1, string& s2) { return s1 + s2 < s2 + s1; });
    for (auto &str : s) cout << str;
}
