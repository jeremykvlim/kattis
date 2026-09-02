#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] ^= v;
    }

    T pref_xor(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum ^= BIT[i];
        return sum;
    }

    T range_xor_query(int l, int r) {
        if (l >= r) return 0;
        return pref_xor(r) ^ pref_xor(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    string s;
    cin >> n >> q >> s;

    int m = (n + 63) >> 6, b = 0;
    vector<int> lazy(m + 1, 0);
    vector<unsigned long long> blocks(m + 1, 0), block_pref(m);
    for (int i = 0; i < n; i++) blocks[i >> 6] |= (unsigned long long) (s[i] - '0') << (i & 63);
    FenwickTree<int> fw(m + 1);
    auto block_mask = [&](int i) {
        auto mask = blocks[i] ^ (0ULL - b);
        b ^= lazy[i];
        return mask;
    };

    while (q--) {
        int l, r;
        cin >> l >> r;
        l--;

        int len = r - l;
        if (len == n) {
            cout << string(n, '0');
            exit(0);
        }

        auto get = [&](int pos) -> pair<int, int> {
            return {pos >> 6, pos & 63};
        };
        auto [i, shift] = get(l);
        b = fw.pref_xor(i);
        int k = (len + 63) >> 6, flip = 0;
        auto last_mask = ~0ULL >> ((64 - len) & 63);
        auto pref = [&](auto mask) {
            mask ^= mask << 1;
            mask ^= mask << 2;
            mask ^= mask << 4;
            mask ^= mask << 8;
            mask ^= mask << 16;
            mask ^= mask << 32;
            mask ^= 0ULL - flip;
            flip = mask >> 63;
            return mask;
        };

        if (!shift) {
            for (int j = 0; j < k - 1; j++) block_pref[j] = pref(block_mask(i + j));
            block_pref[k - 1] = pref(block_mask(i + k - 1) & last_mask);
        } else {
            auto m1 = block_mask(i);
            for (int j = 0; j < k - 1; j++) {
                auto m2 = block_mask(i + j + 1);
                block_pref[j] = pref(m1 >> shift | m2 << (64 - shift));
                m1 = m2;
            }
            m1 >>= shift;
            if (i + k <= (l + len - 1) >> 6) m1 |= block_mask(i + k) << (64 - shift);
            block_pref[k - 1] = pref(m1 & last_mask);
        }

        if (flip) {
            tie(i, shift) = get(len);
            blocks[i] ^= ~0ULL << shift;
            lazy[i] ^= 1;
            fw.update(i + 1, 1);
        }

        auto apply = [&](int pos, int len) {
            if (!len) return;

            tie(i, shift) = get(pos);
            auto last = block_pref[k - 1] & last_mask;
            if (!shift) {
                for (int j = 0; j < k - 1; j++) blocks[i + j] ^= block_pref[j];
                blocks[i + k - 1] ^= last;
            } else if (k == 1) {
                blocks[i] ^= last << shift;
                blocks[i + 1] ^= last >> (64 - shift);
            } else {
                blocks[i] ^= block_pref[0] << shift;
                for (int j = 1; j < k - 1; j++) blocks[i + j] ^= block_pref[j] << shift | block_pref[j - 1] >> (64 - shift);
                blocks[i + k - 1] ^= last << shift | block_pref[k - 2] >> (64 - shift);
                blocks[i + k] ^= last >> (64 - shift);
            }
        };
        apply(0, len--);
        k -= last_mask == 1;
        last_mask = last_mask == 1 ? ~0ULL : last_mask >> 1;
        apply(n - len, len);
    }

    b = 0;
    for (int i = 0; i < m; i++) {
        auto mask = block_mask(i);
        for (int j = 0; j < 64 && (i << 6) + j < n; j++) s[(i << 6) + j] = '0' + ((mask >> j) & 1);
    }
    cout << s;
}
