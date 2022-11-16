#include <vector>
using namespace std;


vector<vector<unsigned char>> *imageToGray(unsigned char *pixels, int width, int height);

vector<vector<unsigned char>> *floyedSteinberg(vector<vector<unsigned char>> *pixels);

vector<vector<unsigned char>> *halftone(vector<vector<unsigned char>> *pixels);

vector<vector<unsigned char>> *cannyAlgorithm(vector<vector<unsigned char>> *pixels);


void dumpToFile(vector<vector<unsigned char>> *picture, const string &file_name, int width, int height, int intensity);

vector<vector<int>> *getGaussianKernel(float *div);

vector<vector<int>> *getKx(float *div);

vector<vector<int>> *getKy(float *div);

unsigned char trunc16Colors(unsigned char pixel);

unsigned char *inflatePixelsToRGBA(vector<vector<unsigned char>> *pixels, int width, int height);

bool isPointInRange(int x, int y, int x_max, int y_max);

void *paddingBlackBorder(vector<vector<unsigned char>> *pixels);

vector<vector<unsigned char>> *applyFilter(vector<vector<unsigned char>> *pixels, vector<vector<int>> *kernel, float div);

vector<vector<unsigned char>> *applySobel(vector<vector<unsigned char>> *pixels);

vector<vector<unsigned char>> *createZeroMatrix(int height, int width);

vector<vector<float>> *createAngelsMatrix(vector<vector<unsigned char>> *pixels, int height, int width);

vector<vector<unsigned char>> *nonMaxSuppression(vector<vector<unsigned char>> *pixels);

unsigned char getMaxValue(vector<vector<unsigned char>> *matrix, int height, int width);

vector<vector<unsigned char>> *threshold(vector<vector<unsigned char>> *pixels, float low_threshold_ratio, float high_threshould_ratio);

vector<vector<unsigned char>> *hysteresis(vector<vector<unsigned char>> *thresholds);