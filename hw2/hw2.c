/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* 
 *  Student: raal4953
 *  Date: June 18, 2017
 *  CSCI-4229
 *
 */



/*
 *  Projections
 *
 *  Key bindings:
 *  m/M        Toggle between perspective and orthogonal
 *  +/-        Changes field of view for perspective
 *  x/X        Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  f          Toggle first person
 *  w/s        Move in first person
 *  a/d        Rotate camera in first person
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
int fp=0;
int rot=0.0;      //  Rotaion var fp
double Ex=0;
double Ey=0;      //  Eye for lookat
double Ez=5;
double Cx=0;      //  Camera for lookat
double Cz=0;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if(fp)
   {
     gluPerspective(fov,asp,dim/4,4*dim);
   }
   else 
   {
     if (mode)
        gluPerspective(fov,asp,dim/4,4*dim);
     //  Orthogonal projection
     else
        glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   }
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a house
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void house(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   // Enable Polygon Offset
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(2,1); 
   // house_Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3d(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3d(1,0,0);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3d(1,0,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3d(1,0,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3d(1,0,0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3d(1,0,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);
   //house-top
   glColor3f(1,1,1);
   glBegin(GL_TRIANGLES);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3d(0,1.5,0);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3d(0,1.5,0);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   glVertex3d(0,1.5,0);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3d(0,1.5,0);
   glEnd();
   //  Window
   glColor3f(0,0,0);
   glBegin(GL_LINE_LOOP);
   glVertex3d(0.8,0.8,1);
   glVertex3d(0.8,0.6,1);
   glVertex3d(0.6,0.6,1);
   glVertex3d(0.6,0.8,1); 
   glEnd(); 
   //  window_frame 
   glBegin(GL_LINES);
   glVertex3d(0.7,0.8,1);
   glVertex3d(0.7,0.6,1);
   glEnd(); 
   // Back window
   glColor3f(0,0,0);
   glBegin(GL_LINE_LOOP);
   glVertex3d(0.8,0.8,-1);
   glVertex3d(0.8,0.6,-1);
   glVertex3d(0.6,0.6,-1);
   glVertex3d(0.6,0.8,-1); 
   glEnd(); 
   //  window_frame 
   glBegin(GL_LINES);
   glVertex3d(0.7,0.8,-1);
   glVertex3d(0.7,0.6,-1);
   glEnd(); 
   //  Window
   glColor3f(0,0,0);
   glBegin(GL_LINE_LOOP);
   glVertex3d(-0.8,0.8,1);
   glVertex3d(-0.8,0.6,1);
   glVertex3d(-0.6,0.6,1);
   glVertex3d(-0.6,0.8,1); 
   glEnd(); 
   //  window_frame 
   glBegin(GL_LINES);
   glVertex3d(-0.7,0.8,1);
   glVertex3d(-0.7,0.6,1);
   glEnd(); 
   //  Enable Polygon Offset
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   //  Door
   glColor3f(0,0,0);
   glBegin(GL_QUADS);
   glVertex3d(0.21,-1,1);
   glVertex3d(-0.21,-1,1);
   glVertex3d(-0.21,0,1);
   glVertex3d(0.21,0,1);
   glEnd();
   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);
   //  Door grip
   glColor3f(1,1,1);
   glPointSize(3);
   glBegin(GL_POINTS);
   glVertex3d(0.18,-0.5,1);
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (fp)
   {
      Cx = +2*dim*Sin(rot); 
      Cz = -2*dim*Cos(rot);
      gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0);
   }
   else
   {
      if (mode)
      {
         double px = -2*dim*Sin(th)*Cos(ph);
         double py = +2*dim        *Sin(ph);
         double pz = +2*dim*Cos(th)*Cos(ph);
         gluLookAt(px,py,pz , 0,0,0 , 0,Cos(ph),0);
      }
      //  Orthogonal - set world orientation
      else
      {
         glRotatef(ph,1,0,0);
         glRotatef(th,0,1,0);
      }
   }
   //  Draw 1st house
   house(0,0,0 , 0.5,0.3,0.3, 1);
   //  Draw 2nd house
   house(2,0,0 , 0.5,0.3,0.3, 1);
   //  Draw 3rd house
   house(-3,0,0 , 0.8,0.4,0.4, 1);
   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   if (fp)
   {
      Print("FP=On View Angle=%d",rot);
   }
   else
   {
      Print("FP=Off Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   }
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   if(!fp) 
   {
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
         th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
         th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP)
         ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
         ph -= 5;
      //  PageUp key - increase dim
      else if (key == GLUT_KEY_PAGE_UP)
         dim += 0.1;
      //  PageDown key - decrease dim
      else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
         dim -= 0.1;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
   }
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle first person
   else if (ch == 'f' || ch == 'F')
   {
      fp = 1-fp;
   }
   
   if (fp)
   {
      double dt = 0.05;
      if (ch == 'w' || ch == 'W')
      {
         Ex += Cx*dt; //Update the eye vector based on the camera vector
         Ez += Cz*dt;
      }
      else if (ch == 'a' || ch == 'A')
      {
         rot -= 3;
      }
      else if (ch == 's' || ch == 'S')
      {
         Ex -= Cx*dt;
         Ez -= Cz*dt;
      }
      else if (ch == 'd' || ch == 'D')
      {
         rot += 3;
      }
      //  Keep angles to +/-360 degrees
      rot %= 360;
   }
   else
   {
     //  Reset view angle
     if (ch == '0')
        th = ph = 0;
     //  Switch display mode
     else if (ch == 'm' || ch == 'M')
        mode = 1-mode;
     //  Change field of view angle
     else if (ch == '-' && ch>1)
        fov--;
     else if (ch == '+' && ch<179)
        fov++;
   }
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Rami AlQunaibit");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
