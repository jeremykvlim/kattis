import java.io.*;
import java.util.*;

public class weakvertices {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        for (int n = Integer.parseInt(br.readLine()); n != -1; n = Integer.parseInt(br.readLine())) {
            var matrix = new int[n][n];
            for (int i = 0; i < n; i++)
                matrix[i] = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();

            for (int i = 0; i < n; i++) {
                var weak = true;
                for (int j = 0; j < n; j++) {
                    for (int k = j + 1; k < n; k++)
                        if (matrix[i][j] * matrix[i][k] * matrix[j][k] != 0) {
                            weak = false;
                            break;
                        }
                    if (!weak) break;
                }
                if (weak) System.out.print(i + " ");
            }
            System.out.println();
        }
    }
}
