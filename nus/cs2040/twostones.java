import java.util.*;

public class twostones {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        
        if ((sc.nextInt() & 1) == 0) System.out.println("Bob");
        else System.out.println("Alice");
    }
}
