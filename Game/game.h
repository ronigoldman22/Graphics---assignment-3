//game!
#pragma once
#include "scene.h"
#include <vector>
using namespace std;

class Game : public Scene
{

public:
	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
//	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);
	void rotateRightWall();
	void rotateLeftWall();
	void rotateUpWall();
	void rotateDownWall();
	void rotateBackWall();
	void rotateFrontWall();
	void spacePressed();
	void ZPressed();
	void APressed();
	void updateCubesData();
	bool lockUp = false;
	bool lockDown = false;
	bool lockLeft = false;
	bool lockRight = false;
	bool lockFront = false;
	bool lockBack = false;
private: //added:
	vector<vector<vector<int>>> cubesData; //9 trios of a vectors holding indices
	vector<vector<vector<pair<int, glm::vec3>>>> index2cordinates; //pairs of the index and the 3 cordinates i,j,k
	float angleOfRotate = 90; 
	glm::vec3 curCenter = glm::vec3(1, 1, 1);
	float rotDirection = 1.0f;
};
