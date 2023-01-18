#pragma once
#include <vector>
using namespace std;

vector<vector<glm::vec3>> getSegmentsLocations(){

    vector<vector<glm::vec3>> segnmentsLocations;
    vector<glm::vec3> locations2Segs;
    locations2Segs.resize(7);
    locations2Segs[0] = glm::vec3(-9, -9, 0);
    locations2Segs[1] = glm::vec3(-9, -3, 0);
    locations2Segs[2] = glm::vec3(-6, 0, 0);
    locations2Segs[3] = glm::vec3(0, 0, 0);
    locations2Segs[4] = glm::vec3(6, 0, 0);
    locations2Segs[5] = glm::vec3(9, -3, 0);
    locations2Segs[6] = glm::vec3(9, -9, 0);

    vector<glm::vec3> locations3Segs;
    locations3Segs.resize(10);
    locations3Segs[0] = glm::vec3(-15, -9, 0);
    locations3Segs[1] = glm::vec3(-15, -3, 0);
    locations3Segs[2] = glm::vec3(-12, 0, 0);
    locations3Segs[3] = glm::vec3(-6, 0, 0);
    locations3Segs[4] = glm::vec3(-3, 0, 0);
    locations3Segs[5] = glm::vec3(3, 0, 0);
    locations3Segs[6] = glm::vec3(6, 0, 0);
    locations3Segs[7] = glm::vec3(12, 0, 0);
    locations3Segs[8] = glm::vec3(15, -3, 0);
    locations3Segs[9] = glm::vec3(15, -9, 0);

    vector<glm::vec3> locations4Segs;
    locations4Segs.resize(13);
    locations4Segs[0] = glm::vec3(-21, -9, 0);
    locations4Segs[1] = glm::vec3(-21, -3, 0);
    locations4Segs[2] = glm::vec3(-18, 0, 0);
    locations4Segs[3] = glm::vec3(-12, 0, 0);
    locations4Segs[4] = glm::vec3(-9, 0, 0);
    locations4Segs[5] = glm::vec3(-3, 0, 0);
    locations4Segs[6] = glm::vec3(0, 0, 0);
    locations4Segs[7] = glm::vec3(3, 0, 0);
    locations4Segs[8] = glm::vec3(9, 0, 0);
    locations4Segs[9] = glm::vec3(12, 0, 0);
    locations4Segs[10] = glm::vec3(18, 0, 0);
    locations4Segs[11] = glm::vec3(21, -3, 0);
    locations4Segs[12] = glm::vec3(21, -9, 0);

    vector<glm::vec3> locations5Segs;
    locations5Segs.resize(16);
    locations5Segs[0] = glm::vec3(-27, -9, 0);
    locations5Segs[1] = glm::vec3(-27, -3, 0);
    locations5Segs[2] = glm::vec3(-24, 0, 0);
    locations5Segs[3] = glm::vec3(-18, 0, 0);
    locations5Segs[4] = glm::vec3(-15, 0, 0);
    locations5Segs[5] = glm::vec3(-9, 0, 0);
    locations5Segs[6] = glm::vec3(-6, 0, 0);
    locations5Segs[7] = glm::vec3(-3, 0, 0);
    locations5Segs[8] = glm::vec3(3, 0, 0);
    locations5Segs[9] = glm::vec3(6, 0, 0);
    locations5Segs[10] = glm::vec3(9, 0, 0);
    locations5Segs[11] = glm::vec3(15, 0, 0);
    locations5Segs[12] = glm::vec3(18, 0, 0);
    locations5Segs[13] = glm::vec3(24, 0, 0);
    locations5Segs[14] = glm::vec3(27, -3, 0);
    locations5Segs[15] = glm::vec3(27, -9, 0);

    vector<glm::vec3> locations6Segs;
    locations6Segs.resize(19);
    locations6Segs[0] = glm::vec3(-33, -9, 0);
    locations6Segs[1] = glm::vec3(-33, -3, 0);
    locations6Segs[2] = glm::vec3(-30, 0, 0);

    locations6Segs[3] = glm::vec3(-24, 0, 0);
    locations6Segs[4] = glm::vec3(-21, 0, 0);
    locations6Segs[5] = glm::vec3(-15, 0, 0);

    locations6Segs[6] = glm::vec3(-12, 0, 0);
    locations6Segs[7] = glm::vec3(-9, 0, 0);
    locations6Segs[8] = glm::vec3(-3, 0, 0);
    locations6Segs[9] = glm::vec3(0, 0, 0);
    locations6Segs[10] = glm::vec3(3, 0, 0);
    locations6Segs[11] = glm::vec3(9, 0, 0);
    locations6Segs[12] = glm::vec3(12, 0, 0);

    locations6Segs[13] = glm::vec3(15, 0, 0);
    locations6Segs[14] = glm::vec3(21, 0, 0);
    locations6Segs[15] = glm::vec3(24, 0, 0);

    locations6Segs[16] = glm::vec3(30, 0, 0);
    locations6Segs[17] = glm::vec3(33, -3, 0);
    locations6Segs[18] = glm::vec3(33, -9, 0);

    segnmentsLocations.push_back(locations2Segs);
    segnmentsLocations.push_back(locations3Segs);
    segnmentsLocations.push_back(locations4Segs);
    segnmentsLocations.push_back(locations5Segs);
    segnmentsLocations.push_back(locations6Segs);
    return segnmentsLocations;
}