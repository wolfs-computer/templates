#include <X11/Xlib.h>


#ifndef GFX_H
#define GFX_H


// to use it in my program
typedef struct _x11_data {
    Display *gfx_display;
    Window  gfx_window;
    GC      gfx_gc;
    Colormap gfx_colormap;
    int      gfx_fast_color_mode;
} x11_data;


x11_data get_x11_data();


/* Open a new graphics window. */
void gfx_open( int width, int height, const char *title );

/* Draw a point at (x,y) */
void gfx_point( int x, int y );

/* Draw a line from (x1,y1) to (x2,y2) */
void gfx_line( int x1, int y1, int x2, int y2 );

/* Change the current drawing color. */
void gfx_color( int red, int green, int blue );

/* Clear the graphics window to the background color. */
void gfx_clear();

/* Change the current background color. */
void gfx_clear_color( int red, int green, int blue );

/* Wait for the user to press a key or mouse button. */
int gfx_wait();

/* Return the X and Y coordinates of the last event. */
int gfx_xpos();
int gfx_ypos();

/* Return the X and Y dimensions of the window. */
int gfx_xsize();
int gfx_ysize();

/* Check to see if an event is waiting. */
int gfx_event_waiting();

/* Flush all previous output to the window. */
void gfx_flush();



void check_events(XEvent *event);


#endif
