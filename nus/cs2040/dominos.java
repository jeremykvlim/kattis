import java.io.*;
import java.util.*;

public class dominos {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            var input = br.readLine().split(" ");
            int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

            var adjList = new ArrayList[n + 1];
            for (int i = 1; i <= n; i++) adjList[i] = new ArrayList<>();

            for (int i = 0; i < m; i++) {
                input = br.readLine().split(" ");
                adjList[Integer.parseInt(input[0])].add(Integer.parseInt(input[1]));
            }

            var visited = new boolean[n + 1];
            var s = new Stack<Integer>();
            for (int i = 1; i <= n; i++)
                if (!visited[i]) dfs(i, visited, adjList, s);

            visited = new boolean[n + 1];
            int count = 0;
            while (!s.isEmpty()) {
                int v = s.pop();
                
                if (!visited[v]) {
                    dfs(v, visited, adjList, s);
                    count++;
                }
            }

            System.out.println(count);
        }
    }

    static void dfs(int v, boolean[] visited, ArrayList<Integer>[] adjList, Stack<Integer> s) {
        visited[v] = true;
        for (var u : adjList[v])
            if (!visited[u]) dfs(u, visited, adjList, s);
        s.push(v);
    }
}
