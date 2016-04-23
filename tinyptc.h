//
// TinyPTC by Gaffer
// www.gaffer.org/tinyptc
//

#ifndef __TINYPTC_WINDOWS_H
#define __TINYPTC_WINDOWS_H


// integer types
typedef unsigned int int32;
typedef unsigned short short16;
typedef unsigned char char8;

// tinyptc api
extern int ptc_open(char *title,int width,int height);
extern int ptc_update(void *buffer);
extern void ptc_close();
//extern void ptc_mouse(int * mov_x, int * mov_y);

// display target
//#define __PTC_DDRAW__
#define __PTC_GDI__
//#define __PTC_VFW__

// configuration
#define __PTC_WINDOWED__
#define __PTC_CENTER_WINDOW__
#define __PTC_RESIZE_WINDOW__
#define __PTC_SYSTEM_MENU__
#define __PTC_ALLOW_CLOSE__
#define __PTC_CLOSE_ON_ESCAPE__
#define __PTC_DISABLE_SCREENSAVER__


#endif
