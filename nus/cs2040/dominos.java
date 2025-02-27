import java.io.*;
import java.util.*;

public class dominos {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int T = Integer.parseInt(br.readLine());
        while (T-- > 0) {
            var input = br.readLine().split(" ");
            int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

            var adjList = new ArrayList[n + 1];
            for (int i = 1; i <= n; i++) adjList[i] = new ArrayList<>();

            for (int i = 0; i < m; i++) {
                input = br.readLine().split(" ");
                adjList[Integer.parseInt(input[0])].add(Integer.parseInt(input[1]));
            }

            var t = tarjan(n, adjList);
            var sccs = (int) t.first;
            var component = (int[]) t.second;

            var degree = new int[sccs + 1];
            for (int v = 1; v <= n; v++)
                for (var u : adjList[v])
                    if (component[(int) u] != component[v]) degree[component[(int) u]]++;

            int count = 0;
            for (int i = 1; i <= sccs; i++)
                if (degree[i] == 0) count++;
            pw.println(count);
        }
        pw.flush();
    }

    record Tuple<T, U>(T first, U second) {}

    static Tuple<Integer, int[]> tarjan(int n, ArrayList<Integer>[] adjList) {
        int[] order = new int[n + 1], low = new int[n + 1], component = new int[n + 1], counts = new int[2];
        var stacked = new boolean[n + 1];
        var st = new Stack<Integer>();

        for (int v = 1; v <= n; v++)
            if (order[v] == 0) dfs(v, adjList, order, low, component, stacked, st, counts);

        return new Tuple<>(counts[1], component);
    }

    static void dfs(int v, ArrayList<Integer>[] adjList, int[] order, int[] low, int[] component, boolean[] stacked, Stack<Integer> st, int[] counts) {
        order[v] = low[v] = ++counts[0];
        st.add(v);
        stacked[v] = true;

        for (int u : adjList[v])
            if (order[u] == 0) {
                dfs(u, adjList, order, low, component, stacked, st, counts);
                low[v] = Math.min(low[v], low[u]);
            } else if (stacked[u]) low[v] = Math.min(low[v], order[u]);

        if (order[v] == low[v]) {
            counts[1]++;
            int u;
            do {
                u = st.pop();
                stacked[u] = false;
                component[u] = counts[1];
            } while (u != v);
        }
    }
}
