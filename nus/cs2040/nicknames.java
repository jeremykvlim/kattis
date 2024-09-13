import java.io.*;
import java.util.*;

public class nicknames {
    static class Trie {
        static class TrieNode {
            int[] next = new int[26];
            int count = 0;

            TrieNode() {
                Arrays.fill(next, -1);
            }
        }

        ArrayList<TrieNode> T;

        Trie() {
            T = new ArrayList<>();
            T.add(new TrieNode());
        }

        void add(String s) {
            int node = 0;
            for (int i = 0; i < s.length(); i++) {
                int pos = s.charAt(i) - 'a';

                if (T.get(node).next[pos] == -1) {
                    T.get(node).next[pos] = T.size();
                    T.add(new TrieNode());
                }
                node = T.get(node).next[pos];
                T.get(node).count++;
            }
        }

        int occurrences(String s) {
            int node = 0;
            for (int i = 0; i < s.length(); i++) {
                int pos = s.charAt(i) - 'a';

                if (T.get(node).next[pos] == -1) return 0;
                node = T.get(node).next[pos];
            }

            return T.get(node).count;
        }
    }

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.out)));

        var trie = new Trie();
        int a = Integer.parseInt(br.readLine());
        for (int i = 0; i < a; i++) trie.add(br.readLine());

        int b = Integer.parseInt(br.readLine());
        for (int i = 0; i < b; i++) pw.println(trie.occurrences(br.readLine()));
        pw.flush();
    }
}
