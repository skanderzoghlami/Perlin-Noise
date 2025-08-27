#include <iostream>
#include <fstream>
#include <memory>
#include <random>

using namespace std;


class Pixel{
    public:
    int r,g,b;
    Pixel(int r , int g , int b  ): r(r) , g(g) ,b(b){};
    const string getPixel() const {
        return to_string(r) + ' ' + to_string(g) + ' '+ to_string(b) + ' ';
    }
};
 /* Here we're gonna use a seed so that our texture is reproducible (terrains need this) if we need other structure we can add offsets to seed*/
Pixel getRandomPixel(unsigned int seed) {
    // uint32_t seed = x * 374761393 + y * 668265263; // combine x and y togenerate a seed 
    mt19937 gen(seed);                   
    uniform_int_distribution<> dis(0,255);
    return Pixel(dis(gen), dis(gen), dis(gen));
}


class Image {
    private:
    string type = "P2";
    int width ; 
    int height;
    int maxEncoding = 255 ; 
    public:
    Image(int width, int height) : width(width) , height(height){}
    void writeBlankImage(const string& filePath) {
        fstream strm( filePath, ios::out) ;
        strm << type + '\n' ;
        strm << to_string(width) + ' ' + to_string(height) + '\n' ;
        strm << to_string(maxEncoding) + '\n' ;
        for (int i =  0 ; i < height ; ++i ){
            for (int j= 0 ; j< width ; ++j){
                unsigned int seed = i * 374761393 + j * 668265263; // combine x and y togenerate a seed 
                Pixel p = getRandomPixel(seed);
                strm << to_string(p.r) + ' ' ;
            }
        }
    }
};




int main(int, char **)
{
    unique_ptr<Image> image_ptr( new Image(512,512));
    image_ptr->writeBlankImage("blank.pgm");
    cout << "Texture generated and saved!" << endl;
    return 0;
}
