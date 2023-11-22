import java.io.*;
import java.util.*;

public class airports {
    static ArrayList<Integer>[] adjList;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);
      
        adjList = new ArrayList[m];
        for (int i = 0; i < m; i++) adjList[i] = new ArrayList<>();

        var inspections = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        var time = new int[n][];
        for (int i = 0; i < n; i++) {
            time[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            for (int j = 0; j < n; j++) time[i][j] += inspections[j];
        }

        var flights = new int[m][];
        for (int i = 0; i < m; i++) {
            var line = br.readLine().split(" ");
            int s = Integer.parseInt(line[0]), f = Integer.parseInt(line[1]), t = Integer.parseInt(line[2]), e = t + time[s - 1][f - 1];
            flights[i] = new int[]{s, f, t, e};
        }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    time[j][k] = Math.min(time[j][k], time[j][i] + time[i][k]);


        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++)
                if ((flights[i][1] == flights[j][0] && flights[i][3] <= flights[j][2]) ||
                        (flights[i][3] + time[flights[i][1] - 1][flights[j][0] - 1] <= flights[j][2]))
                    adjList[i].add(j);


        System.out.println(m - match(m));
    }

    static boolean dfs(int v, int[] match, boolean[] visited) {
        if (visited[v]) return false;
        visited[v] = true;
        for (var u : adjList[v])
            if (match[u] == -1 || dfs(match[u], match, visited)) {
                match[u] = v;
                return true;
            }
        return false;
    }

    static int match(int m) {
        var match = new int[m];
        var visited = new boolean[m];
        Arrays.fill(match, -1);
        int count = 0;
        for (int i = 0; i < m; i++) {
            Arrays.fill(visited, false);
            if (dfs(i, match, visited)) count++;
        }
        return count;
    }
}
