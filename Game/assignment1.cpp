#include <vector>
using namespace std;

unsigned char trunc16Colors(unsigned char pixel)
{
    return pixel - pixel % 16;
}

vector<vector<unsigned char>> *imageToGray(unsigned char *pixels, int width, int height){
    vector<vector<unsigned char>> *grayImage = new vector<vector<unsigned char>>(); 
    for (int i = 0; i < width; i++)
    {
        vector<unsigned char> row;
        for (int j = 0; j < height; j++)
        {
            char grayPixel = (pixels[4 * (i * width + j)] + pixels[4 * (i * width + j) + 1] + pixels[4 * (i * width + j) + 2]) / 3;
            row.push_back(grayPixel);
        }
        grayImage->push_back(row);
    }
    return grayImage;
}

unsigned char *deepCopyArray(unsigned char *array, int size){
    unsigned char *copy = (unsigned char*)(malloc(size));
    for (int i = 0; i < size; i++)
        copy[i] = array[i];
    return copy;
}

unsigned char *grayImageToOriginalRgbaTemplate(vector<vector<unsigned char>> *grayImage, unsigned char *original, int width, int height){
    unsigned char *newImage = deepCopyArray(original, width * height * 4);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            newImage[4 * (i * width + j)] = (*grayImage)[i][j];
            newImage[4 * (i * width + j) + 1] = (*grayImage)[i][j];
            newImage[4 * (i * width + j) + 2] = (*grayImage)[i][j];
        }
    }
    return newImage;
}

vector<vector<unsigned char>> *arrayToMatrix(unsigned char *array, int width, int height)
{
    vector<vector<unsigned char>> *matrix = new vector<vector<unsigned char>>();
    for (int i = 0; i < height; i++)
    {
        std::vector<unsigned char> row;
        for (int j = 0; j < width; j++)
            row.push_back(array[i * width + j]);
        matrix->push_back(row);
    }
    return matrix;
}

unsigned char *matrixToArray(vector<vector<unsigned char>> *matrix, int *width, int *height)
{
    *width = matrix -> size();
    *height = (*matrix)[0].size();
    unsigned char *array = (unsigned char*)(malloc(4 * (*width) * (*height)));

    for (int i = 0; i < (*height) * (*width); i++)
    {
        array[i] = (*matrix)[i / (*width)][i - ((i / *width) * (*width))];
        //for (int j = 0; j < *width; j++){
        //    array[i * (*width) + j] = (*matrix)[i][j];
        //    cout << "i: "<< i << ", j: " << j << ", i * width + j:"<<  (i * (*width) + j)<<endl;
        //}
    }
    return array;
}



bool compareArrays(unsigned char *arr1, unsigned char *arr2, int size){
    for (int i = 0; i < size; i++)
        if (arr1[i] != arr2[i])
            return false;
    return true;
}

vector<vector<unsigned char>> *deepCopyMatrix(vector<vector<unsigned char>> *matrix)
{
    vector<vector<unsigned char>> *newMatrix = new vector<vector<unsigned char>>();
    for (int i = 0; i < matrix->size(); i++)
    {
        vector<unsigned char> row;
        for (int j = 0; j < (*matrix)[i].size(); j++){
            row.push_back((*matrix)[i][j]);
        }
        newMatrix->push_back(row);
    }
    return newMatrix;
}

bool isPointInRange(int x, int y, int x_max, int y_max)
{
    return (x >= 0 && x <= x_max && y >= 0 && y <= y_max);
}

vector<vector<unsigned char>> *floyedSteinberg(vector<vector<unsigned char>> *pixels)
{

    int height = pixels->size();
    int width = pixels[0].size();

    vector<vector<unsigned char>> copy = *deepCopyMatrix(pixels);

    vector<vector<unsigned char>> *newPixels = new vector<vector<unsigned char>>();

    char ALPHA = 7 / 16, BETA = 3 / 16, GAMMA = 5 / 16, DELTA = 1 / 16;

    for (int i = 0; i < width; i++)
    {
        vector<unsigned char> row;
        for (int j = 0; j < height; j++)
        {
            unsigned char oldPixel = copy.at(i).at(j);
            unsigned char newPixel = trunc16Colors(oldPixel);
            row.push_back(newPixel);
            unsigned char error = oldPixel - newPixel;
            if (isPointInRange(i, j + 1, width - 1, height - 1))
                copy[i][j + 1] += ALPHA * error;
            if (isPointInRange(i + 1, j - 1, width - 1, height - 1))
                copy[i + 1][j - 1] += BETA * error;
            if (isPointInRange(i + 1, j, width - 1, height - 1))
                copy[i + 1][j] += GAMMA * error;
            if (isPointInRange(i + 1, j + 1, width - 1, height - 1))
                copy[i + 1][j + 1] += DELTA * error;
        }
        newPixels->push_back(row);
    }
    return newPixels;
}