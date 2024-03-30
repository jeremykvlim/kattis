import java.io.*;

public class t9spelling {
    static int prev = -1;
    
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        int n = Integer.parseInt(br.readLine());
        for (int x = 1; x <= n; x++) {
            int num, repetitions;
            System.out.print("Case #" + x + ": ");
            var line = br.readLine().toCharArray();
            for (char c : line) {
                if (c == ' ') {
                    repetitions = 1;
                    num = 0;
                    press(repetitions, num);
                } else if (c == 's') {
                    repetitions = 4;
                    num = 7;
                    press(repetitions, num);
                } else if (c == 'z') {
                    repetitions = 4;
                    num = 9;
                    press(repetitions, num);
                } else if (c < 's') {
                    repetitions = (c - 'a') % 3 + 1;
                    num = (c - 'a') / 3 + 2;
                    press(repetitions, num);
                } else if (c < 'z') {
                    repetitions = (c - 'a' - 1) % 3 + 1;
                    num = (c - 'a' - 1) / 3 + 2;
                    press(repetitions, num);
                }
            }
            System.out.println();
        }
    }

    static void press(int repetitions, int num) {
        if (prev == num) System.out.print(" ");
        while (repetitions-- > 0) System.out.print(num);
        prev = num;
    }
}
