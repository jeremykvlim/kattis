#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, len = 0, curr = 1;
    cin >> n;
    vector<long long> arr(n + 1, 0), pref(n + 1, 0);
    unordered_map<int, long long> compress;
    unordered_map<long long, int> count, pos{{0, 0}};
    mt19937_64 rng(random_device{}());
    uniform_int_distribution<long long> dis;

    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        if (!compress[x]) compress[x] = dis(rng);
        arr[i] = compress[x];
        count[arr[i]]++;
        while (count[arr[i]] > 2 && curr < i) count[arr[curr++]]--;
        pref[i] = pref[i - 1] ^ arr[i];
        if (pos.count(pref[i]) && pos[pref[i]] + 1 >= curr) len = max(len, i - pos[pref[i]]);
        pos[pref[i]] = i;
    }

    cout << len;
}
