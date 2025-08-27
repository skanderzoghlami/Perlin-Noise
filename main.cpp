#include <iostream>
#include <fstream>
#include <memory>
#include <random>
#include <map>

using namespace std;


 /* Here we're gonna use a seed so that our texture is reproducible (terrains need this) if we need other structure we can add offsets to seed*/
double getRandomValue(unsigned int seed) {
    mt19937 gen(seed);                            
    uniform_real_distribution<double> dis(0.0, 1.0);   
    return dis(gen);
}

map<pair<unsigned int, unsigned int>,double> makeRandomGrid(unsigned int width, unsigned int height){
    map<pair<unsigned int, unsigned int>,double> result ;
    for(unsigned int i = 0 ; i<= height ; ++i ){
         for(unsigned int j = 0 ; j<= height ; ++j ){
            result.insert({{i,j} ,getRandomValue(i * 374761393 + j * 668265263) }) ;
         }
    }
    return result;
} 

double smoothstep(double a, double b, double t) {
    double s = t * t * (3.0 - 2.0 * t);
    return a + s * (b - a);
}


class Pixel{
    public:
    int r,g,b;
    Pixel(int r , int g , int b  ): r(r) , g(g) ,b(b){};
    const string getPixel() const {
        return to_string(r) + ' ' + to_string(g) + ' '+ to_string(b) + ' ';
    }
};


class Image {
    private:
    string type = "P2";
    int width ; 
    int height;
    int maxEncoding = 255 ; 
    public:
    Image(int width, int height) : width(width) , height(height){}
    void writeBlankImage(const string& filePath) {
        map<pair<unsigned int, unsigned int>,double> randomGrid = makeRandomGrid(width,height);
        fstream strm( filePath, ios::out) ;
        strm << type + '\n' ;
        strm << to_string(width) + ' ' + to_string(height) + '\n' ;
        strm << to_string(maxEncoding) + '\n' ;
        for (int i =  0 ; i < height ; ++i ){
            for (int j= 0 ; j< width ; ++j){
                // No subpixel sampling so t = 0.5
                double s1 = smoothstep(randomGrid[{i,j}] , randomGrid[{i,j+1}] , 0.5);
                double s2 = smoothstep(randomGrid[{i+1,j}] , randomGrid[{i+1,j+1}] , 0.5);
                int pixelValue = static_cast<int>(smoothstep( s1, s2 , 0.5) * 255.0 + 0.5);
                Pixel p(pixelValue, pixelValue, pixelValue );
                strm << to_string(p.r) + ' ' ;
            }
        }
    }
};




int main(int, char **)
{
    unique_ptr<Image> image_ptr( new Image(512,512));
    image_ptr->writeBlankImage("value_noise.pgm");
    cout << "Texture generated and saved!" << endl;
    return 0;
}
