#include <bits/stdc++.h>
using namespace std;

constexpr int OFFSET = 5e4;

void fft(vector<complex<double>> &fourier, int n, int sign, vector<int> &reverse) {
    for (int i = 0; i < n; i++)
        if (i < reverse[i]) swap(fourier[i], fourier[reverse[i]]);
    for (int i = 1; i < n; i <<= 1)
        for (int k = 0; k < i; k++) {
            auto twiddle = exp(complex<double>(0, sign * M_PI / i * k));
            for (int j = k; j < n; j += 2 * i) {
                auto temp = twiddle * fourier[i + j];
                fourier[i + j] = fourier[j] - temp;
                fourier[j] += temp;
            }
        }
    if (sign == -1)
        for (int i = 0; i < n; i++) fourier[i] /= n;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, max_sum = 2*(abs(-OFFSET) + OFFSET);
    cin >> n;
    vector<int> a(n), freq(abs(-OFFSET) + OFFSET + 1);
    vector<long long> count(max_sum + 1);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        a[i] += OFFSET;
        freq[a[i]]++;
        count[a[i] * 2]--;
    }

    n = 1;
    while (n < max_sum) n <<= 1;
    vector<complex<double>> fourier(n);
    vector<int> reverse(n);
    for (int i = 1; i < n; i++) reverse[i] = (i & 1) * n >> 1 | reverse[i >> 1] >> 1;
    for (int i = 0; i < freq.size(); i++) fourier[i] = complex<double>(freq[i], freq[i]);
    fft(fourier, n, 1, reverse);
    for (auto &i : fourier) i *= i;
    fft(fourier, n, -1, reverse);
    for (int i = 0; i < max_sum; i++) count[i] += fourier[i].imag() / 2 + .5;
    long long ways = 0;
    for (int i : a) {
        ways += count[i + OFFSET];
        if (i == OFFSET) ways -= 2 * a.size() - 2;
    }
    cout << ways;
}
