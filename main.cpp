#include <iostream>
#include <fstream>

using namespace std;
double random_double( double min, double max){
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

struct vec3 {
    double x;
    double y;
    double z;
    vec3(double x, double y , double z)  : x(x), y(y)  , z(z){}
};
void generate_random_texture(int width  , const char* filename){
    double aspect_ratio = 16.0 / 9.0;
    int height = static_cast<int>(width / aspect_ratio);
    double viewport_height = 2.0 ;
    double viewport_width = aspect_ratio * viewport_height ;
    auto start = vec3(0,0,0);
    auto pixel_00 = vec3(start .x - viewport_width / 2 , start.y - viewport_height / 2 , start.z);
    auto horizental_distance = viewport_width / width;
    auto vertical_distance = viewport_height / height;
    fstream image ;
    image.open( filename , ios::out);
    image << "P3\n"
          << width << " " << height << "\n255\n";
    for(int i=0 ; i< width ; i++){
        for(int j=0 ; j<height ; j++){
            auto pixel = vec3(pixel_00.x + i * horizental_distance , pixel_00.y + j * vertical_distance , pixel_00.z);
            auto value = random_double(0,1);
            image << static_cast<int>(255.99 * value) << ' '
                  << static_cast<int>(255.99 * value) << ' '
                  << static_cast<int>(255.99 * value) << '\n';
        }
    }
    image.close();
}






int main(int, char**){
    for(int i=0 ; i< 100 ; ++i){
        std::string filename = "images/random_texture" + std::to_string(i) + ".ppm";
        generate_random_texture(800, filename.c_str());
    }
    cout << " textures generated and saved " << endl;
    return 0;
}
