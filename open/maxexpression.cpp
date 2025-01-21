#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, n0, n1, n2;
    string s;
    cin >> n >> s >> n0 >> n1 >> n2;
    s += '+';

    vector<int> plus, minus;
    bool p = true;
    for (int i = 1, count = 1; i < s.size(); i += 2) {
        char op = s[i];
        if (op == '*') count++;
        else {
            (p ? plus : minus).emplace_back(count);
            count = 1;
            p = op == '+';
        }
    }
    sort(plus.begin(), plus.end());
    sort(minus.begin(), minus.end());

    int reduce = 0;
    while (!minus.empty() && n0) {
        reduce += minus.back() - 1;
        minus.pop_back();

        n0--;
    }

    if (n0) {
        int delta = min(n0, reduce);
        n0 -= delta;
        reduce -= delta;
    }

    auto val = 0LL;
    if (!n0) {
        while (!plus.empty()) {
            int p2 = min(plus.back(), n2);
            plus.pop_back();

            val += 1LL << p2;
            n2 -= p2;
        }

        int delta = min(n2, reduce);
        n2 -= delta;

        if (!minus.empty()) {
            vector<long long> remove(minus.size(), 1LL);
            for (int i = 0; n2; i++) {
                i %= minus.size();
                if (minus[i]) {
                    remove[i] <<= 1;
                    minus[i]--;
                    n2--;
                }
            }
            val -= accumulate(remove.begin(), remove.end(), 0LL);
        }
    } else if (!n2)
        for (int count : plus) {
            int delta = min(count, n1);
            n1 -= delta;
            val += delta == count;
        }
    else if (!plus.empty() && n1 + n2 >= plus[0]) {
        auto dp = 0LL, total = 0LL;
        while (!plus.empty() && n1 + n2 >= plus[0]) {
            int i = 0, sum = 0;
            while (i < plus.size() && n1 + n2 >= sum + plus[i]) {
                sum += plus[i];
                i++;
            }

            auto curr = 0LL;
            for (int j = i - 1, temp2 = n2; ~j; j--) {
                int p2 = min(temp2, plus[j]);

                curr += 1LL << p2;
                temp2 -= p2;
            }
            dp = max(dp, curr + total);

            int k = -1;
            for (int j = 0; j < plus.size(); j++)
                if (n1 + n2 >= plus[j])
                    if (!~k || min(n2, plus[j]) > min(n2, plus[k]) || (min(n2, plus[j]) == min(n2, plus[k]) && plus[j] < plus[k])) k = j;
            if (!~k) break;

            int p2 = min(n2, plus[k]);
            total += 1LL << p2;
            n2 -= p2;
            n1 -= plus[k] - p2;
            plus.erase(plus.begin() + k);
            dp = max(dp, total);
        }
        val += dp;
    }
    cout << val;
}
