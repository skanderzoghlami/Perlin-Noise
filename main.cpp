#include <iostream>
#include <fstream>
#include <memory>
#include <random>
#include <map>
#include <cmath>
using namespace std;


struct vec2d{
    double x , y ;
    vec2d(double x , double y ) : x(x) , y(y) {}
    vec2d() : x(0.0) , y(0.0) {}
};
inline double dotProduct( vec2d a , vec2d b){
    return (a.x * b.x ) + (a.y * b.y );
}
inline vec2d vecMinus(vec2d a , vec2d b){
    return vec2d(a.x - b.x , a.y - b.y);
}


 /* Here we're gonna use a seed so that our texture is reproducible (terrains need this) if we need other structure we can add offsets to seed*/
double getRandomValue(unsigned int seed , double start = 0.0 , double end = 1.0) {
    mt19937 gen(seed);                            
    uniform_real_distribution<double> dis(start, end);   
    return dis(gen);
}

/* ---------------------- GRID CALCULATION FUNCTIONS ---------------------- */
map<pair<unsigned int, unsigned int>,double> makeValueNoiseGrid(unsigned int width, unsigned int height){
    map<pair<unsigned int, unsigned int>,double> result ;
    for(unsigned int i = 0 ; i<= height ; ++i ){
         for(unsigned int j = 0 ; j<= height ; ++j ){
            result.insert({{i,j} ,getRandomValue(i * 374761393 + j * 668265263) }) ;
         }
    }
    return result;
} 

map<pair<unsigned int, unsigned int>,vec2d> makePerlinNoiseGrid(unsigned int width, unsigned int height){
    map<pair<unsigned int, unsigned int>,vec2d> result ;
    for(unsigned int i = 0 ; i<= height ; ++i ){
         for(unsigned int j = 0 ; j<= height ; ++j ){
            unsigned int seed = i * 374761393 + j * 668265263 ;
            vec2d randomVect(getRandomValue(seed  , -1.0 , 1.0) , getRandomValue(seed +1,-1.0,1.0));
            result.insert({{i,j}, randomVect  });
        }
    }
    return result;
} 

map<pair<unsigned int, unsigned int>,vec2d> makeImprovedPerlinNoiseGrid(unsigned int width, unsigned int height){
    map<pair<unsigned int, unsigned int>,vec2d> result ;
    for(unsigned int i = 0 ; i<= height ; ++i ){
         for(unsigned int j = 0 ; j<= height ; ++j ){
            unsigned int seed = i * 374761393 + j * 668265263 ;
            double theta = getRandomValue(seed, 0.0, 2.0 * M_PI);
            vec2d randomVect(cos(theta), sin(theta));
            result.insert({{i,j}, randomVect  });
        }
    }
    return result;
} 

/* ---------------------- --------------------------- ---------------------- */
/* ---------------------- INTERPOLATION FUNCTIONS ---------------------- */

double smoothstep(double a, double b, double t) {
    double s = t * t * (3.0 - 2.0 * t);
    return a + s * (b - a);
}
double betterSmoothstep(double a, double b, double t) {
    double s = t * t * t  * (10.0 - 15.0 * t + 6.0 * t * t);
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
/* ------------------------------------------------------------------------*/

class Image {
    private:
    string type = "P2";
    int width ; 
    int height;
    int maxEncoding = 255 ; 
    public:
    Image(int width, int height) : width(width) , height(height){}
    void writeValueNoiseImage(const string& filePath) {
        map<pair<unsigned int, unsigned int>,double> randomGrid = makeValueNoiseGrid(width,height);
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
       void writePerlinNoiseImage(const string& filePath) {
        map<pair<unsigned int, unsigned int>,vec2d> randomGrid = makePerlinNoiseGrid(width,height);
        fstream strm( filePath, ios::out) ;
        strm << type + '\n' ;
        strm << to_string(width) + ' ' + to_string(height) + '\n' ;
        strm << to_string(maxEncoding) + '\n' ;
        for (int i =  0 ; i < height ; ++i ){
            for (int j= 0 ; j< width ; ++j){
                vec2d center(i+0.5, j+0.5);

                vec2d up_lft_offset   = vecMinus(center , vec2d(i,   j ) );     // (0.5,0.5)
                vec2d up_right_offset = vecMinus(center , vec2d(i,   j+1));   // (0.5,-0.5)
                vec2d bot_lft_offset  = vecMinus(center , vec2d(i+1, j));     // (-0.5,0.5)
                vec2d bot_right_offset= vecMinus(center , vec2d(i+1, j+1));   // (-0.5,-0.5)
                double up_lft   = dotProduct(randomGrid[{i,j}],   up_lft_offset);
                double up_right = dotProduct(randomGrid[{i,j+1}], up_right_offset);
                double bot_lft  = dotProduct(randomGrid[{i+1,j}], bot_lft_offset);
                double bot_right= dotProduct(randomGrid[{i+1,j+1}], bot_right_offset);

                // No subpixel sampling so t = 0.5
                double s1 = smoothstep(up_lft, up_right , 0.5);
                double s2 = smoothstep(bot_lft, bot_right , 0.5);
                int pixelValue = static_cast<int>((smoothstep( s1, s2 , 0.5) + 1)/2 * 255.0 + 0.5);
                Pixel p(pixelValue, pixelValue, pixelValue );
                strm << to_string(p.r) + ' ' ;
            }
        }
    }
    void writeImprovedPerlinNoiseImage(const string& filePath) {
        map<pair<unsigned int, unsigned int>,vec2d> randomGrid = makeImprovedPerlinNoiseGrid(width,height);
        fstream strm( filePath, ios::out) ;
        strm << type + '\n' ;
        strm << to_string(width) + ' ' + to_string(height) + '\n' ;
        strm << to_string(maxEncoding) + '\n' ;
        for (int i =  0 ; i < height ; ++i ){
            for (int j= 0 ; j< width ; ++j){
                vec2d center(i+0.5, j+0.5);

                vec2d up_lft_offset   = vecMinus(center , vec2d(i,   j ) );     // (0.5,0.5)
                vec2d up_right_offset = vecMinus(center , vec2d(i,   j+1));   // (0.5,-0.5)
                vec2d bot_lft_offset  = vecMinus(center , vec2d(i+1, j));     // (-0.5,0.5)
                vec2d bot_right_offset= vecMinus(center , vec2d(i+1, j+1));   // (-0.5,-0.5)
                double up_lft   = dotProduct(randomGrid[{i,j}],   up_lft_offset);
                double up_right = dotProduct(randomGrid[{i,j+1}], up_right_offset);
                double bot_lft  = dotProduct(randomGrid[{i+1,j}], bot_lft_offset);
                double bot_right= dotProduct(randomGrid[{i+1,j+1}], bot_right_offset);

                // No subpixel sampling so t = 0.5
                double s1 = betterSmoothstep(up_lft, up_right , 0.5);
                double s2 = betterSmoothstep(bot_lft, bot_right , 0.5);
                int pixelValue = static_cast<int>((betterSmoothstep( s1, s2 , 0.5) + 1)/2 * 255.0 + 0.5);
                Pixel p(pixelValue, pixelValue, pixelValue );
                strm << to_string(p.r) + ' ' ;
            }
        }
    }
};




int main(int, char **)
{
    unique_ptr<Image> image_ptr( new Image(512,512));
    image_ptr->writeValueNoiseImage("value_noise.pgm");
    image_ptr->writePerlinNoiseImage("perlin_noise.pgm");
    image_ptr->writeImprovedPerlinNoiseImage("improved_perlin_noise.pgm");

    cout << "Texture generated and saved!" << endl;
    return 0;
}
