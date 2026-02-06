#include <bits/stdc++.h>
using namespace std;

vector<int> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;
            spf[j] = p;
            if (p == spf[i]) break;
        }
    }
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto primes = sieve(99999);
    vector<int> five_digit_primes(lower_bound(primes.begin(), primes.end(), 1e4), primes.end());
    shuffle(five_digit_primes.begin(), five_digit_primes.end(), mt19937(random_device{}()));

    int n;
    cin >> n;

    array<int, 5> masks;
    array<int, 10> count, count_l, count_r;
    while (n--) {
        fill(masks.begin(), masks.end(), (1 << 10) - 1);
        fill(count_l.begin(), count_l.end(), 0);
        fill(count_r.begin(), count_r.end(), 5);
        for (int turn = 1, k = 0; turn <= 6; turn++) {
            int prime;
            if (turn == 1) prime = 10243;
            else if (turn == 2) prime = 56897;
            else {
                for (;; k++) {
                    fill(count.begin(), count.end(), 0);
                    for (int i = 4, p = five_digit_primes[k]; ~i; i--, p /= 10) {
                        int d = p % 10;
                        if (!((masks[i] >> d) & 1)) goto next;
                        count[d]++;
                    }
                    for (int i = 0; i < 10; i++)
                        if (!(count_l[i] <= count[i] && count[i] <= count_r[i])) goto next;
                    prime = five_digit_primes[k++];
                    break;
                    next:;
                }
            }
            cout << prime << "\n" << flush;

            string s;
            cin >> s;
            if (s == "ggggg") break;

            auto p = to_string(prime);
            fill(count.begin(), count.end(), 0);
            vector<int> w_digits;
            for (int i = 0; i < 5; i++) {
                int d = p[i] - '0';
                if (s[i] == 'g') {
                    masks[i] = 1 << d;
                    count[d]++;
                    count_l[d] = max(count_l[d], count[d]);
                } else if (s[i] == 'y') {
                    masks[i] &= ~(1 << d);
                    count[d]++;
                    count_l[d] = max(count_l[d], count[d]);
                } else {
                    masks[i] &= ~(1 << d);
                    w_digits.emplace_back(d);
                }
            }

            for (int d : w_digits) count_r[d] = min(count_r[d], count[d]);
        }
    }
}
