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
            for (int i = 0; i < n; i++) sets[i] = i;
        }

        boolean unite(int u, int v) {
            u = find(u);
            v = find(v);
            if (u != v) {
                sets[v] = u;
                return true;
            }
            return false;
        }

        int find(int v) {
            return sets[v] == v ? v : (sets[v] = find(sets[v]));
        }
    }
}
