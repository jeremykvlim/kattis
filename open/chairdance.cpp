#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
}

template <typename T>
T inv(T a, T mod) {
    T x = 0, y = 1, m = mod;
    while (a) {
        T t = m / a;
        m -= t * a;
        swap(a, m);
        x -= t * y;
        swap(x, y);
    }

    return (x + mod) % mod;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int sum = 0, prod = 1, remaining = n, step = 1;
    vector<int> players(n);
    iota(players.begin(), players.end(), 0);
    while (m--) {
        char c;
        int x;
        cin >> c >> x;

        if (c == '+') sum = (sum + x) % n;
        else if (c == '*') {
            int g = __gcd(x, remaining);
            if (g == 1) {
                sum = mul(sum, x, n);
                prod = mul(prod, x, remaining);
                continue;
            }

            vector<int> dist(n, 1e9), temp(n, -1);
            for (int i = 0; i < n; i++)
                if (players[i] != -1) {
                    int l = (mul(i, prod, n) + sum) % n, r = mul(l, x, n), d = (r - l + n) % n;
                    if (dist[r] > d) {
                        dist[r] = d;
                        temp[r] = players[i];
                    }
                }
            players = temp;

            auto it = find_if(players.begin(), players.end(), [](int p) { return ~p; });
            sum = it - players.begin();
            prod = 1;
            remaining /= g;
            step *= g;
            rotate(players.begin(), it, players.end());
        } else {
            x = (x - sum + n) % n;
            if (x % step) {
                cout << "-1\n";
                continue;
            }
            x = mul(x / step, inv(prod, remaining), remaining);
            cout << (players[x * step] ? players[x * step] : n) << "\n";
        }
    }

}
