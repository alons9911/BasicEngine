#include "./Bezier1D.h"


Bezier1D::Bezier1D()
{  
}

Bezier1D::Bezier1D(int segNum,int res,int mode, int viewport)
{  
    segmentsNum = segNum;
    resT = res;
    this->setMode(mode);
}


Bezier1D::Bezier1D(int segNum,int res, Scene* scn, int viewport)
{  
    segmentsNum = segNum;
    resT = res;
    this->setMode(Scene::LINE_STRIP);

    shapes = scn->getShapes();
    int shape_index = 0;

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
    for (int i = 0; i < number_of_octahedrons; i++) {
        scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
        scn->SetShapeTex(shape_index, 0);
        (*shapes)[shape_index]->MyScale(glm::vec3(0.5, 0.5, 0.5));
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
}

IndexedModel Bezier1D::GetLine() const
{
    IndexedModel model;

    return model;
}

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
    if (segment < segmentsNum)
        return segments[segment][indx];
    return segments[segmentsNum - 1][3];
}

glm::vec4 Bezier1D::GetPointOnCurve(int segment, int t)
{
    return glm::vec4();
}

glm::vec3 Bezier1D::GetVelosity(int segment, int t)
{
    return glm::vec3();
}

void Bezier1D::SplitSegment(int segment, int t)
{
}

void Bezier1D::AddFirstSegment(glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3) {
    segments.clear();
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments.back()[3];
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::ChangeSegment(int segIndx,glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments[segIndx-1][3];
    segments[segIndx] = glm::mat4(p0, p1, p2, p3);
}

float Bezier1D::MoveControlPoint(int segment, int indx, float dx,float dy,bool preserveC1)
{
    return 0; //not suppose to reach here
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