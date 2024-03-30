import java.io.*;
import java.util.*;

public class kattissquest {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        
        int n = Integer.parseInt(br.readLine());
        
        var set = new TreeSet<Quest>();
        while (n-- > 0) {
            var command = br.readLine().split(" ");
            if (command[0].equals("add")) set.add(new Quest(Long.parseLong(command[1]), Long.parseLong(command[2]), n));
            else {
                long energy = Long.parseLong(command[1]), gold = 0;
                while (set.floor(new Quest(energy, Integer.MAX_VALUE, Integer.MAX_VALUE)) != null) {
                    var bestQuest = set.floor(new Quest(energy, Integer.MAX_VALUE, Integer.MAX_VALUE));
                    energy -= bestQuest.consumption;
                    gold += bestQuest.reward;
                    set.remove(bestQuest);
                }
                pw.println(gold);
            }
        }
        pw.flush();
    }

    static class Quest implements Comparable<Quest> {
        long consumption, reward;
        int index;

        Quest(long c, long r, int i) {
            consumption = c;
            reward = r;
            index = i;
        }

        @Override
        public int compareTo(Quest q) {
            if (consumption != q.consumption) return Long.compare(consumption, q.consumption);
            else if (reward != q.reward) return Long.compare(reward, q.reward);
            else return index - q.index;
        }
    }
}
