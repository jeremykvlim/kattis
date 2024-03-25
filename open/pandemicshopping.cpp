#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    string a, b;
    vector<string> aisles(n);
    while (k--) {
        string id, dir;
        cin >> id >> dir;

        if (id == "A") a = dir;
        else if (id == "B") b = dir;
        else aisles[stoi(id) - 1] = dir;
    }

    auto check = [&](auto d1, auto d2) {
        return none_of(aisles.begin(), aisles.end(), [&](auto &d) {return !d.empty() && (((&d - &aisles[0]) & 1) ? d == d1 : d == d2);});
    };
    int c1 = check("W2E", "E2W"), c2 = check("E2W", "W2E"), c3 = aisles[0] != "W2E" && aisles[n - 1] != "E2W", c4 = aisles[0] != "E2W" && aisles[n - 1] != "W2E";

    auto count = 1LL << count_if(aisles.begin() + 1, aisles.end() - 1, [](auto d) {return d.empty();}), ways = 0LL;
    if (a.empty() && b.empty()) ways += (n & 1) * (c1 + c2) + count * (c3 + c4);
    else if (a.empty()) ways += (n & 1) * (b == "S2N" ? c1 : c2) + count * (b == "S2N" ? c4 : c3);
    else if (b.empty()) ways += (n & 1) * (a == "S2N" ? c1 : c2) + count * (a == "S2N" ? c3 : c4);
    else if (a == b) ways += (n & 1) * (a == "S2N" ? c1 : c2);
    else ways += count * (a == "S2N" ? c3 : c4);

    cout << ways;
}
