import java.io.*;
import java.util.*;

//Solution checker for this problem is erroneous
public class babynames {
    static class TrieNode {
        TrieNode[] letters = new TrieNode[26]; //potential nodes to continue the prefix
        boolean word_end; //signifies end of a word
        int word_count; //number of words with a given prefix
        Set<String> lookup = new HashSet<>(); //words within the trie

        TrieNode() {
            word_end = false;
            word_count = 0;
        }
    }
    static void insert(TrieNode root, String name) {
        TrieNode node = root;
        node.word_count++;
        node.lookup.add(name);
        for (char c : name.toCharArray()) {
            int index = c - 'A';
            if (node.letters[index] == null) //if this prefix has not been added
                node.letters[index] = new TrieNode(); //create new node to represent next letter in prefix
            node = node.letters[index];
            node.word_count++;
        }
        node.word_end = true; //once nodes for every letter in the word have been created, mark end of word
    }

    static void remove(TrieNode root, String key) {
        if (!root.lookup.contains(key)) return; //if word does not exist, return
        var stack = new ArrayList<TrieNode>();
        TrieNode node = root;
        node.word_count--;
        for (char c : key.toCharArray()) {
            int index = c - 'A';
            node = node.letters[index]; //iterate through every letter in the word
            stack.add(node); //push node references for each letter into stack
        }
        node.word_end = false; //unmark end of word

        for (int i = stack.size() - 1; i >= 0; i--) { //iterating from top of stack
            int index = key.charAt(i) - 'A';
            for (var letter : stack.get(i).letters)
                if (letter != null) { //if this prefix is part of another word
                    stack.get(i).word_count--; //decrement word count for this letter
                    while (i-- > 0) stack.get(i).word_count--; //decrement word count for the other letters in this prefix
                    return;
                }
            stack.get(i).letters[index] = null; //set back to NULL
            stack.get(i).word_count--; //decrement word count
        }
    }

    static int check(TrieNode root, String start, String end) {
        int countStart = 0, countEnd = 0;
        //range of [start, end) = range of [A, end) - range of [A, start)
        //example: CHECK ASD ZXC
        countStart += countInterval(root, start); //first count [A, ASD), "ASD" excluded; equivalent to checking [A, ASC]
        countStart += countPrefix(root, start); //check if "A" or "AS" are words in the trie as they are not accounted for yet
        countEnd += countInterval(root, end); //now count [A, ZXC), "ZXC" excluded; equivalent to checking [A, ZXB]
        countEnd += countPrefix(root, end); //check if "Z" or "ZX" are words in the trie as they are not accounted for yet

        return countEnd - countStart;
    }

    static int countInterval(TrieNode node, String range_end) {
        //counts words from [A, range_end), "range_end" excluded
        int count = 0;
        for (char c : range_end.toCharArray()) {
            int index = c - 'A';
            for (int i = 0; i < index; i++)
                if (node.letters[i] != null)
                    count += node.letters[i].word_count;
            if (node.letters[index] == null) break;
            node = node.letters[index];
        }
        return count;
    }

    static int countPrefix(TrieNode node, String prefix) {
        //counts words if they are prefixes of a prefix
        //i.e. if prefix = "ASD", counts "A" and "AS" if they are in the trie
        int same = 0, count = 0;
        if (node.letters[prefix.charAt(0) - 'A'] != null) {
            for (char c : prefix.toCharArray()) {
                same++;
                int index = c - 'A';
                if (node.letters[index] != null) {
                    node = node.letters[index];
                    if (node.word_end && prefix.length() != same) count++;
                } else break;
            }
        }
        return count;
    }

    public static void main(String[] args) throws IOException {
        var male = new TrieNode();
        var female = new TrieNode();
        var br = new BufferedReader(new InputStreamReader(System.in));
        var pw = new PrintWriter(System.out);

        label:
        for(;;) {
            var input = br.readLine().split(" ");

            switch (input[0]) {
                case "1": {
                    String name = input[1], gender = input[2];
                    if (gender.equals("1")) insert(male, name);
                    else insert(female, name);
                    break;
                }
                case "2": {
                    String name = input[1];
                    remove(male, name);
                    remove(female, name);
                    break;
                }
                case "3": {
                    String start = input[1], end = input[2], gender = input[3];
                    int count = 0;
                    if (gender.equals("1")) count += check(male, start, end);
                    else if (gender.equals("2")) count += check(female, start, end);
                    else count += check(male, start, end) + check(female, start, end);
                    pw.println(count);
                    break;
                }
                default:
                    break label;
            }
        }
        pw.flush();
    }

}
