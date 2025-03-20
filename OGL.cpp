
/******************************************* 
	Sun 3D Noise 
	Date : 25.07.2024 - 14.12.2024
*******************************************/

// Common Header Files
#include<windows.h>
#include<stdio.h> 
#include<stdlib.h> // for exit() 

#include"OGL.h"

// for PlaySound() api
#include <mmsystem.h>	

// opengl header Files
#include<gl/glew.h> // this must be before gl/GL.h
#include<gl/GL.h>

#include"vmath.h"
using namespace vmath;

// For Sphere
#include"Sphere.h"

// Macrose
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// link with OpenGL library
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"Sphere.lib")     // for Sphere 

// openGl related global variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXTURE0
};

// For Sphere 
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

GLuint NumVertices = 0;
GLuint NumElements = 0;

mat4 perspectiveProjectionMatrix; // mat4 in vmath.h 

//GLfloat lightPosition[] = { 0.0, 2.0, 4.0, 1.0 };
GLfloat gScaleFactor = 1.0f; 
BOOL    isScaled = TRUE;

GLfloat MovingAngle_1 = -90.0f;

GLfloat Width = 0.0f;
GLfloat Height = 0.0f;

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable declarations
HWND ghwnd = NULL;  
DWORD dwStyle = 0;

WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
BOOL gbfullscreen = FALSE; 

FILE* gpFILE = NULL;
BOOL gbActive = FALSE;

GLuint cnt = 0;

HINSTANCE hInst = NULL;


// for Noise 
#include"Noise.h"

#include"1_Sun.h"
#include"2_Cloud.h"
#include"3_Marble.h"
#include"4_Wood.h"



// Entry point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{ 
	// Function Declarations 
	int initialize(void);
	void uninitialize(void);
	void display(void); 
	void update(void);

	// local variable declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("SVDWindow"); 
	int iResult = 0;  // for base code
	BOOL bDone = FALSE; 

	// Local Variable for finding the center of the window
	int SW = 0;   // Screen width
	int SH = 0;   // Screen Height

	float x = 0.0f;
	float y = 0.0f;

	// Code
	//gpFILE = fopen("log.txt","w");
	//if (gpFILE == NULL)
	
	if(fopen_s(&gpFILE, "log.txt", "w") != 0) // success zal tr 0 return krt nhitr non zero return krt 
	{
		MessageBox(NULL, TEXT("Log File cannot be opend..."), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(gpFILE, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
	}

	fprintf(gpFILE , "Program Started Successfully... \n");
	fprintf(gpFILE, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");


	// WNDCLASSEX initialization  
	// structer ch inline initialization

	wndclass.cbSize			= sizeof(WNDCLASSEX);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wndclass.cbWndExtra		= 0;
	wndclass.cbClsExtra		= 0;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.hInstance		= hInstance;
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor		= LoadCursor(NULL,IDC_ARROW);
	wndclass.lpszClassName	= szAppName;
	wndclass.lpszMenuName	= NULL;
	wndclass.hIconSm		= LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));


	// Register WNDCLASSEX 
	RegisterClassEx(&wndclass);

	// Centering of window code
	SW = GetSystemMetrics(SM_CXSCREEN); 
	SH = GetSystemMetrics(SM_CYSCREEN); 

	x = (SW / 2.0) - (WIN_WIDTH / 2.0); 
	y = (SH / 2.0) - (WIN_HEIGHT / 2.0); 


	// Create Window
	hwnd = CreateWindowEx( WS_EX_APPWINDOW,
						 szAppName,
						 TEXT("Saurabh Dhoke"),
						 WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, 
						 x,
				   		 y,
		                 WIN_WIDTH,  
		                 WIN_HEIGHT,  
						 NULL,
						 NULL,
						 hInstance,
						 NULL );

	// gloabal handel madhi value takli 
	ghwnd = hwnd; 

	//initialization
	iResult = initialize();

	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() Failed..."), TEXT("Error"), MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);

		//uninitialize();
		//exit(0);	//  yala call kela tri chalel
	}

	// Show the Window
	ShowWindow(hwnd,iCmdShow); 
	
	SetForegroundWindow(hwnd); 
	SetFocus(hwnd); 

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActive == TRUE)  // setfocus mdhi true zalelay he gbActive 
			{
				// Render
				display();

				// Update 
				update();

			}
		}
	}

	// uninitialization
	uninitialize();
	
	return((int)msg.wParam);  
}

// CALLBACK function
LRESULT CALLBACK WndProc(HWND   hwnd, UINT   iMsg, WPARAM wParam, LPARAM lParam)
{
	// Function declarations
	void ToggleFullscreen(void);
	void resize(int,int); 


	// Code

	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE; 
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE; 
		break;

	case WM_SIZE: //  Width           Height
		resize(LOWORD(lParam), HIWORD(lParam)); 
		break;

	case WM_ERASEBKGND:  // os ch WS_Paint vaparayach nahiy mhanun he vapralay 
		return(0);

	case WM_KEYDOWN :
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE :
			DestroyWindow(hwnd);
			break;
		}
		break;
	
	case WM_CHAR :
		switch (LOWORD(wParam))
		{
		case 'F' :
		case 'f' :
			if (gbfullscreen == FALSE)
			{
				ToggleFullscreen();
				gbfullscreen = TRUE;
			}
			else
			{
				ToggleFullscreen();
				gbfullscreen = FALSE; 
			}
			break;
		}
		break;

	case WM_DESTROY : 
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	default :
		break;
	}

	return(DefWindowProc(hwnd,iMsg,wParam,lParam));
}
void ToggleFullscreen(void)
{
	// Local Variable Declarations
	MONITORINFO mi = { sizeof(MONITORINFO) };

	// Code
	if (gbfullscreen == FALSE)
	{	
		dwStyle = GetWindowLong(ghwnd,GWL_STYLE);
		
		if (dwStyle & WS_OVERLAPPEDWINDOW) 
		{	
			if (GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{    
				SetWindowLong(ghwnd,GWL_STYLE,dwStyle & ~WS_OVERLAPPEDWINDOW); // dwStyle shodh aani WS_OVERLAPPEDWINDOW kadhun tak (~ remove karto)                             
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);			
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED); // window placement allredy set zali first line la mhanun 0 dilet
		ShowCursor(TRUE); 
	}
}
int initialize(void)
{
	// Function Declarations
	void printGLInfo(void);
	void uninitialize();
	void resize(int, int);

	// Code
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	
	ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

	// step 1 - initialization of pixel format descriptor
	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;  /// ???????????? PFD_RGBA
	pfd.cColorBits = 32;
	pfd.cRedBits   = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits  = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	// step 2 - get the DC (device context)
	ghdc = GetDC(ghwnd);
	if (ghdc == 0)
	{
		fprintf(gpFILE, "GetDC() failed.. \n\n");
		return -1;
	}

	// step 3
	iPixelFormatIndex = ChoosePixelFormat(ghdc,&pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFILE, "ChoosePixelFormat() failed.. \n\n");
		return -2;
	}

	// Step 4
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat() failed.. \n\n");
		return -3;
	}

	// tell wgl library to give opengl compatable device context to this device context
	// create opengl context from device context 
	ghrc = wglCreateContext(ghdc);
	if (ghrc == 0)
	{
		fprintf(gpFILE, "wglCreateContext() failed.. \n\n");
		return -4;
	}

	// Make rendering Context current 
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE, "wglMakeCurrent() failed.. \n\n");
		return -5;
	}

	//----------------------------------------------------
	// PP cha code ya shivay eligible hot nahi 
	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFILE, "glewInint() failed.. \n\n");
		return -6;
	}

	//-----------------------------------
	// Print OpenGL info 
	// printGLInfo();
	//-----------------------------------
	
	initialize_Sun();
	initialize_Cloud();
	initialize_Marble();
	initialize_Wood();

	 glEnable(GL_TEXTURE_3D);
	//--------------------------------------------------------------------------------

	// Enabling Depth
	glClearDepth(1.0f);							// compulsary 
	glEnable(GL_DEPTH_TEST);					// compulsary
	glDepthFunc(GL_LEQUAL);						// compulsary

	// here openGl starts
	// Set the clear color of window to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//-----------------------------
	PlaySound(TEXT("soundName"), hInst, SND_RESOURCE | SND_ASYNC);
	//-----------------------------
	
	// initialize orthograficprojection matrix
	perspectiveProjectionMatrix = vmath::mat4::identity();

	// warm up resize call
	resize(WIN_WIDTH, WIN_HEIGHT);
	return(0);
}

void printGLInfo(void)
{
	// Local Variable declarations 
	GLint Num_Extensions;  // As a Array Use krtoy aapan ith
	GLint i = 0;

	// Code 
	fprintf(gpFILE, "OpenGL Vendor   : %s \n", glGetString(GL_VENDOR));
	fprintf(gpFILE, "OpenGL Rendorer : %s \n", glGetString(GL_RENDERER)); 
	fprintf(gpFILE, "OpenGL Version  : %s \n", glGetString(GL_VERSION)); 
	fprintf(gpFILE, "GLSL Version    : %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));   // Grafic library shading language

	// Listing of Supported Extensions 
	glGetIntegerv(GL_EXTENSIONS, &Num_Extensions); 

	for (i = 0; i < Num_Extensions; i++)
	{
		fprintf(gpFILE, "%s \n", glGetStringi(GL_EXTENSIONS, i));  
	}

}

void resize(int width, int height)
{
	// Code

	if (height <= 0) // precausion (height na division krnar y pudh )
		height = 1;

	// set Perspective projection matrix
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	Width = width;
	Height = height;
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // glViewport = binocular - durbin
}

void display(void)
{
	display_Sun();
	display_Cloud();
	display_Marble();
	display_Wood();

	SwapBuffers(ghdc);

	update_Sun();
	update_Cloud();
	update_Marble();
	update_Wood();
}

GLfloat increment = 0.062f;

void update(void)
{
	// Code

	if (MovingAngle_1 <= 90.0f)
	{
		MovingAngle_1 = MovingAngle_1 + increment;
	}
	else
	{
		MovingAngle_1 = -90.0f;

		if (cnt < 4)
		{
			cnt = cnt + 1;
		}
		else
		{
			cnt = 0;
			increment = 0.14f;
		}
	}
}

void uninitialize(void)
{
	// Function Declarations
	void ToggleFullscreen(void);
	
	// Code

	unintialize_Sun();
	unintialize_Cloud();
	unintialize_Marble();

	unintialize_Wood();
		

	// if application is exiting in fullDcreen
	if (gbfullscreen == TRUE)    
	{
		ToggleFullscreen();     // jar kuni madhich esc kel tr window small kr fullscreen asl tr an mng close kr 
		gbfullscreen = FALSE; 
	}

	// Make the hdc as the current dc
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// destroy / delte rendering context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// release the HDC 
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// Destroy Window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}
	
	// Close the log file 
	if (gpFILE)
	{	
		fprintf(gpFILE, "\n\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n");
		fprintf(gpFILE, "Program Ended Successfully...\n"); 
		fprintf(gpFILE, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n\n");
		fclose(gpFILE);
		gpFILE = NULL;
	}

}

                                                                                                        



