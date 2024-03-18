#include <bits/stdc++.h>
using namespace std;

vector<vector<unsigned>> matmul(vector<vector<unsigned>> &a, vector<vector<unsigned>> &b) {
    vector<vector<unsigned>> c(33, vector<unsigned>(33, 0));
    for (int i = 0; i < 33; i++)
        for (int j = 0; j < 33; j++)
            for (int k = 0; k < 33; k++) c[i][j] += a[i][k] * b[k][j];

    return c;
}

vector<vector<unsigned>> matpow(vector<vector<unsigned>> a, long long n) {
    vector<vector<unsigned>> b(33, vector<unsigned>(33));
    for (int i = 0; i < 33; i++)
        for (int j = 0; j < 33; j++) b[i][j] = (i == j);

    for (; n; n >>= 1) {
        if (n & 1) b = matmul(a, b);
        a = matmul(a, a);
    }

    return b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<unsigned>> r(33, vector<unsigned>(33, 0)), temp(33, vector<unsigned>(33));
    for (int i = 0; i < k; i++) cin >> r[0][i];
    r[0][32] = 1;

    for (int i = 0; i < 33; i++)
        for (int j = 0; j < 33; j++) temp[i][j] = (i == j);
    stack<pair<vector<vector<unsigned>>, long long>> st;
    st.emplace(temp, 1);
    while (n--) {
        string s;
        cin >> s;

        if (s == "add") {
            int x, y, z;
            char _;
            cin >> _ >> x >> _ >> _ >> y >> _ >> _ >> z;

            for (int i = 0; i < 33; i++)
                for (int j = 0; j < 33; j++) temp[i][j] = (i == j);
            temp[x][x]--;
            temp[y][x]++;
            temp[z][x]++;

            st.top().first = matmul(st.top().first, temp);
        } else if (s == "addi") {
            int x, y;
            unsigned c;
            char _;
            cin >> _ >> x >> _ >> _ >> y >> _ >> c;

            for (int i = 0; i < 33; i++)
                for (int j = 0; j < 33; j++) temp[i][j] = (i == j);
            temp[x][x]--;
            temp[y][x]++;
            temp[32][x] += c;

            st.top().first = matmul(st.top().first, temp);
        } else if (s == "move") {
            int x, y;
            char _;
            cin >> _ >> x >> _ >> _ >> y;

            for (int i = 0; i < 33; i++)
                for (int j = 0; j < 33; j++) temp[i][j] = (i == j);
            temp[x][x]--;
            temp[y][x]++;

            st.top().first = matmul(st.top().first, temp);
        } else if (s == "li") {
            int x;
            unsigned c;
            char _;
            cin >> _ >> x >> _ >> c;

            for (int i = 0; i < 33; i++)
                for (int j = 0; j < 33; j++) temp[i][j] = (i == j);
            temp[x][x]--;
            temp[32][x] += c;

            st.top().first = matmul(st.top().first, temp);
        } else if (s == "for") {
            unsigned c;
            cin >> c;

            for (int i = 0; i < 33; i++)
                for (int j = 0; j < 33; j++) temp[i][j] = (i == j);
            st.emplace(temp, c);
        } else {
            auto temp2 = matpow(st.top().first, st.top().second);

            st.pop();
            st.top().first = matmul(st.top().first, temp2);
        }
    }

    r = matmul(r, st.top().first);
    for (int i = 0; i < k; i++) cout << r[0][i] << " ";
}
