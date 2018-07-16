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
 * Student: raal4953
 * 
 * Lorez Attractor
 * 
 * I used ex5.c that can be found on moodle as the template to start.  
 *
 *  Key bindings:
 *  s/S    Increase/decrease sigma
 *  b/B    Increase/decrease beta
 *  r/R    Increase/decrease rho
 *  d      Reset all(sigma, beta, rho) to default values
 *  arrows Change view angle
 *  0      Reset view angle
 *  ESC    Exit
 */
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
// Globals
int th=0;  //  Rotation angle
int ph=0;       // Elevation of view angle
double zh=0;   // for z vars
//lorenz params
int maxPts=50000; // I took this number from lorenz.c from prinmath.com
double pts[50000][3]; // For storig the attractor points
double color[50000][3];
double s= 10;
double b= 2.6666;
double r=28;
double w=0.02;


/*
 * Convenience function for text
 */
#define LEN 8192  //  Maximum amount of text
void Print(const char* format , ...)
{
   char    buf[LEN]; // Text storage
   char*   ch=buf;   // Text pointer
   //  Create text to be display
   va_list args;
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display text string
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Function to print any errors encountered
 */
void ErrCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 * This functions is to generate the point for the display function to use.
 */
void lorenzGen(void)
{
   int i;
   //coordinates
   double x = 1;
   double y = 1;
   double z = 1;
   double dx = 0;
   double dy = 0;
   double dz = 0;
   // Time step
   double dt = 0.001;
   for(i=0;i<maxPts-1;i++)
   {
      dx = s*(y-x);
      dy = x*(r-z)-y;
      dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;

      pts[i][0] = x*w;
      pts[i][1] = y*w;
      pts[i][2] = z*w;
   }
}

/*
 * This function is called by GLUT to display the scene
 */
void display()
{
   // generate lorenz points
   lorenzGen();
   //  Clear screen and Z-buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   // reset trans
   glLoadIdentity();
   // set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   //  RGB lorenz attractor
   int i=0; 
   glBegin(GL_LINE_STRIP);
   for(i=0;i<maxPts;i++)
   {
      glColor3dv(pts[i]);
      glVertex3dv(pts[i]); 
   }
   glEnd();
   //  Draw axes in white
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(1,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,1,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,1);
   glEnd();
   //  Label axes
   glRasterPos3d(1,0,0);
   Print("X");
   glRasterPos3d(0,1,0);
   Print("Y");
   glRasterPos3d(0,0,1);
   Print("Z");
   //  Display parameters
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d",th,ph);
   //  Sanity check
   ErrCheck("display");
   //  Flush and swap buffer
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   // Adjust the angle
   else if(ch=='0')
      th=ph=0;
   // Adjust sigma
   else if(ch=='s') 
      s -=1;
   else if(ch=='S')
      s +=1;
   // Adjust beta
   else if(ch=='b')
      b -=0.5;
   else if(ch=='B')
      b +=0.5;
   // Adjust rho
   else if(ch=='r')
      r -=1;
   else if(ch=='R')
      r +=1;
   // Reset to default
   else if(ch=='d')
   {
      s = 10;
      b = 2.666;
      r = 28;
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 * This function is called by GLUT when special keys are pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 * This function is called by GLUT when the window is resized
 */
void reshape(int width,int height)
{
   //  Calculate width to height ratio
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set viewport as entire window
   glViewport(0,0, width,height);
   //  Select projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Set projection to identity
   glLoadIdentity();
   //  Orthogonal projection:  unit cube adjusted for aspect ratio
   glOrtho(-w2h,+w2h, -1.0,+1.0, -1.0,+1.0);
   //  Select model view matrix
   glMatrixMode(GL_MODELVIEW);
   //  Set model view to identity
   glLoadIdentity();
}

/*
 * GLUT based Lorenz Attractor
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered true color window with Z-buffer
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   //request 500*500pxs window size
   glutInitWindowPosition(50, 50);
   glutInitWindowSize(600,700);
   //  Create window
   glutCreateWindow("Rami AlQunaibit");
   //  Register display, reshape, idle and key callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   //glutIdleFunc(idle);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Enable Z-buffer depth test
   //glEnable(GL_DEPTH_TEST);
   //  Pass control to GLUT for events
   glutMainLoop();
   //  Return to OS
   return 0;
}
