import java.io.*;
import java.util.*;

public class grandpabernie {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int n = Integer.parseInt(br.readLine());
        var map = new HashMap<String, ArrayList<Integer>>();
        while (n-- > 0) {
            var input = br.readLine().split(" ");
            var list = map.getOrDefault(input[0], new ArrayList<>());
            list.add(Integer.parseInt(input[1]));
            map.put(input[0], list);
        }

        for (var list : map.values()) Collections.sort(list);

        int q = Integer.parseInt(br.readLine());
        while (q-- > 0) {
            var input = br.readLine().split(" ");
            var list = map.get(input[0]);
            pw.println(list.get(Integer.parseInt(input[1]) - 1));
        }
        pw.flush();
    }
}
