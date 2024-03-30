import java.util.*;

public class exactlyelectrical {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        
        int a = sc.nextInt(), b = sc.nextInt(), c = sc.nextInt(), d = sc.nextInt(), t = sc.nextInt(), dist = Math.abs(a - c) + Math.abs(b - d);
        
        if (((t - dist) & 1) == 0 && (t - dist >= 0)) System.out.println("Y");
        else System.out.println("N");
    }
}
