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

            var lnds = new ArrayList<Integer>();
            for (var d : dolls) {
                int l = 0, r = lnds.size(), mid;
                while (l < r) {
                    mid = l + (r - l) / 2;
                    if (lnds.get(mid) > d.height) r = mid;
                    else l = mid + 1;
                }
                if (l == lnds.size()) lnds.add(d.height);
                else lnds.set(l, d.height);
            }

            pw.println(lnds.size());
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
