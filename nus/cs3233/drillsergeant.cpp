#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    unordered_set<pair<int, int>, Hash> edges;
    while (m--) {
        int x, y;
        cin >> x >> y;

        edges.emplace(x, y);
    }

    int q;
    cin >> q;

    int sum = 0;
    map<int, int> masks{{0, 0}, {2e9, 0}};
    array<int, 4> discontentment{3, 32, 323, 3233};
    while (q--) {
        int d, z;
        cin >> d >> z;

        if (d == 1) {
            auto it_z = masks.emplace(z, 0).first, it_x = prev(it_z), it_y = next(it_z);
            auto &[_, mask_z] = *it_z;
            auto &[x, mask_x] = *it_x;
            auto &[y, mask_y] = *it_y;
            sum -= discontentment[mask_x] + discontentment[mask_y];

            mask_x &= 2;
            mask_y &= 1;
            if (edges.count(minmax(z, x))) {
                mask_x |= 1;
                mask_z |= 2;
            }
            if (edges.count(minmax(z, y))) {
                mask_y |= 2;
                mask_z |= 1;
            }
            sum += discontentment[mask_x] + discontentment[mask_y] + discontentment[mask_z];
        } else {
            auto it_z = masks.find(z), it_x = prev(it_z), it_y = next(it_z);
            auto &[_, mask_z] = *it_z;
            auto &[x, mask_x] = *it_x;
            auto &[y, mask_y] = *it_y;
            sum -= discontentment[mask_x] + discontentment[mask_y] + discontentment[mask_z];

            mask_x &= 2;
            mask_y &= 1;
            if (edges.count(minmax(x, y))) {
                mask_x |= 1;
                mask_y |= 2;
            }
            sum += discontentment[mask_x] + discontentment[mask_y];
            masks.erase(it_z);
        }
        cout << sum << "\n";
    }
}
