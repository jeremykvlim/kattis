import java.io.*;
import java.util.*;

public class exitsinexcess {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int m = Integer.parseInt(br.readLine().split(" ")[1]);

        var half1 = new ArrayList<Integer>();
        var half2 = new ArrayList<Integer>();
        for (int i = 1; i <= m; i++) {
            var line = br.readLine().split(" ");
            if (Integer.parseInt(line[0]) < Integer.parseInt(line[1])) half1.add(i);
            else half2.add(i);
        }
        var remove = half1.size() < half2.size() ? half1 : half2;
        pw.println(remove.size());
        for (var i : half1.size() < half2.size() ? half1 : half2) pw.println(i);
        pw.flush();
    }
}
