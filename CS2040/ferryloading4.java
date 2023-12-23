import java.io.*;
import java.util.*;

public class ferryloading4 {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int c = Integer.parseInt(br.readLine());
        while (c-- > 0) {
            var input = br.readLine().split(" ");
            int l = Integer.parseInt(input[0]) * 100, m = Integer.parseInt(input[1]), count = 0;
            var sides = new ArrayDeque[]{new ArrayDeque<Integer>(), new ArrayDeque<Integer>()};

            while (m-- > 0) {
                var car = br.readLine().split(" ");
                sides[car[1].equals("right") ? 1 : 0].add(Integer.parseInt(car[0]));
            }

            var right = false;
            while (!sides[0].isEmpty() || !sides[1].isEmpty()) {
                int length = 0;
                while (!sides[right ? 1 : 0].isEmpty() && (length + (Integer) sides[right ? 1 : 0].peekFirst()) <= l)
                    length += (Integer) sides[right ? 1 : 0].removeFirst();
                count++;
                right = !right;
            }
            
            System.out.println(count);
        }
    }
}
