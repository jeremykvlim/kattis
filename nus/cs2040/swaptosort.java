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
