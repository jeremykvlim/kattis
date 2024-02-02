import java.util.*;

public class lastfactorialdigit {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        int t = sc.nextInt();
        while (t-- > 0) {
            int n = sc.nextInt();
            switch (n) {
                case 1: System.out.println(1); break;
                case 2: System.out.println(2); break;
                case 3: System.out.println(6); break;
                case 4: System.out.println(4); break;
                default: System.out.println(0); break;
            }
        }
    }
}
