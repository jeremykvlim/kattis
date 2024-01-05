#include <bits/stdc++.h>
using namespace std;

constexpr int OFFSET = 5e4;

void fft(vector<complex<double>> &fourier, int sign, vector<int> &reverse) {
    for (int i = 0; i < reverse.size(); i++)
        if (i < reverse[i]) swap(fourier[i], fourier[reverse[i]]);

    for (int i = 1; i < fourier.size(); i *= 2)
        for (int k = 0; k < i; k++)
            for (int j = k; j < fourier.size(); j += 2 * i) {
                auto temp = exp(complex<double>(0, sign * M_PI / i * k)) * fourier[i + j];
                fourier[i + j] = fourier[j] - temp;
                fourier[j] += temp;
            }

    if (sign == -1)
        for (auto &f : fourier) f /= fourier.size();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> nums(n), freq(2 * OFFSET + 1);
    vector<long long> count(4 * OFFSET + 1);
    for (int &a : nums) {
        cin >> a;

        a += OFFSET;
        freq[a]++;
        count[a * 2]--;
    }

    int size = pow(2, ceil(log2(count.size())));
    vector<int> reverse(size);
    for (int i = 1; i < size; i++) reverse[i] = (i & 1) * size >> 1 | reverse[i >> 1] >> 1;
    vector<complex<double>> fourier(size);
    for (int i = 0; i < freq.size(); i++) fourier[i] = complex<double>(freq[i], freq[i]);

    fft(fourier, 1, reverse);
    for (auto &f : fourier) f *= f;
    fft(fourier, -1, reverse);

    for (int i = 0; i < count.size(); i++) count[i] += round(fourier[i].imag() / 2);
    auto ways = 0LL;
    for (int a : nums) {
        ways += count[a + OFFSET];
        if (a == OFFSET) ways -= 2 * nums.size() - 1;
    }

    cout << ways;
}
