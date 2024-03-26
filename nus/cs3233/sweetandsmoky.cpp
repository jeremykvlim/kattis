#include <bits/stdc++.h>
using namespace std;

string word(int n, int s) {
    if (!n || (n == 1 && s != 4) || (n == 2 && s != 29)) return "IMPOSSIBLE";
    if (n <= 2) return string(n, 'a');

    int j = -1;
    for (int i = 0; i <= n && j == -1; i++)
        if (4 * n + 16 * i == s && !(n - i < i + 1 || n - i > 2 * i + 1)) j = i;
    if (j == -1) return "IMPOSSIBLE";
    
    string word = "ab";
    int count = n - 1 - 2 * j--;
    while (j--) word += count-- > 0 ? "aab" : "ab";
    return word + (count > 0 ? "aa" : "a");
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s;
    cin >> n >> s;

    auto w = word(n, s);
    cout << (w != "IMPOSSIBLE" ? w : (w = word(n - 1, s - 25)) == "IMPOSSIBLE" ? w : "a" + w);
}
