#include <vector>
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include "parser.h"


using namespace std;

class Tracer
{
public:
    Tracer() = default;
    void render(sceneDesription scene);
    vector<vector<unsigned char>> *getFinalImage() {return finalImage;}
    glm::vec4 calcPixelColor(glm::vec2 coordinate);
    unsigned char *getImageData() {return imageData;}

    int const IMG_HEIGHT = 800, IMG_WIDTH = 800;
private:
    unsigned char *imageData = nullptr;
    vector<vector<unsigned char>> *finalImage;
};