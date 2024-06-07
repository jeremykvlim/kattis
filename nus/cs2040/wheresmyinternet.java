import java.io.*;

public class wheresmyinternet {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

        var dsu = new DisjointSet(n + 1);
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

    static class DisjointSet {
        int[] sets;

        DisjointSet(int n) {
            sets = new int[n];
            for (int i = 0; i < n; i++) sets[i] = i;
        }

        boolean unite(int p, int q) {
            int p_set = find(p), q_set = find(q);
            if (p_set != q_set) {
                sets[q_set] = p_set;
                return true;
            }
            return false;
        }

        int find(int p) {
            return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
        }
    }
}
