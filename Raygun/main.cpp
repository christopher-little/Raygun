#include <windows.h>
#include <iostream>

#include "RayTracer.h"
#include "ImageBuffer.h"
#include "Colour.h"
#include "TRACE.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////
LPCWSTR szClassName = L"RayTracerDisplay"; // Make the class name into a global variable

RayTracer *rt = NULL; // Ray tracer object
ImageBuffer *buf = NULL; // Buffer for rendered image



////////////////////////////////////////////////////////////////////////////////
// Function Declarations
////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM); // Message callback




int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    DWORD windowStyle;		// Window style

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

	// Set window to be non resizable
	windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	// Adjust the window size (for borders, etc.)
	RECT wr = {0, 0, 800, 800}; // Default window rect size
	AdjustWindowRect(&wr, windowStyle, FALSE);

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           L"Ray Tracer Display",	// Title Text
           windowStyle, 		// Set window style
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           wr.right - wr.left,		// Window width
           wr.bottom - wr.top,		// Window height
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );



	
	// Initialize the ray tracer object
	rt = new RayTracer();




    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
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
			RECT cr;
			GetClientRect(hwnd, &cr);
			buf = new ImageBuffer(cr.right-cr.left, cr.bottom-cr.top);

			// Ray trace an image!
			rt->render(buf);

			// Grow window for fun
			//GetWindowRect(hwnd, &cr);
			//SetWindowPos(hwnd, NULL, cr.left, cr.top, (cr.right-cr.left)+10, (cr.bottom-cr.top)+10, NULL);

			// Repaint the display window
			InvalidateRect(hwnd, NULL, true);
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			if(buf == NULL)
			{
				// Throw in some pixel colours
				for(int row=0; row<64; row++)
				{
					for(int col=0; col<64; col++)
					{
						SetPixel(hdc, col, row, RGB(255,255,0));
					}
				}
			}
			else
			{
				// Draw the rt image
				for(int row=0; row<buf->height(); row++)
				{
					for(int col=0; col<buf->width(); col++)
					{
									  //X  //Y
						SetPixel(hdc, col, row, buf->getPixel(row, col).getWinRGB());
					}
				}
			}
			//BitBlt(hdc, 0, 0, wr.right-wr.left, wr.bottom-wr.top, hdcDisplayBitmap, 0, 0, SRCCOPY);
			EndPaint(hwnd, &ps);
		}
		break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
		break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
		break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
