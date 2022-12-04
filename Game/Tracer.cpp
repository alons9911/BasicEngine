#include "Tracer.h"
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>


void Tracer::render() {
    imageData = (unsigned char*) malloc(IMG_HEIGHT * IMG_WIDTH * 4);
    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            glm::vec2 coordinate = {(float)x / (float)IMG_WIDTH, (float)y / (float)IMG_HEIGHT};
            coordinate = coordinate * 2.0f - 1.0f; // Converting 0 -> 1 to -1 -> 1
            imageData[4 * (x + y * IMG_WIDTH)] = calcPixelColor(coordinate);
            imageData[4 * (x + y * IMG_WIDTH) + 1] = calcPixelColor(coordinate);
            imageData[4 * (x + y * IMG_WIDTH) + 2] = calcPixelColor(coordinate);

        }
    }
}

unsigned char Tracer::calcPixelColor(glm::vec2 coor) {
    return (unsigned char)100;

    glm::vec3 rayDirection = {coor.x, coor.y -1.0f};
    glm::vec3 rayOrigin(0.0f);
    float raduis = 0.5f;

    // (bx^2 + by^2 + bz^2)t^2 + 2t(axbx + ayby + azbz) + (ax^2 + ay^2 + az^2 - r^2) = 0
    // where
    // a = ray origin
    // b = ray direction
    // r = raduis
    // t = hit distance

    float a = glm::dot(rayDirection, rayDirection); // = (bx^2 + by^2 + bz^2) by definition 
    float b = 2.0f * glm::dot(rayOrigin, rayDirection); // = 2(axbx + ayby + azbz) by definition
    float c = glm::dot(rayOrigin, rayOrigin) - raduis * raduis;

    //Quadric formula discriminant:
    // b^2 - 4ac

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant >= 0)
        return (unsigned char)0xf0;
    return (unsigned char)0x0f;
    
}