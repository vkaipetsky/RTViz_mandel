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

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "spatial_grid.hpp"

struct AABB
{
  AABB( const Vector3& newMin, const Vector3& newMax )
    : m_min(newMin), m_max(newMax) {}
  Vector3 m_min;
  Vector3 m_max;
};

void drawAABB( const AABB& aabb, unsigned int color = 0xff00ffff ); // draw the AABBs purple by default
void interpolateColor( float r0, float g0, float b0,
                       float r1, float g1, float b1, float t );

float randInRange(float min, float max);
float distanceSquared( const Vector3& a, const Vector3& b );

inline float pow2( float val ) { return val*val; };

AABB cellAABB( int i, int j, int k );
 
#endif /* __UTILS_HPP__ */

