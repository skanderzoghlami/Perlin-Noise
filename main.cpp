#include <iostream>
#include <fstream>
#include <cstdint>
#include <functional>
#include <limits>
#include <cmath>

using namespace std;

uint32_t whatever_hash(int x, int y, uint32_t seed = 0)
{
    uint32_t h = seed;
    h ^= 0x85ebca6b * x;
    h ^= 0xc2b2ae35 * y;
    h ^= h >> 13;
    h *= 0x27d4eb2d;
    h ^= h >> 15;
    return h;
}
double hashToUnitInterval(int x, int y, std::uint32_t seed = 0)
{
    return static_cast<double>(whatever_hash(x, y, seed)) / static_cast<double>(UINT32_MAX);
}

double smoothstep(double x)
{
    return 3 * x * x - 2 * x * x * x;
}
double lerp(double floor_value, double ceil_value, double t)
{
    return floor_value + (ceil_value - floor_value) * (t);
}

struct vec2
{
    double x;
    double y;
    vec2(double x, double y) : x(x), y(y) {}
};

double valueNoise(double x, double y, std::size_t seed = 0)
{
    int x0 = static_cast<int>(floor(x));
    int x1 = static_cast<int>(x0 + 1);
    int y0 = static_cast<int>(floor(y));
    int y1 = static_cast<int>(y0 + 1);

    double sx = smoothstep(x - x0);
    double sy = smoothstep(y - y0);

    double n00 = hashToUnitInterval(x0, y0, seed);
    double n10 = hashToUnitInterval(x1, y0, seed);
    double n01 = hashToUnitInterval(x0, y1, seed);
    double n11 = hashToUnitInterval(x1, y1, seed);

    double ix0 = lerp(n00, n10, sx);
    double ix1 = lerp(n01, n11, sx);
    return lerp(ix0, ix1, sy);
}

void generate_random_texture(int width, const char *filename, int seed = 0)
{
    double aspect_ratio = 16.0 / 9.0;
    int height = static_cast<int>(width / aspect_ratio);

    fstream image;
    image.open(filename, ios::out);
    image << "P3\n"
          << width << " " << height << "\n255\n";
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            auto scale = 0.2 ;
            double noise = valueNoise(i * scale, j * scale, seed);
            int r = static_cast<int>(noise * 255);
            int g = r, b = r;
            image << r << " " << g << " " << b << "\n";
        }
    }
    image.close();
}

int main(int, char **)
{
    for (int i = 0; i < 30; ++i)
    {
        std::string filename = "images/random_texture" + std::to_string(i) + ".ppm";
        generate_random_texture(400, filename.c_str(), i);
    }
    cout << " texture generated and saved " << endl;
    return 0;
}
