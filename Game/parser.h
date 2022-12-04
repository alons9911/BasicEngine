#ifndef PARSER_H
#define PARSER_H

#include <vector>

using namespace std;

enum objectType {normal, reflective, transparent};

struct descriptor{};

struct objectDescriptor : descriptor
{
    objectType type;
};

struct eye : descriptor
{
  eye(float x, float y, float z, float additional){
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

  float x;
  float y;
  float z;
  float additional;
};

struct ambientLight : descriptor
{
  ambientLight(float R, float G, float B, float A){
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
  float R;
  float G;
  float B;
  float A;
};

struct lightDir : descriptor
{
  lightDir(float x, float y, float z, bool isSpotlight){
    this->x = x;
    this->y = y;
    this->z = z;
    this->isSpotlight = isSpotlight;
  };
  float x;
  float y;
  float z;
  bool isSpotlight;
};

struct spotlightPosition : descriptor
{
  spotlightPosition(float x, float y, float z, float w){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  };
  float x;
  float y;
  float z;
  float w;
};

struct intensity : descriptor
{
  intensity(float R, float G, float B, float A){
    this->R = R;
    this->G = G;
    this->B = B;
    this->A = A;
  };
  float R;
  float G;
  float B;
  float A;
};

struct sphere : objectDescriptor
{
  sphere(float x, float y, float z, float raduis, objectType type){
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = radius;
    this->type = type;
  };
  float x;
  float y;
  float z;
  float radius;
};

struct plane : objectDescriptor
{
  plane(float a, float b, float c, float d, objectType type){
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->type = type;
  };
  float a;
  float b;
  float c;
  float d;
};

struct color : descriptor
{
  color(float R, float G, float B, float A){
    this->R = R;
    this->G = G;
    this->B = B;
    this->A = A;
  };
  float R;
  float G;
  float B;
  float A;
};

struct sceneDesription
{
  sceneDesription(){
    this->e = new eye();
    this->light = new ambientLight();
    this->lightDirs = new vector<lightDir>();
    this->spotlightPositions = new vector<spotlightPosition>();
    this->intensities = new vector<intensity>();
    this->objects = new vector<objectDescriptor>();
    this->colors = new vector<color>();
  };
  eye *e;
  ambientLight *light;
  vector<lightDir> *lightDirs;
  vector<spotlightPosition> *spotlightPositions;
  vector<intensity> *intensities;
  vector<objectDescriptor> *objects;
  vector<color> *colors;

};




static sceneDesription parseInputFile(string fileName);
static void parseLine(char op, float x1, float x2, float x3, float x4, sceneDesription *scene);
eye *parseEye(float x1, float x2, float x3, float x4);
ambientLight *parseAmbientLight(float x1, float x2, float x3, float x4);
lightDir parseLightDirection(float x1, float x2, float x3, float x4);
spotlightPosition parseSpotlightPosition(float x1, float x2, float x3, float x4);
intensity parseLightIntensity(float x1, float x2, float x3, float x4);
color parseColor(float x1, float x2, float x3, float x4);
sphere parseSphere(char op, float x1, float x2, float x3, float x4);
plane parsePlane(char op, float x1, float x2, float x3, float x4);

objectType getType(char c);

#endif