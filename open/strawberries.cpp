#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bitset<(int) 25e4 + 1> dp;
    int n = 1;
    string s;
    while (getline(cin, s)) {
        istringstream iss(s);
        vector<int> count{istream_iterator<int>{iss}, istream_iterator<int>{}};
        if (all_of(count.begin(), count.end(), [&](int c) { return !c; })) exit(0);

        cout << "Box #" << n++ << ":\n";

        int total = 0;
        for (int i = 1; i <= 10; i++) total += i * count[i - 1];
        if (total & 1) {
            cout << "Can't be divided.\n\n";
            continue;
        }

        dp.reset();
        dp[0] = 1;
        for (int i = 1; i <= 10; i++)
            for (int k = 1; count[i - 1]; k <<= 1) {
                int c = min(k, count[i - 1]);
                dp |= dp << (c * i);
                count[i - 1] -= c;
            }

        cout << (dp[total / 2] ? "Can be divided.\n\n" : "Can't be divided.\n\n");
    }
}
