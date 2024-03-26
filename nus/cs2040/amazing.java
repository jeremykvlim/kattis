import java.io.*;

public class amazing {
    static BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    static PrintWriter pw = new PrintWriter(System.out);

    public static void main(String[] args) throws IOException {
        var visited = new boolean[220][220];
        dfs(110, 110, -1, visited);
        pw.println("no way out");
        pw.flush();
    }

    static boolean send(int i) throws IOException {
        String[] directions = {"down", "up", "right", "left"};
        pw.println(directions[i]);
        pw.flush();
        var response = br.readLine();
        if (response.equals("solved")) System.exit(0);
        return response.equals("ok");
    }

    static void dfs(int x, int y, int prev, boolean[][] visited) throws IOException {
        visited[x][y] = true;
        int[] dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};
        for (int i = 0; i < 4; i++)
            if (!visited[x + dx[i]][y + dy[i]] && i != prev && send(i))
                dfs(x + dx[i], y + dy[i], i ^ 1, visited);
        if (prev != -1) send(prev);
    }
}
