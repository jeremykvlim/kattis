#include <bits/stdc++.h>
using namespace std;

void init(int n, vector<long long> &freq) {
    fill(freq.begin(), freq.end(), 0);
    for (int i = 0; i < n; i++) freq[((long long) i * i) % n]++;
}

long long count(int pf, int power, vector<long long> &freq) {
    auto c = 0LL;

    int p = pow(pf, power);
    init(p, freq);
    for (int i = 0; i < p; i++) c += freq[i] * freq[(i + 1) % p];
    for (int i = 0, j = 0; i < p; i += pf, j = ((long long) i * i) % p) c += freq[(j + 1) % p];

    c *= p / pf * (pf - 1);
    return c + (power > 1 ? count(pf, power - 2, freq) * (long long) pow(pf, 3) : 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int temp = n;
    auto triples = 1LL;
    vector<long long> freq(n, 0);
    for (int p = 2; p <= sqrt(n); p == 2 ? p++ : p += 2) {
        int pow = 0;
        while (!(n % p)) {
            n /= p;
            pow++;
        }
        if (pow) triples *= count(p, pow, freq);
    }
    if (n > 1) triples *= count(n, 1, freq);

    swap(temp, n);
    init(n, freq);
    freq[0]--;
    for (int i = 1; i < n; i++) triples += freq[((long long) 2 * i * i) % n];
    for (int i = 0; i < n; i++) triples -= freq[i] * (2 * freq[i] + freq[(n - i) % n]);
    triples -= 3 * freq[0] + 1;
    cout << triples / 2;
}
