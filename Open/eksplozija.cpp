#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string mirko, explosion;
    cin >> mirko >> explosion;

    vector<int> state(1, 0);
    string remaining;
    for (auto c : mirko) {
        remaining += c;
        state.emplace_back(c == explosion[state.back()] ? state.back() + 1 : (c == explosion[0] ? 1 : 0));

        if (state.back() == explosion.size()) {
            state.resize(state.size() - explosion.size());
            remaining.resize(remaining.size() - explosion.size());
        }
    }

    cout << (remaining.empty() ? "FRULA" : remaining);
}
