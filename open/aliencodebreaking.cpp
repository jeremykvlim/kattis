#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    string cipher;
    cin >> n >> x;
    cin.ignore();
    getline(cin, cipher);

    const int MOD = 1 << 20;
    vector<int> f(MOD, 1);
    for (int i = 1; i < MOD; i++) f[i] = (33 * f[i - 1] + 1) % MOD;

    int sum = 0, j = 0;
    for (int i = 0; i < x; i++) {
        (sum += f[j]) %= MOD;
        (j += x) %= MOD;
    }

    string digits10;
    for (int i = 0; i < x; i++) {
        digits10 += to_string(sum);
        sum = (33 * sum + x) % MOD;
    }

    string digits27, cmd = "ruby -e 's=STDIN.read; puts s.to_i(10).to_s(27).upcase' < \"temp.txt\"";
    ofstream ofs("temp.txt");
    ofs << digits10;
    ofs.close();
    auto p = popen(cmd.c_str(), "r");
    array<char, MOD> buf;
    for (;;) {
        int k = fread(buf.data(), 1, buf.size(), p);
        if (!k) break;
        digits27.append(buf.data(), k);
    }
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
