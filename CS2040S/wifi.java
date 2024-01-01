import java.io.*;
import java.util.*;

public class wifi {
    static final double EPSILON = 1e-2;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int t = Integer.parseInt(br.readLine());
        while (t-- > 0) {
            var input = br.readLine().split(" ");
            int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

            var houses = new int[m];
            for (int i = 0; i < m; i++) houses[i] = Integer.parseInt(br.readLine());

            Arrays.sort(houses);
            double l = 0, r = houses[m - 1], mid;
            while (l + EPSILON < r) {
                mid = l + (r - l) / 2;

                double dist = houses[0] + mid;
                int count = 1;
                for (int i = 1; i < m; i++)
                    if (Math.abs(dist - houses[i]) > mid) {
                        count++;
                        dist = houses[i] + mid;
                    }

                if (count <= n) r = mid;
                else l = mid;
            }


            pw.printf("%.1f\n", l);
        }
        pw.flush();
    }
}
