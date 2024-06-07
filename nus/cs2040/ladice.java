import java.io.*;

public class ladice {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), l = Integer.parseInt(input[1]);
        var dsu = new DisjointSet(l + 1);
        var count = new int[l + 1];
        while (n-- > 0) {
            var pair = br.readLine().split(" ");
            int a = Integer.parseInt(pair[0]), b = Integer.parseInt(pair[1]), a_set = dsu.find(a), b_set = dsu.find(b);

            if (dsu.unite(a, b)) count[a_set] += count[b_set];
            if (count[a_set] == dsu.size[a_set]) pw.println("SMECE");
            else {
                count[a_set]++;
                pw.println("LADICA");
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
