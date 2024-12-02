//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Creates a white RGB image with a black line and displays it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************


// Include class files
#include "Image.h"
#include <iostream>

#define MAX_LOADSTRING 100
#define TEST FALSE

// Global Variables:
MyImage			inImage;						// image objects
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	
	char imageName[MAX_PATH] = "";
	int width = 0;
	int height = 0;
	int optionSelect = 0;
	char optionType[3] = "";

	// If no command-line arguments, print error message and terminate
	if (strlen(lpCmdLine) == 0) {
		printf("\nError: No input parameters provided. Terminating.\n");
		return -1;
	}
	else {
		// Parse command-line arguments
		sscanf(lpCmdLine, "%s %d %d %d %s", imageName, &width, &height, &optionSelect, optionType);

		// Ensure all inputs are valid
		if (imageName[0] == 0 || width <= 0 || height <= 0 || optionSelect <= 0 || strlen(optionType) == 0) {
			printf("\nError: Invalid or missing input parameters. Terminating.\n");
			return -1;
		}
	}

	// Create a separate console window to display output to stdout
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	// Print the inputs (either default or parsed from command line)
	printf("Image Name: %s\n", imageName);
	printf("Width: %d\n", width);
	printf("Height: %d\n", height);
	printf("Option Select: %d\n", optionSelect);
	printf("Option Type: %s\n", optionType);

	// Ensure image file is set
	if (imageName[0] == 0) {
		wprintf(L"No command-line argument for image file.");
		return -1;
	}

	// Set up the image with parsed or default dimensions
	inImage.setWidth(width);
	inImage.setHeight(height);
	inImage.setImagePath(imageName);
	inImage.ReadImage();

	int newWidth = 0, newHeight = 0;
	if (strcmp(optionType, "O1") == 0) {
		newWidth = 1920;
		newHeight = 1080;
		printf("1");
	}
	else if (strcmp(optionType, "O2") == 0) {
		newWidth = 1280;
		newHeight = 720;
		printf("2");
	}
	else if (strcmp(optionType, "O3") == 0) {
		newWidth = 640;
		newHeight = 480;
		printf("3");
	}
	else {
		std::cerr << "Invalid output format specified." << std::endl;
		return -1;
	}

	// Determine if we need to upsample or downsample
	if (width >= newWidth && height >= newHeight) {
		// Downsample the image
		if (optionSelect == 1) {
			inImage.DownsampleSpecificPixel(newWidth, newHeight, "top-left");
			printf("4");
		}
		else if (optionSelect == 2) {
			inImage.DownsampleAverage(newWidth, newHeight);
			printf("5");
		}
		else {
			std::cerr << "Invalid resampling method for downsampling." << std::endl;
			return -1;
		}
	}
	else {
		// Upsample the image
		if (optionSelect == 1) {
			inImage.UpscaleNearestNeighbor(newWidth, newHeight);
			printf("6");
		}
		else if (optionSelect == 2) {
			inImage.UpscaleBilinear(newWidth, newHeight);
			printf("7");
		}
		else {
			std::cerr << "Invalid resampling method for upsampling." << std::endl;
			return -1;
		}
	}



	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	// Load accelerators for the program
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// TO DO: part useful to render video frames, may place your own code here in this function
// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case ID_MODIFY_IMAGE:
				   InvalidateRect(hWnd, &rt, false); 
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				// TO DO: Add any drawing code here...
				//char text[1000];
				//strcpy(text, "The original image is shown as follows. \n");
				//DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
				//strcpy(text, "\nUpdate program with your code to modify input image. \n");
				//DrawText(hdc, text, strlen(text), &rt, DT_LEFT);

				BITMAPINFO bmi;
				CBitmap bitmap;
				memset(&bmi,0,sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth = inImage.getWidth();
				bmi.bmiHeader.biHeight = -inImage.getHeight();  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = inImage.getWidth()*inImage.getHeight();

				SetDIBitsToDevice(hdc,
								  0,0,inImage.getWidth(),inImage.getHeight(),
								  0,0,0,inImage.getHeight(),
								  inImage.getImageData(),&bmi,DIB_RGB_COLORS);
							   
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}




// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


