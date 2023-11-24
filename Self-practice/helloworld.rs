use std::io::{stdout, BufWriter, Write};

fn main() {
    let stdout = stdout();
    let mut bw = BufWriter::new(stdout.lock());

    writeln!(bw, "Hello World!");
}
