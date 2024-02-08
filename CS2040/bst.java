import java.io.*;
import java.util.*;

public class bst {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int n = Integer.parseInt(br.readLine());
        
        var tree = new TreeSet<Integer, Integer>();
        long total = 0;
        while (n-- > 0) {
            int i = Integer.parseInt(br.readLine()), c = 0;
            Integer floor = tree.floorKey(i), ceiling = tree.ceilingKey(i);
            
            if (floor == null && ceiling != null) c = tree.get(ceiling) + 1;
            else if (ceiling == null && floor != null) c = tree.get(floor) + 1;
            else if (ceiling != null) c = Math.max(tree.get(ceiling), tree.get(floor)) + 1;

            tree.put(i, c);
            total += c;
            pw.println(total);
        }
        pw.flush();
    }
}
