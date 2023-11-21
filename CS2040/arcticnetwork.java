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
            var sets = new int[p];
            var visited = new boolean[p];
            var pq = new PriorityQueue<Edge>();

            for (int i = 0; i < p; i++) {
                sets[i] = i;
                var line = br.readLine().split(" ");
                outposts[i] = new Pair(Integer.parseInt(line[0]), Integer.parseInt(line[1]));
            }

            for (int i = 0; i < p; i++)
                for (int j = i + 1; j < p; j++)
                    pq.add(new Edge(i, j, Math.hypot(outposts[i].first - outposts[j].first, outposts[i].second - outposts[j].second)));

            var output = 0.00;
            while (p > s && !pq.isEmpty()) {
                var e = pq.poll();
                if (!visited[e.source] || !visited[e.destination] || (find(e.source, sets) != find(e.destination, sets))) {
                    p--;
                    visited[e.source] = visited[e.destination] = true;
                    sets[find(e.destination, sets)] = find(e.source, sets);
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

    static int find(int p, int[] sets) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
    }
}
