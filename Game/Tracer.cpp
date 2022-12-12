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
    // p = p * 2.0f - 1.0f;
    return glm::normalize(p);

    glm::vec2 coordinate = {x / (float)IMG_WIDTH, y / (float)IMG_HEIGHT};
    coordinate = coordinate * 2.0f - 1.0f; // Converting 0 -> 1 to -1 -> 1s
    // return glm::normalize(glm::vec3(coordinate, -1.0f));

    glm::vec3 pixelCoordinate = glm::vec3(coordinate, 0.0f);
    return glm::normalize(pixelCoordinate - origin);
}

ClosestSphereInfo Tracer::findClosestSphere(const Ray &ray, vector<Sphere *> spheres, float maxHitDistance, Sphere *excludedSphere)
{
    Sphere *closestSphere = nullptr;
    float hitDistance = maxHitDistance;

    for (Sphere *sphere : spheres)
    {
        if (sphere == excludedSphere)
            continue;
        
        // (bx^2 + by^2 + bz^2)t^2 + 2t(axbx + ayby + azbz) + (ax^2 + ay^2 + az^2 - r^2) = 0
        // where
        // a = ray origin
        // b = ray direction
        // r = raduis
        // t = hit distance
        glm::vec3 origin = ray.origin - sphere->position;

        float a = 1.0f;                                   // glm::dot(rayDirection, rayDirection); // = (bx^2 + by^2 + bz^2) by definition
        float b = 2.0f * glm::dot(origin, ray.direction); // = 2(axbx + ayby + azbz) by definition
        float c = glm::dot(origin, origin) - sphere->radius * sphere->radius;

        // Quadric formula discriminant:
        //  b^2 - 4ac

        float discriminant = b * b - 4.0f * a * c;

        if (discriminant >= 0.0f)
        {
            float t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a), t1 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
            float closestT = -1;
            if (t0<0 & t1> 0)
                closestT = t1;
            else if (t0 > 0 && t1 > 0)
                closestT = glm::min(t0, t1);

            if (closestT > 0.0001 && closestT < hitDistance)
            {
                hitDistance = closestT;
                closestSphere = sphere;
            }
        }
    }
    return ClosestSphereInfo(closestSphere, hitDistance);
}

Tracer::RayInfo Tracer::traceRay(const Ray &ray)
{
    vector<Sphere *> spheres = *scene->spheres;
    ///////
    /*
    spheres = *(new vector<Sphere*>());
    Sphere *s1 = new Sphere(0,0,-20,0.5f,Object);
    s1->setRadius(1);
    s1->setColor(1.0f,0.0f,1.0f);
    s1->setShininess(10.0f);

    Sphere *s2 = new Sphere(0,-51,-20,1.5f,Object);
    s2->setRadius(50);
    s2->setColor(0.2f,0.3f,1.0f);
    s2->setShininess(10.0f);
    spheres.push_back(s1);
    spheres.push_back(s2);
    */
    // rayOrigin = glm::vec3(0, 0, 2);
    ///////
    ClosestSphereInfo closest = findClosestSphere(ray, spheres, FLT_MAX, nullptr);

    if (closest.closestSphere == nullptr)
        return miss(ray);
    return closestHit(ray, closest.closestSphere, closest.hitDistance);
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

Ray Tracer::calcSnellLaw(Tracer::RayInfo traceInfo, Ray ray, glm::vec3 N, glm::vec3 rayDirection, float snellFrac)
{
    float cosTheta1 = glm::dot(N, glm::normalize(rayDirection));
    //cout << "cos theta1 - " << cosTheta1 << endl;
    float theta1 = glm::acos(glm::min(cosTheta1, 1.0f));
    //cout <<"depth - " << depth <<endl;
    //cout <<"t1 - " << theta1 * (180.0f / (float)M_PI) <<endl;
    float sinTheta1 = glm::clamp(glm::sin(theta1), -1.0f, 1.0f);
    float sinTheate2 = glm::clamp(snellFrac * sinTheta1, -1.0f , 1.0f);
    float theta2 = glm::asin(sinTheate2);
    //cout <<"t2 - " << theta2 * (180.0f / (float)M_PI)<<endl;
    float cosTheta2 = glm::cos(theta2);
    if (isnan(theta2))
    {
        Ray newRay;
        newRay.origin = traceInfo.worldPosition + traceInfo.worldNormal * 0.0001f;
        newRay.direction = glm::reflect(ray.direction, traceInfo.worldNormal);
    }
    

    Ray newRay;
    newRay.direction = (snellFrac * cosTheta1 - cosTheta2) * N - snellFrac * (-ray.direction);
    newRay.origin = traceInfo.worldPosition + 0.001f * glm::normalize(newRay.direction);
    return newRay;
}

glm::vec3 Tracer::innerRayGenerator(const Ray &ray, int reflectionsDepth, float Ks, glm::vec3 backgroundColor, float snellFrac, int trasparentDepth){
    Tracer::RayInfo traceInfo = traceRay(ray);

    if (traceInfo.hitDistance < 0.0f)
        return backgroundColor;
    glm::vec3 sphereColor = getSphereColor(traceInfo, ray);
    
    if (traceInfo.closestSphere->type == Reflective)
    {
        if (reflectionsDepth >= 5)
            return backgroundColor;
        Ray newRay;
        newRay.origin = traceInfo.worldPosition + traceInfo.worldNormal * 0.0001f;
        newRay.direction = glm::reflect(ray.direction, traceInfo.worldNormal);

        sphereColor += Ks * innerRayGenerator(newRay, reflectionsDepth + 1, Ks, backgroundColor, snellFrac, trasparentDepth);
    }
    else if (traceInfo.closestSphere->type == Transparent)
    {
        glm::vec3 transparentColor(0.0f);
        if (trasparentDepth >= 5)
            return glm::vec3(0.0f);
        
        Ray inTransRay = calcSnellLaw(traceInfo, ray, traceInfo.worldNormal, -ray.direction, snellFrac);

        Tracer::RayInfo inTransInfo = traceRay(inTransRay);
        if (inTransInfo.hitDistance < 0.0f)
            return glm::vec3(0.0f);
        
        if (inTransInfo.closestSphere != traceInfo.closestSphere)
            transparentColor = innerRayGenerator(inTransRay, reflectionsDepth, Ks, backgroundColor, snellFrac, trasparentDepth + 1);
        else{

            Ray outTransRay = calcSnellLaw(inTransInfo, inTransRay, -traceInfo.worldNormal, inTransRay.direction, 1.0f / snellFrac);

            Tracer::RayInfo outTransInfo = traceRay(outTransRay);
            if (outTransInfo.hitDistance < 0.0f)
                return glm::vec3(0.0f);
            
            //Ray newRay;
            //newRay.origin = outTransInfo.worldPosition + outTransInfo.worldNormal * 0.0001f;
            //newRay.direction =outTransRay.direction;
            transparentColor = innerRayGenerator(outTransRay, reflectionsDepth, Ks, backgroundColor, snellFrac, trasparentDepth + 1);   
        }
        sphereColor += Ks * transparentColor;
        
        /*
        if (trasparentDepth >= 5)
            return backgroundColor;
        Ray newRay;
        newRay.origin = traceInfo.worldPosition + traceInfo.worldNormal * 0.0001f;
        newRay.direction = glm::reflect(ray.direction, traceInfo.worldNormal);

        sphereColor += Ks * innerRayGenerator(newRay, reflectionsDepth + 1, Ks, backgroundColor, snellFrac, trasparentDepth);
    `   */
    }

    return sphereColor;
    
    
    
}

/*glm::vec3 Tracer::innerRayGenerator(const Ray &ray, glm::vec3 color, int reflections, float multiplier, glm::vec3 backgroundColor, float snellN, float otherSnellN, int depth)
{
    Tracer::RayInfo traceInfo = traceRay(ray);

    if (traceInfo.hitDistance < 0.0f)
    {
        color += backgroundColor * multiplier;
        return color;
    }
    glm::vec3 sphereColor = getSphereColor(traceInfo, ray);
    color += sphereColor * multiplier;
    if (reflections > 1)
    {
        Ray newRay;
        newRay.origin = traceInfo.worldPosition + traceInfo.worldNormal * 0.0001f;
        newRay.direction = glm::reflect(ray.direction, traceInfo.worldNormal);

        color + 0.7f * innerRayGenerator(newRay, color, reflections - 1, multiplier * 0.7f, backgroundColor, snellN, otherSnellN, depth);
    }

    if (traceInfo.closestSphere->type == Transparent)
    {
        glm::vec3 transparentColor(0.0f);
        if (depth == 10)
        {
            return transparentColor;
        }

            
        Ray secondRay = calcSnellLaw(traceInfo, ray, traceInfo.worldNormal, -ray.direction, snellN / otherSnellN);

        Tracer::RayInfo transInfo = traceRay(secondRay);
        if (transInfo.hitDistance < 0.0f)
            return glm::vec3(0.0f);
        
        if (transInfo.closestSphere != traceInfo.closestSphere)
            transparentColor = innerRayGenerator(secondRay, transparentColor, reflections, multiplier, backgroundColor, snellN, otherSnellN, depth + 1);
        else{

            Ray thirdRay = calcSnellLaw(transInfo, secondRay, -traceInfo.worldNormal, ray.direction, otherSnellN / snellN);

            Tracer::RayInfo outTransInfo = traceRay(thirdRay);
            if (outTransInfo.hitDistance < 0.0f)
                return glm::vec3(0.0f);
            transparentColor = innerRayGenerator(thirdRay, transparentColor, reflections, multiplier, backgroundColor, snellN, otherSnellN, depth + 1);   
        }
        color += 0.7f * transparentColor;
        
        


        //color += 0.7f * innerRayGenerator(newRay, color, reflections, multiplier, backgroundColor, otherSnellN, snellN, depth + 1);
    }
    return color;
    
}*/

glm::vec4 Tracer::rayGenerator(int x, int y)
{
    Ray ray;
    ray.origin = glm::vec3(scene->eye->x, scene->eye->y, scene->eye->z);
    // ray.origin = glm::vec3(0,0,10);
    ray.direction = getRayDirection(glm::vec2(x, y), ray.origin);

    glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 finalColor = innerRayGenerator(ray, 0, 0.7f, backgroundColor, 1.0f / 1.5f, 0);
    return glm::vec4(finalColor, 1.0f);
}

glm::vec3 Tracer::getSphereColor(const Tracer::RayInfo &traceInfo, const Ray &ray)
{
    glm::vec3 sphereColor(0.0f);
    Sphere *sphere = traceInfo.closestSphere;
    sphereColor += glm::vec3(*(scene->ambientLight));

    glm::vec3 Ks(0.7f, 0.7f, 0.7f);

    glm::vec3 totalDiffuseReflection(0.0f);
    glm::vec3 totalSpecularReflection(0.0f);
    for (int i = 0; i < scene->lights->size(); i++)
    {
        Light *light = scene->lights->at(i);

        if (!isInLight(traceInfo, light))
            continue;

        glm::vec3 Kd = sphere->color;
        float cosTheta = glm::dot(glm::normalize(traceInfo.worldNormal), glm::normalize(-(light->direction)));
        float diffuseValue_r = Kd.r * cosTheta * (light->getRGBIntensity()).r;
        float diffuseValue_g = Kd.g * cosTheta * (light->getRGBIntensity()).g;
        float diffuseValue_b = Kd.b * cosTheta * (light->getRGBIntensity()).b;
        glm::vec3 diffuseValue(diffuseValue_r, diffuseValue_g, diffuseValue_b);
        totalDiffuseReflection += diffuseValue;

        glm::vec3 lightDirecrion = glm::normalize(light->direction);

        if (light->type == Spotlight)
        {
            SpotLight *spotlight = const_cast<SpotLight *>(reinterpret_cast<const SpotLight *>(light));
            lightDirecrion = glm::normalize(traceInfo.worldPosition - spotlight->position);
            //float cosAlpha = glm::dot(lightDirecrion, glm::normalize(spotlight->direction));
            //if (cosAlpha < spotlight->w)
            //    continue;
        }

        glm::vec3 lightReflection = glm::normalize(glm::reflect(lightDirecrion, traceInfo.worldNormal));
        float VR = glm::pow(glm::max(glm::dot(glm::normalize(-ray.direction), lightReflection), 0.0f), sphere->shininess);
        float specularValue_r = Ks.r * VR * (light->getRGBIntensity()).r;
        float specularValue_g = Ks.g * VR * (light->getRGBIntensity()).g;
        float specularValue_b = Ks.b * VR * (light->getRGBIntensity()).b;
        glm::vec3 specularValue(specularValue_r, specularValue_g, specularValue_b);

        totalSpecularReflection += specularValue;
    }
    sphereColor += totalDiffuseReflection + totalSpecularReflection;
    return sphereColor;
}

bool Tracer::isInLight(RayInfo traceInfo, Light *light)
{
    glm::vec3 lightDirection = glm::normalize(light->direction);
    float distanceToLight = FLT_MAX;

    if (light->type == Spotlight)
    {
        SpotLight *spotlight = const_cast<SpotLight *>(reinterpret_cast<const SpotLight *>(light));
        lightDirection = traceInfo.worldPosition - spotlight->position;
        distanceToLight = glm::length(lightDirection);
        lightDirection = glm::normalize(lightDirection);
        float cosAlpha = glm::dot(lightDirection, glm::normalize(spotlight->direction));
        if (glm::acos(cosAlpha) > glm::acos(spotlight->w))
            return false;
    }

    Ray ray;
    ray.direction = -lightDirection;
    ray.origin = traceInfo.worldPosition;
    vector<Sphere *> spheres = *(scene->spheres);
    ClosestSphereInfo closest = findClosestSphere(ray, spheres, distanceToLight, traceInfo.closestSphere);
    //if (distanceToLight != closest.hitDistance)
    //    cout << distanceToLight << "," << closest.hitDistance  << "," << (closest.closestSphere == nullptr) << endl;
    return closest.closestSphere == nullptr;
}
