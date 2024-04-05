import java.io.*;

public class islands3 {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        var input = br.readLine().split(" ");
        int r = Integer.parseInt(input[0]), c = Integer.parseInt(input[1]);

        var grid = new char[r][c];
        for (int i = 0; i < r; i++) grid[i] = br.readLine().toCharArray();

        int islands = 0;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                if (grid[i][j] == 'L') {
                    islands++;
                    dfs(grid, i, j, r, c);
                }

        System.out.println(islands);
    }

    static void dfs(char[][] grid, int i, int j, int r, int c) {
        if (!(0 <= i && i < r && 0 <= j && j < c) || grid[i][j] == 'W') return;

        grid[i][j] = 'W';
        dfs(grid, i, j + 1, r, c);
        dfs(grid, i, j - 1, r, c);
        dfs(grid, i + 1, j, r, c);
        dfs(grid, i - 1, j, r, c);
    }
}
