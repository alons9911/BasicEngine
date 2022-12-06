#include "Tracer.h"
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include <ostream>
#include "parser.h"


void Tracer::render(sceneDesription scene) {
    imageData = (unsigned char*) malloc(IMG_HEIGHT * IMG_WIDTH * 4);
    
    vector<sphere> *spheres = scene.spheres;
    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            glm::vec2 coordinate = {(float)x / (float)IMG_WIDTH, (float)y / (float)IMG_HEIGHT};
            coordinate = coordinate * 2.0f - 1.0f; // Converting 0 -> 1 to -1 -> 1
            glm::vec4 c = calcPixelColor(coordinate);
            c = glm::clamp(c, glm::vec4(0.0f), glm::vec4(1.0f));
            /*if (c.r > 0 || c.g > 0 || c.b > 0)
            {
                cout << c.r << ", " << c.g << ", " << c.b << ", " << c.a << endl;
            }*/
            
            imageData[4 * (x + y * IMG_WIDTH)] = (unsigned char) (c.r * 255.0f);
            imageData[4 * (x + y * IMG_WIDTH) + 1] = (unsigned char) (c.g * 255.0f);
            imageData[4 * (x + y * IMG_WIDTH) + 2] = (unsigned char) (c.b * 255.0f);
            imageData[4 * (x + y * IMG_WIDTH) + 3] = (unsigned char) (c.a * 255.0f);

            if (c.r > 0 || c.g > 0 || c.b > 0)
            {
                //cout << (int)imageData[4 * (x + y * IMG_WIDTH)] << ", " << (int)imageData[4 * (x + y * IMG_WIDTH) + 1] << ", " << (int)imageData[4 * (x + y * IMG_WIDTH) + 2] << ", " << (int)imageData[4 * (x + y * IMG_WIDTH) +3] << endl;
            }

        }
    }
}

glm::vec4 Tracer::calcPixelColor(glm::vec2 coor) {
    glm::vec3 rayDirection = glm::vec3(coor.x, coor.y, -1.0f);
    glm::vec3 rayOrigin(0.0f, 0.0f, -1.0f);
    rayDirection = glm::normalize(rayDirection);
    float raduis = 0.5f;
    glm::vec3 sphereCenter(0.0f);
    glm::vec3 sphereColor(1, 0, 1);
    glm::vec3 lightDir(-1, -1, 1);
    lightDir = glm::normalize(lightDir);

    //camera directions
    glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
    glm::vec3 screenCenter(0.0f);
    glm::vec3 cameraDirection = rayOrigin - screenCenter;
    glm::vec3 rightDirection = glm::normalize(glm::cross(cameraDirection, upDirection));
    glm::vec3 cameraUpDirection = glm::normalize(glm::cross(rightDirection, cameraDirection));


    // (bx^2 + by^2 + bz^2)t^2 + 2t(axbx + ayby + azbz) + (ax^2 + ay^2 + az^2 - r^2) = 0
    // where
    // a = ray origin
    // b = ray direction
    // r = raduis
    // t = hit distance

    float a = 1.0f; //glm::dot(rayDirection, rayDirection); // = (bx^2 + by^2 + bz^2) by definition 
    float b = 2.0f * glm::dot(rayDirection, rayOrigin - sphereCenter); // = 2(axbx + ayby + azbz) by definition
    float c = glm::dot(rayOrigin - sphereCenter, rayOrigin - sphereCenter) - raduis * raduis;

    //Quadric formula discriminant:
    // b^2 - 4ac

    float discriminant = b * b - 4.0f * a * c;
    //cout << a << "," << b << "," << c << "," << discriminant << endl;

    if (discriminant >= 0){
        /*float t[] = {
            -b + sqrt(discriminant) / (2.0f * a),
            -b - sqrt(discriminant) / (2.0f * a)
        };*/
        float t = (-b + glm::sqrt(discriminant)) / (2.0f * a);

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
        float intensity = glm::max(glm::dot(normal, -lightDir), 0.0f);
        return glm::vec4(intensity * sphereColor, 1.0f);
    }
    return glm::vec4(0, 0, 0, 1);
    
}