#include <fstream>
#include "parser.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

static void printParsedInput(SceneDesription scene){
    cout << "----- configuration ----" << endl;

    Eye eye = *scene.eye;
    cout << "<eye: x=" << eye.x << ", y=" << eye.y << ", z=" << eye.z << ", additional=" << eye.additional  << " >" << endl;

    glm::vec4 ambientLight = *scene.ambientLight;
    cout << "<ambientLight: R=" << ambientLight.r << ", G=" << ambientLight.g << ", B=" << ambientLight.b << ", A=" << ambientLight.a  << " >" << endl;

    vector<Sphere*> spheres = *scene.spheres;
    cout << "<spheres: " << endl;
    for (int i = 0; i < spheres.size(); i++)
    {
        Sphere obj = *spheres.at(i);
        cout << "    <Sphere: type=" << obj.type << ", radius="<< obj.radius << ", position=" << obj.position.x << "," << obj.position.y << "," << obj.position.z << ", color="<< obj.color.r << "," << obj.color.g << "," << obj.color.b << ", shininess=" << obj.shininess << " >" << endl;
    }
    cout << ">" << endl;

    vector<Plane*> planes = *scene.planes;
    cout << "<planes: " << endl;
    for (int i = 0; i < planes.size(); i++)
    {
        Plane obj = *planes.at(i);
        cout << "    <Plane: type=" << obj.type << ", values=" << obj.a << "," << obj.b << "," << obj.c << "," << obj.d << ", color="<< obj.color.r << "," << obj.color.g << "," << obj.color.b << ", shininess=" << obj.shininess <<" >" << endl;
    }
    cout << ">" << endl;

    vector<Light*> lights = *scene.lights;
    cout << "<lights: " << endl;
    for (int i = 0; i < lights.size(); i++)
    {
        Light l = *lights.at(i);
        cout << "    <Light: type=" << l.type << ", direction=" << l.direction.x << "," << l.direction.y << "," << l.direction.z << ", intensity="<< l.intensity.r << "," << l.intensity.g << "," << l.intensity.b << "," << l.intensity.a <<" >" << endl;
    }
    cout << ">" << endl;

    vector<SpotLight*> spotlights = *scene.spotlights;
    cout << "<spotlights: " << endl;
    for (int i = 0; i < spotlights.size(); i++)
    {
        SpotLight l = *spotlights.at(i);
        cout << "    <spotlights: direction=" << l.direction.x << "," << l.direction.y << "," << l.direction.z << ", intensity="<< l.intensity.r << "," << l.intensity.g << "," << l.intensity.b << "," << l.intensity.a << ", position="<< l.position.x << "," << l.position.y << "," << l.position.z << ", w="<< l.w << " >" << endl;
    }
    cout << ">" << endl;
}

static SceneDesription parseInputFile(string fileName)
{
    ifstream infile(fileName);
    string brain_file;

    double x1, x2, x3, x4;
    char op, c;

    SceneDesription scene;
    vector<ObjectDescriptor*> *orderedObjects = new vector<ObjectDescriptor*>();
    int nextColoredObject = 0, nextSpotlightPosition = 0, nextIntensityPosition = 0;

    while (infile >> op >> x1 >> x2 >> x3 >> x4)
    {
        // successfully extracted one line, data is in op, x1, ..., x4, c.
        cout << op << "," << x1 << "," << x2 << "," << x3 << "," << x4 << endl;
        parseLine(op, x1, x2, x3, x4, &scene, orderedObjects, &nextColoredObject, &nextSpotlightPosition, &nextIntensityPosition);
    }
    return scene;
}

static void parseLine(char op, double x1, double x2, double x3, double x4, SceneDesription *scene, 
vector<ObjectDescriptor*> *orderedObjects, int *nextColoredObject, int *nextSpotlightPosition, int *nextIntensityPosition)
{
    Light *light = nullptr; // For 'd' case

    switch (op)
    {
    case 'e':
        scene->eye = parseEye(x1, x2, x3, x4);
        break;
    case 'a':
        scene->ambientLight = parseAmbientLight(x1, x2, x3, x4);
        break;
    case 'd':
        light  = parseLightDirection(x1, x2, x3, x4);
        scene->lights->push_back(light);
        if (light->type == Spotlight)
            scene->spotlights->push_back(const_cast<SpotLight*>(reinterpret_cast<const SpotLight*>(light)));
        break;
    case 'p':
        scene->spotlights->at(*nextSpotlightPosition)->setPosition(x1, x2, x3);
        scene->spotlights->at(*nextSpotlightPosition)->setAngle(x4);
        (*nextSpotlightPosition)++;
        break;
    case 'i':
        scene->lights->at(*nextIntensityPosition)->setIntensity(x1, x2, x3, x4);
        (*nextIntensityPosition)++;
        break;
    case 'c':
        orderedObjects->at(*nextColoredObject)->setColor(x1, x2, x3);
        orderedObjects->at(*nextColoredObject)->setShininess(x4);
        (*nextColoredObject)++;
        break;
    default:
        //objectDescriptor obj;
        if (x4 > 0)
        {
            Sphere *obj = parseSphere(op, x1, x2, x3, x4);
            scene->spheres->push_back(obj);
            obj = scene->spheres->at(scene->spheres->size() - 1);
            orderedObjects->push_back(obj);
        }
        else
        {
            Plane *obj = parsePlane(op, x1, x2, x3, x4);
            scene->planes->push_back(obj);
            obj = scene->planes->at(scene->planes->size() - 1);
            orderedObjects->push_back(obj);
        }
    }
}

Eye *parseEye(double x1, double x2, double x3, double x4)
{
    return new Eye{
        .x = x1,
        .y = x2,
        .z = x3,
        .additional = x4
    };
}

glm::vec4 *parseAmbientLight(double x1, double x2, double x3, double x4)
{
    return new glm::vec4(x1, x2, x3, x4);
}

Light *parseLightDirection(double x1, double x2, double x3, double x4)
{
    Light *light = nullptr;
    if ((bool) x4)
        light = new SpotLight();
    else
        light = new DirectionalLight();
    light->setDirection(x1, x2, x3);
    return light;
}

ObjectType getType(char c)
{
    switch (c)
    {
    case 'r':
        return Reflective;
    case 't':
        return Transparent;
    default:
        return Object;
    }
}

Sphere *parseSphere(char op, double x1, double x2, double x3, double x4)
{
    ObjectType type = getType(op);
    Sphere *s = new Sphere(x1, x2, x3, 0, type);
    s->setRadius(x4);
    return s;
}

Plane *parsePlane(char op, double x1, double x2, double x3, double x4)
{
    ObjectType type = getType(op);
    return new Plane(x1, x2, x3, x4, type);
}
