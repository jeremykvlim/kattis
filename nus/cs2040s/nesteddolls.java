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
            Arrays.sort(dolls);

            var lnds = new int[m];
            lnds[0] = dolls[0].height;
            int len = 1;
            for (int i = 1; i < m; i++) {
                if (dolls[i].height > lnds[0]) lnds[0] = dolls[i].height;
                else if (dolls[i].height <= lnds[len - 1]) lnds[len++] = dolls[i].height;
                else {
                    int l = -1, r = len, mid;
                    while (l + 1 < r) {
                        mid = l + (r - l) / 2;

                        if (lnds[mid] < dolls[i].height) r = mid;
                        else l = mid;
                    }
                    lnds[r] = dolls[i].height;
                }
            }

            pw.println(len);
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
