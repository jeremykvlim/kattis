import java.util.*;

public class coconut {
    public static void main(String[] args) {
        var sc = new Scanner(System.in);
        int s = sc.nextInt(), n = sc.nextInt();

        var players = new ArrayList<Player>();
        for (int i = 0; i < n; i++) players.add(new Player("folded", i + 1));

        int i = 0;
        while (players.size() > 1) {
            i = (i + s - 1) % players.size();
            if (players.get(i).hands.equals("folded")) {
                players.get(i).hands = "fist";
                players.add(i, new Player("fist", players.get(i).number));
            } else if (players.get(i).hands.equals("fist"))
                players.get(i++).hands = "palm";
            else players.remove(i);
        }

        System.out.println(players.get(0).number);
    }

    static class Player {
        String hands;
        int number;

        Player(String h, int n) {
            hands = h;
            number = n;
        }
    }
}
