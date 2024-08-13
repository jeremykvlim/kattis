import java.io.*;
import java.util.*;

public class virtualfriends {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            int f = Integer.parseInt(br.readLine()), i = 0;
            var dsu = new DisjointSet(2 * f + 1);
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

    static class DisjointSet {
        int[] sets, size;

        DisjointSet(int n) {
            sets = new int[n];
            size = new int[n];
            for (int i = 0; i < n; i++) {
                sets[i] = i;
                size[i] = 1;
            }
        }

        boolean unite(int p, int q) {
            int p_set = find(p), q_set = find(q);
            if (p_set != q_set) {
                sets[q_set] = p_set;
                size[p_set] += size[q_set];
                return true;
            }
            return false;
        }

        int find(int p) {
            return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
        }
    }
}
