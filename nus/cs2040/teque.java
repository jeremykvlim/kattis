import java.io.*;

public class teque {
    static final int SIZE = (int) 1e6, MID = (int) 5e5;
    static String[] front = new String[SIZE], back = new String[SIZE];
    static int backHead = MID, backTail = MID + 1, backSize = 0, frontHead = MID - 1, frontTail = MID, frontSize = 0;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        int n = Integer.parseInt(br.readLine());
        while (n-- > 0) {
            var command = br.readLine().split(" ");

            switch (command[0]) {
                case "push_front" -> {
                    front[frontHead--] = command[1];
                    frontSize++;
                    balance();
                }
                case "push_back" -> {
                    back[backTail++] = command[1];
                    backSize++;
                    balance();
                }
                case "push_middle" -> {
                    if (frontSize >= backSize) {
                        back[backHead--] = command[1];
                        backSize++;
                    } else {
                        front[frontTail++] = back[backHead + 1];
                        back[backHead + 1] = command[1];
                        frontSize++;
                    }
                    balance();
                }
                default -> {
                    int i = Integer.parseInt(command[1]);
                    pw.println(i < frontSize ? front[frontHead + 1 + i] : back[backHead + 1 + i - frontSize]);
                }
            }
        }
        pw.flush();
    }

    static void balance() {
        if (frontSize > backSize + 1) {
            back[backHead--] = front[--frontTail];
            backSize++;
            frontSize--;
        } else if (frontSize + 1 < backSize) {
            front[frontTail++] = back[++backHead];
            backSize--;
            frontSize++;
        }
    }
}
