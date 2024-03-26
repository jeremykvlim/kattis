#include <bits/stdc++.h>
using namespace std;

long long pi(long long n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;

    int sqrtn = sqrt(n), size = (sqrtn + 1) / 2;
    vector<int> small(size), approx(size);
    vector<long long> large(size);
    iota(small.begin(), small.end(), 0);
    for (int i = 0; i < size; i++) {
        approx[i] = 2 * i + 1;
        large[i] = (n / approx[i] - 1) / 2;
    }

    auto div = [](double n, long long d) -> int {return n / d;};
    auto half = [](int n) {return (n - 1) / 2;};

    vector<bool> prime(sqrtn + 1, true);
    int count = 0;
    for (int p = 3; p <= sqrtn; p += 2)
        if (prime[p]) {
            int sq = p * p;
            if ((long long) sq * sq > n) break;

            prime[p] = false;
            for (int i = sq; i <= sqrtn; i += 2 * p) prime[i] = false;

            int s = 0;
            for (int i = 0; i < size; i++) {
                int q = approx[i];
                if (!prime[q]) continue;

                auto semiprime = (long long) p * q;
                large[s] = large[i] + count;
                large[s] -= semiprime <= sqrtn ? large[small[semiprime / 2] - count] : small[half(div(n, semiprime))];
                approx[s++] = q;
            }
            size = s;

            for (int i = half(sqrtn), j = (sqrtn / p - 1) | 1; j >= p; j -= 2)
                for (int k = small[j / 2] - count; i >= j * p / 2; i--) small[i] -= k;

            count++;
        }

    for (int i = 1; i < size; i++) large[0] -= large[i];
    large[0] += (long long) (size + 2 * (count - 1)) * (size - 1) / 2;
    for (int i = 1; i < size; i++) {
        int q = approx[i];
        long long m = n / q;

        int k = small[half(m / q)] - count;
        if (k <= i) break;

        for (int j = i + 1; j <= k; j++) large[0] += small[half(div(m, approx[j]))];
        large[0] -= (long long) (k - i) * (count + i - 1);
    }

    return large[0] + 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    cout << pi(n);
}
