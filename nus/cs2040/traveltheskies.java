import java.io.*;
import java.util.*;

public class traveltheskies {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        var input = br.readLine().split(" ");
        int k = Integer.parseInt(input[0]), n = Integer.parseInt(input[1]), m = Integer.parseInt(input[2]);

        var adjMatrix = new int[k + 1][n + 1];
        var flights = new Flight[m];
        for (int i = 0; i < m; i++) {
            var line = br.readLine().split(" ");
            int u = Integer.parseInt(line[0]), v = Integer.parseInt(line[1]), d = Integer.parseInt(line[2]), z = Integer.parseInt(line[3]);
            flights[i] = new Flight(u, v, d, z);
        }

        Arrays.sort(flights);

        for (int i = 0; i < k * n; i++) {
            var line = br.readLine().split(" ");
            int a = Integer.parseInt(line[0]), b = Integer.parseInt(line[1]), c = Integer.parseInt(line[2]);
            adjMatrix[a][b] += c;
        }

        var people = new int[k + 1];
        for (int i = 0, prev = 1; i < m; i++) {
            var f = flights[i];
            if (f.d != prev) {
                prev = f.d;
                for (int j = 1; j < k + 1; j++)
                    adjMatrix[j][prev] += adjMatrix[j][prev - 1] + people[j];
                people = new int[k + 1];
            }

            if (adjMatrix[f.u][f.d] < f.z) {
                System.out.println("suboptimal");
                return;
            } else {
                people[f.v] += f.z;
                adjMatrix[f.u][f.d] -= f.z;
            }
        }
        System.out.println("optimal");
    }

    static class Flight implements Comparable<Flight> {
        int u, v, d, z;

        Flight(int source, int destination, int day, int capacity) {
            u = source;
            v = destination;
            d = day;
            z = capacity;
        }

        @Override
        public int compareTo(Flight f) {
            return d - f.d;
        }
    }
}
