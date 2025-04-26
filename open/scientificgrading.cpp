#include <bits/stdc++.h>
using namespace std;

int sgn(vector<pair<__int128, __int128>> v) {
    sort(v.begin(), v.end(), [](auto p1, auto p2) { return p1.second > p2.second; });

    pair<__int128, __int128> p = {0, 0};
    auto &[s, e] = p;
    auto prev = v[0].second;
    for (auto [significand, exponent] : v) {
        if (s) e += prev - exponent;
        prev = exponent;

        while (e) {
            s *= 10;
            e--;

            if (abs(s) > (__int128) 1e20) goto done;
        }

        s += significand;
        if (!s) e = 0;
    }

    done:;
    return (s > 0) - (s < 0);
}

pair<__int128, __int128> read() {
    pair<__int128, __int128> values;
    auto &[significand, exponent] = values;

    string s;
    cin >> s;

    significand = s[1] - '0';
    for (int i = 3; i < 12; i++) {
        significand *= 10;
        significand += s[i] - '0';
    }
    if (s[0] == '-') significand *= -1;

    exponent = stoi((s.substr(13, s.size() - 13))) - 9;
    return values;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto x = read(), y = read(), sum = read(), diff = read(), product = read(), quotient = read();

    auto add = [&](pair<__int128, __int128> x, pair<__int128, __int128> y, pair<__int128, __int128> z) -> bool {
        int s1 = sgn({x, y}), s2 = sgn({{-x.first, x.second}, {-y.first, y.second}, z});
        if (!s1) return !sgn({z});

        return sgn({{s1 * -x.first, x.second}, {s1 * -y.first, y.second}, {s2 * -x.first, x.second + 9}, {s2 * -y.first, y.second + 9}, {s2 * z.first, z.second + 9}}) < 0
            && sgn({{-1, 0}, {s2 * -x.first, x.second + 9}, {s2 * -y.first, y.second + 9}, {s2 * z.first, z.second + 9}}) < 0;
    };

    auto sub = [&](pair<__int128, __int128> x, pair<__int128, __int128> y, pair<__int128, __int128> z) -> bool {
        return add(x, {-y.first, y.second}, z);
    };

    auto mul = [&](pair<__int128, __int128> x, pair<__int128, __int128> y, pair<__int128, __int128> z) -> bool {
        return add({x.first * y.first, x.second + y.second}, {0, 0}, z);
    };

    auto div = [&](pair<__int128, __int128> x, pair<__int128, __int128> y, pair<__int128, __int128> z) -> bool {
        int s1 = (x.first > 0) - (x.first < 0), s2 = (y.first > 0) - (y.first < 0), s3 = sgn({{-x.first, x.second}, {y.first * z.first, y.second + z.second}});

        return sgn({{s1 * -x.first, x.second}, {s3 * y.first * z.first, y.second + z.second + 9}, {s3 * -x.first, x.second + 9}}) < 0
            && sgn({{s2 * -y.first, y.second}, {s3 * y.first * z.first, y.second + z.second + 9}, {s3 * -x.first, x.second + 9}}) < 0;
    };

    auto grade = [](bool correct) -> string {
        return correct ? "Correct\n" : "Incorrect\n";
    };

    cout << grade(add(x, y, sum)) << grade(sub(x, y, diff)) << grade(mul(x, y, product)) << grade(div(x, y, quotient));
}
