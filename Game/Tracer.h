#include <vector>
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include "parser.h"


using namespace std;


struct Ray{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct ClosestSphereInfo{
    ClosestSphereInfo(Sphere *closestSphere, float hitDistance){
        this->closestSphere = closestSphere;
        this->hitDistance = hitDistance;
    }

    Sphere *closestSphere;
    float hitDistance;
};

struct ClosestPlaneInfo{
    ClosestPlaneInfo(Plane *closestPlane, float hitDistance){
        this->closestPlane = closestPlane;
        this->hitDistance = hitDistance;
    }

    Plane *closestPlane;
    float hitDistance;
};

class Tracer
{
public:
    Tracer() = default;
    void render(SceneDesription *scene);
    vector<vector<unsigned char>> *getFinalImage() {return finalImage;}
    unsigned char *getImageData() {return imageData;}

    int const IMG_HEIGHT = 800, IMG_WIDTH = 800;
    struct RayInfo{
        float hitDistance;
        glm::vec3 worldPosition;
        glm::vec3 worldNormal;
        ObjectDescriptor *closestObject;
    };
    private:
    unsigned char *imageData = nullptr;
    vector<vector<unsigned char>> *finalImage;
    SceneDesription *scene = nullptr;


    glm::vec3 getRayDirection(glm::vec2 boardCoordinate, glm::vec3 origin);
    RayInfo traceRay(const Ray &ray);
    ClosestSphereInfo findClosestSphere(const Ray &ray, vector<Sphere*> spheres, float maxHitDistance, Sphere *excludedSphere);
    ClosestPlaneInfo findClosestPlane(const Ray &ray, vector<Plane *> planes, float maxHitDistance);
    RayInfo closestHitSphere(const Ray &ray, Sphere *closestSphere, float hitDistance);
    RayInfo closestHitPlane(const Ray &ray, Plane *closestPlane, float hitDistance);
    RayInfo miss(const Ray &ray);
    
    glm::vec3 getSphereColor(const Tracer::RayInfo &traceInfo, const Ray &ray);
    bool isBlackSquare(Plane* plane, glm::vec3 position);
    bool isInLight(RayInfo traceInfo, Light *light);
    Ray calcSnellLaw(Tracer::RayInfo traceInfo, Ray ray, glm::vec3 N, glm::vec3 rayDirection, float snellFrac);


    glm::vec4 rayGenerator(int x, int y);
    glm::vec3 innerRayGenerator(const Ray &ray, int reflectionsDepth, float Ks, glm::vec3 backgroundColor, float snellFrac, int trasparentDepth);
};