#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> aunt(n);
    for (int &i : aunt) cin >> i;
    cin.ignore();

    string s;
    while (getline(cin, s)) {
        istringstream iss(s);
        vector<int> p{istream_iterator<int>{iss}, istream_iterator<int>{}};

        vector<int> indices(n);
        for (int i = 0; i < n; i++) indices[p[i] - 1] = i;

        vector<int> lis;
        for (int i : indices) {
            int k = aunt[i] - 1;

            auto it = lower_bound(lis.begin(), lis.end(), k);
            if (it == lis.end()) lis.emplace_back(k);
            else *it = k;
        }
        cout << lis.size() << "\n";
    }
}
