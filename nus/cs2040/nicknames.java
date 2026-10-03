import java.io.*;
import java.util.*;

public class nicknames {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var trie = new Trie();
        int a = Integer.parseInt(br.readLine());
        while (a-- > 0) trie.add(br.readLine());

        int b = Integer.parseInt(br.readLine());
        while (b-- > 0) pw.println(trie.occurrences(br.readLine()));
        pw.flush();
    }

    static class Trie {
        static class TrieNode {
            int[] next = new int[26];
            int count;

            TrieNode() {
                Arrays.fill(next, -1);
                count = 0;
            }
        }

        ArrayList<TrieNode> T;

        Trie() {
            T = new ArrayList<>();
        }

        void add(String s) {
            int v = 0;
            for (int i = 0; i < s.length(); i++) {
                int pos = s.charAt(i) - 'a';

                if (T.get(v).next[pos] == -1) {
                    T.add(new TrieNode());
                    T.get(v).next[pos] = T.size() - 1;
                }
                v = T.get(v).next[pos];
                T.get(v).count++;
            }
        }

        int occurrences(String s) {
            int v = 0;
            for (int i = 0; i < s.length(); i++) {
                int pos = s.charAt(i) - 'a';

                if (T.get(v).next[pos] == -1) return 0;
                v = T.get(v).next[pos];
            }

            return T.get(v).count;
        }
    }
}
