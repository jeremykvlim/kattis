import java.io.*;
import java.util.*;

public class virtualfriends {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            int f = Integer.parseInt(br.readLine()), i = 0;
            var dsu = new DisjointSets(2 * f + 1);
            var compress = new HashMap<String, Integer>();

            while (f-- > 0) {
                var friends = br.readLine().split(" ");

                for (var fr : friends)
                    if (!compress.containsKey(fr)) compress.put(fr, i++);

                int a = compress.get(friends[0]), b = compress.get(friends[1]);
                dsu.unite(a, b);
                pw.println(dsu.size(a));
            }
        }
        pw.flush();
    }

    static class DisjointSets {
        int[] sets;

        DisjointSets(int n) {
            sets = new int[n];
            for (int i = 0; i < n; i++) sets[i] = -1;
        }

        boolean unite(int u, int v) {
            u = find(u);
            v = find(v);
            if (u == v) return false;

            if (sets[u] > sets[v]) {
                int temp = u;
                u = v;
                v = temp;
            }
            sets[u] += sets[v];
            sets[v] = u;
            return true;
        }

        int size(int v) {
            return -sets[find(v)];
        }

        int find(int v) {
            while (sets[v] >= 0) {
                int p = sets[v];
                if (sets[p] >= 0) sets[v] = sets[p];
                v = p;
            }
            return v;
        }
    }
}