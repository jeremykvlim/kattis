import java.io.*;
import java.util.*;

public class zipfsong {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = br.readLine().split(" ");
        int n = Integer.parseInt(input[0]), m = Integer.parseInt(input[1]);

        var songs = new ArrayList<Pair<Long, Integer>>(n);
        var names = new String[n];
        for (int i = 1; i <= n; i++) {
            var song = br.readLine().split(" ");
            var freq = Long.parseLong(song[0]);
            names[i - 1] = song[1];

            songs.add(new Pair<>(-freq * i, i - 1));
        }

        Collections.sort(songs);
        for (int i = 0; i < m; i++) pw.println(names[songs.get(i).second]);
        pw.flush();
    }

    record Pair<T extends Comparable<T>, U extends Comparable<U>>(T first, U second) implements Comparable<Pair<T, U>> {
        @Override
        public int compareTo(Pair<T, U> p) {
            int cmp = first.compareTo(p.first);
            return (cmp == 0) ? second.compareTo(p.second) : cmp;
        }
    }
}
