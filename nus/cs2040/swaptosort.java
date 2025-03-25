import java.io.*;

public class swaptosort {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), k = Integer.parseInt(input[1]);

        var dsu = new DisjointSets(n + 1);

        while (k-- > 0) {
            var pair = br.readLine().split(" ");
            int a = Integer.parseInt(pair[0]), b = Integer.parseInt(pair[1]);
            dsu.unite(a, b);
        }

        for (int i = 1, j = n; i <= n / 2; i++, j--)
            if (dsu.find(i) != dsu.find(j)) {
                System.out.println("No");
                return;
            }

        System.out.println("Yes");
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
