    import java.io.*;

    public class teque {

        private static final int SIZE = (int) Math.pow(10, 6);
        private static String[] frontHalf = new String[SIZE], backHalf = new String[SIZE];
        private static int backHead, backTail, backSize = 0, frontHead, frontTail, frontSize = 0;
        private static BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        private static PrintWriter pw = new PrintWriter(System.out);

        public static void main(String[] args) throws IOException {
            int middleIndex = SIZE / 2;
            frontHead = middleIndex - 1;
            backTail = middleIndex + 1;
            backHead = middleIndex;
            frontTail = middleIndex;

            int numberOfLines = Integer.parseInt(br.readLine());

            for (int i = 0; i < numberOfLines; i++) {
                String[] command = br.readLine().split(" ");

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
                        pw.println(getString(Integer.parseInt(command[1])));
                        break;
                    }
                }
            }
            pw.flush();
        }

        private static void balance() {
            if (frontSize > backSize + 1) {
                backHalf[backHead--] = frontHalf[frontTail - 1];
                frontHalf[frontTail-- - 1] = null;
                backSize++;
                frontSize--;
            }
            else if (frontSize + 1 < backSize) {
                frontHalf[frontTail++] = backHalf[backHead + 1];
                backHalf[backHead++ + 1] = null;
                backSize--;
                frontSize++;
            }
        }

        private static void addFront(String s) {
            frontHalf[frontHead--] = s;
            frontSize++;
        }

        private static void addBack(String s) {
            backHalf[backTail++] = s;
            backSize++;
        }

        private static void addMiddle(String s) {
            if (frontSize >= backSize) {
                backHalf[backHead--] = s;
                backSize++;
            } else {
                frontHalf[frontTail++] = backHalf[backHead + 1];
                backHalf[backHead + 1] = s;
                frontSize++;
            }
        }

        private static String getString(int i) {
            if (i > frontSize - 1) {
                i -= frontSize;
                return backHalf[backHead + 1 + i];
            } else return frontHalf[frontHead + 1 + i];
        }

    }
