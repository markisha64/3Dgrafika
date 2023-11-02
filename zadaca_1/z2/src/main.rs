use std::fs;

#[derive(Debug, Clone)]
struct Vertex {
    x: f32,
    y: f32,
    z: f32,
    index: u32,
}

#[derive(Debug, Clone)]
struct Normal {
    x: f32,
    y: f32,
    z: f32,
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
    normals: Vec<Normal>,
    faces: Vec<Face<'a>>,
}

impl<'a> Obj<'a> {
    fn to_obj(&'a mut self) -> String {
        let mut output: Vec<String> = Vec::new();

        for vertex in self.vertices.iter() {
            output.push(format!("v {} {} {}", vertex.x, vertex.y, vertex.z));
        }

        for normal in self.normals.iter() {
            output.push(format!("vn {} {} {}", normal.x, normal.y, normal.z));
        }

        for face in self.faces.iter() {
            output.push(format!(
                "f {}//{} {}//{} {}//{}",
                face.vertex_1.index + 1,
                face.vertex_1.index + 1,
                face.vertex_2.index + 1,
                face.vertex_2.index + 1,
                face.vertex_3.index + 1,
                face.vertex_3.index + 1
            ));
        }

        output.join("\n")
    }
}

const RADIJUS: f32 = 1.0;
const VISINA: f32 = 2.0;

const VISINA_POLA: f32 = VISINA / 2.0;
const PRECIZNOST: u32 = 1000;

fn main() {
    let kut_podijela = 2.0 * std::f32::consts::PI / PRECIZNOST as f32;

    let mut base: Vec<Vertex> = Vec::new();
    for i in 0..PRECIZNOST {
        let kut = kut_podijela * i as f32;

        base.push(Vertex {
            x: RADIJUS * kut.cos(),
            y: -VISINA_POLA,
            z: RADIJUS * kut.sin(),
            index: 0,
        });
    }

    let base_copy = base.clone();
    let mut base_top = base.clone();
    let mut base_top_copy = base.clone();

    let mut faces: Vec<Face> = Vec::new();

    let len = base.len();

    for vertex in &mut base_top {
        vertex.y = VISINA_POLA;
    }

    for vertex in &mut base_top_copy {
        vertex.y = VISINA_POLA;
    }

    let mut normals: Vec<Normal> = Vec::new();

    for _ in base.iter() {
        normals.push(Normal {
            x: 0.0,
            y: -1.0,
            z: 0.0,
        });
    }

    for _ in base_top.iter() {
        normals.push(Normal {
            x: 0.0,
            y: 1.0,
            z: 0.0,
        });
    }

    for vertex in base_copy.iter() {
        normals.push(Normal {
            x: vertex.x,
            y: 0.0,
            z: vertex.z,
        });
    }

    for vertex in base_top_copy.iter() {
        normals.push(Normal {
            x: vertex.x,
            y: 0.0,
            z: vertex.z,
        });
    }

    base.extend(base_top);
    base.extend(base_copy);
    base.extend(base_top_copy);

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
            vertex_2: &base[len + i + 1],
            vertex_3: &base[len + i],
        })
    }

    for i in 0..len - 1 {
        faces.push(Face {
            vertex_1: &base[len * 2 + i],
            vertex_2: &base[len * 3 + i],
            vertex_3: &base[len * 2 + i + 1],
        });

        faces.push(Face {
            vertex_1: &base[len * 2 + i + 1],
            vertex_2: &base[len * 3 + i],
            vertex_3: &base[(len * 3 + i) % (len * 4 - 1) + 1],
        });
    }

    faces.push(Face {
        vertex_1: &base[len * 3 - 1],
        vertex_2: &base[len * 4 - 1],
        vertex_3: &base[len * 2],
    });

    faces.push(Face {
        vertex_1: &base[len * 3],
        vertex_2: &base[len * 2],
        vertex_3: &base[len * 4 - 1],
    });

    let mut obj = Obj {
        vertices: &base,
        normals,
        faces,
    };

    fs::write("cilindar.obj", obj.to_obj()).unwrap();
}
