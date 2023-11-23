import java.util.*;
import java.io.*;

public class bst {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int n = Integer.parseInt(br.readLine());
        var map = new TreeMap<Integer, Integer>();
        long total = 0;

        while (n-- > 0) {
            int i = Integer.parseInt(br.readLine());
            Integer floor = map.floorKey(i), ceiling = map.ceilingKey(i);
            int c = 0;
            if (floor == null && ceiling != null) c = map.get(ceiling) + 1;
            else if (ceiling == null && floor != null) c = map.get(floor) + 1;
            else if (ceiling != null) c = Math.max(map.get(ceiling), map.get(floor)) + 1;

            map.put(i, c);
            total += c;
            pw.println(total);
        }
        pw.flush();
    }
}
