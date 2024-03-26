import java.util.*;

public class shatteredcake {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        int W = sc.nextInt(), n = sc.nextInt(), area = 0;
        while (n-- > 0) {
            int w = sc.nextInt(), l = sc.nextInt();
            area += w*l;
        }
        System.out.println(area/W);
    }
}
