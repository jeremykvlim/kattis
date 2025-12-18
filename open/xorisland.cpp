#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> hats(n);
    for (int &a : hats) cin >> a;
    sort(hats.begin(), hats.end());

    vector<array<int, 3>> triples;
    for (int a = 0; a < n; a++)
        for (int b = a + 1; b < n; b++)
            for (int c = b + 1; c < n; c++)
                if ((hats[a] ^ hats[b]) == hats[c]) triples.push_back({a, b, c});

    vector<bool> visited(n, false);
    int meetings = n, count = 0;
    auto dfs = [&](auto &&self, int i = 0) -> void {
        if (i == triples.size()) {
            meetings = min(meetings, count);
            return;
        }

        auto [a, b, c] = triples[i];
        if (visited[a] || visited[b] || visited[c]) {
            self(self, i + 1);
            return;
        }

        if (meetings == count) return;

        visited[a] = true;
        count++;
        self(self, i + 1);
        count--;
        visited[a] = false;

        visited[b] = true;
        count++;
        self(self, i + 1);
        count--;
        visited[b] = false;

        visited[c] = true;
        count++;
        self(self, i + 1);
        count--;
        visited[c] = false;
    };
    dfs(dfs);
    cout << meetings;
}
