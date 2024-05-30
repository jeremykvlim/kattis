#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

struct Hash {
    static uint64_t h(uint64_t key) {
        auto hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(int i) const {
        auto key = 0 ^ (i + 0x9e3779b9 + (i << 6) + (i >> 2));
        return h(key);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, q;
    cin >> n >> m >> k >> q;

    gp_hash_table<int, int, Hash> H;
    auto query = [&](int x, int y, int z) {
        if (!(0 <= x && x < n && 0 <= y && y < m && 0 <= z && z < k)) return 0;

        int &h = H[(x * m + y) * k + z];
        if (h) return h;

        cout << "? " << x + 1 << " " << y + 1 << " " << z + 1 << "\n" << flush;
        q--;

        cin >> h;
        return h;
    };

    auto guess = [](int x, int y, int z) {
        cout << "! " << x + 1 << " " << y + 1 << " " << z + 1 << "\n" << flush;
        exit(0);
    };

    auto _1D = [&]() {
        int fib1 = 1, fib2 = 1;
        while (fib2 < n) {
            fib1 += fib2;
            swap(fib1, fib2);
        }

        int l = -1, r = fib2 - 1;
        while (fib1 > 1) {
            fib2 -= fib1;

            if (query(l + fib2, 0, 0) > query(r - fib2, 0, 0)) r -= fib2;
            else l += fib2;

            swap(fib1, fib2);
        }

        guess(l + 1, 0, 0);
    };

    auto _2D = [&]() {
        pair<int, int> xy{-1, -1};
        auto &[x, y] = xy;
        int l_x = -1, r_x = n, l_y = -1, r_y = m;
        while (l_x + 1 < r_x || l_y + 1 < r_y) {
            if (l_x + 1 < r_x) {
                int mid = l_x + (r_x - l_x) / 2;

                for (int i = l_y; i <= r_y; i++)
                    if (query(mid, i, 0) > query(x, y, 0)) xy = {mid, i};

                if (x < mid) r_x = mid;
                else if (x > mid) l_x = mid;
                else {
                    if (query(mid - 1, y, 0) > query(mid, y, 0)) r_x = mid;
                    else if (query(mid + 1, y, 0) > query(mid, y, 0)) l_x = mid;
                    else guess(mid, y, 0);
                }
            }

            if (l_y + 1 < r_y) {
                int mid = l_y + (r_y - l_y) / 2;

                for (int i = l_x; i <= r_x; i++)
                    if (query(i, mid, 0) > query(x, y, 0)) xy = {i, mid};

                if (y < mid) r_y = mid;
                else if (y > mid) l_y = mid;
                else {
                    if (query(x, mid - 1, 0) > query(x, mid, 0)) r_y = mid;
                    else if (query(x, mid + 1, 0) > query(x, mid, 0)) l_y = mid;
                    else guess(x, mid, 0);
                }
            }
        }

        guess(l_x, l_y, 0);
    };

    auto _3D = [&]() {
        int x = -1, y = -1, z = -1, half = q / 2;
        mt19937 rng(random_device{}());
        while (q > half) {
            int X = rng() % n, Y = rng() % m, Z = rng() % k;
            if (query(X, Y, Z) > query(x, y, z)) tie(x, y, z) = tie(X, Y, Z);
        }

        auto update = [&](int X, int Y, int Z) -> bool {
            if (query(X, Y, Z) > query(x, y, z)) {
                tie(x, y, z) = tie(X, Y, Z);
                return true;
            }

            return false;
        };

        while (q)
            if (!update(x + 1, y, z) && !update(x - 1, y, z) &&
                !update(x, y + 1, z) && !update(x, y - 1, z) &&
                !update(x, y, z + 1) && !update(x, y, z - 1)) guess(x, y, z);

        guess(x, y, z);
    };

    if (m == 1 && k == 1) _1D();
    else if (k == 1) _2D();
    else _3D();
}
