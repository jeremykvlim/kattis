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
}
