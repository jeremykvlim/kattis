#include <bits/stdc++.h>
using namespace std;

bool cut1(string &s) {
    auto temp = s;
    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

    int len = s.size(), j = -1;
    for (int i = 0; i < len; i++)
        if (temp[i] == temp[(i + 1) % len] && s[i] != s[(i + 1) % len]) {
            j = i;
            break;
        }
    if (j == -1) return false;

    string r;
    for (int i = 0; i < len; i++)
        if (i != j && i != (j + 1) % len) r += s[i];

    s = r;
    return true;
}

bool cut2(string &s, int &arm) {
    auto it = adjacent_find(s.begin(), s.end());
    if (it == s.end()) return false;

    s.erase(it, it + 2);
    arm++;
    return true;
}

bool cut3(string &s, int &leg) {
    if (s.size() < 4) return false;

    auto temp = s;
    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

    int len = s.size(), j = -1;
    for (int i = 0; i < len; i++)
        if (temp[i] == temp[(i + 2) % len] &&
            temp[(i + 1) % len] == temp[(i + 3) % len] &&
            s[i] != s[(i + 2) % len] &&
            s[(i + 1) % len] != s[(i + 3) % len]) {
            j = i;
            break;
        }
    if (j == -1) return false;

    string r;
    for (int i = 0; i < len; i++)
        if (i != j && i != (j + 1) % len && i != (j + 2) % len && i != (j + 3) % len) r += s[i];

    s = r;
    leg++;
    return true;
}

void cut4(string &s) {
    auto temp = s;
    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

    mt19937 rng(random_device{}());
    int len = s.size(), j = -1, k = rng() % len;
    for (int i = 0; i < len; i++)
        if (i != k && temp[i] == temp[k]) {
            j = i;
            break;
        }

    if (j < k) swap(j, k);
    int l = rng() % (j - k);
    auto s1 = s.substr(k + 1, l) + (char) tolower(s[k]) + s.substr(0, k), s2 = s.substr(j + 1) + (char) toupper(s[j]) + s.substr(k + l + 1, j - k - l - 1);
    if (s[j] == s[k]) {
        transform(s2.begin(), s2.end(), s2.begin(), [](char c) { return islower(c) ? toupper(c) : tolower(c); });
        reverse(s2.begin(), s2.end());
    }

    s = s1 + s2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    while (cin >> s && s != "END") {
        int arm = 0, leg = 0;

        while (!s.empty())
            if (!cut1(s) && !cut2(s, arm) && !cut3(s, leg)) cut4(s);

        if (arm && leg) cout << arm + 2 * leg << " arms\n";
        else if (arm) cout << arm << (arm == 1 ? " arm\n" : " arms\n");
        else if (leg) cout << leg << (leg == 1 ? " leg\n" : " legs\n");
        else cout << "none\n";
    }
}
