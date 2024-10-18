import java.io.*;
import java.util.*;

public class dungeonmonsterfarm {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));

        var input = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int x = input[0], n = input[1];

        try (var farm = new Farm(x, n)) {
            for (var op = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray(); op[0] != 3; op = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray()) {
                switch (op[0]) {
                    case 0 -> farm.buyMonster(op[1], op[2], op[3]);
                    case 1 -> farm.sellMonster(op[1], op[2]);
                    case 2 -> farm.raiseMonster(op[1]);
                }
            }
        }
    }
}

class Monster {
    int type, daysRaised, weight, dailyWeightGain;

    Monster(int t, int w, int dwg) {
        type = t;
        daysRaised = 0;
        weight = w;
        dailyWeightGain = dwg;
    }
}

class Farm implements AutoCloseable {
    int coins, capacity;
    ArrayList<Monster> monsters;

    final int[] buyingPrice = {5, 20, 84}, initialWeight = {1, 2, 4}, sellingPrice = {1, 4, 6};

    Farm(int X, int N) {
        coins = X;
        capacity = N;
        monsters = new ArrayList<>();
    }

    int buyMonster(int type, int count, int dailyWeightGain) {
        int cost = count * buyingPrice[type];
        if (coins < cost || capacity < count) return monsters.size();

        coins -= cost;
        capacity -= count;
        while (count-- > 0) monsters.add(new Monster(type, initialWeight[type], dailyWeightGain));
        print();
        return monsters.size();
    }

    int sellMonster(int type, int count) {
        var list = new ArrayList<Monster>();
        for (var m : monsters)
            if (m.type == type) list.add(m);

        if (list.size() < count) return coins;

        list.sort((a, b) -> b.weight - a.weight);
        int profit = 0, sold = 0;
        for (var m : list) {
            profit += m.weight * sellingPrice[m.type];
            monsters.remove(m);
            if (++sold == count) break;
        }
        capacity += sold;
        coins += profit;
        print();
        return coins;
    }

    void raiseMonster(int days) {
        for (var m : monsters) {
            m.weight += m.dailyWeightGain * days;
            m.daysRaised += days;
        }
        print();
    }

    void print() {
        System.out.println(coins + " " + capacity);
    }

    @Override
    public void close() {
        int profit = 0;
        for (var m : monsters) profit += m.weight * sellingPrice[m.type];
        capacity += monsters.size();
        monsters.clear();
        coins += profit;
        print();
    }
}
