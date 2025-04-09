#include <iostream>
#include <fstream>
#include <cstdint>
#include <functional>
#include <limits>

using namespace std;

std::size_t betterHash(double x, double y, std::size_t seed) {
    std::size_t hx = std::hash<double>{}(x);
    std::size_t hy = std::hash<double>{}(y);
    std::size_t hs = std::hash<std::size_t>{}(seed);

    std::size_t combined = hx ^ (hy << 1) ^ (hs << 2);
    return combined;
}

double hashToUnitInterval(double x, double y, std::size_t seed = 0) {
    std::size_t hash = betterHash(x, y, seed);
    return static_cast<double>(hash) / static_cast<double>(std::numeric_limits<std::size_t>::max());
}

double smoothstep(double x)
{
    return 3 * x * x - 2 * x * x * x;
}
double lerp(double floor_value, double ceil_value, double t) {
    return floor_value + (ceil_value - floor_value) * smoothstep(t);
}

struct vec3
{
    double x;
    double y;
    double z;
    vec3(double x, double y, double z) : x(x), y(y), z(z) {}
};

void generate_random_texture(int width, const char *filename , int seed = 0)
{
    double aspect_ratio = 16.0 / 9.0;
    int height = static_cast<int>(width / aspect_ratio);
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    auto start = vec3(0, 0, 0);
    auto pixel_00 = vec3(start.x - viewport_width / 2, start.y - viewport_height / 2, start.z);
    auto horizental_distance = viewport_width / width;
    auto vertical_distance = viewport_height / height;
    fstream image;
    image.open(filename, ios::out);
    image << "P3\n"
          << width << " " << height << "\n255\n";
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double tx = static_cast<double>(i) / width;
            double ty = static_cast<double>(j) / height;
            auto pixel = vec3(pixel_00.x + i * horizental_distance, pixel_00.y + j * vertical_distance, pixel_00.z);
            auto n01 = hashToUnitInterval(pixel.x - horizental_distance/2  +  pixel.y - vertical_distance/2 , seed); ;
            auto n02 = hashToUnitInterval(pixel.x + horizental_distance/2  +  pixel.y - vertical_distance/2 , seed); ;
            auto n03 = hashToUnitInterval(pixel.x - horizental_distance/2  + pixel.y + vertical_distance/2 , seed); ;
            auto n04 = hashToUnitInterval(pixel.x + horizental_distance/2  + pixel.y + vertical_distance/2 , seed); ;
            auto value = lerp(lerp(n01, n02, tx), lerp(n03, n04, tx), ty);
            auto r = static_cast<int>(value * 255);
            auto g = static_cast<int>(value * 255);
            auto b = static_cast<int>(value * 255);
            image << r << " " << g << " " << b << "\n";
        }
    }
    image.close();
}

int main(int, char **)
{
    for(int i=0 ; i< 100 ; ++i){
    std::string filename = "images/random_texture" + std::to_string(i) + ".ppm";
    generate_random_texture(800, filename.c_str() , i);
    }
    cout << " texture generated and saved " << endl;
    return 0;
}
