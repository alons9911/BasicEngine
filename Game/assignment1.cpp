#include <vector>
#include <cstdlib>
#include <iostream>

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

unsigned char *inflatePixelsToSquare(unsigned char *pixels, int width, int height)
{
    unsigned char *inflated = (unsigned char*)(malloc(width * height * 4));
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            inflated[(i * 2) * (width * 2) + (j * 2)] = pixels[i * width + j];
            inflated[(i * 2 + 1) * (width * 2) + (j * 2)] = pixels[i * width + j];
            inflated[(i * 2) * (width * 2) + (j * 2 + 1)] = pixels[i * width + j];
            inflated[(i * 2 + 1) * (width * 2) + (j * 2 + 1)] = pixels[i * width + j];
        }  
    }
    return inflated;
}

unsigned char *inflatePixelsToRGBA(vector<vector<unsigned char>> *pixels, int width, int height)
{
    static char ALPHA = 255;
    unsigned char *inflated = (unsigned char*)(malloc(width * height * 4));
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            inflated[4 * (i * width + j)] = (*pixels)[i][j];
            inflated[4 * (i * width + j) + 1] = (*pixels)[i][j];
            inflated[4 * (i * width + j) + 2] = (*pixels)[i][j];
            inflated[4 * (i * width + j) + 3] = ALPHA;
        }  
    }
    return inflated;
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

vector<vector<unsigned char>> *halftone(vector<vector<unsigned char>> *pixels)
{

    int height = pixels->size();
    int width = pixels[0].size();

    vector<vector<unsigned char>> *newPixels = new vector<vector<unsigned char>>();

    unsigned char WHITE = 0;
    unsigned char BLACK = 255;

    unsigned char HALFTONE_PATTERNS[5][4] = 
    {
        {WHITE, WHITE, WHITE, WHITE},
        {WHITE, WHITE, BLACK, WHITE},
        {WHITE, BLACK, BLACK, WHITE},
        {WHITE, BLACK, BLACK, BLACK},
        {BLACK, BLACK, BLACK, BLACK}
    };


    for (int i = 0; i < width; i++)
    {
        vector<unsigned char> row1, row2;
        for (int j = 0; j < height; j++)
        {
            int pattern = (4 * (*pixels)[i][j]) / 255;
            row1.push_back(HALFTONE_PATTERNS[pattern][0]);
            row1.push_back(HALFTONE_PATTERNS[pattern][1]);
            row2.push_back(HALFTONE_PATTERNS[pattern][2]);
            row2.push_back(HALFTONE_PATTERNS[pattern][3]);
        }
        newPixels->push_back(row1);
        newPixels->push_back(row2);
    }
    return newPixels;
}

vector<vector<int>> *getGaussianKernel(float *div)
{
    vector<vector<int>> *kernel = new vector<vector<int>>();
    kernel->push_back(vector<int>{1, 2, 1});
    kernel->push_back(vector<int>{2, 4, 2});
    kernel->push_back(vector<int>{1, 2, 1});
    *div = (float) 16;
    return kernel;
}

vector<vector<int>> *getKx(float *div)
{
    vector<vector<int>> *kernel = new vector<vector<int>>();
    kernel->push_back(vector<int>{-1, 0, 1});
    kernel->push_back(vector<int>{-2, 0, 2});
    kernel->push_back(vector<int>{-1, 0, 1});
    *div = (float) 1;
    return kernel;
}


vector<vector<int>> *getKy(float *div)
{
    vector<vector<int>> *kernel = new vector<vector<int>>();
    kernel->push_back(vector<int>{1, 2, 1});
    kernel->push_back(vector<int>{0, 0, 0});
    kernel->push_back(vector<int>{-1, -2, -1});
    *div = (float) 1;
    return kernel;
}

void *paddingBlackBorder(vector<vector<unsigned char>> *pixels)
{
    static unsigned char BLACK = 0;

    int height = pixels->size();
    int width = (*pixels)[0].size();

    vector<unsigned char> black_row;
    for (int i = 0; i < width + 2; i++)
        black_row.push_back(BLACK);
    for (int i = 0; i < height; i++)
    {
        (*pixels)[i].push_back(BLACK);
        (*pixels)[i].insert((((*pixels)[i]).begin()), BLACK);
    }
    pixels->push_back(black_row);
    pixels->insert(pixels->begin(), black_row);
}

vector<vector<unsigned char>> *applyFilter(vector<vector<unsigned char>> *pixels, vector<vector<int>> *kernel, float div)
{
    int height = pixels->size();
    int width = pixels[0].size();

    int kernel_size = kernel->size();
    int size = (int)(kernel_size / 2);

    vector<vector<unsigned char>> *filtered = new vector<vector<unsigned char>>();
    

    for (int i = 1; i < height - size; i++)
	{
        vector<unsigned char> row;
		for (int j = 1; j < width - size; j++)
		{
			float sum = 0;
            
			for (int x = 0; x < kernel_size; x++)
				for (int y = 0; y < kernel_size; y++)
				{
                    sum += (*kernel)[x][y] * (*pixels)[i + x - size][j + y - size];
				}
            sum = ((float)sum) / div;
            unsigned char new_pixel = (char)((int) min(max(sum, (float)0), (float)255));
            row.push_back(new_pixel);
		}
        filtered->push_back(row);
	}
    paddingBlackBorder(filtered);
    return filtered;
}

vector<vector<unsigned char>> *canny(vector<vector<unsigned char>> *pixels)
{
    float div_g;
    vector<vector<int>> *gaussian_kernel = getGaussianKernel(&div_g);
    vector<vector<unsigned char>> *gaussianed = applyFilter(pixels, gaussian_kernel, div_g);

    float div_x, div_y;

    vector<vector<int>> *k_x = getKx(&div_x);
    vector<vector<int>> *k_y = getKy(&div_y);

    vector<vector<unsigned char>> *sobel = applyFilter(gaussianed, k_x, div_x);
    sobel = applyFilter(sobel, k_y, div_y);

    return sobel;
}