import java.io.*;
import java.util.*;

public class dominos {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            var inputs = br.readLine().split(" ");
            int n = Integer.parseInt(inputs[0]), m = Integer.parseInt(inputs[1]);

            var adjList = new ArrayList[n + 1];
            for (int i = 1; i <= n; i++) adjList[i] = new ArrayList<Integer>();

            var indegree = new int[n + 1];
            while (m-- > 0) {
                var edge = br.readLine().split(" ");
                int x = Integer.parseInt(edge[0]), y = Integer.parseInt(edge[1]);

                adjList[x].add(y);
                indegree[y]++;
            }

            var visited = new boolean[n + 1];
            int count = 0;
            for (int i = 1; i <= n; i++) {
                if (dfs(visited, i, i, adjList)) indegree[i]--;
                if (indegree[i] == 0) count++;
            }

            System.out.println(count);
        }
    }

    static boolean dfs(boolean[] visited, int v, int root, ArrayList[] adjList) {
        visited[v] = true;

        for (var u : adjList[v]) {
            if ((int) u == root) return true;
            else if (!visited[(int) u]) return dfs(visited, (int) u, root, adjList);
        }

        return false;
    }
}
