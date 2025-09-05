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

    string digits27, temp = "temp.txt", cmd = "ruby -e 's=STDIN.read; s=s.gsub(/\\s+/m,\"\"); puts s.to_i(10).to_s(27).upcase' < \"" + temp + "\"";
    ofstream ofs(temp);
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
    remove(temp.c_str());

    string text(n, ' ');
    for (int i = 0; i < n; i++) {
        char c = digits27[i];
        int w = ((cipher[i] == ' ' ? 26 : cipher[i] - 'A') + ('0' <= c && c <= '9' ? c - '0' : c - 'A' + 10)) % 27;
        if (w != 26) text[i] = (char) 'A' + w;
    }
    cout << text;
}
