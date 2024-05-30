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
        auto key = 0ULL ^ (i + 0x9e3779b9 + (i << 6) + (i >> 2));
        return h(key);
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
