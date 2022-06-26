#include <GL/gl.h>
#include <bits/stdc++.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "include/BmpLoader.h"
#define PI 3.1414926535898
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))
#define grass 1
#define road 2
#define sky 3
#define sun 4
#define rock1 5
#define rock2 6
#define house1 7
#define house2 8
#define house_door 9
using namespace std;

double Txval=0,Tyval=0,Tzval=0;
double windowHeight=700, windowWidth=700;
GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;

static GLfloat eyeX=0.0,eyeY=15.5,eyeZ=50,posX=0,posY=10,posZ=0,rot=0, runX=0,runZ=10;
static int point=0, max_point=0;
/*
GLfloat car_pos_x=0,car_pos_y=2.0,car_pos_z=0.0;
GLfloat car_next_x=0.5,car_next_y=0.5,car_next_z=0.2;
*/
float carX[4] = {-13.8, -9.7, 12.5, 8.2}, carZ[4] = {-2, -18, -30, -40};
float carSpeed[8] = {0.2, 0.3, 0.35, 0.15, 0.25, 0.2, 0.25, 0.3};
float obstLeftX[12], obstRightX[12], obstLeftZ[12], obstRightZ[12];
static bool sun_light=false, head_light= false;
static bool collision = false, hLight = false, light1 = false, light3 = false,  start = false, running = false;
float map_length = 3000;
float map_width = 500;
float mapBoundary;
int counter = 0;
int road_length = 3000;
int road_width = 30;

int car_forward = 0;
int car_backward = 0;
int anglex= 0, angley = 0, anglez = 0;
int window;
int wired=0;
int shcpt=1;
int animat = 0;
const int L=20;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 40;
const int ntheta = 20;

float angle;


bool paused = false;


unsigned int ID[10];

GLfloat ctrlpoints[L+1][3] =
{
    {0.35,-0.425,0},
    {0.975, -0.375,0},
    {2.9,-0.25,0},
    {3.75, -0.175,0},
    {4.6, -0.175,0},
    {5.5, -0.175,0},
    {6.025, -0.2,0},
    {7.175,0 -0.65,0},
    {7.25, -1.25,0},
    {7.325, -2.225,0},
    {7.175, -3.2,0},
    {6.825, -3.425,0},
    {5.725, -3.575,0},
    {5.625, -4,0},
    {5.375, -4.275,0},
    {4.7, -4.3,0},
    {4.475, -3.675,0},
    {4.75, -3.275,0},
    {5.425, -3.3,0},
    {5.925, -3.7,0},
    {6.05, -3.9,0}
};


GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},
    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

GLubyte c_ind[6][4] =
{
    {3,1,5,7},
    {2,0,1,3},
    {7,5,4,6},
    {2,3,7,6},
    {1,0,4,5},
    {6,4,0,2}
};


void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=0.3, float specFactor=1.0);

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void drawCube(float colorRed, float colorGreen, float colorBlue, bool e=false)
{
    GLfloat no_mat[] = {0, 0, 0, 1.0};
    GLfloat mat_emission[] = {colorRed, colorGreen, colorBlue,1};
    GLfloat mat_ambient[] = {colorRed,colorGreen,colorBlue,1};
    GLfloat mat_diffuse[] = {colorRed,colorGreen,colorBlue,1};
    GLfloat mat_specular[] = {1,1,1,1};
    GLfloat mat_shininess[] = {60};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, e?mat_emission:no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);



    glBegin(GL_QUADS);
    for(GLint i = 0; i<6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        glTexCoord2f(1,1);
        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(0,1);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);

    }
    glEnd();
}



GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_ambient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back, float ambFactor, float specFactor)
{
    GLfloat mat_ambient[]    = { kdr*ambFactor, kdg*ambFactor, kdb*ambFactor, 1.0f };
    GLfloat mat_emission[] = {kdr, kdg, kdb,1};
    GLfloat mat_diffuse[]    = { kdr, kdg, kdb, 1.0f };
    GLfloat mat_specular[]   = { 1.0f*specFactor, 1.0f*specFactor, 1.0f*specFactor, 1.0f };
    GLfloat mat_shininess[] = { shiny };

    if(frnt_Back==0)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    }
    else if(frnt_Back==1)
    {
        glMaterialfv(GL_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
    }
    else if(frnt_Back==2)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    }

}

void SolidCube(float size,float colR = 1.0,float colG = 1.0,float colB = 1.0,float alpha = 1.0)
{

    size = size /2.0;

    static GLfloat cube_vertex [8][3] =
    {
        {-size, size, -size},
        {size, size,-size},
        {size,-size, -size},
        {-size, -size, -size},
        {-size, size, size},
        {size, size,size},
        {size,-size, size},
        {-size, -size, size}
    };
    static GLubyte cube_indices[6][4] =
    {
        {0, 1, 5, 4},
        {3, 2, 6, 7},
        {1, 2, 6, 5},
        {0, 3, 7, 4},
        {4, 5, 6, 7},
        {0, 1, 2, 3}
    };

    matColor(colR,colG,colB,60);

    glBegin(GL_QUADS);
    for (int i = 0; i <6; i++)
    {
        //glColor4f(colR,colG,colB,alpha);
        getNormal3p(cube_vertex[cube_indices[i][0]][0], cube_vertex[cube_indices[i][0]][1], cube_vertex[cube_indices[i][0]][2],
                    cube_vertex[cube_indices[i][1]][0], cube_vertex[cube_indices[i][1]][1], cube_vertex[cube_indices[i][1]][2],
                    cube_vertex[cube_indices[i][2]][0], cube_vertex[cube_indices[i][2]][1], cube_vertex[cube_indices[i][2]][2]);
        glVertex3fv(&cube_vertex[cube_indices[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&cube_vertex[cube_indices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&cube_vertex[cube_indices[i][2]][0]);
        glTexCoord2f(0,1);
        glVertex3fv(&cube_vertex[cube_indices[i][3]][0]);
        glTexCoord2f(0,0);
    }
    glEnd();
}

void drawText(string str,float colorRed, float colorGreen, float colorBlue,int width=1, float val=0.5,int col=0)
{
    glPushMatrix();
    glLineWidth(width);


    glScalef(.05f*val,.05f*val,15);

    glPushMatrix();
    glScalef(0,0,0);
    drawCube(colorRed, colorGreen, colorBlue);
    glPopMatrix();

    for (int c=0; c != str.size(); c++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, str[c]);
    }
    glPopMatrix();

}

void light(float colR,float colG, float colB)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_diffuse[]  = { colR,colG, colB, 1.0 };
    GLfloat light_specular[] = { colR, colG, colB, 1.0 };


    /*
        glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv( GL_LIGHT0, GL_POSITION, light_position);

        GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
        glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);*/
}


void Ground()
{
    glPushMatrix();
    glScalef(map_width,0.0,map_length);
    SolidCube(1,0,1,0);
    glPopMatrix();
}
void Sky(int val)
{
    if(val==1)
    {
        glPushMatrix();
        glBegin(GL_QUADS);
        glVertex3f(4*map_width,2*map_length,-map_length);
        glTexCoord2d(0,0);
        glVertex3f(-4*map_width,2*map_length,-map_length);
        glTexCoord2d(0,1);
        glVertex3f(-4*map_width,-map_length/2.0,-map_length);
        glTexCoord2d(1,0);
        glVertex3f(4*map_width,-map_length/2.0,-map_length);
        glTexCoord2d(1,1);
        glEnd();
        glPopMatrix();
    }
    else if(val=2)
    {


        glPushMatrix();
        glBegin(GL_QUADS);
        glVertex3f(4*map_width,2*map_length,-map_length);
        glTexCoord2d(0,0);
        glVertex3f(4*map_width,-map_length/2.0,-map_length);
        glTexCoord2d(0,1);
        glVertex3f(4*map_width,-map_length/2.0,map_length);
        glTexCoord2d(1,0);
        glVertex3f(4*map_width,2*map_length,map_length);
        glTexCoord2d(1,1);
        glEnd();
        glPopMatrix();
    }
}

void sky_choose(int val)
{
    if(val==1)
    {
        glPushMatrix();
        glBegin(GL_QUADS);
        glVertex3f(-4*map_width,2*map_length,-2*map_length);
        glTexCoord2d(0,0);
        glVertex3f(-4*map_width,2*map_length,2*map_length);
        glTexCoord2d(0,1);
        glVertex3f(-4*map_width,-map_length/2.0,2*map_length);
        glTexCoord2d(1,0);
        glVertex3f(-4*map_width,-map_length/2.0,-2*map_length);
        glTexCoord2d(1,1);
        glEnd();
        glPopMatrix();
    }

    else
    {
        glPushMatrix();
        glBegin(GL_QUADS);
        glVertex3f(-4*map_width,2*map_length,2*map_length);
        glTexCoord2d(0,0);
        glVertex3f(-4*map_width,-map_length/2.0,2*map_length);
        glTexCoord2d(0,1);
        glVertex3f(4*map_width,-map_length/2.0,2*map_length);
        glTexCoord2d(1,0);
        glVertex3f(4*map_width,2*map_length,2*map_length);
        glTexCoord2d(1,1);
        glEnd();
        glPopMatrix();
    }
}

void Road()
{
    glPushMatrix();
    glScalef(road_width,0.05,road_length);
    glTranslatef(0,0.2,0);
    SolidCube(1,1,1,1);
    glPopMatrix();
    /*
        glPushMatrix();
        glScalef(road_width-3,0.5,road_length);
        glTranslatef(0,1,0);
        SolidCube(1,1,1,1);
        glPopMatrix();
        */
}

float car_length=12.0;
float car_width=5.0;
float base_height = 3;
float pos_len=1.5;
float middle_len=4.5;
float neg_len=2;
float front_back_extend= 2.0;
float base_to_top_height= 2.0;

static GLfloat car_vertex [12][3] =
{
    {-(car_length/2.0-front_back_extend),base_height/2.0,-car_width/2.0},
    {-(car_length/2.0-front_back_extend-neg_len),base_height/2.0,-car_width/2.0},
    {-(car_length/2.0-front_back_extend-neg_len),base_height/2.0 + base_to_top_height,- car_width/2.0},

    {-(car_length/2.0-front_back_extend),base_height/2.0, car_width/2.0},
    {-(car_length/2.0-front_back_extend-neg_len),base_height/2.0, car_width/2.0},
    {-(car_length/2.0-front_back_extend-neg_len),base_height/2.0 + base_to_top_height, car_width/2.0},

    {(car_length/2.0-front_back_extend),base_height/2.0,-car_width/2.0},
    {(car_length/2.0-front_back_extend-pos_len),base_height/2.0,-car_width/2.0},
    {(car_length/2.0-front_back_extend-pos_len),base_height/2.0 + base_to_top_height,- car_width/2.0},

    {(car_length/2.0-front_back_extend),base_height/2.0, car_width/2.0},
    {(car_length/2.0-front_back_extend-pos_len),base_height/2.0, car_width/2.0},
    {(car_length/2.0-front_back_extend-pos_len),base_height/2.0 + base_to_top_height, car_width/2.0},
};
static GLubyte car_tri_indices[4][3] =
{
    {0, 1, 2},
    {3, 4, 5},
    {7, 6, 8},
    {10, 9, 11}
};
static GLubyte car_quad_indices[12][4] =
{
    {1, 2, 5, 4},
    {1, 0, 3, 4},
    {0, 3, 5, 2},

    {1, 2, 8, 7},
    {7, 8, 11, 10},
    {10, 11, 5, 4},
    {1, 2, 5, 4},
    {2, 5, 11, 8},
    {1, 4, 10, 7},

    {7, 8, 11, 10},
    {7, 10, 9, 6},
    {6, 8, 11, 9},
};

long long nCr(int n, int r)
{
    if(r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

void BezierCurve ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

}

void TreeBeizer()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;

    const float startx = 0, endx = ctrlpoints[L][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                getNormal3p(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i

}


void carGlass();

void drawCarBody()
{
    // glPushMatrix();
    //glRotatef(alpha,0,1,0);

    glPushMatrix();
    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <2; i++)
    {
        //  glColor3f(colors[i][0],colors[i][1],colors[i][2]);
        //glColor4f(191/255.0,0,0,1);
        matColor(191/255.0,0.0,0.0,70);
        getNormal3p(car_vertex[car_tri_indices[i][0]][0], car_vertex[car_tri_indices[i][0]][1], car_vertex[car_tri_indices[i][0]][2],
                    car_vertex[car_tri_indices[i][1]][0], car_vertex[car_tri_indices[i][1]][1], car_vertex[car_tri_indices[i][1]][2],
                    car_vertex[car_tri_indices[i][2]][0], car_vertex[car_tri_indices[i][2]][1], car_vertex[car_tri_indices[i][2]][2]);

        glVertex3fv(&car_vertex[car_tri_indices[i][0]][0]);
        glVertex3fv(&car_vertex[car_tri_indices[i][1]][0]);
        glVertex3fv(&car_vertex[car_tri_indices[i][2]][0]);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    for (int ii = 0; ii <3; ii++)
    {
        if(ii==2)
            //glColor4f(118/255.0,118/255.0,118/255.0,1);
            matColor(118/255.0,118/255.0,118/255.0,70);
        getNormal3p(car_vertex[car_quad_indices[ii][0]][0], car_vertex[car_quad_indices[ii][0]][1], car_vertex[car_quad_indices[ii][0]][2],
                    car_vertex[car_quad_indices[ii][1]][0], car_vertex[car_quad_indices[ii][1]][1], car_vertex[car_quad_indices[ii][1]][2],
                    car_vertex[car_quad_indices[ii][2]][0], car_vertex[car_quad_indices[ii][2]][1], car_vertex[car_quad_indices[ii][2]][2]);

        glVertex3fv(&car_vertex[car_quad_indices[ii][0]][0]);
        //glTexCoord2f(1,1);
        glVertex3fv(&car_vertex[car_quad_indices[ii][1]][0]);
        //glTexCoord2f(1,0);
        glVertex3fv(&car_vertex[car_quad_indices[ii][2]][0]);
        //glTexCoord2f(0,1);
        glVertex3fv(&car_vertex[car_quad_indices[ii][3]][0]);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    for (int i = 3; i <9; i++)
    {
        //glColor4f(191/255.0,0,0,1);
        matColor(191/255.0,0,0,70);
        getNormal3p(car_vertex[car_quad_indices[i][0]][0], car_vertex[car_quad_indices[i][0]][1], car_vertex[car_quad_indices[i][0]][2],
                    car_vertex[car_quad_indices[i][1]][0], car_vertex[car_quad_indices[i][1]][1], car_vertex[car_quad_indices[i][1]][2],
                    car_vertex[car_quad_indices[i][2]][0], car_vertex[car_quad_indices[i][2]][1], car_vertex[car_quad_indices[i][2]][2]);

        glVertex3fv(&car_vertex[car_quad_indices[i][0]][0]);
        //glTexCoord2f(1,1);
        glVertex3fv(&car_vertex[car_quad_indices[i][1]][0]);
        //glTexCoord2f(1,0);
        glVertex3fv(&car_vertex[car_quad_indices[i][2]][0]);
        //glTexCoord2f(0,1);
        glVertex3fv(&car_vertex[car_quad_indices[i][3]][0]);
    }
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glBegin(GL_TRIANGLES);
    for (GLint i = 2; i <4; i++)
    {
        //glColor4f(191/255.0,0,0,1);
        matColor(191/255.0,0,0,70);
        getNormal3p(car_vertex[car_tri_indices[i][0]][0], car_vertex[car_tri_indices[i][0]][1], car_vertex[car_tri_indices[i][0]][2],
                    car_vertex[car_tri_indices[i][1]][0], car_vertex[car_tri_indices[i][1]][1], car_vertex[car_tri_indices[i][1]][2],
                    car_vertex[car_tri_indices[i][2]][0], car_vertex[car_tri_indices[i][2]][1], car_vertex[car_tri_indices[i][2]][2]);

        glVertex3fv(&car_vertex[car_tri_indices[i][0]][0]);
        glVertex3fv(&car_vertex[car_tri_indices[i][1]][0]);
        glVertex3fv(&car_vertex[car_tri_indices[i][2]][0]);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    for (int i = 9; i <12; i++)
    {
        if(i==11)
            //glColor4f(118/255.0,118/255.0,118/255.0,1);
            matColor(118/255.0,118/255.0,118/255.0,70);
        getNormal3p(car_vertex[car_quad_indices[i][0]][0], car_vertex[car_quad_indices[i][0]][1], car_vertex[car_quad_indices[i][0]][2],
                    car_vertex[car_quad_indices[i][1]][0], car_vertex[car_quad_indices[i][1]][1], car_vertex[car_quad_indices[i][1]][2],
                    car_vertex[car_quad_indices[i][2]][0], car_vertex[car_quad_indices[i][2]][1], car_vertex[car_quad_indices[i][2]][2]);

        glVertex3fv(&car_vertex[car_quad_indices[i][0]][0]);
        //glTexCoord2f(1,1);
        glVertex3fv(&car_vertex[car_quad_indices[i][1]][0]);
        //glTexCoord2f(1,0);
        glVertex3fv(&car_vertex[car_quad_indices[i][2]][0]);
        //glTexCoord2f(0,1);
        glVertex3fv(&car_vertex[car_quad_indices[i][3]][0]);
    }
    glEnd();
    glPopMatrix();


    //glPopMatrix();
}
void wheel()
{
    glPushMatrix();
    //glColor3f(0,0,0);
    matColor(0,0,0,70);
    //glRotatef(alpha,0,1,0);

    glPushMatrix();
    //glRotatef(alpha,0,1,0);
    glTranslatef(car_length/2.0-front_back_extend,-1.0,car_width/2.0);
    glutSolidTorus(0.5,0.5,50,50);
    glPopMatrix();

    glPushMatrix();
    //glRotatef(alpha,0,1,0);
    glTranslatef(-(car_length/2.0-front_back_extend),-1.0,car_width/2.0);
    glutSolidTorus(0.5,0.5,50,50);
    glPopMatrix();

    glPushMatrix();
    //glRotatef(alpha,0,1,0);
    glTranslatef(car_length/2.0-front_back_extend,-1.0,-car_width/2.0);
    glutSolidTorus(0.5,0.5,50,50);
    glPopMatrix();

    glPushMatrix();
    //glRotatef(alpha,0,1,0);
    glTranslatef(-(car_length/2.0-front_back_extend),-1.0,-car_width/2.0);
    glutSolidTorus(0.5,0.5,50,50);
    glPopMatrix();

    glPopMatrix();
}

void head_Light();

void drawCar()
{
    glPushMatrix();
//    glTranslatef(car_pos_x,car_pos_y,car_pos_z);
    glTranslatef(0,2,0);
    glRotatef(-90,0,1,0);
    //glRotatef(alpha,0,1,0);
    glPushMatrix();
    // glRotatef(alpha,0,1,0);
    //glTranslatef(0,2,0);
    glScalef(car_length,base_height,car_width);
    // glTranslatef(0,1,0);
    SolidCube(1,191/255.0,0,0);
    glPopMatrix();

    glPushMatrix();
    //glRotatef(alpha,0,1,0);
    //glScalef(.9,1,1);
    glTranslatef(-car_length/2.0+.4,0,car_width/2.0-0.51); ///car light front left (-5.6,0,2.5-0.51)
    SolidCube(1,1,1,1);
    glPopMatrix();

    glPushMatrix();
    // glRotatef(alpha,0,1,0);
    //glScalef(.9,1,1);
    glTranslatef(-car_length/2.0+.4,0,-car_width/2.0 +.51); ///car light front right
    SolidCube(1,1,1,1);
    glPopMatrix();

    glPushMatrix();
    drawCarBody();
    glPopMatrix();

    glPushMatrix();

    wheel();
    glPopMatrix();
//    Sky();

    glPopMatrix();

    glPushMatrix();
    head_Light();
    glPopMatrix();

}

/*{-5.6,2,2.5-0.51};
   { -car_length/2.0+.4,0,car_width/2.0-0.51 1.0 };
   */
void head_Light()
{
    //light(1.0,1.0,1.0);
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 1.0,1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_pos[] = {-2,-1,-7,1};

    glEnable( GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, head_light?light_ambient:no_light);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, head_light?light_diffuse:no_light);
    glLightfv(GL_LIGHT0,GL_SPECULAR, head_light?light_specular:no_light);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    GLfloat direction[] = {-3, 1, -10, 1};
    GLfloat cut_off = 80.0;
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction );
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cut_off );

}

void Sun_Light()
{

    glPushMatrix();
    glTranslatef(0,50,-100);
    SolidCube(1);

    glPopMatrix();


    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[]  = { 1.0,1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_pos[] = {-20,30,50};

    glEnable( GL_LIGHT1);

    glLightfv(GL_LIGHT1, GL_AMBIENT, sun_light?light_ambient:no_light);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, sun_light?light_diffuse:no_light);
    glLightfv(GL_LIGHT1,GL_SPECULAR, sun_light?light_specular:no_light);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);

    //GLfloat direction[] = {-3, 1, -10, 1};
    //GLfloat cut_off = 40.0;
    //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction );
    //glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cut_off );
}



void carGlass()
{
    /// front black glass
    float delx=0.5,dely=1,delz= 0.5;
    delx=1.0;
    dely=0.5;
    delz=0.5;

    glPushMatrix();
    //glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glVertex3f(-(car_length/2.0-front_back_extend-delx),base_height/2.0+dely, car_width/2.0 + delz);
    glVertex3f((car_length/2.0-front_back_extend-delx),base_height/2.0 + dely,(car_width/2.0+delz));
    glVertex3f((car_length/2.0-front_back_extend-neg_len),base_height/2.0 + base_to_top_height -dely,car_width/2.0+delz);
    glVertex3f(-(car_length/2.0-front_back_extend-neg_len),base_height/2.0+base_to_top_height-dely,car_width/2.0-delz);
    glEnd();
    glPopMatrix();

}
/*
void maketree()
{
    glPushMatrix();
    //glTranslatef(-20,4,270);
    //glClearColor(1,1,1,1);

    glPushMatrix();

    //glColor3f(0.42, .26, .15);
    matColor(0.42, .26, .15,70);
    glTranslatef(0,5,20);
    glRotatef(90,1,0,0);
    GLUquadricObj* obj = gluNewQuadric();

    gluCylinder(obj, 1, 1, 10, 10, 8);

    glPopMatrix();


    glPushMatrix();
    //glColor3f(0,1,0);
    matColor(0,1,0,70);
    //glClearColor(0,0,0,1);
    glTranslatef(0, 5, 20);
    glRotatef(270,1,0,0);

    glScalef(7,7,7);
    glBegin(GL_TRIANGLES);
    for (int k = 0; k <= 360; k += 5)
    {
        //glColor3f(1.0, 0.0, 0.0);
        matColor(1.0,0.0,0.0,60);
        glVertex3f(0, 0, 1);
        glVertex3f(Cos(k), Sin(k), 0);
        glVertex3f(Cos(k + 5), Sin(k + 5), 0);
    }
    glEnd();
    glPopMatrix();

    glPopMatrix();


}
*/
void drawTrees()
{
    for(int i=- map_length/2.0; i<=map_length/2.0; i+=100)
    {
        glPushMatrix();

        glTranslatef(-30,0,i);
        glScalef(2,2.5,2);
        glRotatef(90,0,0,1);
        TreeBeizer();
        glPopMatrix();
    }

    for(int i=- map_length/2.0 + 50; i<= map_length/2.0 - 50; i+=100)
    {
        glPushMatrix();

        glTranslatef(30,0,i);
        glScalef(2,2.5,2);
        glRotatef(90,0,0,1);
        TreeBeizer();
        glPopMatrix();
    }
}
void building(void)
{
    int height1 = 18,height2,height3,height4;
    int width = 13,length = 15;

    for(int j=0; j<10; j++)
    {
        int space = j*165;
        for(int i=0; i<4; i++)
        {
            glPushMatrix();
            ///House
            for (int loop_val=-30,loop_cond=1; loop_cond<=2; loop_val+=120,loop_cond++)
            {
                glEnable(GL_TEXTURE_2D);

                if(i&1)
                    glBindTexture(GL_TEXTURE_2D,house1);
                else
                    glBindTexture(GL_TEXTURE_2D,house2);

                int dst = i*10;

                glPushMatrix();
                glTranslatef(-20+loop_val- width, -2, -100 -i*length - dst - space);
                glScalef(width, height1, length);
                drawCube(1,1,1);
                glPopMatrix();

                glPushMatrix();
                glTranslatef(-20+loop_val- width, -2+height1, -100 -i*length - dst - space);
                glScalef(width, height1, length);
                drawCube(1,1,1);
                glPopMatrix();

                glDisable(GL_TEXTURE_2D);

                ///HouseDoor
                glPushMatrix();
                glTranslatef(-20+loop_val, -2, -96.5 - i*length - dst -space);
                glScalef(.1, 9, 7);
                drawCube(.8,.8,.8);
                glPopMatrix();

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, house_door);

                glPushMatrix();

                glTranslatef(-19.5+loop_val, -1, -95 - i*length -dst - space);
                glScalef(.01, 5, 5.3);
                drawCube(1,1,1);
                glPopMatrix();

                glDisable(GL_TEXTURE_2D);
                ///Stair
                glPushMatrix();

                glTranslatef(-19.5+loop_val, -2, -95 - i*length - dst - space);
                glScalef(1, 1, 5.3);
                drawCube(0.5, 0.5, 0.5);
                glPopMatrix();

                glPushMatrix();

                glTranslatef(-18.5+loop_val, -2, -95 - i*length - dst - space);
                glScalef(1, 0.5, 5.3);
                drawCube(0.5, 0.5, 0.5);
                glPopMatrix();
            }
            glPopMatrix();
        }
    }
}


void obstacle()
{
    srand(time(0));
    int random_val;
    ///Left
    for(int i=1; i<=10; i++)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, (i&1)?rock1:rock2);
        glPushMatrix();
        if(i%2==0)
            obstLeftX[i] = ((rand()+i)&1)?-10:-6;
        else
            obstLeftX[i] = ((rand()+i)&1)?-13:-3;
        obstLeftZ[i] = -100*i;
        glTranslatef(obstLeftX[i], 2, obstLeftZ[i]);
        glScalef(3, 1.5, 0.7);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }
    ///Right
    for(int i=1; i<=10; i++)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, (i&1)?rock2:rock1);
        glPushMatrix();
        if(i%2==0)
            obstRightX[i] = ((rand()+i)&1)?9:5;
        else
            obstRightX[i] = ((rand()+i)&1)?2:12;

        obstRightZ[i] = -80*i;
        glTranslatef(obstRightX[i], 2, obstRightZ[i]);
        glScalef(3, 1.5, 0.7);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}

void display()
{
    glViewport(0, 0, (GLsizei)windowWidth,(GLsizei)windowHeight);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glFrustum(-10,10,-5,15, 1, 10000);
    gluPerspective(70,(GLfloat) windowWidth/(GLfloat) windowHeight,0.1,10000.0);
    //glOrtho(-5,5,-5,5, 4, 50);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    // gluLookAt(	eyeX, eyeY, eyeZ,posX, posY, posZ,0, 1.0f, 0.0f);
    /// runx=0  , runy=10
    gluLookAt(	eyeX+runX, eyeY, eyeZ+runZ,
                runX, 0, runZ-5,
                0, 1.0f, 0.0f);

    ///Start text Section
    stringstream ss;
    ss<<"'Game Over' Press 'C' for play again";

    string s1 = ss.str();

    if(collision)
    {
        runX = 0;
        runZ = 10;
        max_point = max(max_point, point);
        gluLookAt(eyeX-1, eyeY-15, eyeZ-1.3, runX, 0, runZ-5, 0, 1, 0);
        glPushMatrix();
        glTranslatef(runX-50, 35, runZ);
        //glRotatef(0,0,0,1);
        drawText(s1,1, 0, 0, 5);
        //const char *str= s1.c_str();
        //drawStrokeText(str,-5,20,-10);
        glPopMatrix();
        start = false;

        ss.str(string());
        ss<<max_point;
        s1 = "Highest Score: " + ss.str();
        glPushMatrix();
        glTranslatef(runX-20, 20.5, runZ);
        glRotatef(-2, 0,0,1);
        drawText(s1, 0,1,0, 3);
        glPopMatrix();
    }

    ss.str(string());

    ss<<point;
    s1 ="Score:" + ss.str();

    glPushMatrix();
    glTranslatef(runX - 10, 25, runZ);
    glRotatef(-2, 0, 0, 1);
    drawText(s1, 0, 0, 1, 3);
    glPopMatrix();
    ///End text section


    /// Ground Grass with texture
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,road);
    Ground();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    /// road with texture
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,grass);
    Road();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    ///sky

    for(int j=1; j<=2; j++)
    {
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,sky);
        Sky(j);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,sky);
    sky_choose(1);
    sky_choose(2);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

/// Draw Car
    glPushMatrix();
    glTranslatef(runX,0,runZ);
    drawCar();
    glPopMatrix();

/// Draw Trees
    glPushMatrix();
    drawTrees();
    glPopMatrix();

    ///Draw Building
    glPushMatrix();
    building();
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,sun);
    //sky_choose();
    Sun_Light();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    /// Draw Obstacle

    glPushMatrix();
    obstacle();
    glPopMatrix();

    for(int i=0; i<4; i++)
    {
        glPushMatrix();
        glTranslatef(carX[i], 0, carZ[i]);
        drawCar();
        //car(start && !light3)
        glPopMatrix();
    }

    glFlush();
    glutSwapBuffers();
}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    if(key=='+')
    {
        eyeZ--;
        posZ--;
    }

    else if(key=='-')
    {
        eyeZ++;
        posZ++;
    }
    else if(key=='U')
    {
        eyeY++;
    }
    else if(key=='R')
    {
        eyeX++;
    }
    else if(key=='L')
    {
        eyeX--;
    }
    else if(key=='F')
    {
        eyeZ++;
    }
    else if(key=='B')
    {
        eyeZ--;
    }
    else if(key=='h'||key =='H')
    {
        head_light= !head_light;
    }
    else if(key=='s'||key=='S')
    {
        sun_light = !(sun_light);
    }

    else if(key == 'w'|| key == 'W')
    {
        running = true;
        start = true;
        runZ = runZ - 1;

    }
    else if(key == 'z'|| key == 'Z')
    {
        start = true;
        running = false;
        runZ = runZ + 0.3;
    }
    else if(key == 'a'|| key == 'a')
    {
        if ( runX > - road_width/2.0)
            runX = runX - 0.15;
    }
    else if(key == 'd'|| key == 'D')
    {
        if ( runX < road_width/2.0)
            runX = runX + 0.15;
    }
    else if(key == 'c')
    {
        point = 0;
        collision = false;
    }
    else if( key == 27)	// Escape key
        exit(1);

    glutPostRedisplay();
}

void mySpecialFunc(int key, int x, int y)
{
    switch (key)
    {

    case GLUT_KEY_PAGE_UP:
        eyeY++;
        break;

    case GLUT_KEY_PAGE_DOWN:
        eyeY--;
        break;

    case GLUT_KEY_LEFT:
        eyeX--;
        break;

    case GLUT_KEY_RIGHT:
        eyeX++;
        break;

    default:
        break;
    }
}

void animate()
{
    if(runZ<-1000)
        runZ = 0;
    else if(runZ>3)
    {
        runZ=3;
    }
    if(rot>=360)
        rot = 0;
    if(running)
    {
        runZ-=0.4;
        point++;
        rot += 0.2;
    }

    if(start)
    {
        for(int i=0; i<8; i++)
        {
            if(carZ[i]<-1300)
                carZ[i] = -80 + i*5;
            else
            {
                carZ[i] -= carSpeed[i];
            }
        }
    }
    ///Collision detection
    for(int i=0; i<4; i++)
    {
        if((carX[i]<=runX+2 && carX[i]+2 >= runX+2)&&(carZ[i]<=runZ && carZ[i]+5>=runZ))
        {
            runX = runX-0.3;
            runZ = runZ+1;
            collision = true;
        }
        else if((carX[i]<=runX && carX[i]+2>=runX) &&(carZ[i]<=runZ && carZ[i]+5>=runZ))
        {
            runX = runX+0.3;
            runZ = runZ+1;
            collision = true;
        }
        else if((carX[i]<=runX && carX[i]+2>=runX) &&(carZ[i]<=runZ+5 && carZ[i]+5>=runZ+5))
        {
            runX = runX+0.3;
            runZ = runZ - 1;
            collision = true;
        }
        else if((carX[i]>=runX+2 && carX[i]+2<=runX+2) &&(carZ[i]<=runZ+5 && carZ[i]+5>=runZ+5))
        {
            runX = runX-3;
            runZ = runZ - 1;
            collision = true;
        }
    }
    for(int i=1; i<=10; i++)
    {
        if(((obstLeftX[i] <= runX && obstLeftX[i]+3 >= runX) || (obstLeftX[i] <= runX+2 && obstLeftX[i]+3 >= runX+2 )) && (obstLeftZ[i] <= runZ && obstLeftZ[i]+0.7 >= runZ))
        {
            runX = runX+0.3;
            runZ = runZ+2.5;
            collision = true;
        }
        else if(((obstLeftX[i] <= runX && obstLeftX[i]+3 >= runX)||(obstLeftX[i] <= runX+2 && obstLeftX[i]+3 >= runX+2 )) && (obstLeftZ[i] <= runZ+5 && obstLeftZ[i]+0.7 >= runZ+5))
        {
            runX = runX+0.3;
            runZ = runZ-1;
            collision = true;
        }
    }

    for(int i=1; i<=10; i++)
    {
        if(((obstRightX[i] <= runX && obstRightX[i]+3 >= runX)||(obstRightX[i] <= runX+2 && obstRightX[i]+3 >= runX+2 )) && (obstRightZ[i] <= runZ && obstRightZ[i]+0.7 >= runZ))
        {
            runX = runX-0.3;
            runZ = runZ+2.5;
            collision = true;
        }
        else if(((obstRightX[i] <= runX && obstRightX[i]+3 >= runX)||(obstRightX[i] <= runX+2 && obstRightX[i]+3 >= runX+2 )) && (obstRightZ[i] <= runZ+5 && obstRightZ[i]+0.7 >= runZ+5))
        {
            runX = runX-0.3;
            runZ = runZ-1;
            collision = true;
        }
    }

    if(collision)
    {
        running = false;
    }


    glutPostRedisplay();

}


void LoadTexture(const char*filename,int id)
{
    glGenTextures(1, &ID[id]);
    glBindTexture(GL_TEXTURE_2D, ID[id]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID[id]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename,id);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

void reshape(int width,int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glFrustum(-10,10,-5,15, 1, 10000);
    gluPerspective(70,(GLfloat) width/(GLfloat) height,0.1,10000.0);
    //glOrtho(-5,5,-5,5, 4, 50);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(	eyeX, eyeY, eyeZ,
                posX, posY, posZ,
                0, 1.0f, 0.0f);



}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(300,0);
    glutInitWindowSize(windowHeight, windowWidth);

    glutCreateWindow("1607018 3D Car Animation");
    glClearColor(0,0,0,1);
    //glutReshapeFunc(reshape);

    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(mySpecialFunc);
    glEnable(GL_BLEND);
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);

    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\road.bmp",road);
    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\grass.bmp",grass);
    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\sky.bmp",sky);
    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\sun.bmp",sun);
    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\rock1.bmp",rock1);
    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\rock2.bmp",rock2);
    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\house1.bmp",house1);
    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\house2.bmp",house2);
    LoadTexture("F:\\Education\\4-2\\CSE 4208 LAB\\Racing\\image\\house_door.bmp",house_door);


    printf("Press  'w' for Forward\n");
    printf("Press  'z' for Backward\n");
    printf("Press  'a' for Left\n");
    printf("Press  'd' for Right\n");
    printf("Press  'h' for Turn On & Turn off Car Head Light\n");
    printf("Press  's' for Turn On & Turn off Sun Light\n");
    printf("Press  'c' for Restart the game\n");
    printf("Press  '+' for Zoom in\n");
    printf("Press  '-' for Zoom out\n");
    glutMainLoop();

    return 0;

}




