/*
 * Some structs, class and function
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

#include <GL/freeglut.h>   // Header File For The GLUT Library

#include <stdlib.h>

#include "utils.hpp"

float randInRange(float min, float max)
{
  return min + (max - min) * rand() / (float)RAND_MAX;
}

float distanceSquared( const Vector3& a, const Vector3& b )
{
  return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z);
}

AABB cellAABB( int i, int j, int k )
{
  const float offsetX = -2.0f;
  const float offsetZ = -2.0f;
  const float cellSize = 8.0f / 64.0f;
  AABB res( Vector3( offsetX + i * cellSize, j * cellSize, offsetZ + k * cellSize ),
            Vector3( offsetX + (i+1) * cellSize, (j+1) * cellSize, offsetZ + (k+1) * cellSize ) );
  return res;
}

void drawAABB( const AABB& aabb, unsigned int color /* = 0 */ )
{
  glBegin(GL_LINES);
  // draw the 12 lines that make up this box:
  // bottom 4 lines
  glVertex3f( aabb.m_min.x, aabb.m_min.y, aabb.m_min.z );
  glVertex3f( aabb.m_max.x, aabb.m_min.y, aabb.m_min.z );

  glVertex3f( aabb.m_max.x, aabb.m_min.y, aabb.m_min.z );
  glVertex3f( aabb.m_max.x, aabb.m_min.y, aabb.m_max.z );

  glVertex3f( aabb.m_max.x, aabb.m_min.y, aabb.m_max.z );
  glVertex3f( aabb.m_min.x, aabb.m_min.y, aabb.m_max.z );

  glVertex3f( aabb.m_min.x, aabb.m_min.y, aabb.m_max.z );
  glVertex3f( aabb.m_min.x, aabb.m_min.y, aabb.m_min.z );
  // top 4 lines
  // connecting 4 lines
  glEnd();
}

void interpolateColor( float r0, float g0, float b0,
                       float r1, float g1, float b1, float t )
{
  glColor3f( r0 * (1.0f-t) + r1 * t, g0 * (1.0f-t) + g1 * t, b0 * (1.0f-t) + b1 * t );
}


