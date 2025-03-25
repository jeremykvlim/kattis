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
                    if (!compress.containsKey(fr)) {
                        compress.put(fr, i);
                        dsu.sets[i] = i;
                        dsu.size[i++] = 1;
                    }

                int a = compress.get(friends[0]), b = compress.get(friends[1]);
                dsu.unite(a, b);
                pw.println(dsu.size[dsu.find(a)]);
            }
        }
        pw.flush();
    }

    static class DisjointSets {
        int[] sets, size;

        DisjointSets(int n) {
            sets = new int[n];
            size = new int[n];
            for (int i = 0; i < n; i++) {
                sets[i] = i;
                size[i] = 1;
            }
        }

        boolean unite(int u, int v) {
            u = find(u);
            v = find(v);
            if (u != v) {
                sets[v] = u;
                size[u] += size[v];
                return true;
            }
            return false;
        }

        int find(int v) {
            while (v != sets[v]) {
                sets[v] = sets[sets[v]];
                v = sets[v];
            }
            return v;
        }
    }
}
