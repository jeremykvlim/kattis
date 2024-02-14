import java.io.*;

public class detaileddifferences {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(br.readLine());
        while (n-- > 0) {
            String first = br.readLine(), second = br.readLine();
            System.out.println(first + "\n" + second);
            
            for (int i = 0; i < first.length(); i++)
                System.out.print(first.charAt(i) != second.charAt(i) ? "*" : ".");
            System.out.println("\n");
        }
    }
}
