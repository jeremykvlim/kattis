import java.util.*;

public class numberfun {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        
        int n = sc.nextInt();
        while (n-- > 0) {
            int a = sc.nextInt(), b = sc.nextInt(), c = sc.nextInt();
            if (a + b == c || a - b == c || a * b == c || c * b == a ||
                b - a == c || c * a == b) System.out.println("Possible");
            else System.out.println("Impossible");
        }
    }
}
