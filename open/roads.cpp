#include <bits/stdc++.h>
using namespace std;

pair<int, int> process(int &i, string &decree) {
    i += 2;
    pair<int, int> roads;
    if (decree[i - 2] == 'V') return {1, 1};
    else if (decree[i - 2] == 'U') {
        auto r1 = process(i, decree), r2 = process(i, decree);
        roads = {r1.first + r2.first, max(r1.second, r2.second)};
    } else if (decree[i - 2] == 'C') {
        roads = process(i, decree);
        swap(roads.first, roads.second);
    }

    return roads;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string decree;
    while (getline(cin, decree)) {
        int i = 0;
        cout << process(i, decree).first << "\n";
    }
}
