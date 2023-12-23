import java.io.*;

public class t9spelling {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(br.readLine());

        int[] prev = {-1};
        int number, repetitions, count = 1;
        while (n-- > 0) {
            System.out.print("Case #"+(count++)+": ");
            var line = br.readLine();
            for (int i = 0; i < line.length(); i++) {
                char letter = line.charAt(i);

                if (letter == ' ') {
                    repetitions = 1;
                    number = 0;
                    pressNumber(repetitions, number, prev);
                }
                else if (letter == 's') {
                    repetitions = 4;
                    number = 7;
                    pressNumber(repetitions, number, prev);
                }
                else if (letter == 'z') {
                    repetitions = 4;
                    number = 9;
                    pressNumber(repetitions, number, prev);
                }
                else if (letter < 116) {
                    repetitions = (letter - 'a') % 3 + 1;
                    number = (letter - 'a') / 3 + 2;
                    pressNumber(repetitions, number, prev);
                }
                else if (letter < 123) {
                    repetitions = (letter - 'a' - 1) % 3 + 1;
                    number = (letter - 'a' - 1) / 3 + 2;
                    pressNumber(repetitions, number, prev);
                }
            }
            System.out.println();
        }
    }

    static void pressNumber(int repetitions, int number, int[] prev) {
        if (prev[0] == number) System.out.print(" ");
        while (repetitions-- > 0) System.out.print(number);
        prev[0] = number;
    }
}
