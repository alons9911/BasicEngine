#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <glm/glm.hpp>

using namespace std;

enum ObjectType {Object, Reflective, Transparent};
enum ObjectClass {PlaneClass, SphereClass};
enum LightType {Directional, Spotlight};


struct Descriptor
{
};

struct ObjectDescriptor : Descriptor
{
    ObjectType type;
    glm::vec3 color;
    float shininess;
    ObjectClass objectClass;


    void setColor(float r, float g, float b) {
      this->color = glm::vec3(r, g, b); 
    }
    void setShininess(float shininess){
      this->shininess = shininess;
    }
};

struct Eye : Descriptor
{
  Eye(double x, double y, double z, double additional){
    this->x = x;
    this->y = y;
    this->z = z;
    this->additional = additional;
  };
  Eye(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->additional = 0;
  };

  double x;
  double y;
  double z;
  double additional;
};

struct Light : Descriptor
{
  glm::vec3 direction;
  glm::vec4 intensity;
  LightType type;

  void setDirection(float x, float y, float z) {
      this->direction = glm::vec3(x, y, z); 
  }

  void setIntensity(float r, float g, float b, float a) {
      this->intensity = glm::vec4(r, g, b, a); 
  }

  glm::vec3 getRGBIntensity() {
    return glm::vec3(this->intensity.r, this->intensity.g, this->intensity.b);
  }
};

struct DirectionalLight : Light
{
  DirectionalLight(){
    this->type = Directional;
  }
};

struct SpotLight : Light
{
  SpotLight(){
    this->type = Spotlight;
  }
  glm::vec3 position;
  float w;

  void setPosition(float x, float y, float z) {
      this->position = glm::vec3(x, y, z);
  }
  void setAngle(float w){
    this->w = w;
  }
};

struct Sphere : ObjectDescriptor
{
  Sphere(double x, double y, double z, double raduis, ObjectType type){
    this->position = glm::vec3(x, y, z);
    this->radius = radius;
    this->type = type;
    this->objectClass = SphereClass;
  };

  void setRadius(double r){this->radius = r;}
  glm::vec3 position;
  double radius;
};

struct Plane : ObjectDescriptor
{
  Plane(double a, double b, double c, double d, ObjectType type){
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->type = type;
    this->objectClass = PlaneClass;
  };
  double a;
  double b;
  double c;
  double d;
};

struct SceneDesription
{
  SceneDesription(){
    this->eye = new Eye();
    this->ambientLight = new glm::vec4(0.0f);
    this->lights = new vector<Light*>();
    this->spotlights = new vector<SpotLight*>();
    this->spheres = new vector<Sphere*>();
    this->planes = new vector<Plane*>();
  };
  Eye *eye;
  glm::vec4 *ambientLight;
  vector<Light*> *lights;
  vector<SpotLight*> *spotlights;
  vector<Sphere*> *spheres;
  vector<Plane*> *planes;
};




static SceneDesription parseInputFile(string fileName);
static void parseLine(char op, double x1, double x2, double x3, double x4, SceneDesription *scene, vector<ObjectDescriptor*> *orderedObjects, int *nextColoredObject, int *nextSpotlightPosition, int *nextIntensityPosition);
Eye *parseEye(double x1, double x2, double x3, double x4);
glm::vec4 *parseAmbientLight(double x1, double x2, double x3, double x4);
Light *parseLightDirection(double x1, double x2, double x3, double x4);
Sphere *parseSphere(char op, double x1, double x2, double x3, double x4);
Plane *parsePlane(char op, double x1, double x2, double x3, double x4);

ObjectType getType(char c);

#endif