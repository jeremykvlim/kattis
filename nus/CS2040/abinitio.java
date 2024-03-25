import java.io.*;

public class abinitio {
    static final int MODULO = (int) (1e9 + 7);

    public static void main(String[] args) throws Exception {
        var io = new IO(System.in);
        var pw = new PrintWriter(System.out);

        int v = io.nextInt(), e = io.nextInt(), q = io.nextInt();
        var adjMatrix = new int[v + q][v + q];
        var status = new int[2];

        while (e-- > 0) adjMatrix[io.nextInt()][io.nextInt()] = 1;

        while (q-- > 0) {
            int c = io.nextInt(), x, y;
            switch (c) {
                case 1:
                    for (int i = 0; i < v; i++) adjMatrix[i][v] = adjMatrix[v][i] = status[1] == 0 ? 0 : 1;
                    adjMatrix[v][v] = 0;
                    v++;
                    break;
                case 2:
                    x = io.nextInt();
                    y = io.nextInt();
                    if (status[0] == 0) adjMatrix[x][y] ^= 1;
                    else adjMatrix[y][x] ^= 1;
                    break;
                case 3:
                    x = io.nextInt();
                    for (int i = 0; i < v; i++) adjMatrix[i][x] = adjMatrix[x][i] = status[1] == 0 ? 0 : 1;
                    break;
                case 4:
                    x = io.nextInt();
                    y = io.nextInt();
                    if (status[0] == 0) adjMatrix[x][y] = status[1];
                    else adjMatrix[y][x] = status[1];
                    break;
                case 5:
                    status[0] ^= 1;
                    break;
                case 6:
                    status[1] ^= 1;
                    break;
            }
        }
        pw.println(v);

        for (int i = 0; i < v; i++) {
            long d = 0, h = 0;
            for (int j = v - 1; j >= 0; j--) 
                if (i != j && adjMatrix[status[0] == 1 ? j : i][status[0] == 1 ? i : j] == (status[1] == 1 ? 0 : 1)) {
                    d++;
                    h = (7 * h + j) % MODULO;
                }
            pw.println(d + " " + h);
        }

        pw.flush();
    }

    static class IO {
        InputStream dis;
        byte[] buffer = new byte[1 << 17];
        int pointer = 0;

        IO(InputStream is) {
            dis = is;
        }
        
        int nextInt() throws Exception {
            int n = 0;
            byte b;
            do {
                b = nextByte();
            } while (b <= ' ');
            boolean negative = false;
            if (b == '-') {
                negative = true;
                b = nextByte();
            }
            while (b >= '0' && b <= '9') {
                n = 10 * n + b - '0';
                b = nextByte();
            }
            return (negative) ? -n : n;
        }
        
        byte nextByte() throws Exception {
            if (pointer == buffer.length) {
                dis.read(buffer, 0, buffer.length);
                pointer = 0;
            }
            return buffer[pointer++];
        }
    }
}
