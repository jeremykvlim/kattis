import java.util.*;

public class coconut {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);

        int s = sc.nextInt(), n = sc.nextInt();
        var dq = new ArrayDeque<Integer>();
        for (int i = 0; i < n; i++) dq.add(2 * (i + n));

        while (dq.size() > 1) {
            for (int i = 0; i < s - 1; i++) dq.add(dq.pop());
            
            int front = dq.pop();
            if (front >= 2 * n) {
                front -= 2 * n;
                dq.push(front);
                dq.push(front);
            } else if ((front & 1) == 0) dq.add(front + 1);
        }

        System.out.println(dq.pop() % (2 * n) / 2 + 1);
    }
}
