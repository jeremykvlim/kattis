#include <bits/stdc++.h>
using namespace std;

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

vector<int> convolve(vector<int> &a, vector<int> &b) {
    int n = 1;
    while (n < a.size() + b.size() - 1) n <<= 1;

    vector<complex<double>> dft_a(a.begin(), a.end()), dft_b(b.begin(), b.end()), dft_c(n);
    dft_a.resize(n);
    fft(dft_a);
    dft_b.resize(n);
    fft(dft_b);

    for (int i = 0; i < n; i++) dft_c[i] = dft_a[i] * dft_b[i];
    fft(dft_c, -1);

    vector<int> c(n);
    for (int i = 0; i < n; i++) c[i] = round(dft_c[i].real());
    c.resize(a.size() + b.size() - 1);

    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> count(60001, 0);
    auto total = 0LL;
    int smallest = 1e5, biggest = 0;
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k;

        count[k]++;
        smallest = min(smallest, k);
        biggest = max(biggest, k);
        total += k;
    }

    auto c = convolve(count, count);
    for (int &e : c) e = e ? 1 : 0;
    c = convolve(c, c);

    cout << fixed << setprecision(4) << 4 * biggest << " " << 4 * smallest << " " << count_if(c.begin(), c.end(), [](int e) {return abs(e) > 0.1;}) << " " << 4 * (double) total / n;
}
