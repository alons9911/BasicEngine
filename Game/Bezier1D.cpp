#include "./Bezier1D.h"
#include <iostream>
using namespace std;

Bezier1D::Bezier1D()
{
}

Bezier1D::Bezier1D(int segNum, int res, int mode, int viewport)
{
    segmentsNum = segNum;
    resT = res;
    this->setMode(mode);
}

Bezier1D::Bezier1D(int segNum, int res, Scene *scn, int viewport)
{
    segmentsNum = segNum;
    resT = res;
    this->setMode(Scene::LINE_STRIP);

    shapes = scn->getShapes();

    // Cube + Octahedrons texture
    scn->AddTexture("../res/textures/box0.bmp", false);

    // Bezier 1D texture
    scn->AddTexture("../res/textures/grass.bmp", false);
}

void Bezier1D::Init(Scene *scn)
{
    int shape_index = 0;
    first_point_indx = shape_index;

    // Octahedrons
    for (int i = 0; i < number_of_octahedrons; i++)
    {
        cout << i << endl;
        scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
        scn->SetShapeTex(shape_index, 0);
        (*shapes)[shape_index]->MyScale(glm::vec3(0.25, 0.25, 0.25));

        //(*shapes)[shape_index]->MyTranslate(glm::vec3((float)i, 0.5, 0.5), 0);
        (*shapes)[shape_index]->Hide();
        shape_index++;
    }

    // Cube
     scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
     scn->SetShapeTex(shape_index, 0);

    last_point_indx = shape_index - 1;
    cube_indx = shape_index;

    // Setting segments
    glm::vec4 zero_vector = glm::vec4(0, 0, 0, 0);
    AddFirstSegment(zero_vector, zero_vector, zero_vector, zero_vector);

    for (int i = 4; i < number_of_octahedrons; i += 3)
        AddSegment(zero_vector, zero_vector, zero_vector);

    // Configurations
    std::vector<glm::vec3> config2;
    config2.resize(7);
    config2[0] = glm::vec3(-9, -9, 0);
    config2[1] = glm::vec3(-9, -3, 0);
    config2[2] = glm::vec3(-6, 0, 0);
    config2[3] = glm::vec3(0, 0, 0);
    config2[4] = glm::vec3(6, 0, 0);
    config2[5] = glm::vec3(9, -3, 0);
    config2[6] = glm::vec3(9, -9, 0);

    std::vector<glm::vec3> config3;
    config3.resize(10);
    config3[0] = glm::vec3(-15, -9, 0);
    config3[1] = glm::vec3(-15, -3, 0);
    config3[2] = glm::vec3(-12, 0, 0);
    config3[3] = glm::vec3(-6, 0, 0);
    config3[4] = glm::vec3(-3, 0, 0);
    config3[5] = glm::vec3(3, 0, 0);
    config3[6] = glm::vec3(6, 0, 0);
    config3[7] = glm::vec3(12, 0, 0);
    config3[8] = glm::vec3(15, -3, 0);
    config3[9] = glm::vec3(15, -9, 0);

    std::vector<glm::vec3> config4;
    config4.resize(13);
    config4[0] = glm::vec3(-21, -9, 0);
    config4[1] = glm::vec3(-21, -3, 0);
    config4[2] = glm::vec3(-18, 0, 0);
    config4[3] = glm::vec3(-12, 0, 0);
    config4[4] = glm::vec3(-9, 0, 0);
    config4[5] = glm::vec3(-3, 0, 0);
    config4[6] = glm::vec3(0, 0, 0);
    config4[7] = glm::vec3(3, 0, 0);
    config4[8] = glm::vec3(9, 0, 0);
    config4[9] = glm::vec3(12, 0, 0);
    config4[10] = glm::vec3(18, 0, 0);
    config4[11] = glm::vec3(21, -3, 0);
    config4[12] = glm::vec3(21, -9, 0);

    std::vector<glm::vec3> config5;
    config5.resize(16);
    config5[0] = glm::vec3(-27, -9, 0);
    config5[1] = glm::vec3(-27, -3, 0);
    config5[2] = glm::vec3(-24, 0, 0);
    config5[3] = glm::vec3(-18, 0, 0);
    config5[4] = glm::vec3(-15, 0, 0);
    config5[5] = glm::vec3(-9, 0, 0);
    config5[6] = glm::vec3(-6, 0, 0);
    config5[7] = glm::vec3(-3, 0, 0);
    config5[8] = glm::vec3(3, 0, 0);
    config5[9] = glm::vec3(6, 0, 0);
    config5[10] = glm::vec3(9, 0, 0);
    config5[11] = glm::vec3(15, 0, 0);
    config5[12] = glm::vec3(18, 0, 0);
    config5[13] = glm::vec3(24, 0, 0);
    config5[14] = glm::vec3(27, -3, 0);
    config5[15] = glm::vec3(27, -9, 0);

    std::vector<glm::vec3> config6;
    config6.resize(19);
    config6[0] = glm::vec3(-33, -9, 0);
    config6[1] = glm::vec3(-33, -3, 0);
    config6[2] = glm::vec3(-30, 0, 0);

    config6[3] = glm::vec3(-24, 0, 0);
    config6[4] = glm::vec3(-21, 0, 0);
    config6[5] = glm::vec3(-15, 0, 0);

    config6[6] = glm::vec3(-12, 0, 0);
    config6[7] = glm::vec3(-9, 0, 0);
    config6[8] = glm::vec3(-3, 0, 0);
    config6[9] = glm::vec3(0, 0, 0);
    config6[10] = glm::vec3(3, 0, 0);
    config6[11] = glm::vec3(9, 0, 0);
    config6[12] = glm::vec3(12, 0, 0);

    config6[13] = glm::vec3(15, 0, 0);
    config6[14] = glm::vec3(21, 0, 0);
    config6[15] = glm::vec3(24, 0, 0);

    config6[16] = glm::vec3(30, 0, 0);
    config6[17] = glm::vec3(33, -3, 0);
    config6[18] = glm::vec3(33, -9, 0);

    // Add all configs
    conf.push_back(config2);
    conf.push_back(config3);
    conf.push_back(config4);
    conf.push_back(config5);
    conf.push_back(config6);

    NumberOfSegmentsToDisplay(segmentsNum);

}

///////

void Bezier1D::NumberOfSegmentsToDisplay(int segNum)
{
    int res = ((resT - 1) / segmentsNum * segNum) + 1;
    segmentsNum = segNum;
    resT = res;

    int config_num = segNum - 2;

    // Reset Cube position
    glm::vec3 cube_new_position = conf[config_num][0];
    glm::vec4 cube_old_position = (*shapes)[cube_indx]->GetTranslate()[3];
    glm::vec3 movement = cube_new_position - glm::vec3(cube_old_position.x, cube_old_position.y, cube_old_position.z);
    (*shapes)[cube_indx]->MyTranslate(movement, 0);
    (*shapes)[cube_indx]->SetRotate(glm::mat4(1));

    for (int i = 0; i < number_of_octahedrons; i++)
    {
        if (i < conf[config_num].size())
        {
            (*shapes)[i]->Unhide();

            // Reset Octahedrons posotions
            glm::vec3 point_new_position = conf[config_num][i];
            glm::vec4 point_old_position = (*shapes)[i]->GetTranslate()[3];
            glm::vec3 movement = point_new_position - glm::vec3(point_old_position.x, point_old_position.y, point_old_position.z);
            (*shapes)[i]->MyTranslate(movement, 0);
        }
        else
        {
            (*shapes)[i]->Hide();
        }
    }

    last_point_indx = conf[config_num].size() - 1;
    UpdateCurveByShapes();
    this->SetMesh(GetLine());
}

void Bezier1D::UpdateCurveByShapes() 
{
    glm::vec4 p0 = (*shapes)[0]->GetTranslate()[3];
    glm::vec4 p1 = (*shapes)[1]->GetTranslate()[3];
    glm::vec4 p2 = (*shapes)[2]->GetTranslate()[3];
    glm::vec4 p3 = (*shapes)[3]->GetTranslate()[3];
    ChangeFirstSegment(p0, p1, p2, p3);

    int segIndx = 1;
    for (int i = 4; i < shapes->size() - 2; i += 3) {
        glm::vec4 p1 = (*shapes)[i]->GetTranslate()[3];
        glm::vec4 p2 = (*shapes)[i + 1]->GetTranslate()[3];
        glm::vec4 p3 = (*shapes)[i + 2]->GetTranslate()[3];

        ChangeSegment(segIndx, p1, p2, p3);
        segIndx++;
    }
    SetMesh(GetLine());
}

///////

IndexedModel Bezier1D::GetLine()
{
    IndexedModel model;

    ///

    int num_of_dots_on_line = (resT - 1) / segmentsNum;

    // Verifying that resT value is valid
    if ((resT - 1) % segmentsNum != 0)
    {
        std::cout << "Error: 'resT' isn't divisible by 'segmentsNum' with a remainder of '1'!" << std::endl;
        throw std::invalid_argument("");
    }

    for (int i = 0; i < resT; i++)
    {
        model.indices.push_back(i);
    }

    glm::vec4 p_0 = GetControlPoint(0, 0);
    model.positions.push_back(glm::vec3(p_0.x, p_0.y, p_0.z));

    for (int i = 0; i < segmentsNum; i++)
    {
        for (int j = 0; j < num_of_dots_on_line; j++)
        {
            float t = (1.f / (float)num_of_dots_on_line) * (j + 1);
            glm::vec4 p_t = GetPointOnCurve(i, t);

            // Normal to the Curve (2D)
            glm::vec3 dt = GetVelosity(i, t);
            float sqrt_t = sqrt(pow(dt.x, 2) + pow(dt.y, 2));
            glm::vec3 normal = glm::vec3(-dt.y / sqrt_t, dt.x / sqrt_t, 0);

            model.positions.push_back(glm::vec3(p_t.x, p_t.y, p_t.z));
            model.colors.push_back(glm::vec3(1.f, 1.f, 1.f));
            model.normals.push_back(normal);
        }
    }

    ///

    return model;
}


///// 





/////

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
    if (segment < segmentsNum)
        return segments[segment][indx];
    return segments[segmentsNum - 1][3];
}

glm::vec4 Bezier1D::GetPointOnCurve(int segment, float t)
{
    // Cubic:
    // b(t) = (1-t)^3 * b_0 + 3(1-t)^2 * t * b_1 + 3(1-t) * t^2 * b_2 + t^3 * b_3

    glm::mat4 b = segments[segment];

    float a_0 = pow(1 - t, 3);
    float a_1 = 3 * pow(1 - t, 2) * t;
    float a_2 = 3 * (1 - t) * pow(t, 2);
    float a_3 = pow(t, 3);

    return a_0 * b[0] + a_1 * b[1] + a_2 * b[2] + a_3 * b[3];
}

glm::vec3 Bezier1D::GetVelosity(int segment, float t)
{
    // Cubic:
    // v(t) = b'(t) = -3(1-t)^2 * b_0 + (3(1-t)^2 - 6(1-t)t) * b_1  + (6t(1-t) - 3t^2) * b_2 + 3t^2 * b_3

    glm::mat4 b = segments[segment];

    float a_0 = -3 * pow(1 - t, 2);
    float a_1 = 3 * pow(1 - t, 2) - 6 * (1 - t) * t;
    float a_2 = 6 * t * (1 - t) - 3 * pow(t, 2);
    float a_3 = 3 * pow(t, 2);

    glm::vec4 res = a_0 * b[0] + a_1 * b[1] + a_2 * b[2] + a_3 * b[3];
    glm::vec3 a(res.x, res.y, res.z);
    cout << "t: "<<t<<", v: " << a.x <<","<<a.y<<","<<a.z<<endl;
    return a;
}

void Bezier1D::SplitSegment(int segment, int t)
{
}

void Bezier1D::AddFirstSegment(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    segments.clear();
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments.back()[3];
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::ChangeSegment(int segIndx, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments[segIndx - 1][3];
    segments[segIndx] = glm::mat4(p0, p1, p2, p3);
}

void Bezier1D::ChangeFirstSegment(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    segments[0] = glm::mat4(p0, p1, p2, p3);
}

float Bezier1D::MoveControlPoint(int segment, int indx, float dx, float dy, bool preserveC1)
{
    return 0; // not suppose to reach here
}

void Bezier1D::CurveUpdate(int pointIndx, float dx, float dy, bool preserveC1)
{
}

void Bezier1D::ResetCurve(int segNum)
{
}

Bezier1D::~Bezier1D(void)
{
}