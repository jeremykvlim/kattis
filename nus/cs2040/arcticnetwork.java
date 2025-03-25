import java.io.*;
import java.util.*;

public class arcticnetwork {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        int n = Integer.parseInt(br.readLine());
        while (n-- > 0) {
            var input = br.readLine().split(" ");
            int s = Integer.parseInt(input[0]), p = Integer.parseInt(input[1]);

            var outposts = new ArrayList<Pair<Integer, Integer>>(p);
            for (int i = 0; i < p; i++) {
                var line = br.readLine().split(" ");
                outposts.add(new Pair<>(Integer.parseInt(line[0]), Integer.parseInt(line[1])));
            }

            var pq = new PriorityQueue<Triple<Double, Integer, Integer>>();
            for (int i = 0; i < p; i++)
                for (int j = i + 1; j < p; j++)
                    pq.add(new Triple<>(Math.hypot(outposts.get(i).first - outposts.get(j).first, outposts.get(i).second - outposts.get(j).second), i, j));

            var output = 0.0;
            var visited = new boolean[p];
            var dsu = new DisjointSets(p);
            while (p > s && !pq.isEmpty()) {
                var e = pq.poll();

                if (!visited[e.second] || !visited[e.third] || dsu.find(e.second) != dsu.find(e.third)) {
                    visited[e.second] = visited[e.third] = true;
                    dsu.unite(e.second, e.third);
                    output = e.first;
                    p--;
                }
            }

            System.out.printf("%.2f\n", output);
        }
    }
    
    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }

    record Triple<T extends Comparable<T>, U extends Comparable<U>, V extends Comparable<V>>(T first, U second, V third) implements Comparable<Triple<T, U, V>> {
        @Override
        public int compareTo(Triple<T, U, V> t) {
            int cmp = first.compareTo(t.first);
            if (cmp != 0) return cmp;
            cmp = second.compareTo(t.second);
            return (cmp == 0) ? third.compareTo(t.third) : cmp;
        }
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
