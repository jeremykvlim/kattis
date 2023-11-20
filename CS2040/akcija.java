import java.util.*;

public class akcija {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        int n = sc.nextInt(), max = 0, total = 0, free = 0;
        var prices = new int[n];
        for (int i = 0; i < n; i++) {
            prices[i] = sc.nextInt();
            max = Math.max(max, prices[i]);
        }
        var count = new int[max + 1];
        for (int price : prices) count[price]++;

        for (int price = max; price >= 0; price--) {
            while (count[price]-- > 0) {
                if (free++ < 2) total += price;
                else free = 0;
            }
        }
        System.out.println(total);
    }
}
