use std::io::{stdin, stdout, BufRead, BufWriter, Write};

fn m(n: usize, x: f64, y: f64, z: f64, c: f64, r: f64) -> f64 {
    let mut dp = vec![vec![0.0; n + 1]; 2];
    dp[0][0] = x;
    for i in 0..n {
        dp[(i + 1) % 2] = vec![0.0; n + 1];
        for j in i..=n {
            for k in j..=n {
                let donate = (k - j) as f64 * y;
                let credit = f64::max(0.0, donate - z) * c / 100.0;
                dp[(i + 1) % 2][k] = f64::max(dp[(i + 1) % 2][k], (dp[i % 2][j] - donate + credit) * (1.0 + r / 100.0));
            }
        }
    }
    return dp[n % 2][n];
}

fn main() {
    let stdin = stdin();
    let stdout = stdout();
    let mut reader = Reader::from(stdin.lock());
    let mut writer = BufWriter::new(stdout.lock());
    let n: usize = reader.next();
    let x: f64 = reader.next();
    let y: f64 = reader.next();
    let z: f64 = reader.next();
    let c: f64 = reader.next();
    let r: f64 = reader.next();
    write!(writer, "{:.10}", m(n, x, y, z, c, r));
}

pub struct Reader<B> {
    reader: B,
    buffer: Vec<String>,
}

impl<R: BufRead> Reader<R> {
    pub fn next<T: std::str::FromStr>(&mut self) -> T {
        loop {
            if let Some(token) = self.buffer.pop() {
                return token.parse().ok().expect("Failed parse");
            }
            let mut input = String::new();
            self.reader.read_line(&mut input).expect("Failed read");
            self.buffer = input.split_whitespace().rev().map(String::from).collect();
        }
    }
}

impl<R: BufRead> From<R> for Reader<R> {
    fn from(reader: R) -> Self {
        Self { reader, buffer: Vec::new() }
    }
}
