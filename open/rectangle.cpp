#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    vector<pair<int, int>> edges(k);
    unordered_map<int, vector<int>> indices_a, indices_b;
    int longest = -1;
    auto sum = 0LL;
    for (int i = 0; i < k; i++) {
        auto &[a, b] = edges[i];
        cin >> a >> b;

        indices_a[a].emplace_back(i);
        indices_b[b].emplace_back(i);
        longest = max(longest, a);
        sum += (long long) a * b;
    }

    vector<int> lengths;
    for (auto c = 1LL; c <= longest; c++)
        if (!(sum % c) && sum / c >= c) {
            auto a = c, b = sum / c;
            vector<bool> visited(k, false);
            for (;;) {
                if (a < b) swap(a, b);

                auto visit = [&](auto len1, auto &len2, auto &indices, bool f) {
                    bool v = false;
                    if (len1 <= longest)
                        for (int i : indices[len1])
                            if (!visited[i]) {
                                v = visited[i] = true;
                                len2 -= f ? edges[i].first : edges[i].second;
                            }
                    return v;
                };

                if (!visit(a, b, indices_a, false) && !visit(b, a, indices_b, true) && !visit(b, a, indices_a, false)) break;
            }

            if (!a || !b) lengths.emplace_back(c);
        }
    
    sort(lengths.begin(), lengths.end());
    cout << lengths.size() << "\n";
    for (int l : lengths) cout << l << "\n";
}
