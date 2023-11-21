import java.io.*;
import java.util.*;

public class dragonballs {
    public static void main(String[] args) throws Exception {
        IO io = new IO();
        int n = io.nextInt();
        long d1, d2, d3, d4, x, y;
        while (n-- > 0) {
            io.println("0 0");
            d1 = io.nextLong();
            if (d1 == 0) continue;

            io.println("1 0");
            d2 = io.nextLong();
            if (d2 == 0) continue;

            io.println("0 1");
            d3 = io.nextLong();
            if (d3 == 0) continue;

            x = (d1 - d2 + 1) / 2;
            io.println(x + " " + (long) Math.sqrt(d1 - x*x));

            d4 = io.nextLong();
            if (d4 == 0) continue;

            y = (d1 - d3 + 1) / 2;
            io.println((long) Math.sqrt(d1 - y*y) + " " + y);

            io.nextLong();
        }
    }

    static class IO {
        BufferedReader br;
        PrintWriter pw;
        StringTokenizer st;

        IO() {
            br = new BufferedReader(new InputStreamReader(System.in));
            pw = new PrintWriter(System.out);
            st = null;
        }

        long nextLong() throws Exception {
            if (st == null || !st.hasMoreTokens())
                st = new StringTokenizer(br.readLine());
            return Long.parseLong(st.nextToken());
        }

        int nextInt() throws Exception {
            if (st == null || !st.hasMoreTokens())
                st = new StringTokenizer(br.readLine());
            return Integer.parseInt(st.nextToken());
        }

        void println(String s) {
            pw.println(s);
            pw.flush();
        }
    }

}
