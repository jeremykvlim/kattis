import java.io.*;
import java.util.*;

public class bestrelayteam {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        
        int runners = Integer.parseInt(br.readLine());
        var array = new Runner[runners];
        for (int i = 0; i < runners; i++) {
            var line = br.readLine().split(" ");
            array[i] = new Runner(line[0], Double.parseDouble(line[1]), Double.parseDouble(line[2]));
        }
        Arrays.sort(array);

        var fastest = Double.POSITIVE_INFINITY;
        var best = new ArrayList<Runner>();
        for (var r : array) {
            var time = 0;
            var team = new ArrayList<Runner>();
            time += r.first;
            team.add(r);

            for (var ru : array) {
                if (team.size() == 4) break;
                if (r != ru) {
                    time += ru.second;
                    team.add(ru);
                }
            }

            if (time < fastest) {
                fastest = time;
                best = team;
            }
        }

        System.out.println(fastest);
        for (var r : best) System.out.println(r.name);
    }

    static class Runner implements Comparable<Runner> {
        String name;
        double first, second;

        Runner(String n, double f, double s) {
            name = n;
            first = f;
            second = s;
        }

        @Override
        public int compareTo(Runner r) {
            return Double.compare(second, r.second);
        }
    }
}
