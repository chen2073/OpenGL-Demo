#include "GL\freeglut.h"
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <Commdlg.h>

#define PI 3.14159
#define	DISPLAY_NORMALS false

using namespace std;

typedef struct GLObject
{
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLuint> buf_faces;
	std::vector<GLfloat> buf_colors;
} GLObject;
GLObject objModel;

enum keys { KEY_W, KEY_A, KEY_S, KEY_D, KEY_J, KEY_L, KEY_R, KEY_MAX };
bool keyPressed[KEY_MAX] = { false };

bool teapot = true;

int screenWidth = 650;
int screenHeight = 450;

bool selectLight = true;  
GLfloat light0Pos[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat light1Pos[] = { -1.0f, 0.0f, 0.0f, 1.0f };
GLfloat light2Pos[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat light3Pos[] = { 0.0f, -1.0f, 0.0f, 1.0f };

GLfloat cameraPos_x = 0;
GLfloat cameraPos_y = 0;
GLfloat cameraPos_z = 4.5;

GLfloat localRotateAngleX = 0;  
GLfloat localRotateAngleY = 0;  
GLfloat localRotateAngleZ = 0;  
GLfloat globalRotateVal = 1;  
GLfloat objPos_x = -0.5;
GLfloat objPos_y = 0.5;
GLfloat objPos_z = -0.5;

bool cleared = false;
bool showAxes = false;
bool autoRotate = false;
bool globalRotate = false;
bool localRotate = false;
int localRotateAxes = 0;     
int globalRotateAxes = 0;    
int shadeModelOption = 1;    

string objfilename = "cat-v2.poly";

void look(void);
void displayLoop(void);
void resizeScreen(int width, int height);
void pressNormalKey(unsigned char key, int x, int y);
void releaseNormalKey(unsigned char key, int x, int y);
void generateNormal(std::vector<GLfloat>* vecFaceNormals, GLuint f1, GLuint f2, GLuint f3);
vector<GLfloat> averageNormals(std::vector<GLfloat> vecTempNormals);
void loadModel(string fName);
void menu(int item);
void createMenu(void);
void redraw(int);
void LoadFilePath(char *_szFilePath, int _nFilePathSize);

int main(int argc, char* argv[]) {
	
	loadModel(objfilename);

	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_MULTISAMPLE);
	//glEnable(GL_MULTISAMPLE);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glViewport(0, 0, screenWidth, screenHeight);
	glutCreateWindow("Assignment 2");

	glutDisplayFunc(displayLoop);
	glutReshapeFunc(resizeScreen);

	glutTimerFunc(1000 / 60, redraw, 0);

	createMenu();
	
	glutKeyboardFunc(pressNormalKey);
	glutKeyboardUpFunc(releaseNormalKey);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHT0);
	GLfloat amb_light0[] = { 0.2f, 0.2f, 0.2f, 1 };
	GLfloat diff_light0[] = { 0.7f, 0.7f, 0.7f, 1 };
	GLfloat specular_light0[] = { 0.7f, 0.7f, 0.0f, 1 };
	GLfloat angle_light0[] = { 180 };
	GLfloat  atten_light0[] = { 0.8f };
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb_light0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff_light0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light0);
	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, angle_light0);
	glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, atten_light0);

	//glEnable(GL_LIGHT1);
	GLfloat amb_light1[] = { 0.2f, 0.2f, 0.2f, 1 };
	GLfloat diff_light1[] = { 0.75f, 0.0f, 0.1f, 1 };
	GLfloat specular_light1[] = { 0.7f, 0.0f, 0.7f, 1 };
	GLfloat angle_light1[] = { 180 };
	GLfloat atten_light1[] = { 0.8f };
	glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb_light1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff_light1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, angle_light1);
	glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, atten_light1);

	//glEnable(GL_LIGHT2);
	GLfloat amb_light2[] = { 0.2f, 0.2f, 0.2f, 1 };
	GLfloat diff_light2[] = { 0.0f, 0.8f, 0.1f, 1 };
	GLfloat specular_light2[] = { 0.0f, 0.7f, 0.7f, 1 };
	GLfloat angle_light2[] = { 180 };
	GLfloat atten_light2[] = { 0.8f };
	glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);
	glLightfv(GL_LIGHT2, GL_AMBIENT, amb_light2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diff_light2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular_light2);
	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, angle_light2);
	glLightfv(GL_LIGHT2, GL_LINEAR_ATTENUATION, atten_light2);

	//glEnable(GL_LIGHT3);
	GLfloat amb_light3[] = { 0.2f, 0.2f, 0.2f, 1 };
	GLfloat diff_light3[] = { 0.0f, 0.0f, 0.95f, 1 };
	GLfloat specular_light3[] = { 0.7f, 0.7f, 0.7f, 1 };
	GLfloat angle_light3[] = { 180 };
	GLfloat atten_light3[] = { 0.8f };
	glLightfv(GL_LIGHT3, GL_POSITION, light3Pos);
	glLightfv(GL_LIGHT3, GL_AMBIENT, amb_light3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diff_light3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular_light3);
	glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, angle_light3);
	glLightfv(GL_LIGHT3, GL_LINEAR_ATTENUATION, atten_light3);

	glutMainLoop();
	return 0;
}


void look() {

	GLfloat aspect = (GLfloat)screenWidth / (screenHeight > 0 ? screenHeight : 1);
	gluPerspective(45, aspect, 0.1f, 1000.0f);

	GLfloat dx = 0;
	GLfloat dy = 0;
	GLfloat dz = -1;
	/*GLfloat dx = cos(0) * sin(m_yaw);
	GLfloat dy = sin(0);
	GLfloat dz = cos(0) * cos(m_yaw);*/

	GLfloat moveAmount = 0.1f;
	if (keyPressed[KEY_J]) {
		if (selectLight) {
			light0Pos[0] -= moveAmount;
			glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
		}
		else {
			light1Pos[0] -= moveAmount;
			glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
		}
	}
	if (keyPressed[KEY_L]) {
		if (selectLight) {
			light0Pos[0] += moveAmount;
			glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
		}
		else {
			light1Pos[0] += moveAmount;
			glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
		}
	}

	if (keyPressed[KEY_R]) {
		//rotateAngle += 10;
		//if (rotateAngle == 360) rotateAngle = 0;
	}
	
	if (keyPressed[KEY_W]) {
		cameraPos_x += dx * 0.1f;
		cameraPos_y += dy * 0.1f;
		cameraPos_z += dz * 0.1f;
	}
	if (keyPressed[KEY_S]) {
		cameraPos_x -= dx * 0.1f;
		cameraPos_y -= dy * 0.1f;
		cameraPos_z -= dz * 0.1f;
	}
	if (keyPressed[KEY_A]) {
		cameraPos_x += (GLfloat)cos(PI) * 0.1f;
		cameraPos_z += (GLfloat)-sin(PI) * 0.1f;
	}
	if (keyPressed[KEY_D]) {
		cameraPos_x -= (GLfloat)cos(PI) * 0.1f;
		cameraPos_z -= (GLfloat)-sin(PI) * 0.1f;
	}

	gluLookAt(cameraPos_x, cameraPos_y, cameraPos_z, cameraPos_x + dx, cameraPos_y + dy, cameraPos_z + dz, 0, 1, 0);
}


void displayLoop(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	look();		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!cleared) {


		if (showAxes) {
			glPushMatrix();
			glBegin(GL_LINES);
			glColor3f(1.0, 0.0, 0.0);  
			glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.5f, 0.0, 0.0); 
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, -0.5f, 0.0); 
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 0.0, -0.5f); 
			glEnd();
			glPopMatrix();
		}

		glEnable(GL_LIGHTING);

		glPushMatrix();


		if (autoRotate && globalRotate) {
			if (globalRotateAxes == 0) {
				GLfloat alpha = (GLfloat)PI / 180 * globalRotateVal;
				objPos_y = objPos_y * cos(alpha) - objPos_z * sin(alpha);
				objPos_z = objPos_z * cos(alpha) + objPos_y * sin(alpha);
			}
			else if (globalRotateAxes == 1) {
				GLfloat alpha = (GLfloat)PI / 180 * globalRotateVal;
				objPos_x = objPos_x * cos(alpha) - objPos_z * sin(alpha);
				objPos_z = objPos_z * cos(alpha) + objPos_x * sin(alpha);
			}
			else if (globalRotateAxes == 2) {
				GLfloat alpha = (GLfloat)PI / 180 * globalRotateVal;
				objPos_x = objPos_x * cos(alpha) - objPos_y * sin(alpha);
				objPos_y = objPos_y * cos(alpha) + objPos_x * sin(alpha);
			}
		}
		glTranslatef(objPos_x, objPos_y, objPos_z);

		if (autoRotate && localRotate) {
			if (localRotateAxes == 0) {
				localRotateAngleX += 1;
				if (localRotateAngleX == 360) localRotateAngleX = 0;
			}
			else if (localRotateAxes == 1) {
				localRotateAngleY += 1;
				if (localRotateAngleY == 360) localRotateAngleY = 0;
			}
			else if (localRotateAxes == 2) {
				localRotateAngleZ += 1;
				if (localRotateAngleZ == 360) localRotateAngleZ = 0;
			}
		}
		glRotatef(localRotateAngleX, 1.0f, 0.0f, 0.0f);
		glRotatef(localRotateAngleY, 0.0f, 1.0f, 0.0f);
		glRotatef(localRotateAngleZ, 0.0f, 0.0f, 1.0f);

		GLfloat color[] = { 1.0f, 0.67f, 0.098f, 1.0f };	
		GLfloat colDif[] = { 0.1f, 0.067f, 0.0098f, 1.0f }; 
		GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };	
		GLfloat shininess[] = { 30 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		glMaterialfv(GL_FRONT, GL_AMBIENT, colDif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);


		if (teapot)
		{
			glutSolidTeapot(0.5);
		}
		else
		{
			glVertexPointer(3, GL_FLOAT, 0, &objModel.vertices[0]);
			glNormalPointer(GL_FLOAT, 0, &objModel.normals[0]);
			glDrawElements(GL_TRIANGLES, (GLsizei)objModel.buf_faces.size(), GL_UNSIGNED_INT, &objModel.buf_faces[0]);

			if (DISPLAY_NORMALS) {
				for (size_t i = 0; i < objModel.vertices.size() / 3; i++) {
					GLfloat v1 = objModel.vertices[i * 3] + objModel.normals[i * 3];
					GLfloat v2 = objModel.vertices[i * 3 + 1] + objModel.normals[i * 3 + 1];
					GLfloat v3 = objModel.vertices[i * 3 + 2] + objModel.normals[i * 3 + 2];

					glBegin(GL_LINES);
					glVertex3f(objModel.vertices[i * 3], objModel.vertices[i * 3 + 1], objModel.vertices[i * 3 + 2]);
					glVertex3f(v1, v2, v3);
					glEnd();
				}
			}
		}

		glPopMatrix();
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//gluOrtho2D(0, screenWidth, screenHeight, 0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glutSwapBuffers();
}

void resizeScreen(int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
}

void pressNormalKey(unsigned char key, int x, int y) {
	switch (key) {
		
	case 27:
		exit(1);
		break;

	case 'w':
		keyPressed[KEY_W] = true;
		break;
	case 'a':
		keyPressed[KEY_A] = true;
		break;
	case 's':
		keyPressed[KEY_S] = true;
		break;
	case 'd':
		keyPressed[KEY_D] = true;
		break;


	case '0':
		selectLight = true;
		break;
	case '1':
		selectLight = false;
		break;

	case 'r':
		keyPressed[KEY_R] = true;
		break;

	case 'j':
		keyPressed[KEY_J] = true;
		break;
	case 'l':
		keyPressed[KEY_L] = true;
		break;
	}
}


void releaseNormalKey(unsigned char key, int x, int y) {
	switch (key) {

	case 'w':
		keyPressed[KEY_W] = false;
		break;
	case 'a':
		keyPressed[KEY_A] = false;
		break;
	case 's':
		keyPressed[KEY_S] = false;
		break;
	case 'd':
		keyPressed[KEY_D] = false;
		break;

	case 'r':
		keyPressed[KEY_R] = false;
		break;

	case 'j':
		keyPressed[KEY_J] = false;
		break;
	case 'l':
		keyPressed[KEY_L] = false;
		break;
	}
}


void generateNormal(std::vector<GLfloat>* vecFaceNormals, GLuint f1, GLuint f2, GLuint f3) {

	GLfloat v1a = objModel.vertices[f1 * 3];
	GLfloat v1b = objModel.vertices[f1 * 3 + 1];
	GLfloat v1c = objModel.vertices[f1 * 3 + 2];

	GLfloat v2a = objModel.vertices[f2 * 3];
	GLfloat v2b = objModel.vertices[f2 * 3 + 1];
	GLfloat v2c = objModel.vertices[f2 * 3 + 2];

	GLfloat v3a = objModel.vertices[f3 * 3];
	GLfloat v3b = objModel.vertices[f3 * 3 + 1];
	GLfloat v3c = objModel.vertices[f3 * 3 + 2];

	GLfloat a = v2a - v1a;
	GLfloat b = v2b - v1b;
	GLfloat c = v2c - v1c;

	GLfloat x = v3a - v1a;
	GLfloat y = v3b - v1b;
	GLfloat z = v3c - v1c;

	GLdouble v1 = b * z - c * y;
	GLdouble v2 = c * x - a * z;
	GLdouble v3 = a * y - b * x;

	double len = sqrt(v1 * v1 + v2 * v2 + v3 * v3);
	v1 = v1 / len;
	v2 = v2 / len;
	v3 = v3 / len;

	vecFaceNormals->push_back((GLfloat)v1);
	vecFaceNormals->push_back((GLfloat)v2);
	vecFaceNormals->push_back((GLfloat)v3);
}

std::vector<GLfloat> averageNormals(std::vector<GLfloat> vecTempNormals) {
	std::vector<GLfloat> vecNormal;

	GLfloat avgX = 0.0f;
	GLfloat avgY = 0.0f;
	GLfloat avgZ = 0.0f;

	size_t numNormals = vecTempNormals.size() / 3;
	for (size_t i = 0; i < numNormals; i++) {
		avgX += vecTempNormals[i * 3];
		avgY += vecTempNormals[i * 3 + 1];
		avgZ += vecTempNormals[i * 3 + 2];
	}

	GLdouble v1 = avgX / numNormals;
	GLdouble v2 = avgY / numNormals;
	GLdouble v3 = avgZ / numNormals;

	double len = sqrt(v1 * v1 + v2 * v2 + v3 * v3);
	v1 = v1 / len;
	v2 = v2 / len;
	v3 = v3 / len;

	vecNormal.push_back((GLfloat)v1);
	vecNormal.push_back((GLfloat)v2);
	vecNormal.push_back((GLfloat)v3);

	return vecNormal;
}

void loadModel(string fName) {
	objModel = GLObject();

	std::ifstream objfile(fName);
	if (objfile.is_open()) {
		std::cout << "Loading obj model from " << fName << "... \nPlease wait..." << endl;
		string line;
		vector<GLfloat> vfaceNormals;

		int cnt = 0;
		int vertices = 0;


		while (getline(objfile, line))
		{
			stringstream ss(line);


			string val;
			ss >> val;
			if (val.find("//") != -1) continue;

			if (cnt == 0) {
				vertices = stoi(val);
				cnt++;
			}
			else {
				if (cnt <= vertices) {
					GLfloat v2, v3;
					ss >> v2;
					ss >> v3;
					objModel.vertices.push_back(stof(val));
					objModel.vertices.push_back(v2);
					objModel.vertices.push_back(v3);
					cnt++;
				}
				else {
					GLuint f2, f3;
					ss >> f2;
					ss >> f3;

					objModel.buf_faces.push_back(stoi(val) - 1);
					objModel.buf_faces.push_back(f2 - 1);
					objModel.buf_faces.push_back(f3 - 1);
					if (stoi(val) < 1 || f2 < 1 || f2 < 1) continue;
					generateNormal(&vfaceNormals, stoi(val) - 1, f2 - 1, f3 - 1);
				}
			}
		}
		objfile.close();
		std::cout << "Obj model loaded." << std::endl << std::endl;

		string normal_fName = fName.substr(0, fName.find_last_of('.')) + "-normals.obj";

		ifstream normalfile(normal_fName);
		if (normalfile.is_open()) {
			std::cout << "Loading normals from " << normal_fName << "..." << endl;

			int cnt = 0;
			
			while (std::getline(normalfile, line)) {
				std::stringstream ss(line);
				std::string type;
				ss >> type;

				if (type == "n") {
					GLdouble n1, n2, n3;
					ss >> n1;
					ss >> n2;
					ss >> n3;

					double len = sqrt(n1 * n1 + n2 * n2 + n3 * n3);
					n1 = n1 / len;
					n2 = n2 / len;
					n3 = n3 / len;
					objModel.normals.push_back((GLfloat)n1);
					objModel.normals.push_back((GLfloat)n2);
					objModel.normals.push_back((GLfloat)n3);
				}
			}
			std::cout << "Normals loaded." << std::endl << std::endl;
			normalfile.close();
		}
		else {
			
			objModel.normals.resize(objModel.vertices.size());

			size_t numVertices = objModel.vertices.size() / 3;
			std::cout << "Calculating normal vectors for " << numVertices << " vertices." << std::endl;

			for (size_t i = 0; i < numVertices; i++) {
				std::cout << i << "/" << numVertices << std::endl;

				std::vector<GLfloat> vecTempNormals;

				size_t faces = objModel.buf_faces.size();
				for (size_t j = 0; j < faces; j++)
				{

					if (objModel.buf_faces[j] == i) {
						
						size_t face = j / 3;
						vecTempNormals.push_back(vfaceNormals[face * 3]);
						vecTempNormals.push_back(vfaceNormals[face * 3 + 1]);
						vecTempNormals.push_back(vfaceNormals[face * 3 + 2]);
					}
				}

				std::vector<GLfloat> avgNormal = averageNormals(vecTempNormals);

				objModel.normals[i * 3] = avgNormal[0];
				objModel.normals[i * 3 + 1] = avgNormal[1];
				objModel.normals[i * 3 + 2] = avgNormal[2];
			}

			std::ofstream nFile(normal_fName);
			if (nFile.is_open()) {
				for (size_t i = 0; i < objModel.normals.size() / 3; i++) {
					nFile << "n " << objModel.normals[i * 3] << " " << objModel.normals[i * 3 + 1]
						<< " " << objModel.normals[i * 3 + 2] << std::endl;
				}
				nFile.close();
			}
		}
	} 
}

void menu(int item) {
	switch (item) {
	case 11:
		cleared = true;
		break;
	case 12:
		cleared = false;
		break;
	case 13:
		exit(0);
		break;
	case 31:
		if (!showAxes) showAxes = true; else showAxes = false;
		break;
	case 33:
		teapot = true;
		break;
	case 34:
		teapot = false;
		char szFilepath[MAX_PATH];
		szFilepath[0] = '\0';
		LoadFilePath(szFilepath, MAX_PATH);
		objfilename = string(szFilepath);
		if (objfilename == "") {
			break;
		}
		objModel.buf_faces.clear();
		objModel.normals.clear();
		objModel.vertices.clear();
		loadModel(objfilename);
		break;
	case 32:
		if (!autoRotate) autoRotate = true; else autoRotate = false;
		break;
	case 231:
		globalRotate = false; localRotate = true;
		localRotateAxes = 0;
		break;
	case 232:
		globalRotate = false; localRotate = true;
		localRotateAxes = 1;
		break;
	case 233:
		globalRotate = false; localRotate = true;
		localRotateAxes = 2;
		break;
	case 221:
		globalRotate = true; localRotate = false;
		globalRotateAxes = 0;
		break;
	case 222:
		globalRotate = true; localRotate = false;
		globalRotateAxes = 1;
		break;
	case 223:
		globalRotate = true; localRotate = false;
		globalRotateAxes = 2;
		break;
	case 21: 
		//RESET VIEW
		cameraPos_x = 0;
		cameraPos_y = 0;
		cameraPos_z = 4.5;
		localRotateAngleX = 0;  
		localRotateAngleY = 0;  
		localRotateAngleZ = 0;  
		globalRotateVal = 1;    
		autoRotate = false;
		objPos_x = -0.5;
		objPos_y = 0.5;
		objPos_z = -0.5;
		break;
	case 41:
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		break;
	case 42:
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		break;
	case 43:
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		break;
	case 44:
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		break;
	case 46:

		if (shadeModelOption == 1) {
			glShadeModel(GL_FLAT);
			shadeModelOption = 0;
		}
		else {
			glShadeModel(GL_SMOOTH);
			shadeModelOption = 1;
		}
		break;
	default:
		break;
	}
}

void createMenu() {
	int actionMenu = glutCreateMenu(menu);
	glutAddMenuEntry("CLEAR", 11);
	glutAddMenuEntry("REFRESH", 12);
	glutAddMenuEntry("EXIT", 13);

	int displayMenu = glutCreateMenu(menu);
	glutAddMenuEntry("SHOW/HIDE AXES", 31);
	glutAddMenuEntry("AUTO-ROTATION ON/OFF", 32);
	glutAddMenuEntry("SHOW TEAPOT", 33);
	glutAddMenuEntry("SHOW FILE OBJECT(S)", 34);

	int localRotateMenu = glutCreateMenu(menu);
	glutAddMenuEntry("X AXES", 231);
	glutAddMenuEntry("Y AXES", 232);
	glutAddMenuEntry("Z AXES", 233);

	int globalRotateMenu = glutCreateMenu(menu);
	glutAddMenuEntry("X AXES", 221);
	glutAddMenuEntry("Y AXES", 222);
	glutAddMenuEntry("Z AXES", 223);

	int rotateMenu = glutCreateMenu(menu);
	glutAddMenuEntry("RESET VIEW", 21);
	glutAddSubMenu("ROTATE GLOBAL", globalRotateMenu);
	glutAddSubMenu("LOTATE LOCAL", localRotateMenu);

	int lightMenu = glutCreateMenu(menu);
	glutAddMenuEntry("LIGHT 0", 41);
	glutAddMenuEntry("LIGHT 1", 42);
	glutAddMenuEntry("LIGHT 2", 43);
	glutAddMenuEntry("LIGHT 3", 44);
	glutAddMenuEntry("SMOOTH SHADE ON/OFF", 46);

	int popup = glutCreateMenu(menu);
	glutAddSubMenu("OTHER ACTION", actionMenu);
	glutAddSubMenu("ROTATE", rotateMenu);
	glutAddSubMenu("DISPLAY", displayMenu);
	glutAddSubMenu("LIGHTS", lightMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void redraw(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, redraw, 0);
}


void LoadFilePath(char *_szFilePath, int _nFilePathSize)
{
	if (!_szFilePath) return;
	if (_nFilePathSize < MAX_PATH) return;

	OPENFILENAME ofn;
	wchar_t lpstrFile[MAX_PATH] = L"";

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L"poly files(*.poly)\0*.poly\0";
	ofn.lpstrFile = lpstrFile;
	ofn.Flags = OFN_NOCHANGEDIR;
	ofn.nMaxFile = MAX_PATH;
	if (GetOpenFileName(&ofn) != 0) {
		WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, ofn.lpstrFile, -1, _szFilePath, MAX_PATH, NULL, NULL);
	}
	else {
		return;
	}
}

