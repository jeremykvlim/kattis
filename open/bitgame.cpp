#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    mt19937 rng(random_device{}());
    string paper;
    auto dfs = [&](auto &&self) {
        if (paper.size() == 20) {
            bool num;
            cout << "? " << stoi(paper, nullptr, 2) << "\n" << flush;
            cin >> num;

            return num;
        }

        string bits{"01"};
        if (rng() & 1) swap(bits[0], bits[1]);

        for (char b : bits) {
            paper += b;
            bool win = !self(self);
            paper.pop_back();

            if (win) return true;
        }

        return false;
    };

    cout << (dfs(dfs) ? "! alice" : "! bob");
}
