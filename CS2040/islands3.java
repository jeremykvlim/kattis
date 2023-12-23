import java.io.*;

public class islands3 {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int r = Integer.parseInt(input[0]), c = Integer.parseInt(input[1]);
        var matrix = new char[r][c];
        for (int i = 0; i < r; i++)
            matrix[i] = br.readLine().toCharArray();

        int islands = 0;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) 
                if (matrix[i][j] == 'L') {
                    islands++;
                    dfs(matrix, i, j, r, c);
                }
            
        System.out.println(islands);
    }

    static void dfs(char[][] matrix, int i, int j, int r, int c) {
        if (i < 0 || j < 0 || i == r || j == c || matrix[i][j] == 'W')
            return;
        if (matrix[i][j] == 'L' || matrix[i][j] == 'C') {
            matrix[i][j] = 'W';
            dfs(matrix, i, j + 1, r, c);
            dfs(matrix, i, j - 1, r, c);
            dfs(matrix, i + 1, j, r, c);
            dfs(matrix, i - 1, j, r, c);
        }
    }
}
