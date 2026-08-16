import java.io.*;
import java.util.*;

public class graphds1 {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int n = input[0], m = input[1], t = input[2];

        var indegree = new int[n];
        var outdegree = new int[n];
        var adjListUndirected = new ArrayList[n];
        var adjListDirected = new ArrayList[n];
        for (int i = 0; i < n; i++) {
            adjListUndirected[i] = new ArrayList<>();
            adjListDirected[i] = new ArrayList<>();
        }

        for (int i = 0; i < m; i++) {
            input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            int u = input[0], v = input[1];

            adjListUndirected[u].add(v);
            adjListUndirected[v].add(u);
            if (t == 2) {
                adjListDirected[u].add(v);
                indegree[v]++;
                outdegree[u]++;
            }
        }

        int a = 0, b = 1, c = 1, d = 0;
        if (m != (long) n * (n - 1) / (t == 1 ? 2 : 1)) b = 0;

        var q = new ArrayDeque<Integer>();
        if (t == 1) {
            var color = new int[n];
            Arrays.fill(color, -1);
            int components = 0;

            for (int s = 0; s < n; s++)
                if (color[s] == -1) {
                    components++;
                    color[s] = 0;
                    q.add(s);
                    while (!q.isEmpty()) {
                        int v = q.poll();

                        for (var u : adjListUndirected[v])
                            if (color[(int) u] == -1) {
                                color[(int) u] = color[v] ^ 1;
                                q.add((int) u);
                            } else if (color[(int) u] == color[v]) c = 0;
                    }
                }
            if (components == 1 && m == n - 1) a = 1;
            if (m == 0) d = 1;
        } else {
            int roots = 0;
            for (int i = 0; i < n; i++) {
                if (indegree[i] == 0) {
                    roots++;
                    q.add(i);
                }
                if (indegree[i] > 0 && outdegree[i] > 0) c = 0;
            }

            int count = 0;
            while (!q.isEmpty()) {
                int v = q.poll();

                count++;

                for (var u : adjListDirected[v])
                    if (--indegree[(int) u] == 0) q.add((int) u);
            }

            if (count == n) d = 1;
            if (m == n - 1 && roots == 1 && d == 1) a = 1;
        }

        System.out.println(a + " " + b + " " + c + " " + d);
    }
}