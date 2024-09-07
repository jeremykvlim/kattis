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
    auto query = [&](int x, int y = 0, int z = 0) {
        if (!(0 <= x && x < n && 0 <= y && y < m && 0 <= z && z < k)) return 0;

        int &h = H[(x * m + y) * k + z];
        if (h) return h;

        cout << "? " << x + 1 << " " << y + 1 << " " << z + 1 << "\n" << flush;
        q--;

        cin >> h;
        return h;
    };

    auto guess = [](int x, int y = 0, int z = 0) {
        cout << "! " << x + 1 << " " << y + 1 << " " << z + 1 << "\n" << flush;
        exit(0);
    };

    auto _1D = [&]() {
        int fib1 = 0, fib2 = 1;
        while (fib2 < n) {
            fib1 += fib2;
            swap(fib1, fib2);
        }

        int l = -1, r = fib2 - 1, mid1, mid2;
        while (l + 1 < r) {
            fib2 -= fib1;
            mid1 = l + fib2, mid2 = r - fib2;

            if (query(mid1) > query(mid2)) r = mid2;
            else l = mid1;

            if (fib2 <= 1) break;
            swap(fib1, fib2);
        }

        guess(l + 1);
    };

    auto _2D = [&]() {
        pair<int, int> xy{-1, -1};
        auto &[x, y] = xy;
        
        int l_x = -1, r_x = n, l_y = -1, r_y = m, mid;
        while (l_x + 1 < r_x || l_y + 1 < r_y) {
            if (l_x + 1 < r_x) {
                mid = l_x + (r_x - l_x) / 2;

                for (int i = l_y; i <= r_y; i++)
                    if (query(mid, i, 0) > query(x, y, 0)) xy = {mid, i};

                if (x < mid) r_x = mid;
                else if (x > mid) l_x = mid;
                else {
                    if (query(mid - 1, y) > query(mid, y)) r_x = mid;
                    else if (query(mid + 1, y) > query(mid, y)) l_x = mid;
                    else guess(mid, y);
                }
            }

            if (l_y + 1 < r_y) {
                mid = l_y + (r_y - l_y) / 2;

                for (int i = l_x; i <= r_x; i++)
                    if (query(i, mid) > query(x, y)) xy = {i, mid};

                if (y < mid) r_y = mid;
                else if (y > mid) l_y = mid;
                else {
                    if (query(x, mid - 1) > query(x, mid)) r_y = mid;
                    else if (query(x, mid + 1) > query(x, mid)) l_y = mid;
                    else guess(x, mid);
                }
            }
        }

        guess(l_x, l_y);
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
                !update(x, y, z + 1) && !update(x, y, z - 1)) break;

        guess(x, y, z);
    };

    if (m == 1) _1D();
    else if (k == 1) _2D();
    else _3D();
}
