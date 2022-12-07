#include <fstream>
#include "parser.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

static void printParsedInput(sceneDesription scene){
    cout << "----- configuration ----" << endl;

    eye e = *scene.e;
    cout << "<eye: x=" << e.x << ", y=" << e.y << ", z=" << e.z << ", additional=" << e.additional  << " >" << endl;

    ambientLight light = *scene.light;
    cout << "<light: R=" << light.R << ", G=" << light.G << ", B=" << light.B << ", A=" << light.A  << " >" << endl;

    vector<sphere*> spheres = *scene.spheres;
    cout << "<spheres: " << endl;
    for (int i = 0; i < spheres.size(); i++)
    {
        sphere obj = *spheres.at(i);
        cout << "    <sphere: type=" << obj.type << ", radius="<< obj.radius << ", position=" << obj.position.x << "," << obj.position.y << "," << obj.position.z << ", color="<< obj.objColor.r << "," << obj.objColor.g << "," << obj.objColor.b << "," << obj.objColor.a <<" >" << endl;
    }
    cout << ">" << endl;

    vector<plane*> planes = *scene.planes;
    cout << "<planes: " << endl;
    for (int i = 0; i < planes.size(); i++)
    {
        plane obj = *planes.at(i);
        cout << "    <plane: type=" << obj.type << ", values=" << obj.a << "," << obj.b << "," << obj.c << "," << obj.d << ", color="<< obj.objColor.r << "," << obj.objColor.g << "," << obj.objColor.b << "," << obj.objColor.a <<" >" << endl;
    }
    cout << ">" << endl;

    vector<color> colors = *scene.colors;
    cout << "<colors: " << endl;
    for (int i = 0; i < colors.size(); i++)
    {
        color c = colors.at(i);
        cout << "    <color: R=" << c.R << ", G=" << c.G << ", B=" << c.B << ", A=" << c.A  << " >" << endl;
    }
    cout << ">" << endl;

    vector<lightDir> dirs = *scene.lightDirs;
    cout << "<lightDirs: " << endl;
    for (int i = 0; i < dirs.size(); i++)
    {
        lightDir dir = dirs.at(i);
        cout << "    <lightDir: x=" << dir.x << ", y=" << dir.y << ", z=" << dir.z << ", isSpotlight=" << dir.isSpotlight  << " >" << endl;
    }
    cout << ">" << endl;

    vector<spotlightPosition> positions = *scene.spotlightPositions;
    cout << "<positions: " << endl;
    for (int i = 0; i < positions.size(); i++)
    {
        spotlightPosition pos = positions.at(i);
        cout << "    <spotlightPosition: x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << ", w=" << pos.w  << " >" << endl;
    }
    cout << ">" << endl;

    vector<intensity> intens = *scene.intensities;
    cout << "<positions: " << endl;
    for (int i = 0; i < intens.size(); i++)
    {
        intensity in = intens.at(i);
        cout << "    <intensity: R=" << in.R << ", G=" << in.G << ", B=" << in.B << ", A=" << in.A  << " >" << endl;
    }
    cout << ">" << endl;
}

static sceneDesription parseInputFile(string fileName)
{
    ifstream infile(fileName);
    string brain_file;

    int n1, n2;
    double x1, x2, x3, x4;
    char op, c;

    sceneDesription scene;
    vector<objectDescriptor*> *orderedObjects = new vector<objectDescriptor*>();
    int nextColoredObject = 0;

    while (infile >> op >> x1 >> x2 >> x3 >> x4)
    {
        // successfully extracted one line, data is in op, x1, ..., x4, c.
        cout << op << "," << x1 << "," << x2 << "," << x3 << "," << x4 << endl;
        parseLine(op, x1, x2, x3, x4, &scene, orderedObjects, &nextColoredObject);
    }
    return scene;
}

static void parseLine(char op, double x1, double x2, double x3, double x4, sceneDesription *scene, vector<objectDescriptor*> *orderedObjects, int *nextColoredObject)
{
    switch (op)
    {
    case 'e':
        scene->e = parseEye(x1, x2, x3, x4);
        break;
    case 'a':
        scene->light = parseAmbientLight(x1, x2, x3, x4);
        break;
    case 'd':
        scene->lightDirs->push_back(parseLightDirection(x1, x2, x3, x4));
        break;
    case 'p':
        scene->spotlightPositions->push_back(parseSpotlightPosition(x1, x2, x3, x4));
        break;
    case 'i':
        scene->intensities->push_back(parseLightIntensity(x1, x2, x3, x4));
        break;
    case 'c':
        orderedObjects->at(*nextColoredObject)->setColor(x1, x2, x3, x4);
        (*nextColoredObject)++;
        scene->colors->push_back(parseColor(x1, x2, x3, x4));
        break;
    default:
        //objectDescriptor obj;
        if (x4 > 0)
        {
            sphere *obj = parseSphere(op, x1, x2, x3, x4);
            scene->spheres->push_back(obj);
            obj = scene->spheres->at(scene->spheres->size() - 1);
            orderedObjects->push_back(obj);
        }
        else
        {
            plane *obj = parsePlane(op, x1, x2, x3, x4);
            scene->planes->push_back(obj);
            obj = scene->planes->at(scene->planes->size() - 1);
            orderedObjects->push_back(obj);
        }
    }
}

eye *parseEye(double x1, double x2, double x3, double x4)
{
    return new eye{
        .x = x1,
        .y = x2,
        .z = x3,
        .additional = x4
    };
}

ambientLight *parseAmbientLight(double x1, double x2, double x3, double x4)
{
    return new ambientLight{
        .R = x1,
        .G = x2,
        .B = x3,
        .A = x4
    };
}

lightDir parseLightDirection(double x1, double x2, double x3, double x4)
{
    return lightDir{
        .x = x1,
        .y = x2,
        .z = x3,
        .isSpotlight = (bool) x4
    };
}

spotlightPosition parseSpotlightPosition(double x1, double x2, double x3, double x4)
{
    return spotlightPosition{
        .x = x1,
        .y = x2,
        .z = x3,
        .w = x4
    };
}

intensity parseLightIntensity(double x1, double x2, double x3, double x4)
{
    return intensity{
        .R = x1,
        .G = x2,
        .B = x3,
        .A = x4
    };
}

color parseColor(double x1, double x2, double x3, double x4)
{
    return color{
        .R = x1,
        .G = x2,
        .B = x3,
        .A = x4
    };
}

objectType getType(char c)
{
    switch (c)
    {
    case 'r':
        return reflective;
    case 't':
        return transparent;
    default:
        return object;
    }
}

sphere *parseSphere(char op, double x1, double x2, double x3, double x4)
{
    objectType type = getType(op);
    sphere *s = new sphere(x1, x2, x3, 0, type);
    s->setRadius(x4);
    return s;
}

plane *parsePlane(char op, double x1, double x2, double x3, double x4)
{
    objectType type = getType(op);
    return new plane(x1, x2, x3, x4, type);
}
