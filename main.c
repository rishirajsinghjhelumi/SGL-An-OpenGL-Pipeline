#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<iostream>
#include<stack>

#include<GL/glx.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#define PI M_PI
#define DEG2RAD(deg) (deg * PI / 180)
#define SGL_TRIANGLES 1
#define CLEAR_COLOR 2
#define CURRENT_COLOR 3
#define END_SGL_TRIANGLES 4
#define VERTEX 5
#define MAX 1<<20
#define MAX2 1<<15

using namespace std;

int screen_width;
int screen_height;
float windowWidth;
float windowHeight;
float xmin,xmax,ymin,ymax;
float WHITE[] = {1,1,1};

#include "matrix.h"
#include "vector.h"

Vector3d currentColor;

Vector3d clearColor;
Vector3d currentCoordinates;
Vector3d rotation;

Matrix modelMatrix;
Matrix viewMatrix;
Matrix projectionMatrix;
Matrix viewportMatrix;
Matrix coordinateMatrix;



stack <Matrix> modelStack;

class Ply
{
public:
    Vector3d V;
    Vector3d C;
    int type;
};
Ply points[MAX];
int point_count = 0;

class Model
{
public:
    int num_triangles;
    int num_vertices;
    Vector3d vertice[MAX2];
    Vector3d color[MAX2];
    int triangle[MAX2][3];
}porsche;

#include "ply.h"
#include "sgl.h"


void reshape_window(int w, int h)
{
    windowWidth = w;
    windowHeight = h;

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    glOrtho(-windowWidth/2,windowWidth/2,-windowHeight/2,windowHeight/2,-1,1);

    sglViewport(0, 0, w, h);
    sglProjPerspective(45, (float)w / (float)h, 1.0, 200.0);
}

void initRendering()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_BLEND_COLOR);

    glEnable(GL_SHADE_MODEL);
    glShadeModel(GL_SMOOTH);

    /*glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, WHITE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);*/

    glEnable(GL_NORMALIZE);

    glEnable(GL_CULL_FACE);
    glCullFace ( GL_BACK );

    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.f,-1.f);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glPointSize(8);
    glLineWidth(2);
    glEdgeFlag(1);

    modelMatrix.identity();
    viewMatrix.identity();
    projectionMatrix.identity();
    viewportMatrix.identity();

    porsche = load_ply("beethoven.ply",porsche);

}

void draw_scene()
{
    point_count = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sglClear(119.0/255,136.0/255,153.0/255);

    sglModLoadIdentity();
    sglViewLoadIdentity();
    sglLookAt(0,0,-20,0,0,0,0,1,0);

    sglModPushMatrix();
    sglModTranslate(0,0,-5);

    sglModPushMatrix();

    /*sglModScale(2,2,2);
    sglModRotate(30,0,0,1);


    sglViewRotate(-30,0,0,1);
    sglViewTranslate(2,0,0);

    sglBegin(SGL_TRIANGLES);
    sglColor(1,0,0);
    sglVertex(0,1,0);
    sglColor(0,1,0);
    sglVertex(-1,0,0);
    sglColor(0,0,1);
    sglVertex(1,0,0);

    sglVertex(-2,2,0);
    sglColor(1,1,0);
    sglVertex(-3,0,0);
    sglColor(1,0,1);
    sglVertex(-1,0,0);

    sglEnd();*/
    sglModRotate(rotation.x,1,0,0);
    sglModRotate(rotation.y,0,1,0);
    sglModRotate(rotation.z,0,0,1);
    display_ply_model(porsche);

    sglModPopMatrix();
    sglModPopMatrix();

    sglShow();

    glFlush();
    glutSwapBuffers();
}

void handle_keyboard_keys(unsigned char key,int x,int y)
{
    if(key == 27)
    {
        save_ply();
        exit(0);
    }
    if(key == 'a')
    {
        rotation.x -= 5.0;
    }
    if(key == 'd')
    {
        rotation.x += 5.0;
    }
    if(key == 'w')
    {
        rotation.y += 5.0;
    }
    if(key == 's')
    {
        rotation.y -= 5.0;
    }
    if(key == 'r')
    {
        rotation.z += 5.0;
    }
    if(key == 't')
    {
        rotation.z -= 5.0;
    }

    point_count = 0;
    glutPostRedisplay();
}


int main(int argc ,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    screen_width = glutGet(GLUT_SCREEN_WIDTH);
    screen_height = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(screen_width,screen_height);
    glutCreateWindow("Graphics.... :)");

    initRendering();
    glutReshapeFunc(reshape_window);
    glutDisplayFunc(draw_scene);
    glutKeyboardFunc(handle_keyboard_keys);
    //glutSpecialFunc(handle_special_keyboard_keys);
    //glutSpecialUpFunc(release_keyboard_keys);
    //glutMouseFunc(click_action);
    //glutMotionFunc(click_hold_action);
    //glutPassiveMotionFunc(mouse_action);
    glutMainLoop();
    return 0;
}
