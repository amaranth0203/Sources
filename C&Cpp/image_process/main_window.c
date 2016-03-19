#include <windows.h>
#include <time.h>
#include "funcs.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawPixels(HWND hwnd);
static    double* raw_data ;
static    double* R ;
static    double* G ;
static    double* B ;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow) {

    MSG  msg;
    WNDCLASSW wc = {0};

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = L"Pixels";
    wc.hInstance     = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = WndProc;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&wc);
    CreateWindowW(wc.lpszClassName, L"Pixels",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                0, 0, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);
/*
 *    CreateWindowW(wc.lpszClassName, L"Pixels",
 *                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
 *                0, 0, 500, 500, NULL, NULL, hInstance, NULL);
 *
 */
    while (GetMessage(&msg, NULL, 0, 0)) {
    
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    srand(time(NULL));

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    switch(msg) {
        case WM_CREATE:
            raw_data = ( double* )malloc( WIDTH * HEIGHT * sizeof( double ) ) ;
            R = ( double* )malloc( WIDTH * HEIGHT * sizeof( double ) ) ;
            G = ( double* )malloc( WIDTH * HEIGHT * sizeof( double ) ) ;
            B = ( double* )malloc( WIDTH * HEIGHT * sizeof( double ) ) ;
            read_raw_to_array( FILENAME , raw_data ) ;
            memcpy( R , raw_data , WIDTH * HEIGHT * sizeof( double ) ) ;
            memcpy( G , raw_data , WIDTH * HEIGHT * sizeof( double ) ) ;
            memcpy( B , raw_data , WIDTH * HEIGHT * sizeof( double ) ) ;
            wprintf( L"[+] create\n" ) ;
            break ;

        case WM_PAINT:

            DrawPixels(hwnd);
            break;

        case WM_DESTROY:

            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void DrawPixels(HWND hwnd) {

    PAINTSTRUCT ps;
    RECT r;

    GetClientRect(hwnd, &r);

    if (r.bottom == 0) {
    
        return;
    }

    HDC hdc = BeginPaint(hwnd, &ps);


/*
 *    int i ;
 *    for ( i=0; i<1000; i++) {
 *
 *        int x = rand() % r.right;
 *        int y = rand() % r.bottom;
 *        SetPixel(hdc, i, i, RGB(0, 0, 255));
 *    }
 */

    int i , j ;
    for( i = 0 ; i < HEIGHT ; i ++ ) {
        for( j = 0 ; j < WIDTH ; j ++ ) {
            SetPixel( hdc , j , i , RGB( 
                *(R+i*WIDTH+j) , 
                *(G+i*WIDTH+j) , 
                *(B+i*WIDTH+j) 
            ) ) ;
        }
    }

    EndPaint(hwnd, &ps);
}