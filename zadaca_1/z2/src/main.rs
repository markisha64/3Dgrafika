use std::fs;

#[derive(Debug, Clone)]
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
    vertices: &'a Vec<Vertex>,
    faces: Vec<Face<'a>>,
}

impl<'a> Obj<'a> {
    fn to_obj(&'a mut self) -> String {
        let mut output: Vec<String> = Vec::new();

        for vertex in self.vertices.iter() {
            output.push(format!("v {} {} {}", vertex.x, vertex.y, vertex.z));
        }

        for face in self.faces.iter() {
            output.push(format!(
                "f {} {} {}",
                face.vertex_1.index + 1,
                face.vertex_2.index + 1,
                face.vertex_3.index + 1
            ));
        }

        output.join("\n")
    }
}

const RADIJUS: f32 = 1.0;
const VISINA: f32 = 2.0;
const PRECIZNOST: u32 = 100;

fn main() {
    let kut_podijela = 2.0 * std::f32::consts::PI / PRECIZNOST as f32;

    let mut base: Vec<Vertex> = Vec::new();
    for i in 0..PRECIZNOST {
        let kut = kut_podijela * i as f32;

        base.push(Vertex {
            x: RADIJUS * kut.cos(),
            y: 0.0,
            z: RADIJUS * kut.sin(),
            index: 0,
        });
    }

    let mut base1 = base.clone();

    let mut faces: Vec<Face> = Vec::new();

    let len = base.len();

    for vertex in &mut base1 {
        vertex.y = VISINA;
    }

    base.extend(base1);

    for i in 0..base.len() {
        base[i].index = i as u32;
    }

    for i in 1..len - 1 {
        faces.push(Face {
            vertex_1: &base[0],
            vertex_2: &base[i],
            vertex_3: &base[i + 1],
        })
    }

    for i in 1..len - 1 {
        faces.push(Face {
            vertex_1: &base[len],
            vertex_2: &base[len + i],
            vertex_3: &base[len + i + 1],
        })
    }

    for i in 0..len - 1 {
        faces.push(Face {
            vertex_1: &base[i],
            vertex_2: &base[i + 1],
            vertex_3: &base[len + i],
        });

        faces.push(Face {
            vertex_1: &base[i + 1],
            vertex_2: &base[(len + i) % (len * 2 - 1) + 1],
            vertex_3: &base[len + i],
        });
    }

    faces.push(Face {
        vertex_1: &base[len - 1],
        vertex_2: &base[len + len - 1],
        vertex_3: &base[0],
    });

    faces.push(Face {
        vertex_1: &base[len],
        vertex_2: &base[len + len - 1],
        vertex_3: &base[0],
    });

    let mut obj = Obj {
        vertices: &base,
        faces,
    };

    fs::write("cilindar.obj", obj.to_obj()).unwrap();
}
