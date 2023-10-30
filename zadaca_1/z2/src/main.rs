use clap::Parser;

#[derive(Parser, Debug)]
struct Args {
    /// preciznost u dijelovima, vece -> bolje
    #[arg(short, long)]
    precision: u32,
}

#[derive(Debug)]
struct Vertex {
    x: f32,
    y: f32,
    z: f32,
    index: u32,
}

#[derive(Debug)]
struct Face<'a> {
    vertex_1: &'a Vertex,
    vertex_2: &'a Vertex,
    vertex_3: &'a Vertex,
}

#[derive(Debug)]
struct Obj<'a> {
    verices: Vec<Vertex>,
    faces: Vec<Face<'a>>,
}

const RADIJUS: f32 = 1.0;
const VISINA: f32 = 2.0;

fn main() {
    let args = Args::parse();

    if args.precision <= 1 {
        panic!("Preciznost more biti veca od 1")
    }

    let promjer = RADIJUS * 2.0;
    let dio = promjer / (args.precision as f32);

    let mut base: Vec<Vertex> = Vec::new();
    for i in 0..args.precision + 1 {
        let x = (i as f32) * dio - RADIJUS;
        let z = (RADIJUS * RADIJUS - x * x).sqrt();

        base.push(Vertex {
            x,
            y: 0.0,
            z,
            index: 0,
        });

        if x != RADIJUS && x != -RADIJUS {
            base.push(Vertex {
                x,
                y: 0.0,
                z: -z,
                index: 0,
            });
        }
    }

    dbg!(base);
}
