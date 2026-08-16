import java.io.*;
import java.util.*;

public class buffbuffbuff {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var strongest = new HashMap<Integer, Integer>();
        var pq = new PriorityQueue<Pair<Integer, Integer>>();
        int q = Integer.parseInt(br.readLine()), all = 0;
        while (q-- > 0) {
            var line = br.readLine().split(" ");
            switch (line[0]) {
                case "ADD" -> {
                    int t = Integer.parseInt(line[1]), p = Integer.parseInt(line[2]);

                    int diff = p - all;
                    var s = strongest.getOrDefault(t, Integer.MIN_VALUE);
                    if (diff > s) {
                        strongest.put(t, diff);
                        pq.add(new Pair<>(-diff, t));
                    }
                }
                case "BUFF_ALL" -> all += Integer.parseInt(line[1]);
                case "BUFF" -> {
                    int t = Integer.parseInt(line[1]), b = Integer.parseInt(line[2]);

                    var s = strongest.get(t);
                    if (s != null) {
                        s += b;
                        strongest.put(t, s);
                        pq.add(new Pair<>(-s, t));
                    }
                }
                default -> {
                    while (-pq.peek().first != strongest.get(pq.peek().second)) pq.poll();
                    pw.println(-pq.peek().first + all);
                }
            }
        }
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