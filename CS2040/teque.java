    import java.io.*;

    public class teque {

        static final int SIZE = (int) 1e6;
        static String[] frontHalf = new String[SIZE], backHalf = new String[SIZE];
        static int backHead, backTail, backSize = 0, frontHead, frontTail, frontSize = 0;
        static BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        static PrintWriter pw = new PrintWriter(System.out);

        public static void main(String[] args) throws IOException {
            int middleIndex = SIZE / 2;
            frontHead = middleIndex - 1;
            backTail = middleIndex + 1;
            backHead = middleIndex;
            frontTail = middleIndex;

            int numberOfLines = Integer.parseInt(br.readLine());

            for (int i = 0; i < numberOfLines; i++) {
                var command = br.readLine().split(" ");

                switch (command[0]) {
                    case "push_front": {
                        addFront(command[1]);
                        balance();
                        break;
                    }
                    case "push_back": {
                        addBack(command[1]);
                        balance();
                        break;
                    }
                    case "push_middle": {
                        addMiddle(command[1]);
                        balance();
                        break;
                    }
                    default: {
                        pw.println(get(Integer.parseInt(command[1])));
                        break;
                    }
                }
            }
            pw.flush();
        }

        static void balance() {
            if (frontSize > backSize + 1) {
                backHalf[backHead--] = frontHalf[frontTail - 1];
                frontHalf[frontTail-- - 1] = null;
                backSize++;
                frontSize--;
            } else if (frontSize + 1 < backSize) {
                frontHalf[frontTail++] = backHalf[backHead + 1];
                backHalf[backHead++ + 1] = null;
                backSize--;
                frontSize++;
            }
        }

        static void addFront(String s) {
            frontHalf[frontHead--] = s;
            frontSize++;
        }

        static void addBack(String s) {
            backHalf[backTail++] = s;
            backSize++;
        }

        static void addMiddle(String s) {
            if (frontSize >= backSize) {
                backHalf[backHead--] = s;
                backSize++;
            } else {
                frontHalf[frontTail++] = backHalf[backHead + 1];
                backHalf[backHead + 1] = s;
                frontSize++;
            }
        }

        static String get(int i) {
            if (i > frontSize - 1) {
                i -= frontSize;
                return backHalf[backHead + 1 + i];
            } else return frontHalf[frontHead + 1 + i];
        }
    }
