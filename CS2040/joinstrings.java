import java.io.*;
import java.util.*;

public class joinstrings {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int n = Integer.parseInt(br.readLine());

        var arr = new String[n];
        for (int i = 0; i < n; i++) arr[i] = br.readLine();

        var adjList = new ArrayList[n];
        for (int i = 0; i < n; i++) adjList[i] = new ArrayList<Integer>();

        int start = 0;
        for (int i = 0; i < n - 1; i++) {
            int[] operation = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            start = operation[0] - 1;
            adjList[start].add(operation[1] - 1);
        }

        pw.print(arr[start]);
        dfs(start, adjList, arr, pw);
        pw.flush();
    }

    static void dfs(int v, ArrayList<Integer>[] adjList, String[] arr, PrintWriter pw) {
        for (var u : adjList[v]) {
            pw.print(arr[u]);
            dfs(u, adjList, arr, pw);
        }
    }
}
