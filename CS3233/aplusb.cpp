#include <bits/stdc++.h>
using namespace std;

constexpr int OFFSET = 5e4;

void fft(vector<complex<double>> &a) {
    int n = a.size();
    if (n == 1) return;

    vector<complex<double>> a_even(n / 2), a_odd(n / 2);
    for (int i = 0; 2 * i < n; i++) {
        a_even[i] = a[2 * i];
        a_odd[i] = a[2 * i + 1];
    }

    fft(a_even);
    fft(a_odd);

    for (int i = 0; 2 * i < n; i++) {
        complex<double> twiddle(cos(2 * M_PI * i / n), sin(2 * M_PI * i / n));
        a[i] = a_even[i] + twiddle * a_odd[i];
        a[i + n / 2] = a_even[i] - twiddle * a_odd[i];
    }
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

    for (auto &x : dft_c) x = conj(x);
    fft(dft_c);
    for (auto &x : dft_c) x /= n;

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
    auto zeroes = count[OFFSET];

    auto c = convolve(count, count);
    for (int ai : a) c[2 * ai]--;

    auto ways = 0LL;
    for (int ai : a) ways += c[ai + OFFSET];
    ways -= 2 * zeroes * (n - 1);

    cout << ways;
}
