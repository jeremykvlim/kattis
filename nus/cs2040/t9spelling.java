import java.io.*;

public class t9spelling {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(br.readLine());

        int count = 1;
        while (n-- > 0) {
            int[] prev = {-1};
            int num, repetitions;
            System.out.print("Case #" + (count++) + ": ");
            var line = br.readLine().toCharArray();
            for (char c : line) {
                if (c == ' ') {
                    repetitions = 1;
                    num = 0;
                    press(repetitions, num, prev);
                } else if (c == 's') {
                    repetitions = 4;
                    num = 7;
                    press(repetitions, num, prev);
                } else if (c == 'z') {
                    repetitions = 4;
                    num = 9;
                    press(repetitions, num, prev);
                } else if (c < 's') {
                    repetitions = (c - 'a') % 3 + 1;
                    num = (c - 'a') / 3 + 2;
                    press(repetitions, num, prev);
                } else if (c < 'z') {
                    repetitions = (c - 'a' - 1) % 3 + 1;
                    num = (c - 'a' - 1) / 3 + 2;
                    press(repetitions, num, prev);
                }
            }
            System.out.println();
        }
    }

    static void press(int repetitions, int num, int[] prev) {
        if (prev[0] == num) System.out.print(" ");
        while (repetitions-- > 0) System.out.print(num);
        prev[0] = num;
    }
}
