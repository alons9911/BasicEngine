#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>

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
            int r = 4 * (i * width + j);
            int g = 4 * (i * width + j) + 1;
            int b = 4 * (i * width + j) + 2;
            char grayPixel = (pixels[r] + pixels[g] + pixels[b]) / 3;
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


vector<vector<unsigned char>> *applySobel(vector<vector<unsigned char>> *pixels)
{
    float div_x, div_y;

    vector<vector<int>> *k_x = getKx(&div_x);
    vector<vector<int>> *k_y = getKy(&div_y);

    vector<vector<unsigned char>> *filtered_x = applyFilter(pixels, k_x, div_x);
    vector<vector<unsigned char>> *filtered_y = applyFilter(pixels, k_y, div_y);

    int height = pixels->size();
    int width = pixels[0].size();
    vector<vector<unsigned char>> *sobel = filtered_x;

    for (int i = 1; i < height; i++)
    {
        for (int j = 1; j < width; j++)
        {
            unsigned char pixel_x = (*filtered_x)[i][j];
            unsigned char pixel_y = (*filtered_y)[i][j];
            (*sobel)[i][j] = (unsigned char)((int)sqrt((pixel_x * pixel_x) + (pixel_y * pixel_y)));
        }
    }
    return sobel;
}

vector<vector<unsigned char>> *createZeroMatrix(int height, int width){
    vector<vector<unsigned char>> *zero_matrix = new vector<vector<unsigned char>>();

    for (int i = 0; i < height; i++)
	{
        vector<unsigned char> row;
		for (int j = 0; j < width; j++)
			row.push_back(0);
        zero_matrix->push_back(row);
	}
    return zero_matrix;
}

vector<vector<float>> *createAngelsMatrix(vector<vector<unsigned char>> *pixels, int height, int width){
    vector<vector<float>> *angels = new vector<vector<float>>();

    for (int i = 0; i < height; i++)
	{
        vector<float> row;
		for (int j = 0; j < width; j++){
            float pixel = (float)(*pixels)[i][j] * 180.0 / M_PI;
            if (pixel < 0)
            {
                pixel += 180.0;
            }
            
            row.push_back(pixel);
        }
        angels->push_back(row);
	}
    return angels;
}

vector<vector<unsigned char>> *nonMaxSuppression(vector<vector<unsigned char>> *pixels) 
{
    int height = pixels->size();
    int width = pixels[0].size();
   
    vector<vector<unsigned char>> *zero_matrix = createZeroMatrix(height, width);

    vector<vector<float>> *angles = createAngelsMatrix(pixels, height, width);

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            unsigned char q = 255, r = 255;
            if ((0 <= (*angles)[i][j] < 22.5) | (157.5 <= (*angles)[i][j] <= 180)){
                q = (*pixels)[i][j + 1];
                r = (*pixels)[i][j - 1];
            }
            else if (22.5 <= (*angles)[i][j] < 67.5){
                q = (*pixels)[i + 1][j - 1];
                r = (*pixels)[i - 1][j + 1];
            }
            else if (67.5 <= (*angles)[i][j] < 112.5){
                q = (*pixels)[i + 1][j];
                r = (*pixels)[i - 1][j];
            }
            else if (112.5 <= (*angles)[i][j] < 157.5){
                q = (*pixels)[i - 1][j - 1];
                r = (*pixels)[i + 1][j + 1];
            }    
            
            if (((*pixels)[i][j] >= q) & ((*pixels)[i][j] >= r))
                (*zero_matrix)[i][j] = (*pixels)[i][j];  
            
        }
        
    }
    return zero_matrix;
}

unsigned char getMaxValue(vector<vector<unsigned char>> *matrix, int height, int width)
{
    unsigned char max_value = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            max_value = max(max_value, (*matrix)[i][j]);
    return max_value;
}

unsigned char WEAK = 50, STRONG = 255;

vector<vector<unsigned char>> *threshold(vector<vector<unsigned char>> *pixels, float low_threshold_ratio, float high_threshould_ratio)
{
    int height = pixels->size();
    int width = pixels[0].size();
    
    unsigned char max_value = getMaxValue(pixels, height, width);
    unsigned char low_threshold = (unsigned char)((int) (max_value * low_threshold_ratio));
    unsigned char high_threshold = (unsigned char)((int) (max_value * high_threshould_ratio));

    vector<vector<unsigned char>> *thresholded_matrix = new vector<vector<unsigned char>>();

    for (int i = 0; i < height; i++)
	{
        vector<unsigned char> row;
		for (int j = 0; j < width; j++){
            if ((*pixels)[i][j] >= high_threshold)
                row.push_back(STRONG);
            else if ((*pixels)[i][j] < low_threshold)
                row.push_back(0);
            else
                row.push_back(WEAK);
        }
			row.push_back(0);
        thresholded_matrix->push_back(row);
	}
    return thresholded_matrix;
}

vector<vector<unsigned char>> *hysteresis(vector<vector<unsigned char>> *thresholds)
{
    int height = thresholds->size();
    int width = thresholds[0].size();
    
    for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++){
            if ((*thresholds)[i][j] == WEAK){
                if (isPointInRange(i - 1, j - 1, height - 1, width - 1) & ((*thresholds)[i - 1][j - 1] == STRONG))
                    (*thresholds)[i][j] = STRONG;
                else if (isPointInRange(i - 1, j, height - 1, width - 1) & ((*thresholds)[i - 1][j] == STRONG))
                    (*thresholds)[i][j] = STRONG;
                else if (isPointInRange(i - 1, j + 1, height - 1, width - 1) & ((*thresholds)[i - 1][j + 1] == STRONG))
                    (*thresholds)[i][j] = STRONG;
                else if (isPointInRange(i, j - 1, height - 1, width - 1) & ((*thresholds)[i][j - 1] == STRONG))
                    (*thresholds)[i][j] = STRONG;
                else if (isPointInRange(i, j + 1, height - 1, width - 1) & ((*thresholds)[i][j + 1] == STRONG))
                    (*thresholds)[i][j] = STRONG;
                else if (isPointInRange(i + 1, j - 1, height - 1, width - 1) & ((*thresholds)[i + 1][j - 1] == STRONG))
                    (*thresholds)[i][j] = STRONG;
                else if (isPointInRange(i + 1, j, height - 1, width - 1) & ((*thresholds)[i + 1][j] == STRONG))
                    (*thresholds)[i][j] = STRONG;
                else if (isPointInRange(i + 1, j + 1, height - 1, width - 1) & ((*thresholds)[i + 1][j + 1] == STRONG))
                    (*thresholds)[i][j] = STRONG;
                else
                    (*thresholds)[i][j] = 0;

            }
        }
	}
    return thresholds;
}

vector<vector<unsigned char>> *cannyAlgorithm(vector<vector<unsigned char>> *pixels)
{
    float div_g;
    vector<vector<int>> *gaussian_kernel = getGaussianKernel(&div_g);
    vector<vector<unsigned char>> *gaussianed = applyFilter(pixels, gaussian_kernel, div_g);
    vector<vector<unsigned char>> *sobel = applySobel(gaussianed);
    vector<vector<unsigned char>> *thined_edges = nonMaxSuppression(sobel); 
    vector<vector<unsigned char>> *canny_filtered = hysteresis(threshold(thined_edges, 0.1, 0.3)); 
    return canny_filtered;
}

void dumpToFile(vector<vector<unsigned char>> *picture, const string &file_name, int width, int height, int intensity){
    ofstream file;
    file.open(file_name);

    int i = 0, j = 0;
    for (i = 0; i < height - 1; i++)
	{
		for (j = 0; j < width; j++){
            file << (int)(((*picture)[i][j] - ((*picture)[i][j] % intensity) + 1) / (256 / intensity)) << ",";
        }
    }
    for (j = 0; j < width - 1; j++){
        file << (int)(((*picture)[i][j] - ((*picture)[i][j] % intensity) + 1) / (256 / intensity)) << ",";
    }
    file << (int)(((*picture)[i][j] - ((*picture)[i][j] % intensity) + 1) / (256 / intensity)) << endl;
    file.close();
}