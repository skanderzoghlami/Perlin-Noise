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

double valueNoise(double x, double y, double stepX, double stepY, size_t seed = 0)
{
    // Calculate the center of the current pixel (x, y) and then offset to the 4 corners
    double centerX = x;
    double centerY = y;

    // Offset for the four corners based on stepX/2, stepY/2
    double offsetX = stepX / 2.0;
    double offsetY = stepY / 2.0;

    // The four corner positions, using offsets
    double x00 = centerX - offsetX;
    double y00 = centerY - offsetY;

    double x01 = centerX - offsetX;
    double y01 = centerY + offsetY;

    double x10 = centerX + offsetX;
    double y10 = centerY - offsetY;

    double x11 = centerX + offsetX;
    double y11 = centerY + offsetY;

    // Interpolation weights (normalized offsets)
    double sx = smoothstep((x - x00) / stepX);
    double sy = smoothstep((y - y00) / stepY);

    // Generate noise at the four grid corners
    double n00 = hashToUnitInterval(static_cast<int>(x00), static_cast<int>(y00), seed);
    double n10 = hashToUnitInterval(static_cast<int>(x10), static_cast<int>(y00), seed);
    double n01 = hashToUnitInterval(static_cast<int>(x00), static_cast<int>(y01), seed);
    double n11 = hashToUnitInterval(static_cast<int>(x10), static_cast<int>(y01), seed);

    // Interpolate between corners
    double ix0 = lerp(n00, n10, sx);
    double ix1 = lerp(n01, n11, sx);
    return lerp(ix0, ix1, sy);
}

void generate_random_texture(int width, const char *filename, int seed = 0)
{
    double aspect_ratio = 16.0 / 9.0;
    int height = static_cast<int>(width / aspect_ratio);

    // Set the desired viewport height (fixed)
    double viewportHeight = 2.0;

    // Calculate the corresponding viewport width based on the aspect ratio
    double viewportWidth = viewportHeight * aspect_ratio;

    // Calculate step sizes based on the viewport and image dimensions
    double stepX = static_cast<double>(width) / viewportWidth;
    double stepY = static_cast<double>(height) / viewportHeight;

    fstream image;
    image.open(filename, ios::out);
    image << "P3\n"
          << width << " " << height << "\n255\n";
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double noise = valueNoise(i, j, stepX, stepY, seed);
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
    cout << "Texture generated and saved!" << endl;
    return 0;
}
