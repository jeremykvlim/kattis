import java.io.*;

public class abinitio {
    static final int MOD = (int) (1e9 + 7);

    public static void main(String[] args) throws Exception {
        var io = new IO(System.in, System.out);

        int v = io.nextInt(), e = io.nextInt(), q = io.nextInt();
        var adjMatrix = new int[v + q][v + q];
        var status = new int[2];

        while (e-- > 0) adjMatrix[io.nextInt()][io.nextInt()] = 1;

        while (q-- > 0) {
            int c = io.nextInt(), x, y;
            switch (c) {
                case 1 -> {
                    for (int i = 0; i < v; i++) adjMatrix[i][v] = adjMatrix[v][i] = status[1] == 0 ? 0 : 1;
                    adjMatrix[v][v] = 0;
                    v++;
                }
                case 2 -> {
                    x = io.nextInt();
                    y = io.nextInt();
                    if (status[0] == 0) adjMatrix[x][y] ^= 1;
                    else adjMatrix[y][x] ^= 1;
                }
                case 3 -> {
                    x = io.nextInt();
                    for (int i = 0; i < v; i++) adjMatrix[i][x] = adjMatrix[x][i] = status[1] == 0 ? 0 : 1;
                }
                case 4 -> {
                    x = io.nextInt();
                    y = io.nextInt();
                    if (status[0] == 0) adjMatrix[x][y] = status[1];
                    else adjMatrix[y][x] = status[1];
                }
                case 5 -> status[0] ^= 1;
                case 6 -> status[1] ^= 1;
            }
        }
        io.println(v);

        for (int i = 0; i < v; i++) {
            long d = 0, h = 0;
            for (int j = v - 1; j >= 0; j--)
                if (i != j && adjMatrix[status[0] == 1 ? j : i][status[0] == 1 ? i : j] == (status[1] == 1 ? 0 : 1)) {
                    d++;
                    h = (7 * h + j) % MOD;
                }
            io.println(d + " " + h);
        }
        io.flush();
    }

    static class IO {
        InputStream in;
        OutputStream out;
        byte[] in_buff, out_buff;
        int in_pointer, out_pointer;

        IO(InputStream is, OutputStream os) {
            in = is;
            out = os;
            in_buff = out_buff = new byte[1 << 17];
            in_pointer = out_pointer = 0;
        }

        int nextInt() throws Exception {
            int n = 0;
            byte b;
            do {
                b = nextByte();
            } while (b <= ' ');

            var negative = false;
            if (b == '-') {
                negative = true;
                b = nextByte();
            }

            while ('0' <= b && b <= '9') {
                n = 10 * n + b - '0';
                b = nextByte();
            }
            return (negative) ? -n : n;
        }

        String nextString() throws Exception {
            var sb = new StringBuilder();
            byte b;
            do {
                b = nextByte();
            } while (b <= ' ');

            while (b != ' ') {
                sb.append((char) b);
                b = nextByte();
            }
            return sb.toString();
        }

        byte nextByte() throws Exception {
            if (in_pointer == in_buff.length) {
                in.read(in_buff, 0, in_buff.length);
                in_pointer = 0;
            }
            return in_buff[in_pointer++];
        }

        void push() throws IOException {
            out.write(out_buff, 0, out_pointer);
            out_pointer = 0;
        }

        void print(int v) throws IOException {
            var s = String.valueOf(v);
            for (int i = 0; i < s.length(); i++) {
                if (out_pointer == out_buff.length) push();
                out_buff[out_pointer++] = (byte) s.charAt(i);
            }
        }

        void println(int v) throws IOException {
            print(v);
            if (out_pointer == out_buff.length) push();
            out_buff[out_pointer++] = (byte) '\n';
        }

        void print(String s) throws IOException {
            for (int i = 0; i < s.length(); i++) {
                if (out_pointer == out_buff.length) push();
                out_buff[out_pointer++] = (byte) s.charAt(i);
            }
        }

        void println(String s) throws IOException {
            print(s);
            if (out_pointer == out_buff.length) push();
            out_buff[out_pointer++] = (byte) '\n';
        }

        void flush() throws IOException {
            push();
            out.flush();
        }
    }
}
