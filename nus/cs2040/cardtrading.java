import java.io.*;
import java.util.*;

public class cardtrading {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        var inputs = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int t = inputs[1], k = inputs[2];
        int[] count = new int[t], cards = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        for (int i : cards) count[i - 1]++;

        var prices = new int[t][2];

        for (int i = 0; i < t; i++) {
            var input = br.readLine().split(" ");
            prices[i] = new int[]{Integer.parseInt(input[0]), Integer.parseInt(input[1])};
        }

        long profit = 0;
        for (int i = 0; i < count.length; i++) profit += count[i] * prices[i][1];

        var opportunity = new int[t];
        for (int i = 0; i < t; i++) opportunity[i] = prices[i][0] * (2 - count[i]) + prices[i][1] * (count[i]);

        Arrays.sort(opportunity);
        for (int i = 0; i < k; i++) profit -= opportunity[i];

        System.out.println(profit);
    }
}
