import java.io.*;
import java.util.*;

public class numberlineartwork {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int n = input[0], m = input[1];

        var colors = br.readLine().split(" ");
        var marks = new String[n + m + 1];
        int head = 1;
        int[] next = new int[n + m + 1], prev = new int[n + m + 1], indices = new int[n];
        for (int i = 1; i <= n; i++) {
            marks[i] = colors[i - 1];
            prev[i] = i - 1;
            next[i] = i == n ? 0 : i + 1;
            indices[i - 1] = i;
        }

        while (m-- > 0) {
            var line = br.readLine().split(" ");
            int i = Integer.parseInt(line[0]);
            var w = line[1];

            int j = indices[i];
            if (w.equals("L")) indices[i] = prev[j];
            else if (w.equals("R")) indices[i] = next[j];
            else {
                n++;
                int temp = prev[j];
                prev[n] = temp;
                prev[j] = n;
                if (temp == 0) head = n;
                else next[temp] = n;
                next[n] = j;
                marks[n] = w;
                indices[i] = n;
            }
        }

        for (int i = head; i != 0; i = next[i]) pw.print(marks[i] + " ");
        pw.flush();
    }
}
