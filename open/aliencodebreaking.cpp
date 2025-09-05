#include <bits/stdc++.h>
using namespace std;

constexpr int MOD = 1 << 20;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    string cipher;
    cin >> n >> x;
    cin.ignore();
    getline(cin, cipher);

    vector<int> f(MOD, 1);
    for (int i = 1; i < MOD; i++) f[i] = (33 * f[i - 1] + 1) % MOD;

    int sum = 0;
    for (int _ = 0, i = 0; _ < x; _++) {
        (sum += f[i]) %= MOD;
        (i += x) %= MOD;
    }

    string digits10;
    for (int _ = 0; _ < x; _++) {
        digits10 += to_string(sum);
        sum = (33 * sum + x) % MOD;
    }

    ofstream ofs("temp.txt");
    ofs << digits10;
    ofs.close();
    
    string digits27;
    auto p = popen("ruby -e 's=STDIN.read; puts s.to_i(10).to_s(27).upcase' < \"temp.txt\"", "r");
    array<char, MOD> buf;
    while (fgets(buf.data(), MOD, p)) digits27 += buf.data();
    pclose(p);
    remove("temp.txt");

    string text(n, ' ');
    for (int i = 0; i < n; i++) {
        char d = digits27[i];
        int r = ((cipher[i] == ' ' ? 26 : cipher[i] - 'A') + ('0' <= d && d <= '9' ? d - '0' : d - 'A' + 10)) % 27;
        if (r != 26) text[i] = (char) 'A' + r;
    }
    cout << text;
}
