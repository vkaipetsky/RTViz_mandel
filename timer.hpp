/*
 * Simple timer class.
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

#ifndef __TIMER_H__
#define __TIMER_H__

// simple Timer class, Jan 2010, thomas{AT}thomasfischer{DOT}biz
// tested under windows and linux
// license: do whatever you want to do with it ;)

#ifndef WIN32
#include <sys/time.h>
#else
#include <windows.h>
#endif

class Timer
{
protected:
#ifdef WIN32
   LARGE_INTEGER start;
#else
   struct timeval start;
#endif

public:
   Timer()
   {
       restart();
   }

   double elapsed()
   {
#ifdef WIN32
       LARGE_INTEGER tick, ticksPerSecond;
       QueryPerformanceFrequency(&ticksPerSecond);
       QueryPerformanceCounter(&tick);
       return ((double)tick.QuadPart - (double)start.QuadPart) / (double)ticksPerSecond.QuadPart;
#else
       struct timeval now;
       gettimeofday(&now, NULL);
       return (now.tv_sec - start.tv_sec) + (now.tv_usec - start.tv_usec)/1000000.0;
#endif
   }

   void restart()
   {
#ifdef WIN32
       QueryPerformanceCounter(&start);       
#else
       gettimeofday(&start, NULL);
#endif
   }
};


#endif /* __TIMER_H__ */

