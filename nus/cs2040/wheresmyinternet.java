import java.io.*;

public class wheresmyinternet {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

        var dsu = new DisjointSets(n + 1);
        while (m-- > 0) {
            var line = br.readLine().split(" ");
            int a = Integer.parseInt(line[0]), b = Integer.parseInt(line[1]);
            dsu.unite(a, b);
        }

        var connected = true;
        for (int i = 2, root = dsu.find(1); i <= n; i++)
            if (root != dsu.find(i)) {
                pw.println(i);
                connected = false;
            }

        if (connected) pw.println("Connected");
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
