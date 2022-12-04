#include <fstream>
#include "parser.h"
#include <iostream>
#include <cstring>
#include <sstream>
using namespace std;

static sceneDesription parseInputFile(string fileName)
{
    ifstream infile(fileName);
    string brain_file;

    int n1, n2;
    float x1, x2, x3, x4;
    char op, c;

    //if (!(infile >> n1 >> n2))
    //{ /* error, could not read first line! Abort. */
    //    cout << "hello" << endl;
    //}

    sceneDesription scene;

    //while(getline(infile, brain_file)) {
    //    istringstream iss(brain_file);
    //    iss >> op >> x1 >> x2 >> x3 >> x4 >> c;
    //    cout << op << "," << x1 << "," << x2 << "," << x3 << "," << x4 << endl;
    //    parseLine(op, x1, x2, x3, x4, scene);
    //}


    while (infile >> op >> x1 >> x2 >> x3 >> x4)
    {
        // successfully extracted one line, data is in op, x1, ..., x4, c.
        cout << op << "," << x1 << "," << x2 << "," << x3 << "," << x4 << endl;
        parseLine(op, x1, x2, x3, x4, &scene);
    }
    cout << scene.e->x << endl;
    return scene;
}

static void parseLine(char op, float x1, float x2, float x3, float x4, sceneDesription *scene)
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
        scene->colors->push_back(parseColor(x1, x2, x3, x4));
        break;
    default:
        if (x4 > 0)
        {
            scene->objects->push_back(parseSphere(op, x1, x2, x3, x4));
        }
        else
        {
            scene->objects->push_back(parsePlane(op, x1, x2, x3, x4));
        }
    }
}

eye *parseEye(float x1, float x2, float x3, float x4)
{
    return new eye{
        .x = x1,
        .y = x2,
        .z = x3,
        .additional = x4
    };
}

ambientLight *parseAmbientLight(float x1, float x2, float x3, float x4)
{
    return new ambientLight{
        .R = x1,
        .G = x2,
        .B = x3,
        .A = x4
    };
}

lightDir parseLightDirection(float x1, float x2, float x3, float x4)
{
    return lightDir{
        .x = x1,
        .y = x2,
        .z = x3,
        .isSpotlight = (bool) x4
    };
}

spotlightPosition parseSpotlightPosition(float x1, float x2, float x3, float x4)
{
    return spotlightPosition{
        .x = x1,
        .y = x2,
        .z = x3,
        .w = x4
    };
}

intensity parseLightIntensity(float x1, float x2, float x3, float x4)
{
    return intensity{
        .R = x1,
        .G = x2,
        .B = x3,
        .A = x4
    };
}

color parseColor(float x1, float x2, float x3, float x4)
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
        return normal;
    }
}

sphere parseSphere(char op, float x1, float x2, float x3, float x4)
{
    objectType type = getType(op);
    return sphere{
        .x = x1,
        .y = x2,
        .z = x3,
        .radius = x4,
        .type = type
    };
}

plane parsePlane(char op, float x1, float x2, float x3, float x4)
{
    objectType type = getType(op);
    return plane{
        .a = x1,
        .b = x2,
        .c = x3,
        .d = x4,
        .type = type
    };
}
