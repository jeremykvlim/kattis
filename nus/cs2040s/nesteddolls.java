import java.io.*;
import java.util.*;

public class nesteddolls {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int t = Integer.parseInt(br.readLine());

        while (t-- > 0) {
            int m = Integer.parseInt(br.readLine());

            var dolls = new Doll[m];
            var input = br.readLine().split(" ");
            for (int i = 0; i < m; i++) dolls[i] = new Doll(Integer.parseInt(input[2 * i]), Integer.parseInt(input[2 * i + 1]));
            Arrays.sort(dolls, Comparator.reverseOrder());

            var dp = new int[2 * m];
            int d = 0;
            for (int i = 0; i < m; i++) {
                int j = d;
                for (; j >= 0; j--)
                    if (dp[j] <= dolls[i].height) {
                        dp[++j] = dolls[i].height;
                        break;
                    }
                d = Math.max(d, j);
            }

            pw.println(d);
        }
        pw.flush();
    }

    static class Doll implements Comparable<Doll> {
        int width, height;

        Doll(int w, int h) {
            width = w;
            height = h;
        }

        @Override
        public int compareTo(Doll d) {
            return width != d.width ? width - d.width : d.height - height;
        }
    }
}
