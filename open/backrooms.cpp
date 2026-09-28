#include <bits/stdc++.h>
using namespace std;

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    T q = a / b, r = a - q * b;
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

template <typename T>
struct GaussianInteger {
    T a, b;

    GaussianInteger(T a = 0, T b = 0) : a(a), b(b) {}

    GaussianInteger operator-() const {
        return {-a, -b};
    }

    GaussianInteger operator~() const {
        return {-b, a};
    }

    GaussianInteger operator+(const GaussianInteger &z) const {
        return {a + z.a, b + z.b};
    }

    GaussianInteger operator-(const GaussianInteger &z) const {
        return {a - z.a, b - z.b};
    }

    GaussianInteger operator*(const GaussianInteger &z) const {
        return {a * z.a - b * z.b, a * z.b + b * z.a};
    }

    GaussianInteger operator/(const GaussianInteger &z) const {
        T N = z.norm(), x = a * z.a + b * z.b, y = b * z.a - a * z.b;
        auto round_div = [N](T a) {
            T q = a / N, r = a % N;
            if (r >= (N + 1) / 2) q++;
            if (r <= -(N + 1) / 2) q--;
            return q;
        };
        return {round_div(x), round_div(y)};
    }

    GaussianInteger operator%(const GaussianInteger &z) const {
        return *this - *this / z * z;
    }

    GaussianInteger & operator+=(const GaussianInteger &z) {
        return *this = *this + z;
    }

    GaussianInteger & operator-=(const GaussianInteger &z) {
        return *this = *this - z;
    }

    GaussianInteger & operator*=(const GaussianInteger &z) {
        return *this = *this * z;
    }

    GaussianInteger & operator/=(const GaussianInteger &z) {
        return *this = *this / z;
    }

    GaussianInteger & operator%=(const GaussianInteger &z) {
        return *this = *this % z;
    }

    bool operator==(const GaussianInteger &) const = default;

    GaussianInteger conj() const {
        return {a, -b};
    }

    T norm() const {
        return a * a + b * b;
    }

    template <typename U>
    static GaussianInteger pow(GaussianInteger base, U exponent) {
        GaussianInteger value = 1;
        while (exponent) {
            if (exponent & 1) value *= base;
            base *= base;
            exponent >>= 1;
        }
        return value;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C;
    cin >> R >> C;

    vector<string> grid(R);
    for (int i = R - 1; ~i; i--) cin >> grid[i];

    auto index = [&](int r, int c) {
        return r * C + c;
    };

    int n = R * C;
    vector<int> basis_id(n, -1);
    vector<array<long long, 3>> bases;
    vector<GaussianInteger<long long>> dist(n);
    vector<int> dr{1, 0, -1, 0}, dc{0, 1, 0, -1};
    queue<int> q;
    for (int src = 0; src < n; src++) {
        int sr = src / C, sc = src % C;
        if (grid[sr][sc] != '#' && !~basis_id[src]) {
            bases.push_back({0, 0, 0});
            int i = bases.size() - 1;
            basis_id[src] = i;
            q.emplace(src);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                int r = v / C, c = v % C;
                for (int k = 0; k < 4; k++) {
                    int y = r + dr[k], x = c + dc[k];
                    GaussianInteger<long long> step{0, 0};
                    if (y < 0) {
                        y += R;
                        step.b--;
                    } else if (y >= R) {
                        y -= R;
                        step.b++;
                    }
                    if (x < 0) {
                        x += C;
                        step.a--;
                    } else if (x >= C) {
                        x -= C;
                        step.a++;
                    }

                    if (grid[y][x] != '#') {
                        int u = index(y, x);
                        if (!~basis_id[u]) {
                            basis_id[u] = i;
                            q.emplace(u);
                            dist[u] = dist[v] + step;
                        } else {
                            auto &[d, e, f] = bases[i];
                            auto [a, b] = dist[v] + step - dist[u];

                            if (!a) {
                                f = gcd(f, b);
                                continue;
                            }

                            if (a < 0) {
                                a = -a;
                                b = -b;
                            }
                            if (!d) {
                                d = a;
                                e = b;
                                continue;
                            }

                            auto [g, s, t] = extended_gcd(d, a);
                            f = gcd(f, (b * d - e * a) / g);
                            e = s * e + t * b;
                            d = g;

                            if (f) {
                                f = abs(f);
                                e = (e % f + f) % f;
                            }
                        }
                    }
                }
            }
        }
    }

    int Q;
    cin >> Q;

    while (Q--) {
        long long sx, sy, gx, gy;
        cin >> sx >> sy >> gx >> gy;

        int s = index(sy % R, sx % C), g = index(gy % R, gx % C);
        if (basis_id[s] != basis_id[g]) {
            cout << "No\n";
            continue;
        }

        GaussianInteger<long long> shift{gx / C - sx / C, gy / R - sy / R};
        shift += dist[s] - dist[g];
        auto [d, e, f] = bases[basis_id[s]];
        if (!d) {
            if (!shift.a && (f ? !(shift.b % f) : !shift.b)) cout << "Yes\n";
            else cout << "No\n";
            continue;
        }

        if (shift.a % d) {
            cout << "No\n";
            continue;
        }

        shift.b -= shift.a / d * e;
        if (f ? !(shift.b % f) : !shift.b) cout << "Yes\n";
        else cout << "No\n";
    }
}