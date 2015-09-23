/*
 * GLUT window class
 *
 * Copyright (C) Fedorenko Maxim <varlllog@gmail.com>
 *               Vlad Kaipetsky <vkaipetsky@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#if 0

#include <windows.h>		// Header File For Windows
#include <math.h>			// Header File For Windows Math Library
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdarg.h>			// Header File For Variable Argument Routines
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#else
#include <GL/freeglut.h>   // Header File For The GLUT Library
#include <GL/gl.h>         // Header File For The OpenGL32 Library
#include <GL/glu.h>        // Header File For The GLu32 Library
#ifdef _MSC_VER
//  #include <glext.h>        // Header file for the glx libraries.
#else
  #include <GL/glx.h>        // Header file for the glx libraries.
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#endif

#ifndef GL_UNSIGNED_SHORT_5_6_5
 #define GL_UNSIGNED_SHORT_5_6_5 0x8363
#endif

//#ifndef GL_CLAMP_TO_EDGE
// #define GL_CLAMP_TO_EDGE 0x812F
//#endif

#include "timer.hpp"
#include "utils.hpp"
#include "glut_window.hpp"

#define NUM_PARTICLES 10000

#define NUM_CELLS_ACROSS_X 32
#define NUM_CELLS_ACROSS_Y 8
#define NUM_CELLS_ACROSS_Z 32

Cell cells[NUM_CELLS_ACROSS_X][NUM_CELLS_ACROSS_Y][NUM_CELLS_ACROSS_Z];
struct Particle particles[NUM_PARTICLES];


const int factor = 4;
const int windowWidth = 64 * factor * 4;
const int windowHeight = 64 * factor * 3;


#define MANDEL_TRY

#ifdef MANDEL_TRY
/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void GLUTWindow::InitGL(int Width, int Height)          // We call this right after our OpenGL window is created.
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);    // This Will Clear The Background Color To Black
  glClearDepth(1.0);        // Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);              // The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);            // Enables Depth Testing
  glShadeModel(GL_SMOOTH);      // Enables Smooth Color Shading
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();        // Reset The Projection Matrix
  
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);  // Calculate The Aspect Ratio Of The Window
  
  glMatrixMode(GL_MODELVIEW);

//  BuildFont();  
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void GLUTWindow::ReSizeGLScene(int Width, int Height)
{
  if (Height==0)        // Prevent A Divide By Zero If The Window Is Too Small
    Height=1;
  
  glViewport(0, 0, Width, Height);    // Reset The Current Viewport And Perspective Transformation
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}
#endif

#ifdef MANDEL_TRY

unsigned char dataBuf[windowWidth*windowHeight];

unsigned short frameBuffer[ windowWidth * windowHeight];

void computeBlock( unsigned char *dataBuf, int rankOffset, int stride, int numRows )
{
  static float angle = 0.0f;
  const float angleCos = cosf( angle );
  const float angleSin = sinf( angle );

//#pragma omp parallel for schedule(dynamic,1)
  for ( int i = 0;  i < windowWidth;  i++ )
  {
    for ( int j = 0;  j < numRows;  j++ )
    {
      // calc the complex plane coordinates for this point

      // project the screen coordinate into the complex plane
      const float rCr = -2.0f + 3.0f * i / windowWidth;
      const float rCi = -1.125f + 2.25f * (j*stride + rankOffset) / windowHeight;

      // and rotate
      const float rotationCenterX = -0.5f;
      const float Cr = rotationCenterX + angleCos * (rCr - rotationCenterX) - angleSin * rCi;
      const float Ci = angleCos * rCi + angleSin * (rCr - rotationCenterX);

      float Zr = 0.0f;
      float Zi = 0.0f;
      
      // and run the Mandelbrot function
      const int iter_count = 19;
      int iter = 0;
      while ( iter < iter_count )
      {
        float Zr_new = Zr * Zr - Zi * Zi + Cr;
        Zi = 2 * Zr * Zi + Ci;
        Zr = Zr_new;
        
        if ( ( Zi * Zi + Zr * Zr ) > 4.0f )
        {
          break;
        }

        ++iter;
      }
      
      dataBuf[ i + j * windowWidth ] = iter_count - iter;
    }
  }

  // rotate slowly
  angle += 0.001f;
}


/* The main drawing function. */
void GLUTWindow::DrawGLScene() {
  static Timer timer;

//  masterTick(0, dataBuf);
  computeBlock( dataBuf, 0, 1, windowHeight );

  double drawTime = timer.elapsed();
  
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);  // Clear The Screen And The Depth Buffer
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5, (windowWidth - 1) + 0.5, (windowHeight - 1) + 0.5, -0.5, -1.0, 1.0);

  glPointSize(1.0f);
  glDisable(GL_POINT_SMOOTH);

  glBegin(GL_POINTS);
  glColor3f(0.0f, 0.0f, 0.0f);

  double pixelComputeTime = timer.elapsed();

  unsigned short *frameBufferPtr = frameBuffer;
  const unsigned char *dataBufPtr = dataBuf;
  for ( int j = 0;  j < windowHeight;  j++ )
  {
    for ( int i = 0;  i < windowWidth;  i++ )
    {
      const unsigned char mandelIter = *dataBufPtr;
      *frameBufferPtr = ( mandelIter << 11 ) | ( mandelIter << 6 ) | mandelIter;
//      *frameBufferPtr = 0x00000000;
      ++dataBufPtr;
      ++frameBufferPtr;
    }
  }

  pixelComputeTime = timer.elapsed() - pixelComputeTime;
  glEnd();

  glRasterPos2f(0.0f, windowHeight-1.0f);
  glDrawPixels( windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, frameBuffer );
  
  // DISABLE DEPTH TEST!
  glDisable( GL_DEPTH_TEST );

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBegin(GL_LINES);

    // draw the 16ms line in white
  glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.016f * 20000.0f, windowHeight-50.0f);
    glVertex2f(0.016f * 20000.0f, windowHeight);

    glEnd();

  // Position The Text On The Screen
  glColor3f(0.2f, 0.5f, 0.7f);
  glRasterPos2f(3.0f, 20.0f);

  char str[1024];
    sprintf( str, "Time elapsed: %8.2lf seconds\n", timer.elapsed());
//    glPrint(str); // print gl text to the screen.
  
  glRasterPos2f(3.0f, 40.0f);
  static int framesRendered = 0;
    sprintf( str, "Average FPS: %8.2lf\n", ++framesRendered / timer.elapsed());
//    glPrint(str); // print gl text to the screen.

  // swap the buffers to display, since double buffering is used.
  glutSwapBuffers();
}
#else
#endif

/* The function called whenever a key is pressed. */
void GLUTWindow::keyPressed( unsigned char key, int x, int y ) {
  /* If escape is pressed, kill everything. */
  if( key == escape_key ) { 
    /* shut down our window */
    glutDestroyWindow(m_window); 
    glutLeaveMainLoop();
  }
}

/* For callbacks */
GLUTWindow *g_current_instance;
extern "C" void drawCallback() {
    g_current_instance->DrawGLScene();
}
extern "C" void resizeCallback( int w, int h ) {
    g_current_instance->ReSizeGLScene( w, h );
}
extern "C" void keyCallback( unsigned char key, int x, int y ) {
    g_current_instance->keyPressed( key, x, y );
}

GLUTWindow::GLUTWindow( int argc, char **argv,  
                        int w_width /* = 1024 */, int w_height /* = 768 */ )
  : m_w_width(w_width),
    m_w_height(w_height)
{
  g_current_instance = this;
  
  m_rotate_x = 30.0f;
  m_rotate_y = 0.0f;
  
  /* Initialize GLUT state */  
  glutInit(&argc, argv);  
    
  /* Select type of Display mode:   
     Double buffer 
     RGBA color
     Alpha components supported 
     Depth buffered for automatic clipping */  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

  /* get a window */
  glutInitWindowSize( w_width, w_height );  

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);  

  /* Open a window */  
  m_window = glutCreateWindow("Simple OpenGL-based 2D real-time rendering framework");

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(::drawCallback);

  /* Go fullscreen. This is as soon as possible. */
  //  glutFullScreen();

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(::drawCallback);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(::resizeCallback);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(::keyCallback);

  /* Initialize our window. */
  InitGL( w_width, w_height );

  /* This will make sure that GLUT doesn't kill the program 
   * when the window is closed by the OS */
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
      GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}


void GLUTWindow::GLUTMainLoop()
{
  /* Start Event Processing Engine */  
  glutMainLoop();
}
