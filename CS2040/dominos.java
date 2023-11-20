import java.util.*;
import java.io.*;

public class dominos {
    static ArrayList<Integer>[] adjList;
    static boolean[] visited;
    static Stack<Integer> s;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            String[] input = br.readLine().split(" ");
            int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

            adjList = new ArrayList[n + 1];
            for (int i = 1; i <= n; i++) adjList[i] = new ArrayList<>();

            for (int i = 0; i < m; i++) {
                input = br.readLine().split(" ");
                int x = Integer.parseInt(input[0]), y = Integer.parseInt(input[1]);
                adjList[x].add(y);
            }

            visited = new boolean[n + 1];
            s = new Stack<>();
            for (int i = 1; i <= n; i++)
                if (!visited[i]) dfs(i);

            visited = new boolean[n + 1];
            int count = 0;
            while (!s.isEmpty()) {
                int node = s.pop();
                if (!visited[node]) {
                    dfs(node);
                    count++;
                }
            }

            System.out.println(count);
        }
    }

    static void dfs(int node) {
        visited[node] = true;
        for (var c : adjList[node])
            if (!visited[c]) dfs(c);
        s.push(node);
    }
}
