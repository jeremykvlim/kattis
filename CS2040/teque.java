import java.io.*;

public class teque {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);
        int n = Integer.parseInt(br.readLine());

        var front = new String[(int) 1e6];
        var back = new String[(int) 1e6];
        int m = front.length >> 1, frontSize = 0, backSize = 0, frontHead = m - 1, backTail = m + 1, backHead = m, frontTail = m;
        while (n-- > 0) {
            var command = br.readLine().split(" ");
            switch (command[0]) {
                case "push_front": {
                    front[frontHead--] = command[1];
                    frontSize++;
                    if (frontSize > backSize + 1) {
                        back[backHead--] = front[--frontTail];
                        backSize++;
                        frontSize--;
                    } else if (frontSize + 1 < backSize) {
                        front[frontTail++] = back[++backHead];
                        backSize--;
                        frontSize++;
                    }
                    break;
                }
                case "push_back": {
                    back[backTail++] = command[1];
                    backSize++;
                    if (frontSize > backSize + 1) {
                        back[backHead--] = front[--frontTail];
                        backSize++;
                        frontSize--;
                    } else if (frontSize + 1 < backSize) {
                        front[frontTail++] = back[++backHead];
                        backSize--;
                        frontSize++;
                    }
                    break;
                }
                case "push_middle": {
                    if (frontSize >= backSize) {
                        back[backHead--] = command[1];
                        backSize++;
                    } else {
                        front[frontTail++] = back[backHead + 1];
                        back[backHead + 1] = command[1];
                        frontSize++;
                    }
                    if (frontSize > backSize + 1) {
                        back[backHead--] = front[--frontTail];
                        backSize++;
                        frontSize--;
                    } else if (frontSize + 1 < backSize) {
                        front[frontTail++] = back[++backHead];
                        backSize--;
                        frontSize++;
                    }
                    break;
                }
                default: {
                    int i = Integer.parseInt(command[1]);
                    pw.println(i > frontSize - 1 ? back[backHead + 1 + i - frontSize] : front[frontHead + 1 + i]);
                    break;
                }
            }
        }
        pw.flush();
    }
}
