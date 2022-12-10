#include <vector>
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include "parser.h"


using namespace std;


struct Ray{
    glm::vec3 origin;
    glm::vec3 direction;
};
class Tracer
{
public:
    Tracer() = default;
    void render(SceneDesription *scene);
    vector<vector<unsigned char>> *getFinalImage() {return finalImage;}
    unsigned char *getImageData() {return imageData;}

    int const IMG_HEIGHT = 800, IMG_WIDTH = 800;
private:
    struct RayInfo{
        float hitDistance;
        glm::vec3 worldPosition;
        glm::vec3 worldNormal;
        Sphere *closestSphere;
    };
    unsigned char *imageData = nullptr;
    vector<vector<unsigned char>> *finalImage;
    SceneDesription *scene = nullptr;


    glm::vec3 getRayDirection(glm::vec2 boardCoordinate, glm::vec3 origin);
    RayInfo traceRay(const Ray &ray);
    RayInfo closestHit(const Ray &ray, Sphere *closestSphere, float hitDistance);
    RayInfo miss(const Ray &ray);
    glm::vec4 rayGenerator(int x, int y);
};