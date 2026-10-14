#include <bits/stdc++.h>
#include <dlfcn.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    string cipher;
    cin >> n >> x;
    cin.ignore();
    getline(cin, cipher);

    unsigned p33_x = 33, p33_x_sq = 33, total = 0;
    for (int i = 1; i < x; i++) p33_x *= 33;
    for (int i = 0; i < x; i++) {
        total += p33_x_sq;
        p33_x_sq *= p33_x;
    }

    int MOD = 1 << 20;
    string digits(7 * x, ' ');
    char *p = digits.data();
    for (int i = 0, sum = ((total - x) >> 5) % MOD; i < x; i++) {
        p = to_chars(p, digits.data() + digits.size(), sum).ptr;
        sum = (33 * sum + x) % MOD;
    }
    int len = p - digits.data();
    for (int i = 0; i < len; i++) digits[i] -= '0';

    void *gmp = dlopen("libgmp.so.10", RTLD_LAZY);
    auto set_str = (long int (*)(long unsigned int *rp, const unsigned char *str, size_t strsize, int base)) dlsym(gmp, "__gmpn_set_str");
    auto get_str = (size_t (*)(unsigned char *str, int base, long unsigned int *s1p, long int s1n)) dlsym(gmp, "__gmpn_get_str");

    vector<unsigned long> limbs((len + 18) / 19 + 1);
    int count = set_str(limbs.data(), (unsigned char *) digits.data(), len, 10);
    for (; count && !limbs[count - 1]; count--);
    
    get_str((unsigned char *) digits.data(), 27, limbs.data(), count);
    for (int i = 0; i < n; i++) {
        int c = (cipher[i] == ' ' ? 26 : cipher[i] - 'A') + digits[i];
        cipher[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ "[c < 27 ? c : c - 27];
    }
    cout << cipher;
}
