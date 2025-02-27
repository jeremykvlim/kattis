import java.io.*;
import java.util.*;

public class bots {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int n = input[0], m = input[1];

        var adjList = new ArrayList[n + 1];
        for (int i = 1; i <= n; i++) adjList[i] = new ArrayList<>();

        while (m-- > 0) {
            input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            int p = input[0], q = input[1];
            adjList[p + 1].add(q + 1);
        }

        var t = tarjan(n, adjList);
        var sccs = (int) t.first;
        var component = (int[]) t.second;
        var members = (ArrayList<ArrayList>) t.third;

        var degree = new int[sccs + 1];
        for (int v = 1; v <= n; v++)
            for (var u : adjList[v])
                if (component[(int) u] != component[v]) degree[component[(int) u]]++;

        int solo = 0, net = 0;
        for (int i = 1; i <= sccs; i++)
            if (degree[i] == 0) {
                if (members.get(i).size() == 1) solo++;
                else net++;
            }
        pw.println(solo + " " + net);
        pw.flush();
    }

    record Tuple<T, U, V>(T first, U second, V third) {}

    static Tuple<Integer, int[], ArrayList<ArrayList<Integer>>> tarjan(int n, ArrayList<Integer>[] adjList) {
        int[] order = new int[n + 1], low = new int[n + 1], component = new int[n + 1], counts = new int[2];
        var members = new ArrayList<ArrayList<Integer>>();
        members.add(new ArrayList<>());
        var stacked = new boolean[n + 1];
        var st = new Stack<Integer>();

        for (int v = 1; v <= n; v++)
            if (order[v] == 0) dfs(v, adjList, order, low, component, members, stacked, st, counts);

        return new Tuple<>(counts[1], component, members);
    }

    static void dfs(int v, ArrayList<Integer>[] adjList, int[] order, int[] low, int[] component, ArrayList<ArrayList<Integer>> members, boolean[] stacked, Stack<Integer> st, int[] counts) {
        order[v] = low[v] = ++counts[0];
        st.add(v);
        stacked[v] = true;

        for (int u : adjList[v])
            if (order[u] == 0) {
                dfs(u, adjList, order, low, component, members, stacked, st, counts);
                low[v] = Math.min(low[v], low[u]);
            } else if (stacked[u]) low[v] = Math.min(low[v], order[u]);

        if (order[v] == low[v]) {
            counts[1]++;
            members.add(new ArrayList<>());
            int u;
            do {
                u = st.pop();
                stacked[u] = false;
                component[u] = counts[1];
                members.get(counts[1]).add(u);
            } while (u != v);
        }
    }
}
