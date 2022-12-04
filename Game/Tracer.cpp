#include "Tracer.h"
#include "glad/include/glad/glad.h"


void Tracer::render() {
    imageData = (unsigned char*) malloc(IMG_HEIGHT * IMG_WIDTH * 4);
    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            glm::vec2 coordinate = {(float)x / (float)IMG_WIDTH, (float)y / (float)IMG_HEIGHT};
            imageData[4 * (x + y * IMG_WIDTH)] = calcPixelColor(coordinate);
            imageData[4 * (x + y * IMG_WIDTH) + 1] = calcPixelColor(coordinate);
            imageData[4 * (x + y * IMG_WIDTH) + 2] = calcPixelColor(coordinate);

        }
    }
}

unsigned char Tracer::calcPixelColor(glm::vec2 coordinate) {
    return (unsigned char)0xffff00ff;
}