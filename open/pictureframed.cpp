#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, h, w;
    cin >> n >> h >> w;

    vector<pair<int, int>> participants(n);
    for (auto &[a, b] : participants) {
        cin >> a >> b;
        a--;
        b--;
    }
    sort(participants.begin(), participants.end());

    int i = 0;
    multiset<int> s;
    for (int a = 0; a < h && (i < n || !s.empty()); a++) {
        if (s.empty()) a = participants[i].first;
        for (; i < n && participants[i].first == a; i++) s.emplace(participants[i].second);
        for (int b = 0; b < w && !s.empty(); b++) {
            auto it = s.upper_bound(b);
            if (it == s.begin()) b = *it;
            else it--;
            s.erase(it);
        }
    }
    cout << (i == n && s.empty() ? "CHEESE!!!" : "UH OH...");
}
