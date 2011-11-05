#include <windows.h>
#include <iostream>

#include "RayTracer.h"
#include "ImageBuffer.h"
#include "Colour.h"
#include "ImageIO.h"
#include "TRACE.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////
LPCWSTR szClassName = L"RayTracerDisplay"; // Make the class name into a global variable

int defaultWidth = 640;		// Default image window width and height
int defaultHeight = 360;
RayTracer *rt = NULL;		// Ray tracer object
ImageBuffer *buf = NULL;	// Buffer for rendered image
HDC memDC;					// Off screen window buffer, write imagebuffer here, then copy to display



////////////////////////////////////////////////////////////////////////////////
// Function Declarations
////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM); // Message callback




int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;			// Main window handle
    MSG messages;		// Messages to the application are saved here
    WNDCLASSEX wincl;	// Window class for main window
    DWORD windowStyle;	// Window style

    // Set main window class properties
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;	// Window call-back function
    wincl.style = CS_DBLCLKS;				// Catch double clicks
    wincl.cbSize = sizeof (WNDCLASSEX);

    // Use default icon and mouse-pointer
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;	// No window menus
    wincl.cbClsExtra = 0;		// No extra bytes after window class (whatever that means)
    wincl.cbWndExtra = 0;		// Structure of the window instance (whatever that means too)
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;	// Use default window background colour

    // Register the window class, and if it fails quit the program
    if (!RegisterClassEx (&wincl))
        return 0;

	// Set window to be non resizable
	windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	// Adjust the window size (for borders, etc.)
	RECT wr = {0, 0, defaultWidth, defaultHeight}; // Default window rect size
	AdjustWindowRect(&wr, windowStyle, FALSE);

    // Window class and style are set, create the main program window
    hwnd = CreateWindowEx (
           0,						// Extended possibilites for variation (what?)
           szClassName,				// Classname
           L"Ray Tracer Display",	// Window title Text
           windowStyle,				// Set window style
           CW_USEDEFAULT,			// Windows decides the starting window position
           CW_USEDEFAULT,
           wr.right - wr.left,		// Window width and height based on window rect wr
           wr.bottom - wr.top,		// Window height
           HWND_DESKTOP,			// The window is a child-window to desktop
           NULL,					// No menu
           hThisInstance,			// Program Instance handler
           NULL						// No Window Creation data */
           );
	
	// Create the compatible memory context
	memDC = CreateCompatibleDC(GetDC(hwnd));



	
	// Initialize the ray tracer object
	rt = new RayTracer();
	buf = new ImageBuffer(defaultWidth, defaultHeight);
	buf->rainbowStatic();
	//rt->render(buf);
	//writeJPG("..\\image.jpg", buf);





    // Make the window visible
    ShowWindow (hwnd, nCmdShow);

    // Run the message loop. It will run until GetMessage() returns 0
    while (GetMessage (&messages, NULL, 0, 0))
    {
        // Translate virtual-key messages into character messages
        TranslateMessage(&messages);
        // Send message to WindowProcedure
        DispatchMessage(&messages);
    }

    // The program return-value is 0 - The value that PostQuitMessage() gave
    return messages.wParam;
}




//  This function is called by the Windows function DispatchMessage()
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // Make a message pop up
        /*
        case WM_LBUTTONDOWN:
        {
            char szFileName[MAX_PATH];
            HINSTANCE hInstance = GetModuleHandle(NULL);
            GetModuleFileName(hInstance, szFileName, MAX_PATH);
            MessageBox(hwnd, szFileName, "This program is: ", MB_OK | MB_ICONINFORMATION);
        }
            break;
		*/
		case WM_LBUTTONDOWN:
		{
			// Create a buffer the size of the window to render to
			delete buf;
			RECT cr;
			GetClientRect(hwnd, &cr);
			buf = new ImageBuffer(cr.right-cr.left, cr.bottom-cr.top);

			// Ray trace an image!
			rt->render(buf);
			
			// Write image to jpeg file
			writeJPG("..//image.jpg",buf);

			// Copy the rt image to the offscreen buffer which will be copied to the window
			HBITMAP memBmp = CreateCompatibleBitmap(GetDC(hwnd),buf->width(),buf->height());
			if(!SetBitmapBits(memBmp, buf->width()*buf->height()*4, buf->getCharArray()))
				TRACE("Could not display image. Image buffer could not be converted to bitmap.\n");
			
			HGDIOBJ temp = SelectObject(memDC,memBmp);
			DeleteObject(temp);

			// Repaint the display window
			InvalidateRect(hwnd, NULL, true);
		}
		break;
		case WM_PAINT:
		{
			// Copy the offscreen buffer to the window
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			
			BitBlt(hdc, 0, 0, buf->width(), buf->height(), memDC, 0, 0, SRCCOPY);

			EndPaint(hwnd, &ps);
		}
		break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
		break;
        case WM_DESTROY:
            PostQuitMessage (0);	// send a WM_QUIT to the message queue
		break;
        default:					// for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
