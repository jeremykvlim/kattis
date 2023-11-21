import java.io.*;

public class nicknames {
    static class TrieNode {
        TrieNode[] children = new TrieNode[26];
        int count = 0;
    }

    static void insert(TrieNode root, String str) {
        var node = root;
        for (int i = 0; i < str.length(); i++) {
            int index = str.charAt(i) - 'a';
            if (node.children[index] == null)
                node.children[index] = new TrieNode();
            node = node.children[index];
            node.count++;
        }
    }

    static int search(TrieNode root, String str) {
        var node = root;
        for (int i = 0; i < str.length(); i++) {
            int index = str.charAt(i) - 'a';
            node = node.children[index];
            if (node == null) return 0;
        }
        return node.count;
    }

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.out)));
        var root = new TrieNode();

        int a = Integer.parseInt(br.readLine());
        for (int i = 0; i < a; i++) insert(root, br.readLine());

        int b = Integer.parseInt(br.readLine());
        for (int i = 0; i < b; i++) pw.println(search(root, br.readLine()));
        pw.flush();
    }
}
