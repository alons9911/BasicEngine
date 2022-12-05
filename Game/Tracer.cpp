#include "Tracer.h"
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include <ostream>


void Tracer::render() {
    imageData = (unsigned char*) malloc(IMG_HEIGHT * IMG_WIDTH * 4);
    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            glm::vec2 coordinate = {(float)x / (float)IMG_WIDTH, (float)y / (float)IMG_HEIGHT};
            coordinate = coordinate * 2.0f - 1.0f; // Converting 0 -> 1 to -1 -> 1
            glm::vec3 rgb = calcPixelColor(coordinate);
            imageData[4 * (x + y * IMG_WIDTH)] = (unsigned char) rgb.x;
            imageData[4 * (x + y * IMG_WIDTH) + 1] = (unsigned char) rgb.y;
            imageData[4 * (x + y * IMG_WIDTH) + 2] = (unsigned char) rgb.z;

        }
    }
}

glm::vec3 Tracer::calcPixelColor(glm::vec2 coor) {
    glm::vec3 rayDirection = glm::vec3(coor.x, coor.y, 1.0f);
    glm::vec3 rayOrigin(0.0f, 0.0f, 2.0f);
    rayDirection = glm::normalize(rayDirection);
    float raduis = 0.5f;
    glm::vec3 sphereCenter(0.0f);
    glm::vec3 sphereColor(0xf0, 0xf0, 0x0f);
    glm::vec3 lightDir(-1, -1, 1);
    lightDir = glm::normalize(lightDir);

    // (bx^2 + by^2 + bz^2)t^2 + 2t(axbx + ayby + azbz) + (ax^2 + ay^2 + az^2 - r^2) = 0
    // where
    // a = ray origin
    // b = ray direction
    // r = raduis
    // t = hit distance

    float a = 1.0f; //glm::dot(rayDirection, rayDirection); // = (bx^2 + by^2 + bz^2) by definition 
    float b = 2.0f * glm::dot(rayOrigin, rayDirection); // = 2(axbx + ayby + azbz) by definition
    float c = glm::dot(rayOrigin, rayOrigin) - raduis * raduis;

    //Quadric formula discriminant:
    // b^2 - 4ac

    float discriminant = b * b - 4.0f * a * c;
    //cout << a << "," << b << "," << c << "," << discriminant << endl;

    if (discriminant >= 0){
        /*float t[] = {
            -b + sqrt(discriminant) / (2.0f * a),
            -b - sqrt(discriminant) / (2.0f * a)
        };*/
        float t = max(-b + sqrt(discriminant) / (2.0f * a), -b - sqrt(discriminant) / (2.0f * a));

        /*for (int i = 0; i < 2; i++)
        {
            glm::vec3 hitPosition = rayOrigin + rayDirection * t[i];
            glm::vec3 normal = hitPosition - sphereCenter;
            glm::normalize(normal);

            float intensity = max(glm::dot(normal, -lightDir), 0.0f);
            return intensity * sphereColor;
        }*/
        glm::vec3 hitPosition = rayOrigin + rayDirection * t;
        glm::vec3 normal = hitPosition - sphereCenter;
        normal = glm::normalize(normal);

        float intensity = max(glm::dot(normal, -lightDir), 0.0f);
        return intensity * sphereColor;
        

        return glm::vec3(0xf0, 0xf0, 0x0f);
    }
    return glm::vec3(0x0f, 0x0f, 0xf0);
    
}