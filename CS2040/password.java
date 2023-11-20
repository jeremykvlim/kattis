import java.io.*;
import java.util.*;

public class password {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(br.readLine());
        double total = 0;
        var list = new ArrayList<Double>(n);
        for (int i = 0; i < n; i++) {
            var input = br.readLine().split(" ");
            list.add(Double.parseDouble(input[1]));
        }
        list.sort(Collections.reverseOrder());
        for (int i = 1; i <= n; i++) total += list.get(i - 1) * i;
        System.out.println(total);
    }
}
