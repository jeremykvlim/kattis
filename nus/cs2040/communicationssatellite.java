import java.io.*;
import java.util.*;

public class communicationssatellite {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        int n = Integer.parseInt(br.readLine());
        var satellites = new Triplet[n];
        for (int i = 0; i < n; i++) {
            var line = br.readLine().split(" ");
            satellites[i] = new Triplet(Integer.parseInt(line[0]), Integer.parseInt(line[1]), Integer.parseInt(line[2]));
        }

        var pq = new PriorityQueue<Edge>();
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                pq.add(new Edge(i, j, Math.hypot(satellites[i].first - satellites[j].first, satellites[i].second - satellites[j].second) - satellites[i].third - satellites[j].third));

        var sum = 0.0;
        var dsu = new DisjointSet(n);
        while (n > 1 && !pq.isEmpty()) {
            var e = pq.poll();
            
            if (dsu.find(e.source) != dsu.find(e.destination)) {
                n--;
                dsu.unite(e.source, e.destination);
                sum += e.weight;
            }
        }

        System.out.printf("%.6f", sum);
    }

    static class Triplet {
        int first, second, third;

        Triplet(int f, int s, int t) {
            first = f;
            second = s;
            third = t;
        }
    }

    static class Edge implements Comparable<Edge> {
        int source, destination;
        double weight;

        Edge(int src, int dest, double w) {
            source = src;
            destination = dest;
            weight = w;
        }

        @Override
        public int compareTo(Edge e) {
            return Double.compare(weight, e.weight);
        }
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
