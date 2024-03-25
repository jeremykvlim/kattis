#include <bits/stdc++.h>
using namespace std;

constexpr int OFFSET = 5e4;

void fft(vector<complex<double>> &a, int sign = 1) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; i++) {
        int k = n >> 1;
        for (; j & k; k >>= 1) j ^= k;
        j ^= k;

        if (i < j) swap(a[i], a[j]);
    }

    for (int N = 1; 2 * N <= n; N <<= 1) {
        complex<double> twiddle(exp(complex<double>(0, M_PI / N * sign)));
        for (int i = 0; i < n; i += 2 * N) {
            complex<double> root(1);
            for (int j = 0; j < N; j++) {
                auto temp1 = a[i + j], temp2 = root * a[i + j + N];
                a[i + j] += temp2;
                a[i + j + N] = temp1 - temp2;
                root *= twiddle;
            }
        }
    }

    if (sign != 1)
        for (auto &x : a) x /= n;
}

vector<long long> convolve(vector<long long> &a, vector<long long> &b) {
    int n = 1;
    while (n < a.size() + b.size() - 1) n <<= 1;

    vector<complex<double>> dft_a(a.begin(), a.end()), dft_b(b.begin(), b.end()), dft_c(n);
    dft_a.resize(n);
    fft(dft_a);
    dft_b.resize(n);
    fft(dft_b);

    for (int i = 0; i < n; i++) dft_c[i] = dft_a[i] * dft_b[i];
    fft(dft_c, -1);

    vector<long long> c(n);
    for (int i = 0; i < n; i++) c[i] = llround(dft_c[i].real());
    c.resize(a.size() + b.size() - 1);

    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    vector<long long> count(4 * OFFSET, 0);
    for (int &ai : a) {
        cin >> ai;

        ai += OFFSET;
        count[ai]++;
    }

    auto c = convolve(count, count);
    for (int ai : a) c[2 * ai]--;

    auto ways = 0LL;
    for (int ai : a) ways += c[ai + OFFSET];
    ways -= 2 * count[OFFSET] * (n - 1);

    cout << ways;
}
