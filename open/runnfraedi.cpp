#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s1, s2;
    cin >> s1 >> s2;

    if (s1 == s2 || s2 == "0.0") {
        cout << "Veit ekki";
        exit(0);
    }

    int i = s1.find('.'), j = s2.find('.');
    if (i < j) {
        cout << "Runnun";
        exit(0);
    }
    s1.erase(i, 1);
    s2.erase(j, 1);

    for (int k = 0; k <= s2.size(); k++) {
        if (k == s2.size() || s1[k] > s2[k]) {
            cout << (s1[k] >= '5' && s1[k - 1] != '0' ? "Styfun" : "Veit ekki");
            exit(0);
        }
        if (s1[k] < s1[k]) {
            cout << "Runnun";
            exit(0);
        }
    }
}
