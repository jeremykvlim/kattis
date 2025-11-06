#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int c = 1, n; cin >> n; c++) {
        cout << "Learning case " << c << "\n";

        vector<bool> known;
        unordered_map<string, int> compress;
        auto index = [&](string w) {
            for (char &c : w) c = tolower(c);
            if (compress.count(w)) return compress[w];
            known.emplace_back(false);
            return compress[w] = compress.size();
        };
        known[index("joe")] = true;

        vector<unordered_set<vector<int>, Hash>> kgrams(n + 1);
        vector<string> tokens;
        vector<int> indices;
        bool today = false;
        string token;
        while (cin >> token) {
            if (token == "*") {
                tokens.clear();
                indices.clear();
                today = true;
                continue;
            }
            if (token == "#") break;

            if (token.size() == 1 && (token[0] == '.' || token[0] == ',' || token[0] == ':' || token[0] == ';' || token[0] == '?' || token[0] == '!')) {
                if (!indices.empty()) {
                    if (today)
                        for (int k = 2; k <= min(n, (int) indices.size()); k++) {
                            vector<int> seq(k);
                            for (int i = 0; i + k <= indices.size(); i++) {
                                for (int j = 0; j < k; j++) seq[j] = indices[i + j];
                                if (!kgrams[k].count(seq)) {
                                    cout << "What does the sentence \"";
                                    for (int w = 0; w < tokens.size(); w++) {
                                        if (w) cout << " ";
                                        cout << tokens[w];
                                    }
                                    cout << "\" mean?\n";
                                    goto done;
                                }
                            }
                        }
                    done:;
                    for (int k = 2; k <= min(n, (int) indices.size()); k++) {
                        vector<int> seq(k);
                        for (int i = 0; i + k <= indices.size(); i++) {
                            for (int j = 0; j < k; j++) seq[j] = indices[i + j];
                            kgrams[k].emplace(seq);
                        }
                    }
                    tokens.clear();
                    indices.clear();
                }
                continue;
            }

            int i = index(token);
            if (!today) known[i] = true;
            else if (!known[i]) {
                known[i] = true;
                cout << "What does the word \"" << token << "\" mean?\n";
            }
            tokens.emplace_back(token);
            indices.emplace_back(i);
        }
        cout << "\n";
    }
}
