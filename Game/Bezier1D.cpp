#include "./Bezier1D.h"
#include "./BezierConfig.h"
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
    scn->AddTexture("../res/textures/box0.bmp", false);
    scn->AddTexture("../res/textures/grass.bmp", false);
}

void Bezier1D::Init(Scene *scn)
{
    int shape_index = 0;
    first_point_indx = shape_index;

    // Octahedrons
    for (int i = 0; i < number_of_octahedrons; i++)
    {
        scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
        scn->SetShapeTex(shape_index, 0);
        (*shapes)[shape_index]->MyScale(glm::vec3(0.25, 0.25, 0.25));
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

    segmentsLocations = getSegmentsLocations();

    ResetCurve(segmentsNum);

}


void Bezier1D::UpdateSegmentsNumber(int segNum)
{
    int res = ((resT - 1) / segmentsNum * segNum) + 1;
    segmentsNum = segNum;
    resT = res;

    int config_num = segNum - 2;

    // Reset Cube position
    glm::vec3 cube_new_position = segmentsLocations[config_num][0];
    glm::vec4 cube_old_position = (*shapes)[cube_indx]->GetTranslate()[3];
    glm::vec3 loaction_delta = cube_new_position - glm::vec3(cube_old_position.x, cube_old_position.y, cube_old_position.z);
    (*shapes)[cube_indx]->MyTranslate(loaction_delta, 0);
    (*shapes)[cube_indx]->SetRotateInPlace(glm::mat4(1));
    (*shapes)[cube_indx]->SetRotateNotInPlace(glm::mat4(1));

    for (int i = 0; i < number_of_octahedrons; i++)
    {
        if (i < segmentsLocations[config_num].size())
        {
            (*shapes)[i]->Unhide();
            glm::vec3 point_new_position = segmentsLocations[config_num][i];
            glm::vec4 point_old_position = (*shapes)[i]->GetTranslate()[3];
            glm::vec3 loaction_delta = point_new_position - glm::vec3(point_old_position.x, point_old_position.y, point_old_position.z);
            (*shapes)[i]->MyTranslate(loaction_delta, 0);
        }
        else
        {
            (*shapes)[i]->Hide();
        }
    }

    last_point_indx = segmentsLocations[config_num].size() - 1;
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

    int segment_indx = 1;
    for (int i = 4; i < shapes->size() - 2; i += 3) {
        glm::vec4 p1 = (*shapes)[i]->GetTranslate()[3];
        glm::vec4 p2 = (*shapes)[i + 1]->GetTranslate()[3];
        glm::vec4 p3 = (*shapes)[i + 2]->GetTranslate()[3];

        ChangeSegment(segment_indx, p1, p2, p3);
        segment_indx++;
    }
    SetMesh(GetLine());
}


IndexedModel Bezier1D::GetLine()
{
    IndexedModel model;
    int num_of_dots_on_line = (resT - 1) / segmentsNum;

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
            glm::vec4 point = GetPointOnCurve(i, t);

            glm::vec3 vel = GetVelosity(i, t);
            float sqrt_t = sqrt(pow(vel.x, 2) + pow(vel.y, 2));
            glm::vec3 normal = glm::vec3(-vel.y / sqrt_t, vel.x / sqrt_t, 0);

            model.positions.push_back(glm::vec3(point.x, point.y, point.z));
            model.normals.push_back(normal);
            model.colors.push_back(glm::vec3(1.f, 1.f, 1.f));
        }
    }
    return model;
}


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
    //cout << "t: "<<t<<", v: " << a.x <<","<<a.y<<","<<a.z<<endl;
    return a;
}

float Bezier1D::GetT(int position, int direction){
    int num_of_dots_on_line = GetNumOfDotsOnLine();
    float t = (1.0f / (float)num_of_dots_on_line) * (position + 1);
    return fmod(t, 1.0f);
}

int Bezier1D::GetNumOfDotsOnLine(){
    return 10 * ((resT - 1) / segmentsNum);
}

void Bezier1D::MoveCube()
{
    int prev_cube_position = cube_point_position;
    cube_point_position += cube_direction;

    int num_of_dots_on_line = GetNumOfDotsOnLine();
    if ((cube_point_position <= 0 && cube_direction == -1) || 
        (cube_point_position >= 10 * (resT - 1) && cube_direction == 1))
    {
        cube_direction *= -1;
    }
    
    float t = GetT(cube_point_position, cube_direction);
    if (t == 0.0f)
    {
        if (cube_direction == 1)
            cube_point_position++;
        else
            cube_point_position--;
        
        MoveCube();
        return;
    }
    
    float prev_t = GetT(prev_cube_position, cube_direction);

    int cube_segment = cube_point_position / num_of_dots_on_line,
        prev_cube_segment = prev_cube_position / num_of_dots_on_line;

    /*cout << "segment: "<< cube_segment << ", point: " << cube_point_position << 
            ", prev_segment: "<< prev_cube_segment << ", prev_point: " << prev_cube_position << 
            ", direction: "<< cube_direction << endl;*/

    glm::vec4 point = GetPointOnCurve(cube_segment, t);
    glm::vec4 prev_p_t = GetPointOnCurve(prev_cube_segment, prev_t);

    glm::vec3 vel = GetVelosity(cube_segment, t);

    glm::vec3 position(point.x, point.y, point.z);
    glm::vec3 prev_position(prev_p_t.x, prev_p_t.y, prev_p_t.z);

    GetCube()->MyTranslate(position - prev_position,1);
    
    float rotation_angle = glm::degrees(glm::atan((float)vel.y / (float)vel.x));
    float relative_rotation_angle = rotation_angle - cube_prev_angle;
    cube_prev_angle = rotation_angle;
    //cout << "alpha: " << relative_rotation_angle << endl;
    GetCube()->RotateRelative(relative_rotation_angle, position, glm::vec3(0.0f, 0.0f, 1.0f), 0);
}

void Bezier1D::ResetCubePosition(){
    
    glm::vec4 point = GetPointOnCurve(0, 0);
    float prev_t = GetT(cube_point_position, 1);
    int prev_cube_segment = cube_point_position / GetNumOfDotsOnLine();
    glm::vec4 prev_p_t = GetPointOnCurve(prev_cube_segment, prev_t);


    glm::vec3 position(point.x, point.y, point.z);
    glm::vec3 prev_position(prev_p_t.x, prev_p_t.y, prev_p_t.z);


    GetCube()->MyTranslate(position - prev_position,1);

    glm::vec3 vel = GetVelosity(0, 0);
    float rotation_angle = glm::degrees(glm::atan((float)vel.y / (float)vel.x));
    float relative_rotation_angle = rotation_angle - cube_prev_angle;
    cube_prev_angle = rotation_angle;
    GetCube()->RotateRelative(relative_rotation_angle, position, glm::vec3(0.0f, 0.0f, 1.0f), 0);

    cube_point_position = 0;
    cube_direction = 1;
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

void Bezier1D::ChangeSegment(int segment_indx, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments[segment_indx - 1][3];
    segments[segment_indx] = glm::mat4(p0, p1, p2, p3);
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
    if (pointIndx == 0)
    {
        (*shapes)[cube_indx]->MyTranslate(glm::vec3(dx, dy, 0.0f), 0);
    }

    int segment = 0;
    if (pointIndx > 3)
    {
        segment = 1 + ((pointIndx - 4) / 3);
    }
    
    int indx_in_segment = pointIndx;
    if (segment != 0)
    {
        indx_in_segment = pointIndx - ((3 * segment) + 1) + 1;
    }
    if (preserveC1 && indx_in_segment == 2)
    {
        glm::vec4 p2_position = (*shapes)[pointIndx]->GetTranslate()[3];
        glm::vec4 p3_position = (*shapes)[pointIndx + 1]->GetTranslate()[3];
        glm::vec3 p3_pos(p3_position.x, p3_position.y, p3_position.z);
        glm::vec3 p2_pos(p2_position.x, p2_position.y, p2_position.z);

        glm::vec3 movement_vec = (p3_pos - p2_pos) * ((dx - dy) / 2);
        
        glm::vec3 dest = p2_pos + movement_vec;

        if (glm::length(p3_pos - dest) > 0.5f)
        {
            (*shapes)[pointIndx]->MyTranslate(movement_vec, 0);
        }
        else{
            (*shapes)[pointIndx]->MyTranslate((dest - movement_vec) - p2_pos, 0);
        }
    }
    else if (preserveC1 && indx_in_segment == 1)
    {
        glm::vec4 p1_position = (*shapes)[pointIndx]->GetTranslate()[3];
        glm::vec4 p0_position = (*shapes)[pointIndx - 1]->GetTranslate()[3];
        glm::vec3 p0_pos(p0_position.x, p0_position.y, p0_position.z);
        glm::vec3 p1_pos(p1_position.x, p1_position.y, p1_position.z);

        glm::vec3 movement_vec = (p0_pos - p1_pos) * ((dx - dy) / 2);
        
        glm::vec3 dest = p1_pos + movement_vec;

        if (glm::length(p0_pos - dest) > 0.5f)
        {
            (*shapes)[pointIndx]->MyTranslate(movement_vec, 0);
        }
        else{
            (*shapes)[pointIndx]->MyTranslate((dest - movement_vec) - p1_pos, 0);
        }
    }
    else
    {
        (*shapes)[pointIndx]->MyTranslate(glm::vec3(dx, dy, 0.0f), 0);
    }
    

    UpdateCurveByShapes();
}


void Bezier1D::CurveUpdateRotation(int pointIndx, float dx, float dy, bool preserveC1)
{
    int segment = 0;
    if (pointIndx > 3)
    {
        segment = 1 + ((pointIndx - 4) / 3);
    }
    
    int indx_in_segment = pointIndx;
    if (segment != 0)
    {
        indx_in_segment = pointIndx - ((3 * segment) + 1) + 1;
    }

    if (indx_in_segment == 3 && segment < segmentsNum - 1)
    {
        glm::vec4 p0_position = (*shapes)[pointIndx]->GetTranslate()[3];
        glm::vec4 p1_position = (*shapes)[pointIndx + 1]->GetTranslate()[3];
        glm::vec4 p2_prev_seg_position = (*shapes)[pointIndx - 1]->GetTranslate()[3];

        glm::vec3 p0_pos(p0_position.x, p0_position.y, p0_position.z);
        glm::vec3 p1_pos(p1_position.x, p1_position.y, p1_position.z);
        glm::vec3 p2_prev_seg_pos(p2_prev_seg_position.x, p2_prev_seg_position.y, p2_prev_seg_position.z);

        glm::vec3 movement_vec = p0_pos - p1_pos;
        
        float distance = -1 * glm::length(p0_pos - p2_prev_seg_pos);
        float t = distance / glm::length(movement_vec);


        glm::vec3 dest((1 - t) * p0_pos.x + t * p1_pos.x,
                       (1 - t) * p0_pos.y + t * p1_pos.y,
                       (1 - t) * p0_pos.z + t * p1_pos.z);
        
        movement_vec = dest - p2_prev_seg_pos;

        (*shapes)[pointIndx - 1]->MyTranslate(movement_vec, 0);
    }
    
    if (indx_in_segment == 2)
    {
        glm::vec4 p2_position = (*shapes)[pointIndx]->GetTranslate()[3];
        glm::vec4 p3_position = (*shapes)[pointIndx + 1]->GetTranslate()[3];
        glm::vec3 p3_pos(p3_position.x, p3_position.y, p3_position.z);
        glm::vec3 p2_pos(p2_position.x, p2_position.y, p2_position.z);

        float angle = 1.0f;
        if (dx >= 0)
            angle *= -1.0f;
        

        (*shapes)[pointIndx]->RotateRelative(angle, p3_pos, glm::vec3(0.0f, 0.0f, 1.0f),1);
        if (preserveC1 && segment < segmentsNum - 1)
            (*shapes)[pointIndx + 2]->RotateRelative(angle, p3_pos, glm::vec3(0.0f, 0.0f, 1.0f),1);
    }
    else if (indx_in_segment == 1)
    {
        glm::vec4 p1_position = (*shapes)[pointIndx]->GetTranslate()[3];
        glm::vec4 p0_position = (*shapes)[pointIndx - 1]->GetTranslate()[3];
        glm::vec3 p0_pos(p0_position.x, p0_position.y, p0_position.z);
        glm::vec3 p1_pos(p1_position.x, p1_position.y, p1_position.z);

        float angle = 1.0f;
        if (dx >= 0)
            angle *= -1.0f;
        

        (*shapes)[pointIndx]->RotateRelative(angle, p0_pos, glm::vec3(0.0f, 0.0f, 1.0f),1);
        if (preserveC1 && segment > 0)
            (*shapes)[pointIndx - 2]->RotateRelative(angle, p0_pos, glm::vec3(0.0f, 0.0f, 1.0f),1);
    }
    

    UpdateCurveByShapes();
}


void Bezier1D::ResetCurve(int segNum)
{
    for (int i = 0; i < shapes->size(); i++)
    {
        (*shapes)[i]->ResetTrans();
    }
    UpdateSegmentsNumber(segNum);
    
}

Bezier1D::~Bezier1D(void)
{
}