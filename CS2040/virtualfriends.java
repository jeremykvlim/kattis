import java.io.*;
import java.util.*;

public class virtualfriends {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int t = Integer.parseInt(br.readLine());

        while (t-- > 0) {
            int f = Integer.parseInt(br.readLine()), count = 0;
            int[] sets = new int[2 * f + 1], size = new int[2 * f + 1];
            var compress = new HashMap<String, Integer>();
          
            for (int i = 0; i < f; i++) {
                var friends = br.readLine().split(" ");
                
                for (var fr : friends) 
                    if (!compress.containsKey(fr)) {
                        compress.put(fr, count);
                        sets[count] = count;
                        size[count++] = 1;
                    }
                
                int a = find(compress.get(friends[0]), sets), b = find(compress.get(friends[1]), sets);
                if (a != b) {
                    sets[b] = a;
                    size[a] += size[b];
                }
                
                pw.println(size[a]);
            }
        }
        pw.flush();
    }

    static int find(int p, int[] sets) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
    }
}
