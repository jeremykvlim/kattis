import java.io.*;
import java.util.*;

public class signs {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var signs = new Sign[n];
        for (int i = 0; i < n; i++) {
            var sign = br.readLine();

            signs[i] = ((sign.length() & 1) == 0) ? new Sign(sign.substring(sign.length() / 2 - 1, sign.length() / 2 + 1), sign, i)
                                                  : new Sign(String.valueOf(sign.charAt(sign.length() / 2)), sign, i);
        }
        Arrays.sort(signs);

        for (var sign : signs) pw.println(sign.full);
        pw.flush();
    }

    static class Sign implements Comparable<Sign> {
        String middle, full;
        int index;

        Sign(String m, String f, int i) {
            middle = m;
            full = f;
            index = i;
        }

        @Override
        public int compareTo(Sign s) {
            return (!middle.equals(s.middle)) ? middle.compareTo(s.middle) : Integer.compare(index, s.index);
        }
    }
}
