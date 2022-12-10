#include "Tracer.h"
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include <ostream>
#include "parser.h"

void Tracer::render(SceneDesription *scene)
{
    this->scene = scene;

    imageData = (unsigned char *)malloc(IMG_HEIGHT * IMG_WIDTH * 4);

    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            glm::vec4 c = glm::clamp(rayGenerator(x, y), glm::vec4(0.0f), glm::vec4(1.0f));
            /*glm::vec2 coordinate = {(float)x / (float)IMG_WIDTH, (float)y / (float)IMG_HEIGHT};
            coordinate = coordinate * 2.0f - 1.0f; // Converting 0 -> 1 to -1 -> 1
            glm::vec4 c = calcPixel(coordinate, scene);
            c = glm::clamp(c, glm::vec4(0.0f), glm::vec4(1.0f));*/
            /*if (c.r > 0 || c.g > 0 || c.b > 0)
            {
                cout << c.r << ", " << c.g << ", " << c.b << ", " << c.a << endl;
            }*/

            imageData[4 * (x + y * IMG_WIDTH)] = (unsigned char)(c.r * 255.0f);
            imageData[4 * (x + y * IMG_WIDTH) + 1] = (unsigned char)(c.g * 255.0f);
            imageData[4 * (x + y * IMG_WIDTH) + 2] = (unsigned char)(c.b * 255.0f);
            imageData[4 * (x + y * IMG_WIDTH) + 3] = (unsigned char)(c.a * 255.0f);

            if (c.r > 0 || c.g > 0 || c.b > 0)
            {
                // cout << (int)imageData[4 * (x + y * IMG_WIDTH)] << ", " << (int)imageData[4 * (x + y * IMG_WIDTH) + 1] << ", " << (int)imageData[4 * (x + y * IMG_WIDTH) + 2] << ", " << (int)imageData[4 * (x + y * IMG_WIDTH) +3] << endl;
            }
        }
    }
}

glm::vec3 Tracer::getRayDirection(glm::vec2 boardCoordinate, glm::vec3 origin)
{
    float x = boardCoordinate.x, y = boardCoordinate.y;

    // camera directions
    glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
    glm::vec3 screenCenter(0.0f);
    glm::vec3 cameraDirection = screenCenter - origin;
    glm::vec3 rightDirection = glm::cross(glm::normalize(cameraDirection), upDirection);
    glm::vec3 cameraUpDirection = glm::cross(glm::normalize(rightDirection), glm::normalize(cameraDirection));

    


    float Rx = 2.0f / IMG_WIDTH, Ry = 2.0f / IMG_HEIGHT; 

    glm::vec3 p = screenCenter + ((x - (IMG_WIDTH / 2)) * Rx * rightDirection) - ((y - (IMG_HEIGHT / 2)) * Ry * cameraUpDirection) + cameraDirection;
    //p = p * 2.0f - 1.0f;
    return glm::normalize(p);
    
    glm::vec2 coordinate = {x / (float)IMG_WIDTH, y / (float)IMG_HEIGHT};
    coordinate = coordinate * 2.0f - 1.0f; // Converting 0 -> 1 to -1 -> 1s
    //return glm::normalize(glm::vec3(coordinate, -1.0f));



    glm::vec3 pixelCoordinate = glm::vec3(coordinate, 0.0f);
    return glm::normalize(pixelCoordinate - origin);
}

Tracer::RayInfo Tracer::traceRay(const Ray &ray)
{
    vector<Sphere*> spheres = *scene->spheres;
    ///////
    /*
    spheres = *(new vector<Sphere*>());
    Sphere *s1 = new Sphere(0,0,-20,0.5f,object);
    s1->setRadius(1);
    s1->setColor(1.0f,0.0f,1.0f, 1.0f);

    Sphere *s2 = new Sphere(0,-51,-20,1.5f,object);
    s2->setRadius(50);
    s2->setColor(0.2f,0.3f,1.0f, 1.0f);
    spheres.push_back(s1);
    spheres.push_back(s2);
    */
    //rayOrigin = glm::vec3(0, 0, 2);
    ///////

    Sphere *closestSphere = nullptr;
    float hitDistance = FLT_MAX;

    for (Sphere *s : spheres)
    {

        // (bx^2 + by^2 + bz^2)t^2 + 2t(axbx + ayby + azbz) + (ax^2 + ay^2 + az^2 - r^2) = 0
        // where
        // a = ray origin
        // b = ray direction
        // r = raduis
        // t = hit distance
        glm::vec3 origin = ray.origin - s->position;

        float a = 1.0f;                                  // glm::dot(rayDirection, rayDirection); // = (bx^2 + by^2 + bz^2) by definition
        float b = 2.0f * glm::dot(origin, ray.direction); // = 2(axbx + ayby + azbz) by definition
        float c = glm::dot(origin, origin) - s->radius * s->radius;

        // Quadric formula discriminant:
        //  b^2 - 4ac

        float discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0.0f)
            continue;
        float t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a), t1 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
        float closestT = -1;
        if (t0 < 0 & t1 > 0)
            closestT = t1;
        else if (t0 > 0 && t1 > 0)
            closestT = glm::min(t0, t1);
        
        
        // float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
        if (0 < closestT  && closestT < hitDistance)
        {
            hitDistance = closestT;
            closestSphere = s;
        }
    }
    if (closestSphere == nullptr)
        return miss(ray);
    return closestHit(ray, closestSphere, hitDistance);
}

Tracer::RayInfo Tracer::closestHit(const Ray &ray, Sphere *closestSphere, float hitDistance)
{
    glm::vec3 origin = ray.origin - closestSphere->position;
    glm::vec3 hitPosition = origin + ray.direction * hitDistance;
    glm::vec3 normal = glm::normalize(hitPosition);

    RayInfo result;
    result.hitDistance = hitDistance;
    result.closestSphere = closestSphere;
    result.worldPosition = hitPosition + closestSphere->position;
    result.worldNormal = normal;
    return result;
}

Tracer::RayInfo Tracer::miss(const Ray &ray)
{
    Tracer::RayInfo missInfo;
    missInfo.hitDistance = -1;
    return missInfo;
}

glm::vec3 Tracer::innerRayGenerator(const Ray &ray, glm::vec3 color, int reflections, float multiplier, glm::vec3 backgroundColor){
    if (reflections == 0)
        return color;
    
    Tracer::RayInfo traceInfo = traceRay(ray);

    if (traceInfo.hitDistance < 0.0f){
        color +=  backgroundColor * multiplier;
        return color;
    }
    glm::vec3 lightDir = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    float intensity = glm::max(glm::dot(traceInfo.worldNormal, -lightDir), 0.0f); // == cos(alpha)
    
    Sphere *closestSphere = traceInfo.closestSphere;
    glm::vec3 sphereColor = glm::vec3(closestSphere->objColor.r, closestSphere->objColor.g, closestSphere->objColor.b);
    sphereColor *= intensity;
    color += sphereColor * multiplier;

    Ray newRay;
    newRay.origin = traceInfo.worldPosition + traceInfo.worldNormal * 0.0001f;
    newRay.direction = glm::reflect(ray.direction, traceInfo.worldNormal);

    return innerRayGenerator(newRay, color, reflections - 1, multiplier * 0.7f, backgroundColor);
    
}

glm::vec4 Tracer::rayGenerator(int x, int y)
{
    Ray ray;
    ray.origin = glm::vec3(scene->eye->x, scene->eye->y, scene->eye->z);
    //ray.origin = glm::vec3(0,0,10);
    ray.direction = getRayDirection(glm::vec2(x, y), ray.origin);
    
    glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 finalColor = innerRayGenerator(ray, glm::vec3(0.0f), 5, 1.0f, backgroundColor);
    return glm::vec4(finalColor, 1.0f);
}