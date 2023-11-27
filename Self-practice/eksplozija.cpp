#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, explosion, remaining;
    cin >> s >> explosion;
    vector<int> state(1, 0);

    for (auto c : s) {
        remaining += c;
        if (c == explosion[state.back()]) state.emplace_back(state.back() + 1);
        else state.emplace_back((c == explosion[0]) ? 1 : 0);

        if (state.back() == explosion.size()) {
            state.resize(state.size() - explosion.size());
            remaining.resize(remaining.size() - explosion.size());
        }
    }

    cout << (remaining.empty() ? "FRULA" : remaining);
}
