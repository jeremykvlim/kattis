import java.io.*;
import java.util.*;

public class longwait {
    static final int SIZE = (int) 2e6, MID = (int) 1e6;
    static String[] front = new String[SIZE], back = new String[SIZE];
    static int backHead = MID, backTail = MID + 1, backSize = 0, frontHead = MID - 1, frontTail = MID, frontSize = 0;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int q = input[0], k = input[1];
        while (q-- > 0) {
            var op = br.readLine().split(" ");

            switch (op[0]) {
                case "queue" -> {
                    back[backTail++] = op[1];
                    backSize++;
                    balance(k);
                }
                case "member" -> {
                    back[backHead--] = op[1];
                    backSize++;
                    balance(k);
                }
                case "vip" -> {
                    front[frontHead--] = op[1];
                    frontSize++;
                    balance(k);
                }
                case "slower" -> balance(++k);
                case "faster" -> balance(--k);
                default -> {
                    int pos = Integer.parseInt(op[1]);
                    pw.println(pos < frontSize ? front[frontHead + pos] : back[backHead + pos - frontSize]);
                }
            }
        }

        pw.flush();
    }

    static void balance(int k) {
        if (frontSize == k) return;

        if (frontSize > k) 
            while (frontSize > k) {
                back[backHead--] = front[--frontTail];
                backSize++;
                frontSize--;
            }
        else 
            while (frontSize < k && backSize > 0) {
                front[frontTail++] = back[++backHead];
                backSize--;
                frontSize++;
            }
    }
}
