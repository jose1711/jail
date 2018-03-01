/* Copyright (C) 2004,2005 David Antliff <dave.antliff@paradise.net.nz>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *
 *
 * This program allows you to block the mouse a specified area of your
 * Desktop.
 * Usage: ./Jail X1 Y1 X2 Y2
 *
 * Compile with:
 *  gcc -g Jail.c -o Jail -L /usr/X11R6/lib -lX11 -lXtst -lXext
 * 
 * This program was heavily based on switchscreen, by David Antliff.
 * Created by Sebastien Marion <seb.marion@gmail.com>
 * 
 * Original program bz David Antliff modified to take in coordinates
 * instead of whole X Screen
 * (for those twinView guys out there)
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>



/******************************************************************************
FUNCTION    : main
DESCRIPTION :
******************************************************************************/
int main(int argc, char ** argv)
{
    Display * display;
    int majorOpcode, firstEvent, firstError;
	char * displayName;
    int thisScreen;
    int otherScreen = -1;
    int CX1, CY1, CX2, CY2;
	
    if (argc < 5) {
	    fprintf(stderr, "Usage: %s X1 Y1 X2 Y2\n", argv[0]);
	    return 0;
    }
    
    CX1 = atoi(argv[1]);
    CY1 = atoi(argv[2]);
    CX2 = atoi(argv[3]);
    CY2 = atoi(argv[4]);
    

    displayName = getenv("DISPLAY");
    if (displayName == NULL)
    {
        fprintf(stderr, "DISPLAY is not set\n");
        exit(1);
    }

    fprintf(stderr, "Opening display %s\n", displayName);
    display = XOpenDisplay(displayName);


    if (!XQueryExtension(display,
                         XTestExtensionName,
                         &majorOpcode,
                         &firstEvent,
                         &firstError))
    {
        fprintf(stderr, "XTEST extension not available\n");
        return (1);
    }


    thisScreen = DefaultScreen(display);
    if (otherScreen == -1)
    {
        otherScreen = 1 - DefaultScreen(display);
    }

    fprintf(stderr, "thisScreen: %d -- otherScreen: %d", thisScreen, otherScreen);
    if (otherScreen != thisScreen)
    {
 
        int root_x, root_y;
        int previous_x, previous_y;
        int win_x, win_y;
        unsigned int mask;
        Window root, child;
//        Window focus = 0;
//        int revert_to = 0;

//		fprintf(stderr, "Window ID is %x, focus is %x\n", (int)focus, revert_to);
		
//		int width  = DisplayWidth (display, thisScreen);
//		int height = DisplayHeight(display, thisScreen);
		
//        fprintf(stderr, "width: %d, height: %d", width, height);
		int modif = 0;
		while (1){
	        // get current mouse coordinates
	        XQueryPointer(display,
	                      RootWindow(display, thisScreen),
	                      &root,
	                      &child,
	                      &root_x,
	                      &root_y,
	                      &win_x,
	                      &win_y,
	                      &mask);
           
			//only modify 1 coordinate when possible..
			// lets the mouse move more 'freely'
			// at the edge
			   if (root_x < CX1)  { 	
		   		root_x = CX1;
				modif = 1;
			   } else if (root_x > CX2) {
				root_x = CX2;
				modif = 1;
			   } else if (root_y < CY1) {
				root_y = CY1;
				modif = 1;
			   } else if (root_y > CY2) {
				root_y = CY2;
				modif = 1;
			   };
		   	       
		//fprintf(stderr,"display: %d, thisScreen: %d, root: %d, root_x: %d, root_y: %d, OriginalRoot: %d\n",display,thisScreen,root, root_x,root_y,OriginalRoot);               
	       if (modif == 1){
		       	XTestFakeMotionEvent(display, thisScreen, root_x, root_y, CurrentTime);
		      // 	fprintf(stderr,"MODIFYING COORDINATES\n",thisScreen,root_x,root_y);               
	       }
	       usleep(1000);
	       modif=0;
		}
	
    }
    XCloseDisplay(display);

    return (0);
}
