#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include "glut.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")

float angleX = 0;
float angleY = 0;
float angleZ = 0;

long double minX = 0;
long double minY = 0;
long double minZ = 0;

long double maxX = 0;
long double maxY = 0;
long double maxZ = 0;


float cameraDistance = -10;

bool flag = true;
bool osiFlag = true;

int numVetrices;
int numEdges;
int numFaces;


class Vertex
{
public:
	long double x;
	long double y;
	long double z;
};

class Edge
{
public:
	long double index1;
	long double index2;
};

class Face
{
public:
	long double edge1;
	long double edge2;
	long double edge3;
};

class Grid
{
	std::vector <Vertex> vertices; //вершины
	std::vector <Edge> edges;      //ребра
	std::vector <Face> faces;      //стороны
	std::vector <bool> locations;  /*булевый массив(i - поверхность внутри фигуры
												    e - снаружи)*/
public:

	bool LoadData()
	{
		std::string line;

		std::ifstream file("cube.grd");
		if (!file.is_open())
		{
			std::cout << "ERROR: Failed to open file" << std::endl;
			exit(NULL);
			return 0;
		}
		else
		{

			//считывание vetices
			//колличество таких элементов

			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }), line.end());//удал€ем из строки все элементы которые не €вл€ютс€ числом и преобразовываем оставшеес€ число в int
			numVetrices = std::stoi(line);

			for (int i = 0; i < numVetrices; ++i)
			{
				std::getline(file, line);

				std::istringstream sss(line);//дл€ парсинга строки с кучей чисел на отдеьные элементы
				long double x = 0, y = 0, z = 0;
				sss >> x;
				sss.ignore();
				sss >> y;
				sss.ignore();
				sss >> z;
				vertices.push_back({ x, y, z });

			}



			std::getline(file, line);
			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }), line.end());
			numEdges = std::stoi(line);

			for (int i = 0; i < numEdges; ++i)
			{
				std::getline(file, line);

				std::istringstream sss(line);//дл€ парсинга строки с кучей чисел на отдеьные элементы
				long double point1 = 0, point2 = 0;
				sss >> point1;
				sss.ignore();
				sss >> point2;

				edges.push_back({ point1, point2 });


			}


			std::getline(file, line);
			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }), line.end());
			numFaces = std::stoi(line);

			for (int i = 0; i < numFaces; ++i)
			{
				std::getline(file, line);

				std::istringstream sss(line);//дл€ парсинга строки с кучей чисел на отдеьные элементы
				long double edge1 = 0, edge2 = 0, edge3 = 0;
				sss >> edge1;
				sss.ignore();
				sss >> edge2;
				sss.ignore();
				sss >> edge3;

				faces.push_back({ edge1, edge2, edge3 });
			}
		}
		file.close();
	}

	void maxmin() //ф-€ вычислени€ макс и мин элементов по каждой из осей
	{
		minX = vertices[0].x;
		minY = vertices[0].y;
		minZ = vertices[0].z;

		maxX = vertices[0].x;
		maxY = vertices[0].y;
		maxZ = vertices[0].z;

		for (const auto& vertex : vertices) 
		{

			minX = std::min(minX, vertex.x);
			minY = std::min(minY, vertex.y);
			minZ = std::min(minZ, vertex.z);

			maxX = std::max(maxX, vertex.x);
			maxY = std::max(maxY, vertex.y);
			maxZ = std::max(maxZ, vertex.z);
		}

		std::cout << "Min X: " << minX << ", Max X: " << maxX << std::endl;
		std::cout << "Min Y: " << minY << ", Max Y: " << maxY << std::endl;
		std::cout << "Min Z: " << minZ << ", Max Z: " << maxZ << std::endl;

	}


	void Draw() 
	{
		glColor3f(1.0f, 1.0f, 1.0f);

		glLineWidth(4);

		for (int i = 0; i < numEdges; ++i) 
		{
			glBegin(GL_LINES);

			glVertex3f(vertices[edges[i].index1].x - (maxX - abs(minX)) / 2.0, vertices[edges[i].index1].y - (maxY - abs(minY)) / 2.0, vertices[edges[i].index1].z - (maxZ - abs(minZ)) / 2.0);

			glVertex3f(vertices[edges[i].index2].x - (maxX - abs(minX)) / 2.0, vertices[edges[i].index2].y - (maxY - abs(minY)) / 2.0, vertices[edges[i].index2].z - (maxZ - abs(minZ)) / 2.0);


			glEnd();

		}

	}


	void drawTriangles() 
	{

		glColor3f(0.0f, 0.0f, 1.0f);


		for (int i = 0; i < numFaces; ++i) 
		{


			glBegin(GL_TRIANGLES);

			if (faces[i].edge1 < 0) 
			{
				long double a = abs(faces[i].edge1) - 1;

				glVertex3f(vertices[edges[a].index2].x - (maxX - abs(minX)) / 2.0, vertices[edges[a].index2].y - (maxY - abs(minY)) / 2.0, vertices[edges[a].index2].z - (maxZ - abs(minZ)) / 2.0);
			}
			else 
			{
				glVertex3f(vertices[edges[faces[i].edge1].index1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge1].index1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge1].index1].z - (maxZ - abs(minZ)) / 2.0);
			}


			if (faces[i].edge2 < 0) 
			{
				long double b = abs(faces[i].edge2) - 1;
				glVertex3f(vertices[edges[b].index2].x - (maxX - abs(minX)) / 2.0, vertices[edges[b].index2].y - (maxY - abs(minY)) / 2.0, vertices[edges[b].index2].z - (maxZ - abs(minZ)) / 2.0);
			}
			else 
			{
				glVertex3f(vertices[edges[faces[i].edge2].index1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge2].index1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge2].index1].z - (maxZ - abs(minZ)) / 2.0);
			}

			if (faces[i].edge3 < 0) 
			{
				long double c = abs(faces[i].edge3) - 1;

				glVertex3f(vertices[edges[c].index2].x - (maxX - abs(minX)) / 2.0, vertices[edges[c].index2].y - (maxY - abs(minY)) / 2.0, vertices[edges[c].index2].z - (maxZ - abs(minZ)) / 2.0);
			}
			else 
			{
				glVertex3f(vertices[edges[faces[i].edge3].index1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge3].index1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge3].index1].z - (maxZ - abs(minZ)) / 2.0);
			}
			glEnd();
		}
	}
};

void drawAxes()
{
	glLoadIdentity();
	gluLookAt(0, 0, 18.0f, 11.0f, 7.0f, 0, 0, 1, 0);


	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
}

void read_kb(unsigned char key, int, int)
{


	switch (key)
	{
	case 's':
		angleX += 2.0;
		break;
	case 'w':
		angleX -= 2.0;
		break;

	case 'q':
		angleY += 2.0;
		break;
	case 'e':
		angleY -= 2.0;
		break;

	case 'a':
		angleZ += 2.0;
		break;
	case 'd':
		angleZ -= 2.0;
		break;

	case '+':
		cameraDistance++;
		glTranslatef(0, 0, cameraDistance);
		break;
	case '-':
		cameraDistance--;
		glTranslatef(0, 0, cameraDistance);
		break;
	case 'm':
		if (flag == true)
			flag = false;
		else
			flag = true;
		break;
	case '0':
		if (osiFlag == true)
			osiFlag = false;
		else
			osiFlag = true;
		break;
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();
}


Grid mesh;

void RenderScene() 
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	mesh.LoadData();

	glTranslatef(0, 0, cameraDistance);

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleZ, 0, 0, 1);




	if (flag == true)
		mesh.Draw();
	if (flag == false)
		mesh.drawTriangles();
	
	mesh.Draw();
	if(osiFlag == true)
		drawAxes();

	glutSwapBuffers();
}

void ReshapeWindow(GLsizei width, GLsizei height) 
{
	if (height == 0)
		height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

int main(int argc, char* argv[]) 
{

	mesh.LoadData();
	mesh.maxmin();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("3D");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0, 0, 0, 1);

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReshapeWindow);
	glutKeyboardFunc(read_kb);

	glutMainLoop();

	return 0;
}


