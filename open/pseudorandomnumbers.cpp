#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        int b, l;
        cin >> b >> l;

        vector<vector<int>> digits(l);
        for (int i = 0; i < l; i++) {
            int d;
            cin >> d;

            digits[i].emplace_back(d);
        }

        int t;
        cin >> t;

        digits.resize(t);
        vector<int> src(t, 1), dest(t);
        auto match = [&](int i, int d = 1) {
            if (dest[i] < digits[i + 1].size()) {
                if (digits[i + 1][dest[i]] != d) return false;
            } else digits[i + 1].emplace_back(d);
            dest[i]++;
            return true;
        };

        auto generate = [&](int i) {
            int sum = digits[i][src[i] - 1] + digits[i][src[i]++];
            return match(i, sum % b) && (sum < b || match(i));
        };

        bool possible = true;
        for (int i = 0; i < l - 1 && possible;)
            if (dest[i] == digits[i + 1].size()) i++;
            else if (src[i] < digits[i].size()) possible = generate(i);
            else {
                int diff = digits[i + 1][dest[i]++] - digits[i][src[i]++ - 1];
                digits[i].emplace_back((diff + b) % b);
                possible = diff >= 0 || match(i);
                if (i) i--;
            }

        if (!possible) {
            cout << "impossible\n";
            continue;
        }

        for (int i = l - 1; digits[t - 1].empty() && ~i;)
            if (src[i] == digits[i].size()) i--;
            else {
                generate(i);
                i++;
            }

        if (digits[t - 1].empty()) cout << "unpredictable\n";
        else cout << digits[t - 1][0] << "\n";
    }
}
