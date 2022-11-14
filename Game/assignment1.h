#include <vector>
using namespace std;

unsigned char trunc16Colors(unsigned char pixel);
vector<vector<unsigned char>>* arrayToMatrix(unsigned char* array, int width, int height);
vector<vector<unsigned char>>* floyedSteinberg(vector<vector<unsigned char>>* pixels);