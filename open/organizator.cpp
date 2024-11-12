#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

struct Hash {
    size_t operator()(const int &i) const {
        return 0ULL ^ (i + 0x9e3779b9 + (i << 6) + (i >> 2));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    gp_hash_table<int, int, Hash> members;
    int biggest = 0;
    while (n--) {
        int m;
        cin >> m;

        members[m]++;
        biggest = max(biggest, m);
    }

    auto finalists = -1LL;
    for (int i = 1; i <= biggest; i++) {
        auto curr = 0LL;
        for (int j = i; j <= biggest; j += i) curr += members[j];
        if (curr > 1) finalists = max(finalists, curr * i);
    }

    cout << finalists;
}
