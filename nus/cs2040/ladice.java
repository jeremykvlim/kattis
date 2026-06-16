import java.io.*;

public class ladice {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), l = Integer.parseInt(input[1]);
        var dsu = new DisjointSets(l + 1);
        var count = new int[l + 1];

        while (n-- > 0) {
            var pair = br.readLine().split(" ");
            int a = Integer.parseInt(pair[0]), b = Integer.parseInt(pair[1]);

            var p = dsu.unite(a, b);
            int big = p.first(), small = p.second();
            if (small != -1) count[big] += count[small];
            if (count[big] == dsu.size(big)) pw.println("SMECE");
            else {
                count[big]++;
                pw.println("LADICA");
            }
        }

        pw.flush();
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }

    static class DisjointSets {
        int[] sets;

        DisjointSets(int n) {
            sets = new int[n];
            for (int i = 0; i < n; i++) sets[i] = -1;
        }

        Pair<Integer, Integer> unite(int u, int v) {
            u = find(u);
            v = find(v);
            if (u == v) return new Pair<>(u, -1);

            if (sets[u] > sets[v]) {
                int temp = u;
                u = v;
                v = temp;
            }
            sets[u] += sets[v];
            sets[v] = u;
            return new Pair<>(u, v);
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