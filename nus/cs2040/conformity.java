import java.io.*;
import java.util.*;
import java.util.stream.*;

public class conformity {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        int n = Integer.parseInt(br.readLine());
        var map = new HashMap<String, Integer>();

        while (n-- > 0) {
            var s = Arrays.stream(br.readLine().split(" ")).sorted().collect(Collectors.joining(""));
            map.put(s, map.getOrDefault(s, 0) + 1);
        }

        int most = map.values().stream().max(Integer::compare).orElse(0);
        System.out.println(map.values().stream().filter(integer -> integer == most).mapToInt(i -> i).sum());
    }
}
