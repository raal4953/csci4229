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
 *  Final project
 *  Modified from HW3
 *  
 *  Student: raal4953
 *  Date: July 5, 2017
 *  CSCI-4229
 *
 */


/*
 *  Lighting and Textures 
 *
 *  Key bindings:
 *  m/M        Toggle between perspective and orthogonal
 *  +/-        Changes field of view for perspective
 *  x/X        Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  f          Toggle first person
 *  w/s        (While fp)Move in first person
 *  a/d        (While fp)Rotate camera in first person
 *  ESC        Exit
 */
#include "CSCIx229.h"

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int zh=90;         //  Light azimuth
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
unsigned int texture[20]; // Texture names

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
void Project()
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
 * Draw a honey bee
 *
 */
static void bee(double x, double y, double z, double dx, double dy, double dz, double th)
{
   int thf;
   float Or = 1*sqrt(2.0)-1.5;  
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE); 
   //back tail
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_TRIANGLE_FAN);
   glColor3d(1,1,1);
   glTexCoord2f( 0.5, 0.5 ); 
   glVertex3f(0,0,0);
   for (thf=0;thf<=360;thf+=15)
      glTexCoord2f( 0.5 * Cos(thf) + 0.5, 0.5 * Sin(thf) + 0.5 ); glVertex3f(Or*Cos(thf),Or*Sin(thf),0);
   glEnd();
   //back
   glBegin(GL_QUAD_STRIP);
   glColor3d(1,1,0);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf),Or*Sin(thf),0);
      glVertex3f(Or*Cos(thf)*2,Or*Sin(thf)*2,0.1);
   }
   glEnd();
   //wings inter
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf)*2,Or*Sin(thf)*2,0.1);
      glVertex3f(Or*Cos(thf)*2,Or*Sin(thf)*2,0.5);
   }
   glEnd();
   //wings
   glBegin(GL_QUADS);
   glColor3d(1,1,0);
   glVertex3f(0.3,0.3,0.5);
   glVertex3f(0,0,0.1);
   glVertex3f(0.3,0.3,0.1);
   glVertex3f(0,0,0.5);
   glEnd();
   //right wing
   glBegin(GL_QUADS);
   glColor3d(1,1,0);
   glVertex3f(-0.3,0.3,0.5);
   glVertex3f(0,0,0.1);
   glVertex3f(-0.3,0.3,0.1);
   glVertex3f(0,0,0.5);
   glEnd();
   // neck #1
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf)*2,Or*Sin(thf)*2,0.5);
      glVertex3f(Or*Cos(thf),Or*Sin(thf),0.6);
   }
   glEnd();
   //head
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf),Or*Sin(thf),0.6);
      glVertex3f(Or*Cos(thf)*1.5,Or*Sin(thf)*1.5,0.7);
   }
   glEnd();
   //head #2
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf)*1.5,Or*Sin(thf)*1.5,0.7);
      glVertex3f(Or*Cos(thf)*0.01,Or*Sin(thf)*0.01,0.8);
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glPopMatrix(); 
}

/* 
 * Draw an Jellyfish
 *
 */
static void Jellyfish(double x, double y, double z, double dx, double dy, double dz, double th)
{
   int thf;
   float Or = 1*sqrt(2.0)-1.5;  
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE); 
   glColor3d(0,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[4]);
   //head
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glTexCoord2f((thf*0.001),0); glVertex3f(Or*Sin(thf)*0.001,0.5,Or*Cos(thf)*0.001);
      glTexCoord2f(0,(thf*0.001));glVertex3f(Or*Sin(thf)*2,0.4,Or*Cos(thf)*2);
   }
   glEnd();
   //head #2
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Sin(thf)*2,0.4,Or*Cos(thf)*2);
      glVertex3f(Or*Sin(thf)*2,0.2,Or*Cos(thf)*2);
   }
   glEnd();
   //head #3
   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0,0.2,0);
   for (thf=0;thf<=360;thf+=15)
      glVertex3f(Or*Sin(thf)*2,0.2,Or*Cos(thf)*2);
   glEnd();
   //legs
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f((-0.01+Or) + Or*Sin(thf)*0.4,0.2,Or*Cos(thf)*0.4);
      glVertex3f((-0.01+Or) + Or*Sin(thf)*0.4,-0.2,Or*Cos(thf)*0.4);
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f((+0.01-Or) + Or*Sin(thf)*0.4,0.2,Or*Cos(thf)*0.4);
      glVertex3f((+0.01-Or) + Or*Sin(thf)*0.4,-0.2,Or*Cos(thf)*0.4);
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Sin(thf)*0.4,0.2,(+0.01-Or) + Or*Cos(thf)*0.4);
      glVertex3f(Or*Sin(thf)*0.4,-0.2,(+0.01-Or) + Or*Cos(thf)*0.4);
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Sin(thf)*0.4,0.2,(-0.01+Or) + Or*Cos(thf)*0.4);
      glVertex3f(Or*Sin(thf)*0.4,-0.2,(-0.01+Or) + Or*Cos(thf)*0.4);
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glPopMatrix(); 
}


/* 
 * Draw a pig
 *
 */
static void pig(double x, double y, double z, double dx, double dy, double dz, double th)
{
   int thf;
   float Or = 1*sqrt(2.0)-1.5; 
   float Ir = Or-0.01;  
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz); 
   glColor3d(1,0,1);
   //back
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf),Or*Sin(thf),0);
      glVertex3f(Or*Cos(thf)*3,Or*Sin(thf)*3,0.1);
   }
   glEnd();
   //body
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf)*3,Or*Sin(thf)*3,0.1);
      glVertex3f(Or*Cos(thf)*3,Or*Sin(thf)*3,0.5);
   }
   glEnd();
   //neck
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf)*3,Or*Sin(thf)*3,0.5);
      glVertex3f(Or*Cos(thf),Or*Sin(thf),0.6);
   }
   glEnd();
   //back tail
   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0,0,0);
   for (thf=0;thf<=360;thf+=15)
      glVertex3f(Or*Cos(thf),Or*Sin(thf),0);
   glEnd();
   //head
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(Or*Cos(thf),Or*Sin(thf),0.6);
      glVertex3f(Or*Cos(thf)*1.1,Or*Sin(thf)*1.1,0.7);
   }
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   glVertex3f(0,0,0.7);
   for (thf=0;thf<=360;thf+=15)
      glVertex3f(Or*Cos(thf)*1.1,Or*Sin(thf)*1.1,0.7);
   glEnd();
   //legs
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(-0.1 + Ir*Sin(thf)*0.8,0,0.4 + Ir*Cos(thf)*0.8);
      glVertex3f(-0.2 + Ir*Sin(thf)*0.8,-0.3,0.4 + Ir*Cos(thf)*0.8);
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(0.1 + Ir*Sin(thf)*0.8,0,0.4 + Ir*Cos(thf)*0.8);
      glVertex3f(0.2 + Ir*Sin(thf)*0.8,-0.3,0.4 + Ir*Cos(thf)*0.8);
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(-0.1 + Ir*Sin(thf)*0.8,0,0.2 + Ir*Cos(thf)*0.8);
      glVertex3f(-0.2 + Ir*Sin(thf)*0.8,-0.3,0.2 + Ir*Cos(thf)*0.8);
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(0.1 + Ir*Sin(thf)*0.8,0,0.2 + Ir*Cos(thf)*0.8);
      glVertex3f(0.2 + Ir*Sin(thf)*0.8,-0.3,0.2 + Ir*Cos(thf)*0.8);
   }
   glEnd();
   glPopMatrix(); 
}

/* 
 * Draw a sheep
 *
 */
static void sheep(double x, double y, double z, double dx, double dy, double dz, double th)
{
   int thf;
   float Or = 1*sqrt(2.0)-1.5; 
   float Ir = Or-0.01;  
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE); 
   glColor3d(1,1,1);
   //body base
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.5,0,0);
   glTexCoord2f(1,0);glVertex3f(0.5,0,0);
   glTexCoord2f(1,1);glVertex3f(0.5,0,0.5);
   glTexCoord2f(0,1);glVertex3f(-0.5,0,0.5);
   glEnd();
   //body to base
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(-0.5,0,0);
   glTexCoord2f(1,0);glVertex3f(0.5,0,0);
   glTexCoord2f(1,1);glVertex3f(1,0.5,-0.5);
   glTexCoord2f(0,1);glVertex3f(-1,0.5,-0.5);
   glEnd();
   //body to base #2
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(-0.5,0,0);
   glTexCoord2f(1,0);glVertex3f(-0.5,0,0.5);
   glTexCoord2f(1,1);glVertex3f(-1,0.5,1);
   glTexCoord2f(0,1);glVertex3f(-1,0.5,-0.5);
   glEnd();
   //body to base #3
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(-0.5,0,0.5);
   glTexCoord2f(1,0);glVertex3f(0.5,0,0.5);
   glTexCoord2f(1,1);glVertex3f(1,0.5,1);
   glTexCoord2f(0,1);glVertex3f(-1,0.5,1);
   glEnd();
   //body to base #4
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(0.5,0,0);
   glTexCoord2f(1,0);glVertex3f(0.5,0,0.5);
   glTexCoord2f(1,1);glVertex3f(1,0.5,1);
   glTexCoord2f(0,1);glVertex3f(1,0.5,-0.5);
   glEnd();
   //body squares
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(1,0.5,-0.5);
   glTexCoord2f(1,0);glVertex3f(-1,0.5,-0.5);
   glTexCoord2f(1,1);glVertex3f(-1,1,-0.5);
   glTexCoord2f(0,1);glVertex3f(1,1,-0.5);
   glEnd();
   //sqaure #2
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(-1,0.5,1);
   glTexCoord2f(1,0);glVertex3f(-1,0.5,-0.5);
   glTexCoord2f(1,1);glVertex3f(-1,1,-0.5);
   glTexCoord2f(0,1);glVertex3f(-1,1,1);
   glEnd();
   //square #3
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(1,0.5,1);
   glTexCoord2f(1,0);glVertex3f(-1,0.5,1);
   glTexCoord2f(1,1);glVertex3f(-1,1,1);
   glTexCoord2f(0,1);glVertex3f(1,1,1);
   glEnd();
   //square #4 
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(1,0.5,1);
   glTexCoord2f(1,0);glVertex3f(1,0.5,-0.5);
   glTexCoord2f(1,1);glVertex3f(1,1,-0.5);
   glTexCoord2f(0,1);glVertex3f(1,1,1);
   glEnd();
   //head
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(0,0,0);
   glTexCoord2f(0,0);glVertex3f(0.5,0.5,1.25);
   glTexCoord2f(1,0);glVertex3f(-0.5,0.5,1.25);
   glTexCoord2f(1,1);glVertex3f(-0.5,1,1.25);
   glTexCoord2f(0,1);glVertex3f(0.5,1,1.25);
   glEnd();
   //cheeks
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(0,0,0);
   glTexCoord2f(0,0);glVertex3f(0.75,0.5,1);
   glTexCoord2f(1,0);glVertex3f(-0.75,0.5,1);
   glTexCoord2f(1,1);glVertex3f(-0.5,0.5,1.25);
   glTexCoord2f(0,1);glVertex3f(0.5,0.5,1.25);
   glEnd();
   // cheeck #2
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(0,0,0);
   glTexCoord2f(0,0);glVertex3f(0.75,0.5,1);
   glTexCoord2f(1,0);glVertex3f(0.75,1,1);
   glTexCoord2f(1,1);glVertex3f(0.5,1,1.25);
   glTexCoord2f(0,1);glVertex3f(0.5,0.5,1.25);
   glEnd();
   // cheeck #3
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(0,0,0);
   glTexCoord2f(0,0);glVertex3f(-0.75,0.5,1);
   glTexCoord2f(1,0);glVertex3f(-0.75,1,1);
   glTexCoord2f(1,1);glVertex3f(-0.5,1,1.25);
   glTexCoord2f(0,1);glVertex3f(-0.5,0.5,1.25);
   glEnd();
   // cheeck #4
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(0,0,0);
   glTexCoord2f(0,0);glVertex3f(-0.75,1,1);
   glTexCoord2f(1,0);glVertex3f(0.75,1,1);
   glTexCoord2f(1,1);glVertex3f(0.5,1,1.25);
   glTexCoord2f(0,1);glVertex3f(-0.5,1,1.25);
   glEnd();
   //body to top
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(-1,1,-0.5);
   glTexCoord2f(1,0);glVertex3f(1,1,-0.5);
   glTexCoord2f(1,1);glVertex3f(0.5,1.5,0);
   glTexCoord2f(0,1);glVertex3f(-0.5,1.5,0);
   glEnd();
   // body to top #2
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(-0.5,1.5,0);
   glTexCoord2f(1,0);glVertex3f(-0.5,1.5,0.5);
   glTexCoord2f(1,1);glVertex3f(-1,1,1);
   glTexCoord2f(0,1);glVertex3f(-1,1,-0.5);
   glEnd();
   // body to top #3
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(-0.5,1.5,0.5);
   glTexCoord2f(1,0);glVertex3f(0.5,1.5,0.5);
   glTexCoord2f(1,1);glVertex3f(1,1,1);
   glTexCoord2f(0,1);glVertex3f(-1,1,1);
   glEnd();
   //body to top #4
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(0.5,1.5,0);
   glTexCoord2f(1,0);glVertex3f(0.5,1.5,0.5);
   glTexCoord2f(1,1);glVertex3f(1,1,1);
   glTexCoord2f(0,1);glVertex3f(1,1,-0.5);
   glEnd();
   //body top
   glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glTexCoord2f(0,0);glVertex3f(-0.5,1.5,0);
   glTexCoord2f(1,0);glVertex3f(0.5,1.5,0);
   glTexCoord2f(1,1);glVertex3f(0.5,1.5,0.5);
   glTexCoord2f(0,1);glVertex3f(-0.5,1.5,0.5);
   glEnd();
   //legs
   glBegin(GL_QUAD_STRIP);
   glColor3d(0,0,0);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(-0.4 + Ir*Sin(thf),0,0.4 + Ir*Cos(thf));
      glVertex3f(-0.4 + Ir*Sin(thf),-0.5,0.4 + Ir*Cos(thf));
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   glColor3d(0,0,0);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(0.4 + Ir*Sin(thf),0,0.4 + Ir*Cos(thf));
      glVertex3f(0.4 + Ir*Sin(thf),-0.5,0.4 + Ir*Cos(thf));
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   glColor3d(0,0,0);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(0.4 + Ir*Sin(thf),0,0.1 + Ir*Cos(thf));
      glVertex3f(0.4 + Ir*Sin(thf),-0.5,0.1 + Ir*Cos(thf));
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   glColor3d(0,0,0);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(-0.4 + Ir*Sin(thf),0,0.1 + Ir*Cos(thf));
      glVertex3f(-0.4 + Ir*Sin(thf),-0.5,0.1 + Ir*Cos(thf));
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glPopMatrix(); 
}

/* 
 * Draw an Okapi
 *
 */
static void okapi(double x, double y, double z, double dx, double dy, double dz, double th)
{
   int thf;
   float Or = 1*sqrt(2.0)-1.5; 
   float Ir = Or-0.01; 
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz); 
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE); 
   glColor3d(1,1,1);
   //Face
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.25,1.5,1.5);
   glTexCoord2f(1,0);glVertex3f(0.25,1.5,1.5);
   glTexCoord2f(1,1);glVertex3f(0.25,2,1.5);
   glTexCoord2f(0,1);glVertex3f(-0.25,2,1.5);
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   //left cheek
   glTexCoord2f(0,0);glVertex3f(-0.25,1.5,1.5);
   glTexCoord2f(1,0);glVertex3f(-0.25,1.25,2);
   glTexCoord2f(1,1);glVertex3f(-0.25,1.35,2.2);
   glTexCoord2f(0,1);glVertex3f(-0.25,2,1.5);
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   //right cheek 
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(0.25,1.5,1.5);
   glTexCoord2f(1,0);glVertex3f(0.25,1.25,2);
   glTexCoord2f(1,1);glVertex3f(0.25,1.35,2.2);
   glTexCoord2f(0,1);glVertex3f(0.25,2,1.5);
   //mouth 
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.25,1.25,2);
   glTexCoord2f(1,0);glVertex3f(0.25,1.25,2);
   glTexCoord2f(1,1);glVertex3f(0.25,1.35,2.2);
   glTexCoord2f(0,1);glVertex3f(-0.25,1.35,2.2);
   // nose
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.25,1.35,2.2);
   glTexCoord2f(1,0);glVertex3f(0.25,1.35,2.2);
   glTexCoord2f(1,1);glVertex3f(0.25,2,1.5);
   glTexCoord2f(0,1);glVertex3f(-0.25,2,1.5);
   // beard
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.25,1.5,1.5);
   glTexCoord2f(1,0);glVertex3f(-0.25,1.25,2);
   glTexCoord2f(1,1);glVertex3f(0.25,1.5,1.5);
   glTexCoord2f(0,1);glVertex3f(0.25,1.25,2);
   // neck
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.5,0.5,1);
   glTexCoord2f(1,0);glVertex3f(-0.25,1.5,1.5);
   glTexCoord2f(1,1);glVertex3f(-0.25,2,1.5);
   glTexCoord2f(0,1);glVertex3f(-0.5,1,1);
   // neck #2
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(0.5,0.5,1);
   glTexCoord2f(1,0);glVertex3f(0.25,1.5,1.5);
   glTexCoord2f(1,1);glVertex3f(0.25,2,1.5);
   glTexCoord2f(0,1);glVertex3f(0.5,1,1);
   // neck #3
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.5,1,1);
   glTexCoord2f(1,0);glVertex3f(-0.25,2,1.5);
   glTexCoord2f(1,1);glVertex3f(0.25,2,1.5);
   glTexCoord2f(0,1);glVertex3f(0.5,1,1);
   //neck #4
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.5,0.5,1);
   glTexCoord2f(1,0);glVertex3f(-0.25,1.5,1.5);
   glTexCoord2f(1,1);glVertex3f(0.25,1.5,1.5);
   glTexCoord2f(0,1);glVertex3f(0.5,0.5,1);
   //body Upper right
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.5,0.5,1);
   glTexCoord2f(1,0);glVertex3f(-0.5,1,1);
   glTexCoord2f(1,1);glVertex3f(-0.75,1.3,0);
   glTexCoord2f(0,1);glVertex3f(-0.75,0,0);
   //body upper left
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(0.5,0.5,1);
   glTexCoord2f(1,0);glVertex3f(0.5,1,1);
   glTexCoord2f(1,1);glVertex3f(0.75,1.3,0);
   glTexCoord2f(0,1);glVertex3f(0.75,0,0);
   //body lower right 
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.75,1.3,0);
   glTexCoord2f(1,0);glVertex3f(-0.75,0,0);
   glTexCoord2f(1,1);glVertex3f(-0.5,0.5,-1);
   glTexCoord2f(0,1);glVertex3f(-0.5,1,-1);
   //body lower left 
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(0.75,1.3,0);
   glTexCoord2f(1,0);glVertex3f(0.75,0,0);
   glTexCoord2f(1,1);glVertex3f(0.5,0.5,-1);
   glTexCoord2f(0,1);glVertex3f(0.5,1,-1);
   //upper top
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.75,1.3,0);
   glTexCoord2f(1,0);glVertex3f(-0.5,1,1);
   glTexCoord2f(1,1);glVertex3f(0.5,1,1);
   glTexCoord2f(0,1);glVertex3f(0.75,1.3,0);
   //lower top
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.75,1.3,0);
   glTexCoord2f(1,0);glVertex3f(0.75,1.3,0);
   glTexCoord2f(1,1);glVertex3f(0.5,1,-1);
   glTexCoord2f(0,1);glVertex3f(-0.5,1,-1);
   //upper bottom
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[7]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.75,0,0);
   glTexCoord2f(1,0);glVertex3f(-0.5,0.5,1);
   glTexCoord2f(1,1);glVertex3f(0.5,0.5,1);
   glTexCoord2f(0,1);glVertex3f(0.75,0,0);
   //lower bottom
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(-0.75,0,0);
   glTexCoord2f(1,0);glVertex3f(0.75,0,0);
   glTexCoord2f(1,1);glVertex3f(0.5,0.5,-1);
   glTexCoord2f(0,1);glVertex3f(-0.5,0.5,-1);
   //back
   glEnd();
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0);glVertex3f(0.5,0.5,-1);
   glTexCoord2f(1,0);glVertex3f(-0.5,0.5,-1);
   glTexCoord2f(1,1);glVertex3f(-0.5,1,-1);
   glTexCoord2f(0,1);glVertex3f(0.5,1,-1); 
   glEnd();
   //left legs
   glBegin(GL_QUAD_STRIP);
   glColor3d(0.5,0.3,0.3);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(0.5 - Ir*Sin(thf),0.5,-0.5 + Ir*Cos(thf));
      glVertex3f(0.5 - Ir*Sin(thf),-0.5,-0.5 + Ir*Cos(thf));
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   glColor3d(0.5,0.3,0.3);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(0.5 - Ir*Sin(thf),0.5,0.5 + Ir*Cos(thf));
      glVertex3f(0.5 - Ir*Sin(thf),-0.5,0.5 + Ir*Cos(thf));
   }
   glEnd();
   //right legs
   glBegin(GL_QUAD_STRIP);
   glColor3d(0.5,0.3,0.3);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(-0.5 + Ir*Sin(thf),0.5,-0.5 + Ir*Cos(thf));
      glVertex3f(-0.5 + Ir*Sin(thf),-0.5,-0.5 + Ir*Cos(thf));
   }
   glEnd();
   glBegin(GL_QUAD_STRIP);
   glColor3d(0.5,0.3,0.3);
   for (thf=0;thf<=360;thf+=15)
   {
      glVertex3f(-0.5 + Ir*Sin(thf),0.5,0.5 + Ir*Cos(thf));
      glVertex3f(-0.5 + Ir*Sin(thf),-0.5,0.5 + Ir*Cos(thf));
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glPopMatrix(); 
   
}

/*
 *  Draw ground
 * 
 */
static void ground(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz); 
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   // ground
   glColor3f(0,1,0);
   glBindTexture(GL_TEXTURE_2D,texture[8]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-4,0,4);
   glTexCoord2f(1,0); glVertex3f(+4,0,+4);
   glTexCoord2f(1,1); glVertex3f(+4,0,-4);
   glTexCoord2f(0,1); glVertex3f(-4,0,-4);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw wall
 * 
 */
static void wall(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz); 
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   //  Left
   glColor3f(1,0,1);
   glBindTexture(GL_TEXTURE_2D,texture[9]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-4,0,4);
   glTexCoord2f(1,0); glVertex3f(-4,0,-4);
   glTexCoord2f(1,1); glVertex3f(-4,1,-4);
   glTexCoord2f(0,1); glVertex3f(-4,1,4);
   glEnd();
   //back
   glBindTexture(GL_TEXTURE_2D,texture[9]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(4,0,-4);
   glTexCoord2f(1,0); glVertex3f(-4,0,-4);
   glTexCoord2f(1,1); glVertex3f(-4,1,-4);
   glTexCoord2f(0,1); glVertex3f(4,1,-4);
   glEnd();
   //right
   glBindTexture(GL_TEXTURE_2D,texture[9]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(4,0,-4);
   glTexCoord2f(1,0); glVertex3f(4,0,4);
   glTexCoord2f(1,1); glVertex3f(4,1,4);
   glTexCoord2f(0,1); glVertex3f(4,1,-4);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   //  Undo transofrmations
   glPopMatrix();
}

/*
 * Draw a tree
 */
static void tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz); 
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   //wood
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-0.1,0, 0.1);
   glTexCoord2f(1,0); glVertex3f(+0.1,0, 0.1);
   glTexCoord2f(1,1); glVertex3f(+0.1,+1, 0.1);
   glTexCoord2f(0,1); glVertex3f(-0.1,+1, 0.1);
   glEnd();
   //  wood #2
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(0.1,0, 0.1);
   glTexCoord2f(1,0); glVertex3f(+0.1,0, -0.1);
   glTexCoord2f(1,1); glVertex3f(+0.1,+1, -0.1);
   glTexCoord2f(0,1); glVertex3f(0.1,+1, 0.1);
   glEnd();
   // wood #3
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(0.1,0, -0.1);
   glTexCoord2f(1,0); glVertex3f(-0.1,0, -0.1);
   glTexCoord2f(1,1); glVertex3f(-0.1,+1, -0.1);
   glTexCoord2f(0,1); glVertex3f(0.1,+1, -0.1);
   glEnd();
   // wood #4
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-0.1,0, -0.1);
   glTexCoord2f(1,0); glVertex3f(-0.1,0, 0.1);
   glTexCoord2f(1,1); glVertex3f(-0.1,+1, 0.1);
   glTexCoord2f(0,1); glVertex3f(-0.1,+1, -0.1);
   glEnd();
   // green base
   glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-0.2,1, 0.2);
   glTexCoord2f(1,0); glVertex3f(0.2,1, 0.2);
   glTexCoord2f(1,1); glVertex3f(0.2,1, -0.2);
   glTexCoord2f(0,1); glVertex3f(-0.2,1, -0.2);
   glEnd();
   //green top 
   glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-0.2,1.5, 0.2);
   glTexCoord2f(1,0); glVertex3f(0.2,1.5, 0.2);
   glTexCoord2f(1,1); glVertex3f(0.2,1.5, -0.2);
   glTexCoord2f(0,1); glVertex3f(-0.2,1.5, -0.2);
   glEnd();
   // green #1
   glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-0.2,1, -0.2);
   glTexCoord2f(1,0); glVertex3f(-0.2,1, 0.2);
   glTexCoord2f(1,1); glVertex3f(-0.2,+1.5, 0.2);
   glTexCoord2f(0,1); glVertex3f(-0.2,+1.5, -0.2);
   glEnd();
   // green #2
   glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(0.2,1, 0.2);
   glTexCoord2f(1,0); glVertex3f(-0.2,1, -0.2);
   glTexCoord2f(1,1); glVertex3f(-0.2,+1.5, -0.2);
   glTexCoord2f(0,1); glVertex3f(0.2,+1.5, -0.2);
   glEnd();
   // green #3
   glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(0.2,1, 0.2);
   glTexCoord2f(1,0); glVertex3f(-0.2,1, 0.2);
   glTexCoord2f(1,1); glVertex3f(-0.2,+1.5, 0.2);
   glTexCoord2f(0,1); glVertex3f(0.2,+1.5, 0.2);
   glEnd();
   // green #4
   glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(0.2,1, -0.2);
   glTexCoord2f(1,0); glVertex3f(-0.2,1, -0.2);
   glTexCoord2f(1,1); glVertex3f(-0.2,+1.5, -0.2);
   glTexCoord2f(0,1); glVertex3f(0.2,+1.5, -0.2);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   //  Undo transofrmations
   glPopMatrix();
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
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   // house_Cube
   //  Front
   // Enable Polygon Offset
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(2,1);
   glColor3d(1,0,0);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);
   //  Back
   glColor3f(0,0,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(0,0,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(0,0,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(0,0,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(0,0,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //house-top
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_TRIANGLES);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1.0,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1/2,1.0); glVertex3d(0,1.5,0);
   glEnd();
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_TRIANGLES);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,+1,-1);
   glTexCoord2f(1.0,0.0); glVertex3f(-1,+1,-1);
   glTexCoord2f(1/2,1.0); glVertex3d(0,1.5,0);
   glEnd();
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_TRIANGLES);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1.0,0.0); glVertex3f(-1,+1,-1);
   glTexCoord2f(1/2,1.0); glVertex3d(0,1.5,0);
   glEnd();
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_TRIANGLES);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1.0,0.0); glVertex3f(+1,+1,-1);
   glTexCoord2f(1/2,1.0); glVertex3d(0,1.5,0);
   glEnd();
   //  Enable Polygon Offset
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1,1);
   //  Door
   glColor3f(0,0,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3d(0.21,-1,1);
   glTexCoord2f(1,0); glVertex3d(-0.21,-1,1);
   glTexCoord2f(1,1); glVertex3d(-0.21,0,1);
   glTexCoord2f(0,1); glVertex3d(0.21,0,1);
   glEnd();
   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);
   //  Door grip
   glColor3f(1,1,1);
   glPointSize(3);
   glBegin(GL_POINTS);
   glVertex3d(0.18,-0.5,1);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   //  Undo transofrmations
   glPopMatrix();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
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
   //  Draw wall
   wall(0,-2,0, 1,1,1, 1);
   //  Draw ground
   ground(0,-2,0, 1,1,1, 1);
   //  Draw 1st tree
   tree(3,-2,-3, 1,1,1, 1);
   //  Draw 2st tree
   tree(-1,-2,-3, 1,1,1, 1);
   //  Draw sheep
   sheep(-1,-1.9,0 , 0.2,0.2,0.3, 1);
   //  Draw okapi
   okapi(3,-1.85,0 , 0.2,0.3,0.3, 1);
   //  Draw pig
   pig(1,-1.57,0 , 1.5,1.5,1.5, 1);
   //  Draw bee
   bee(0,0,0 , 0.5,0.5,0.5, 1);
   //  Draw Jellyfish
   Jellyfish(2,-1.8,0 , 1,1,1, 1);
   //  Draw 3rd house
   house(-3,-1.6,-3 , 0.8,0.4,0.4, 1);
   //  Draw axes - no lighting from here on
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
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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
   glutIdleFunc(idle);   
   //  Load textures
   texture[0] = LoadTexBMP("door.bmp");
   texture[1] = LoadTexBMP("roof.bmp");
   texture[2] = LoadTexBMP("bricks.bmp");
   texture[3] = LoadTexBMP("bee.bmp");
   texture[4] = LoadTexBMP("Jellyfish.bmp");
   texture[5] = LoadTexBMP("sheep.bmp");
   texture[6] = LoadTexBMP("okapi.bmp");
   texture[7] = LoadTexBMP("okapi2.bmp");
   texture[8] = LoadTexBMP("grass.bmp");
   texture[9] = LoadTexBMP("stones.bmp");
   texture[10] = LoadTexBMP("tree.bmp");
   texture[11] = LoadTexBMP("tree2.bmp");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
