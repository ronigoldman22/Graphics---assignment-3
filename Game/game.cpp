//game !
#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}

void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	AddTexture("../res/textures/plane.png", false);

	int size = 3;
	float distance = 1;
	int index = 0;

	for (float i = -distance; i <= distance; i++)
	{
		vector<vector<int>> vec2; //added
		vector<vector<pair<int, glm::vec3>>> vec2a; //added
		for (float j = -distance; j <= distance; j++)
		{
			vector<int> vec1; //added
			vector<pair<int, glm::vec3>> vec1a; //added
			for (float k = -distance; k <= distance; k++)
			{
				AddShape(Scene::Cube, -1, Scene::TRIANGLES);
				SetShapeTex(index, 0);
				shapes[index]->MyTranslate(glm::vec3(i, j, k), 0);
				shapes[index]->MyScale(glm::vec3(0.5, 0.5, 0.5));
				vec1.push_back(index); //added
				vec1a.push_back(make_pair(index, glm::vec3(i, j, k))); //added
				index++;
			}
			vec2.push_back(vec1); //added
			vec2a.push_back(vec1a); //added
		}
		cubesData.push_back(vec2); //added
		index2cordinates.push_back(vec2a); //added
	}

	AddShape(Plane,-1,TRIANGLES);
	
	pickedShape = 0;
	
	SetShapeTex(0,0);
	MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}

void Game::rotateRightWall() {
	int i = curCenter.x + 1;
	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			int index = cubesData[i][j][k];
			glm::mat4 rot = shapes[index]->rot;
			glm::mat3 rotTranspose = glm::transpose(glm::mat3(rot));
			shapes[index]->MyRotate(angleOfRotate, rotTranspose * glm::vec3(1, 0, 0) * rotDirection, 0);
		}
	}
	if (angleOfRotate == 45) {
		lockUp = !lockUp;
		lockDown = !lockDown;
		lockBack = !lockBack;
		lockFront = !lockFront;
	}
	updateCubesData(); //update the data structure
}

void Game::rotateLeftWall() {
	int i = curCenter.x - 1;
	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			int index = cubesData[i][j][k];
			glm::mat4 rot = shapes[index]->rot;
			glm::mat3 rotTranspose = glm::transpose(glm::mat3(rot));
			shapes[index]->MyRotate(angleOfRotate, rotTranspose * glm::vec3(1, 0, 0) * rotDirection, 0);
		}
	}
	if (angleOfRotate == 45) {
		lockUp = !lockUp;
		lockDown = !lockDown;
		lockBack = !lockBack;
		lockFront = !lockFront;
	}
	updateCubesData(); //update the data structure
}

void Game::rotateUpWall() {
	int j = curCenter.y + 1;
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			int index = cubesData[i][j][k];
			glm::mat4 rot = shapes[index]->rot;
			glm::mat3 rotTranspose = glm::transpose(glm::mat3(rot));
			shapes[index]->MyRotate(angleOfRotate, rotTranspose * glm::vec3(0, 1, 0) * rotDirection, 0);
		}
	}
	if (angleOfRotate == 45) {
		lockRight = !lockRight;
		lockLeft = !lockLeft;
		lockBack = !lockBack;
		lockFront = !lockFront;
	}
	updateCubesData(); //update the data structure
}

void Game::rotateDownWall() {
	int j = curCenter.y - 1;
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			int index = cubesData[i][j][k];
			glm::mat4 rot = shapes[index]->rot;
			glm::mat3 rotTranspose = glm::transpose(glm::mat3(rot));
			shapes[index]->MyRotate(angleOfRotate, rotTranspose * glm::vec3(0, 1, 0) * rotDirection, 0);
		}
	}
	if (angleOfRotate == 45) {
		lockRight = !lockRight;
		lockLeft = !lockLeft;
		lockBack = !lockBack;
		lockFront = !lockFront;
	}
	updateCubesData(); //update the data structure
}

void Game::rotateBackWall() {
	int k = curCenter.z - 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int index = cubesData[i][j][k];
			glm::mat4 rot = shapes[index]->rot;
			glm::mat3 rotTranspose = glm::transpose(glm::mat3(rot));
			shapes[index]->MyRotate(angleOfRotate, rotTranspose * glm::vec3(0, 0, 1) * rotDirection, 0);
		}
	}
	if (angleOfRotate == 45) {
		lockRight = !lockRight;
		lockLeft = !lockLeft;
		lockUp = !lockUp;
		lockDown = !lockDown;
	}
	updateCubesData(); //update the data structure
}

void Game::rotateFrontWall() {
	int k = curCenter.z + 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int index = cubesData[i][j][k];
			glm::mat4 rot = shapes[index]->rot;
			glm::mat3 rotTranspose = glm::transpose(glm::mat3(rot));
			shapes[index]->MyRotate(angleOfRotate, rotTranspose * glm::vec3(0, 0, 1) * rotDirection, 0);
		}
	}
	if (angleOfRotate == 45) {
		lockRight = !lockRight;
		lockLeft = !lockLeft;
		lockUp = !lockUp;
		lockDown = !lockDown;
	}
	updateCubesData(); //update the data structure
}

void Game::spacePressed() {
	rotDirection = rotDirection * (-1.0f);
}

void Game::ZPressed() {
	if (angleOfRotate == 45) {
		std::cout << "Z pressed - Invalid action (angle of rotation is now 45 and can't be lower)" << std::endl;
	}
	else {
		angleOfRotate = angleOfRotate / 2;
		std::cout << "angle of rotation is now " << angleOfRotate << std::endl;
	}
}

void Game::APressed() {
	if (angleOfRotate < 180) {
		angleOfRotate = angleOfRotate * 2;
		std::cout << "angle of rotation is now " << angleOfRotate << std::endl;
	}
	else
		cout << "A pressed - Invalid action (angle of rotation can't be more than 180)" << endl;
}

void Game::updateCubesData() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				int index = index2cordinates[i][j][k].first;
				glm::mat4 rot = shapes[index]->rot;
				glm::mat3 rotTranspose = glm::transpose(glm::mat3(rot));
				glm::vec3 curCordinates = index2cordinates[i][j][k].second;
				glm::vec3 newCord = curCordinates * rotTranspose;
				glm::vec3 change = glm::vec3(round(newCord.x - curCordinates.x), round(newCord.y - curCordinates.y), round(newCord.z - curCordinates.z));
				cubesData[i + change.x][j + change.y][k + change.z] = index;
			}
		}
	}
}
