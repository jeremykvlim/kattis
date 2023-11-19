import java.io.*;
import java.util.*;

public class zipfsong {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);
        var array = new Pair[n];
        for (int i = 1; i <= n; i++) {
            var song = br.readLine().split(" ");
            long freq = Long.parseLong(song[0]);
            var name = song[1];
            array[i - 1] = new Pair(freq * i, name);
        }
        Arrays.sort(array);
        for (int i = 0; i < m; i++)
            System.out.println(array[i].name);
    }

    static class Pair implements Comparable<Pair> {
        long q;
        String name;

        Pair(long qual, String n) {
            q = qual;
            name = n;
        }

        @Override
        public int compareTo(Pair p) {
            return Long.compare(p.q, q);
        }
    }
}
