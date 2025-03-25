import java.io.*;
import java.util.*;

public class communicationssatellite {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var satellites = new ArrayList<Triple<Integer, Integer, Integer>>(n);
        for (int i = 0; i < n; i++) {
            var line = br.readLine().split(" ");
            satellites.add(new Triple<>(Integer.parseInt(line[0]), Integer.parseInt(line[1]), Integer.parseInt(line[2])));
        }

        var pq = new PriorityQueue<Triple<Double, Integer, Integer>>();
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                pq.add(new Triple<>(Math.hypot(satellites.get(i).first - satellites.get(j).first, satellites.get(i).second - satellites.get(j).second) - satellites.get(i).third - satellites.get(j).third, i, j));

        var sum = 0.0;
        var dsu = new DisjointSets(n);
        while (n > 1 && !pq.isEmpty()) {
            var e = pq.poll();
            var w = e.first;
            int u = e.second, v = e.third;

            if (dsu.find(u) != dsu.find(v)) {
                dsu.unite(u, v);
                sum += w;
                n--;
            }
        }
        pw.format("%.6f", sum);
        pw.flush();
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
