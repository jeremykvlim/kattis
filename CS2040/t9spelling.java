import java.io.*;

public class t9spelling {

    static int lastPressedNumber;

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(br.readLine()), count = 1;
        lastPressedNumber = - 1;
        int number, repetitions;

        while (n-- > 0) {
            System.out.print("Case #"+(count++)+": ");
            var line = br.readLine();
            for (int i = 0; i < line.length(); i++) {
                char letter = line.charAt(i);

                if (letter == ' ') {
                    repetitions = 1;
                    number = 0;
                    pressNumber(repetitions, number);
                }
                else if (letter == 's') {
                    repetitions = 4;
                    number = 7;
                    pressNumber(repetitions, number);
                }
                else if (letter == 'z') {
                    repetitions = 4;
                    number = 9;
                    pressNumber(repetitions, number);
                }
                else if (letter < 116) {
                    repetitions = (letter - 'a') % 3 + 1;
                    number = (letter - 'a') / 3 + 2;
                    pressNumber(repetitions, number);
                }
                else if (letter < 123) {
                    repetitions = (letter - 'a' - 1) % 3 + 1;
                    number = (letter - 'a' - 1) / 3 + 2;
                    pressNumber(repetitions, number);
                }
            }
            System.out.println();
        }
    }

    static void pressNumber(int repetitions, int number) {
        if (lastPressedNumber == number) System.out.print(" ");
        while (repetitions-- > 0) System.out.print(number);
        lastPressedNumber = number;
    }
}
