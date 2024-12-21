#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    unordered_map<string, int> compress;
    vector<int> s(n);
    for (int i = 0; i < n; i++) {
        string st;
        cin >> st >> s[i];

        compress[st] = i;
    }

    int l;
    cin >> l;

    vector<int> threshold(n, 0), threshold_count(n, 0), attribute_count(n, 0);
    int max_count = 0;
    while (l--) {
        string st;
        int t;
        cin >> st >> t;

        int i = compress[st];
        if (threshold[i] < t) {
            threshold[i] = t;
            threshold_count[i] = 1;
        } else if (threshold[i] == t) threshold_count[i]++;
        attribute_count[i]++;
        max_count = max(max_count, attribute_count[i]);
    }

    auto score = 0LL, spend = 0LL;
    vector<long long> gain;
    for (int i = 0; i < n; i++)
        if (s[i] > threshold[i]) score += (long long) s[i] * attribute_count[i];
        else {
            spend += threshold[i] - s[i];
            score += (long long) threshold[i] * (attribute_count[i] - threshold_count[i]);
            gain.emplace_back((long long) threshold[i] * threshold_count[i] + attribute_count[i]);
            s[i] = threshold[i];
        }


    if (k < spend) {
        cout << 0;
        exit(0);
    }

    sort(gain.rbegin(), gain.rend());
    for (int i = 0; i < gain.size() && spend < k; i++)
        if (max_count < gain[i]) {
            score += gain[i];
            spend++;
        } else break;

    cout << score + (long long) max_count * (k - spend);
}
