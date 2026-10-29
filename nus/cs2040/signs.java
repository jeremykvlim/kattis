import java.io.*;
import java.util.*;

public class signs {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        var signs = new ArrayList<String>(n);
        for (int i = 0; i < n; i++) signs.add(br.readLine());

        signs.sort(Comparator.comparing(s -> s.substring((s.length() - 1) / 2, s.length() / 2 + 1)));
        for (var s : signs) pw.println(s);
        pw.flush();
    }
}
