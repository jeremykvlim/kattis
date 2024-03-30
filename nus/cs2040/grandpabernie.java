import java.io.*;
import java.util.*;

public class grandpabernie {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        
        int n = Integer.parseInt(br.readLine());
        var years = new HashMap<String, ArrayList<Integer>>();
        while (n-- > 0) {
            var input = br.readLine().split(" ");
            var y = years.getOrDefault(input[0], new ArrayList<>());
            y.add(Integer.parseInt(input[1]));
            years.put(input[0], y);
        }

        for (var y : years.values()) Collections.sort(y);

        int q = Integer.parseInt(br.readLine());
        while (q-- > 0) {
            var input = br.readLine().split(" ");
            var y = years.get(input[0]);
            pw.println(y.get(Integer.parseInt(input[1]) - 1));
        }
        
        pw.flush();
    }
}
