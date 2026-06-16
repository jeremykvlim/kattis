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
                System.exit(0);
            }

        System.out.println("Yes");
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
