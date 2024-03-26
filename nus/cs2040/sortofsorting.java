import java.io.*;
import java.util.*;

public class sortofsorting {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        for (int names = Integer.parseInt(br.readLine()); names != 0; names = Integer.parseInt(br.readLine())) {
            var arr = new String[names];
            for (int i = 0; names > 0; names--, i++) arr[i] = br.readLine();
            Arrays.sort(arr, Comparator.comparing(s -> s.substring(0, 2)));
            for (var name : arr) System.out.println(name);
            System.out.println();
        }
    }
}
