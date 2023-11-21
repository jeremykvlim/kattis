import java.io.*;
import java.util.*;

public class joinstrings {
    static PrintWriter pw = new PrintWriter(System.out);

    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        int n = Integer.parseInt(br.readLine());

        var arr = new String[n];
        for (int i = 0; i < n; i++) arr[i] = br.readLine();

        var adjList = new ArrayList[n];
        for (int i = 0; i < n; i++) adjList[i] = new ArrayList<Integer>();

        int source = 0;
        for (int i = 0; i < n - 1; i++) {
            int[] operation = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
            source = operation[0] - 1;
            adjList[source].add(operation[1] - 1);
        }

        pw.print(arr[source]);
        print(source, adjList, arr);
        pw.flush();
    }

    static void print(int idx, ArrayList<Integer>[] aL, String[] arr) {
        for (var i : aL[idx]) {
            pw.print(arr[i]);
            print(i, aL, arr);
        }
    }
}
