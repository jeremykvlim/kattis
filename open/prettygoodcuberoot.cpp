#include <bits/stdc++.h>
#include <dlfcn.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    void *gmp = dlopen("libgmp.so.10", RTLD_LAZY);
    auto set_str = (long int (*)(long unsigned int *rp, const unsigned char *str, size_t strsize, int base)) dlsym(gmp, "__gmpn_set_str");
    auto get_str = (size_t (*)(unsigned char *str, int base, long unsigned int *s1p, long int s1n)) dlsym(gmp, "__gmpn_get_str");
    auto rootrem = (long int (*)(long unsigned int *r1p, long unsigned int *r2p, const long unsigned int *sp, long int n, long unsigned int k)) dlsym(gmp, "__gmpn_rootrem");
    auto sqr = (void (*)(long unsigned int *rp, const long unsigned int *s1p, long int n)) dlsym(gmp, "__gmpn_sqr");
    auto mul_1 = (long unsigned int (*)(long unsigned int *rp, const long unsigned int *s1p, long int n, long unsigned int s2limb)) dlsym(gmp, "__gmpn_mul_1");
    auto addmul_1 = (long unsigned int (*)(long unsigned int *rp, const long unsigned int *s1p, long int n, long unsigned int s2limb)) dlsym(gmp, "__gmpn_addmul_1");
    auto add_1 = (long unsigned int (*)(long unsigned int *rp, const long unsigned int *s1p, long int n, long unsigned int s2limb)) dlsym(gmp, "__gmpn_add_1");
    auto rshift = (long unsigned int (*)(long unsigned int *rp, const long unsigned int *sp, long int n, unsigned int count)) dlsym(gmp, "__gmpn_rshift");
    auto cmp = (int (*)(const long unsigned int *s1p, const long unsigned int *s2p, long int n)) dlsym(gmp, "__gmpn_cmp");

    string s;
    while (cin >> s) {
        int len = s.size();
        for (char &c : s) c -= '0';

        vector<unsigned long> limbs((len + 18) / 19 + 1);
        int count = set_str(limbs.data(), (unsigned char *) s.data(), s.size(), 10);
        for (; count && !limbs[count - 1]; count--);

        int n = (count + 2) / 3;
        vector<unsigned long> root(n + 1), rem(count), bound(2 * n + 1);
        len = rootrem(root.data(), rem.data(), limbs.data(), count, 3);
        if (len) {
            sqr(bound.data(), root.data(), n);
            bound[2 * n] = mul_1(bound.data(), bound.data(), 2 * n, 3);
            auto carry = addmul_1(bound.data(), root.data(), n, 3);
            add_1(bound.data() + n, bound.data() + n, n + 1, carry);
            add_1(bound.data(), bound.data(), 2 * n + 1, 2);
            rshift(bound.data(), bound.data(), 2 * n + 1, 1);

            count = 2 * n + 1;
            for (; count && !bound[count - 1]; count--);
            if (len > count || (len == count && cmp(rem.data(), bound.data(), count) >= 0)) {
                carry = add_1(root.data(), root.data(), n, 1);
                if (carry) root[n++] = carry;
            }
        }

        string r(20 * n + 1, ' ');
        len = get_str((unsigned char *) r.data(), 10, root.data(), n);
        r.resize(len);
        for (char &c : r) c += '0';
        cout << r << "\n";
    }
}
