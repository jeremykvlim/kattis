import java.io.*;
import java.util.*;

public class kattissquest {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());

        var quests = new TreeSet<Triple<Long, Long, Integer>>();
        while (n-- > 0) {
            var command = br.readLine().split(" ");

            if (command[0].equals("add")) quests.add(new Triple<>(Long.parseLong(command[1]), Long.parseLong(command[2]), n));
            else {
                long energy = Long.parseLong(command[1]), gold = 0;
                while (quests.floor(new Triple<>(energy, Long.MAX_VALUE, Integer.MAX_VALUE)) != null) {
                    var q = quests.floor(new Triple<>(energy, Long.MAX_VALUE, Integer.MAX_VALUE));
                    energy -= q.first;
                    gold += q.second;
                    quests.remove(q);
                }
                pw.println(gold);
            }
        }
        pw.flush();
    }

    record Triple<T extends Comparable<T>, U extends Comparable<U>, V extends Comparable<V>>(T first, U second, V third) implements Comparable<Triple<T, U, V>> {
        @Override
        public int compareTo(Triple<T, U, V> t) {
            int cmp = first.compareTo(t.first);
            if (cmp != 0) return cmp;
            cmp = second.compareTo(t.second);
            return (cmp == 0) ? third.compareTo(t.third) : cmp;
        }
    }
}
