import java.io.*;
import java.util.*;

public class sortofsorting {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        for (;;) {
            int names = Integer.parseInt(br.readLine());
            if (names == 0) return;
            var nameArray = new String[names];
            for (int i = 0; names > 0; names--, i++) nameArray[i] = br.readLine();
            Arrays.sort(nameArray, Comparator.comparing(s -> s.substring(0, 2)));
            for (var name : nameArray) System.out.println(name);
            System.out.println();
        }
    }
}
