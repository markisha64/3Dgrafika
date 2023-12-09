#include "image.h"
#include "geometry.h"
#include "ray.h"
#include "objects.h"
#include "scene.h"
#include "light.h"
#include <cmath>
using namespace std;

// compile with: g++ -std=c++17 raytrace.cpp -o out.exe

Ray ray_to_pixel(Vec3f origin, const Image &img, int i, int j)
{
    Ray ray = Ray();

    float aspect_ratio = (float)img.width / img.height;
 
    float x = (-1 + 2 * (i + 0.5) / (float)img.width) * aspect_ratio;
    float y = (1 - 2 * (j + 0.5) / (float)img.height);
    float z = -1;

    ray.origin = origin;
    ray.direction = Vec3f(x, y, z).normalize();
    return ray;
}

void render(const Scene &scene)
{
    // dimenzije slike
    const int width = 1920;
    const int height = 1080;
    
    Vec3f origin = Vec3f(0, 0, 0);
    
    Image img = Image(width, height);
    
    // crtanje slike
    for (size_t j = 0; j < height; ++j)
    {
        for (size_t i = 0; i < width; ++i)
        {
            Ray ray = ray_to_pixel(origin, img, i, j);
            Vec3f colour = scene.cast_ray(ray, 0);
            img.set_pixel(i, j, colour);
        }
    }
    
    img.save("./img.ppm");
}

int main()
{
    Scene scene;
    
    Material red(Vec3f(1, 0, 0));
    Material green(Vec3f(0, 1, 0));
    Material blue(Vec3f(0, 0, 1));
    Material grey(Vec3f(0.5, 0.5, 0.5));
    Material pink(Vec3f(0.39, 0.29, 0.31));
    green.opacity = 0.2;
    pink.reflexivity = 0.4;
    
    Sphere s1(Vec3f(-3, 0, -16), 2, red);
    Sphere s2(Vec3f(-1, -1.5, -12), 2.1, green);

    Cuboid c1(Vec3f(1, 1, -10), Vec3f(5, 5, -5), blue);
    Cuboid c2(Vec3f(-5, 5, -10), Vec3f(-3, -3, -5), grey);

    Cuboid podloga(Vec3f(100, -6, 100), Vec3f(-100, -4, -100), pink);
    
    scene.add_object(&s1);
    scene.add_object(&s2);
    scene.add_object(&c1);
    scene.add_object(&c2);
    
    scene.add_object(&podloga);
    
    Light l1(Vec3f(-20, 20, 20), 1500);
    Light l2(Vec3f(20, 10, 20), 1800);
    
    scene.add_light(&l1);
    scene.add_light(&l2);
    
    render(scene);
    return 0;
}
