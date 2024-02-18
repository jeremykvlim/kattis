import java.io.*;
import java.util.*;

public class cardtrading {
    public static void main(String[] args) throws IOException {
        var br = new BufferedReader(new InputStreamReader(System.in));
        var inputs = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray();
        int t = inputs[1], k = inputs[2];

        int[] cardCount = new int[t], cards = Arrays.stream(br.readLine().split(" ")).mapToInt(Integer::parseInt).toArray(); 
        for (int i : cards) cardCount[i - 1]++;

        var cardPrices = new Card[t];

        for (int i = 0; i < t; i++) {
            var input = br.readLine().split(" ");
            cardPrices[i] = new Card(Integer.parseInt(input[0]), Integer.parseInt(input[1]));
        }

        long profit = 0;
        for (int i = 0; i < cardCount.length; i++) profit += cardCount[i] * cardPrices[i].sell;

        var opportunity = new int[t];
        for (int i = 0; i < t; i++) opportunity[i] = cardPrices[i].buy * (2 - cardCount[i]) + cardPrices[i].sell * (cardCount[i]);

        Arrays.sort(opportunity);
        for (int i = 0; i < k; i++) profit -= opportunity[i];

        System.out.println(profit);
    }

    static class Card {
        int buy, sell;

        Card(int b, int s) {
            buy = b;
            sell = s;
        }
    }
}
