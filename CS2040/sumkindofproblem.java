import java.util.*;

public class sumkindofproblem {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        int p = sc.nextInt();
        while (p-- > 0) {
            int k = sc.nextInt(), n = sc.nextInt();
            System.out.println(k + " " + n*(n+1)/2 + " " + n*n + " " + n*(n+1));
        }
    }
}
