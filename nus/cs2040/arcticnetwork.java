import java.io.*;
import java.util.*;

public class arcticnetwork {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        int n = Integer.parseInt(br.readLine());
        while (n-- > 0) {
            var input = br.readLine().split(" ");
            int s = Integer.parseInt(input[0]), p = Integer.parseInt(input[1]);

            var outposts = new Pair[p];
            for (int i = 0; i < p; i++) {
                var line = br.readLine().split(" ");
                outposts[i] = new Pair(Integer.parseInt(line[0]), Integer.parseInt(line[1]));
            }

            var pq = new PriorityQueue<Edge>();
            for (int i = 0; i < p; i++)
                for (int j = i + 1; j < p; j++)
                    pq.add(new Edge(i, j, Math.hypot(outposts[i].first - outposts[j].first, outposts[i].second - outposts[j].second)));

            var output = 0.0;
            var visited = new boolean[p];
            var dsu = new DisjointSet(p);
            while (p > s && !pq.isEmpty()) {
                var e = pq.poll();
                if (!visited[e.source] || !visited[e.destination] || dsu.find(e.source) != dsu.find(e.destination)) {
                    p--;
                    visited[e.source] = visited[e.destination] = true;
                    dsu.unite(e.source, e.destination);
                    output = e.weight;
                }
            }

            System.out.printf("%.2f\n", output);
        }
    }

    static class Pair {
        int first, second;

        Pair(int f, int s) {
            first = f;
            second = s;
        }
    }

    static class Edge implements Comparable<Edge>{
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
