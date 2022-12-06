#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <glm/glm.hpp>

using namespace std;

enum objectType {normal, reflective, transparent};

struct descriptor{};

struct color : descriptor
{
  color(double R, double G, double B, double A){
    this->R = R;
    this->G = G;
    this->B = B;
    this->A = A;
  };
  double R;
  double G;
  double B;
  double A;
};

struct objectDescriptor : descriptor
{
    objectType type;
    glm::vec4 objColor;

    void setColor(float r, float g, float b, float a) {
      this->objColor = glm::vec4(r, g, b, a); 
    }
};

struct eye : descriptor
{
  eye(double x, double y, double z, double additional){
    this->x = x;
    this->y = y;
    this->z = z;
    this->additional = additional;
  };
  eye(){
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

struct ambientLight : descriptor
{
  ambientLight(double R, double G, double B, double A){
    this->R = R;
    this->G = G;
    this->B = B;
    this->A = A;
  };
  ambientLight(){
    this->R = 0;
    this->G = 0;
    this->B = 0;
    this->A = 0;
  };
  double R;
  double G;
  double B;
  double A;
};

struct lightDir : descriptor
{
  lightDir(double x, double y, double z, bool isSpotlight){
    this->x = x;
    this->y = y;
    this->z = z;
    this->isSpotlight = isSpotlight;
  };
  double x;
  double y;
  double z;
  bool isSpotlight;
};

struct spotlightPosition : descriptor
{
  spotlightPosition(double x, double y, double z, double w){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  };
  double x;
  double y;
  double z;
  double w;
};

struct intensity : descriptor
{
  intensity(double R, double G, double B, double A){
    this->R = R;
    this->G = G;
    this->B = B;
    this->A = A;
  };
  double R;
  double G;
  double B;
  double A;
};

struct sphere : objectDescriptor
{
  sphere(double x, double y, double z, double raduis, objectType type){
    this->position = glm::vec3(x, y, z);
    this->radius = radius;
    this->type = type;
  };

  void setRaduis(double r){this->radius = r;}
  glm::vec3 position;
  double radius;
};

struct plane : objectDescriptor
{
  plane(double a, double b, double c, double d, objectType type){
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->type = type;
  };
  double a;
  double b;
  double c;
  double d;
};

struct sceneDesription
{
  sceneDesription(){
    this->e = new eye();
    this->light = new ambientLight();
    this->lightDirs = new vector<lightDir>();
    this->spotlightPositions = new vector<spotlightPosition>();
    this->intensities = new vector<intensity>();
    this->spheres = new vector<sphere*>();
    this->planes = new vector<plane*>();
    this->colors = new vector<color>();
  };
  eye *e;
  ambientLight *light;
  vector<lightDir> *lightDirs;
  vector<spotlightPosition> *spotlightPositions;
  vector<intensity> *intensities;
  vector<sphere*> *spheres;
  vector<plane*> *planes;
  vector<color> *colors;

};




static sceneDesription parseInputFile(string fileName);
static void parseLine(char op, double x1, double x2, double x3, double x4, sceneDesription *scene, vector<objectDescriptor*> *orderedObjects, int *nextColoredObject);
eye *parseEye(double x1, double x2, double x3, double x4);
ambientLight *parseAmbientLight(double x1, double x2, double x3, double x4);
lightDir parseLightDirection(double x1, double x2, double x3, double x4);
spotlightPosition parseSpotlightPosition(double x1, double x2, double x3, double x4);
intensity parseLightIntensity(double x1, double x2, double x3, double x4);
color parseColor(double x1, double x2, double x3, double x4);
sphere *parseSphere(char op, double x1, double x2, double x3, double x4);
plane *parsePlane(char op, double x1, double x2, double x3, double x4);

objectType getType(char c);

#endif