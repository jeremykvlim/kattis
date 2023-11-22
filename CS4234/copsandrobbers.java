import java.io.*;
import java.util.*;

public class copsandrobbers {
    static int count = 0;
    static Edge[] edge;
    static int[] curr, depth, start;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]), size = 2*m*n + 1;
        var calirado = new char[m + 1][n + 1];
        edge = new Edge[10*m*n + 2];
        curr = new int[size + 1];
        depth = new int[size + 1];
        start = new int[size + 1];

        for (int i = 1; i <= m; i++) {
            var line = br.readLine();
            for (int j = 1; j <= n; j++) calirado[i][j] = line.charAt(j - 1);
        }

        int[] cost = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray(), dx = {-1, 0, 1, 0}, dy = {0, 1, 0, -1};

        int inflow, outflow, source = 0;
        Arrays.fill(start, -1);
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++) {
                inflow = (i - 1)*n + j;
                outflow = inflow + m*n;
                if (calirado[i][j] == 'B') add_edge(source, inflow, Integer.MAX_VALUE);

                add_edge(inflow, outflow, calirado[i][j] >= 'a' && calirado[i][j] <= 'z' ? cost[calirado[i][j] - 'a'] : Integer.MAX_VALUE);
            }

        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                for (int k = 0; k < 4; k++) {
                    int x = i + dx[k], y = j + dy[k];
                    inflow = (x - 1)*n + y;
                    outflow = (i - 1)*n + j + m*n;

                    add_edge(outflow, x >= 1 && x <= m && y >= 1 && y <= n ? inflow : size, Integer.MAX_VALUE);
                }

        int max_flow = dinic(source, size);
        System.out.println(max_flow != Integer.MAX_VALUE ? max_flow : "-1");
    }

    static void add_edge(int u, int v, int c) {
        edge[count] = new Edge(c, v, start[u]);
        start[u] = count++;
        edge[count] = new Edge(0, u, start[v]);
        start[v] = count++;
    }


    static int dinic(int source, int size) {
        int sum = 0;
        while (bfs(source, size)) {
            System.arraycopy(start, 0, curr, 0, start.length);
            sum += dfs(source, Integer.MAX_VALUE, size);
        }
        return sum;
    }

    static boolean bfs(int source, int size) {
        Arrays.fill(depth, -1);
        var q = new ArrayDeque<Integer>();
        q.add(source);
        depth[source] = 0;

        while (!q.isEmpty()) {
            var v = q.poll();
            for (int i = start[v]; i != -1; i = edge[i].edge) {
                var u = edge[i].destination;
                if (depth[u] == -1 && edge[i].capacity > 0) {
                    q.add(u);
                    depth[u] = depth[v] + 1;
                    if (u == size) return true;
                }
            }
        }
        return false;
    }

    static int dfs(int v, int remaining, int size) {
        if (v == size || remaining == 0) return remaining;
        int flow = 0, f;
        for (int i = curr[v]; i != -1; i = edge[i].edge) {
            curr[v] = i;
            int u = edge[i].destination;
            if (depth[u] > depth[v] && edge[i].capacity > 0 && (f = dfs(u, Math.min(remaining, edge[i].capacity), size)) > 0) {
                edge[i].capacity -= f;
                edge[i ^ 1].capacity += f;
                remaining -= f;
                flow += f;
                if (remaining == 0) break;
            }
        }
        return flow;
    }

    static class Edge {
        int capacity, destination, edge;

        Edge(int c, int d, int e) {
            capacity = c;
            destination = d;
            edge = e;
        }
    }
}
