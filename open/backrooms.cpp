#include <bits/stdc++.h>
using namespace std;

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        if constexpr (requires(T z) { z.real(); z.imag(); }) {
            auto div = [&](const T &x, const T &y) -> T {
                T numer = x * conj(y);
                auto denom = norm(y);
                auto round_div = [&](auto part) {
                    return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
                };
                return (T) {round_div(numer.real()), round_div(numer.imag())};
            };
            T q = div(x, y), r = x - q * y;
            return {q, r};
        } else {
            T q = x / y, r = x - q * y;
            return {q, r};
        }
    };

    auto [q, r] = divmod(a, b);
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

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
    vector<int> component(n, -2);
    for (int y = 0; y < R; y++)
        for (int x = 0; x < C; x++)
            if (grid[y][x] == '.') component[index(y, x)] = -1;

    auto cross = [&](auto &a, auto &b) {
        return (conj(a) * b).imag();
    };

    auto normalize = [&](complex<long long> &Z) {
        if (Z == complex<long long>{0, 0}) return Z;
        if (Z.real() < 0 || !Z.real() && Z.imag() < 0) Z = -Z;
        return Z;
    };

    int count = 0;
    complex<long long> zero{0, 0};
    vector<array<complex<long long>, 2>> states;
    vector<complex<long long>> base(n, zero);
    vector<int> dr{1, 0, -1, 0}, dc{0, 1, 0, -1};
    for (int s = 0; s < n; s++) {
        if (component[s] != -1) continue;
        component[s] = count;
        states.push_back({{zero, zero}});
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int r = v / C, c = v % C;
            for (int k = 0; k < 4; k++) {
                int y = r + dr[k], x = c + dc[k], a = 0, b = 0;
                if (y < 0) {
                    y += R;
                    b--;
                } else if (y >= R) {
                    y -= R;
                    b++;
                }
                if (x < 0) {
                    x += C;
                    a--;
                } else if (x >= C) {
                    x -= C;
                    a++;
                }
                if (grid[y][x] != '.') continue;

                int u = index(y, x);
                complex<long long> step(a, b);
                if (component[u] == -1) {
                    component[u] = count;
                    base[u] = base[v] + step;
                    q.emplace(u);
                } else {
                    auto cycle = base[v] - base[u] + step;
                    if (cycle != zero) {
                        auto &[z1, z2] = states[count];

                        if (z1 == zero) {
                            z1 = cycle;
                            normalize(z1);
                        } else if (z2 == zero) {
                            if (!cross(z1, cycle)) {
                                z1 = extended_gcd(z1, cycle)[0];
                                normalize(z1);
                            } else {
                                z2 = cycle;
                                if (cross(z1, z2) < 0) z2 = -z2;
                            }
                        } else {
                            auto det = cross(z1, z2);

                            auto [g1, x1, y1] = extended_gcd(det, cross(z1, cycle));
                            z2 = z2 * x1 + cycle * y1;
                            if (cross(z1, z2) < 0) z2 = -z2;

                            auto det1 = cross(z1, z2);
                            if (det1 < 0) det1 = -det1;
                            if (det1 != g1)
                                if (det1) z2 /= det1 / g1;

                            auto [g2, x2, y2] = extended_gcd(g1, cross(cycle, z2));
                            z1 = z1 * x2 + cycle * y2;
                            if (cross(z1, z2) < 0) z2 = -z2;

                            auto det2 = cross(z1, z2);
                            if (det2 < 0) det2 = -det2;
                            if (det2 != g2)
                                if (det2) z2 /= det2 / g2;

                            if (cross(z1, z2) < 0) z2 = -z2;
                        }
                    }
                }
            }
        }
        count++;
    }

    int q;
    cin >> q;

    while (q--) {
        long long sx, sy, gx, gy;
        cin >> sx >> sy >> gx >> gy;

        auto convert = [&](long long x, long long y) {
            return make_pair(index(y % R, x % C), complex<long long>(x / C, y / R));
        };
        auto [c_s, cell_s] = convert(sx, sy);
        auto [c_g, cell_g] = convert(gx, gy);
        if (component[c_s] != component[c_g]) {
            cout << "No\n";
            continue;
        }

        auto shift = cell_g - cell_s - base[c_g] + base[c_s];
        auto [z1, z2] = states[component[c_s]];
        bool reach;
        if (z1 == zero) reach = shift == zero;
        else if (z2 == zero) {
            if (cross(shift, z1)) reach = false;
            else {
                if (z1.real()) {
                    if (shift.real() % z1.real()) reach = false;
                    else reach = shift.imag() == z1.imag() * shift.real() / z1.real();
                } else {
                    if (shift.imag() % z1.imag()) reach = false;
                    else reach = shift.real() == z1.real() * shift.imag() / z1.imag();
                }
            }
        } else {
            auto det = cross(z1, z2);
            reach = !(cross(z1, shift) % det) && !(cross(shift, z2) % det);
        }

        if (reach) cout << "Yes\n";
        else cout << "No\n";
    }
}
