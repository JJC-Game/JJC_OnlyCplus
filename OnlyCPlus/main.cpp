#include <windows.h>
#pragma	comment(lib,"Gdiplus.lib")
#include <ole2.h>
#include <gdiplus.h>
#include <iostream>
#include <random>
#include <string>
#include <tchar.h>
using namespace Gdiplus;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
class GameManager;
class Bullet;

GameManager* g_gameManager;

#define SCREEN_W (1280)
#define SCREEN_H (720)

#define CHARA_W (282)
#define CHARA_H (380)

#define TEXT_NUM (4)




class GameManager {
public:
    GameManager() {
        _tcscpy_s(m_text[0], 300 * sizeof(TCHAR), TEXT("�l�͒j�̎q�I"));
        _tcscpy_s(m_text[1], 300 * sizeof(TCHAR), TEXT("���͏��̎q�I"));
        _tcscpy_s(m_text[2], 300 * sizeof(TCHAR), TEXT("�����[���I"));
        _tcscpy_s(m_text[3], 300 * sizeof(TCHAR), TEXT("�I���"));
        m_textId = 0;
    }


    void Update(HWND hwnd) {
    }

    void Next(HWND hwnd) {
        m_textId++;
        if (m_textId >= TEXT_NUM) {
            m_textId = TEXT_NUM;
        }
        InvalidateRect(hwnd, NULL, FALSE);
    }

    void Draw(Graphics* graphics) {
        DrawBackGround(graphics);
        DrawCharacter(graphics);
        DrawTextBox(graphics);
    }

    void DrawBackGround(Graphics* graphics) {
        Image image(L"background.png");
        graphics->DrawImage(&image, 0, 0, SCREEN_W, SCREEN_H);
    }

    void DrawTextBox(Graphics* graphics) {
        Image image(L"text_box.png");
        graphics->DrawImage(&image, 0, 0, SCREEN_W, SCREEN_H);

        if (m_textId < TEXT_NUM) {
            TextOut(graphics->GetHDC(), 100, 450, m_text[m_textId], lstrlen(m_text[m_textId]));
        }
    }

    void DrawCharacter(Graphics* graphics) {
        switch (m_textId) {
        case 0:
        {
            Image man_image(L"man_normal.png");
            graphics->DrawImage(&man_image, SCREEN_W / 2 - CHARA_W / 2 + 300, SCREEN_H / 2 - CHARA_H / 2 - 120, CHARA_W, CHARA_H);
        }
        break;
        case 1:
        {
            Image man_image(L"man_normal.png");
            Image woman_image(L"woman_normal.png");
            graphics->DrawImage(&man_image, SCREEN_W / 2 - CHARA_W / 2 + 300, SCREEN_H / 2 - CHARA_H / 2 - 120, CHARA_W, CHARA_H);
            graphics->DrawImage(&woman_image, SCREEN_W / 2 - CHARA_W / 2 - 300, SCREEN_H / 2 - CHARA_H / 2 - 120, CHARA_W, CHARA_H);
        }
        break;
        case 2:
        default:
        {
            Image man_image(L"man_joy.png");
            Image woman_image(L"woman_joy.png");
            graphics->DrawImage(&man_image, SCREEN_W / 2 - CHARA_W / 2 + 300, SCREEN_H / 2 - CHARA_H / 2 - 120, CHARA_W, CHARA_H);
            graphics->DrawImage(&woman_image, SCREEN_W / 2 - CHARA_W / 2 - 300, SCREEN_H / 2 - CHARA_H / 2 - 120, CHARA_W, CHARA_H);
        }
        break;
        }
    }

public:
    TCHAR m_text[TEXT_NUM+1][300];
    int m_textId;
};

int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    TCHAR szAppName[] = TEXT("Game");
    WNDCLASS wc;
    HWND hwnd;
    g_gameManager = new GameManager();

    // �E�B���h�E�N���X�̑�����ݒ�
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;

    // �E�B���h�E�N���X��o�^
    if (!RegisterClass(&wc)) return 0;
    int cxSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME);
    int cySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME);
    int cyCaption = GetSystemMetrics(SM_CYCAPTION);
    // �E�B���h�E���쐬
    hwnd = CreateWindow(
        szAppName, TEXT("GameTitle"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SCREEN_W + cxSizeFrame * 2, SCREEN_H + cyCaption + cySizeFrame * 2 + 8,
        NULL, NULL,
        hInstance, NULL);

    if (!hwnd) return 0;

    // �E�B���h�E��\��
    ShowWindow(hwnd, nCmdShow);

    // ���b�Z�[�W���[�v�����s
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        g_gameManager->Update(hwnd);
    }

    delete g_gameManager;
    return 0;
}

/*
 * �E�B���h�E�v���V�[�W���֐�
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBitmap;
    static HDC hMemDC;
    switch (uMsg)
    {
    case WM_CREATE:
    {
        //�������f�o�C�X�R���e�L�X�g�����
        HDC hdc = GetDC(hwnd);
        hBitmap = CreateCompatibleBitmap(hdc, SCREEN_W, SCREEN_H);
        hMemDC = CreateCompatibleDC(hdc);
        ReleaseDC(hwnd, hdc);
        SelectObject(hMemDC, hBitmap);
        DeleteObject(hBitmap);
    }
    return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        //        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        SelectObject(hMemDC, GetStockObject(WHITE_PEN));
        Rectangle(hMemDC, 0, 0, SCREEN_W, SCREEN_H);
        GdiplusStartupInput gdiplusStartupInput; // MSDN�ɂ��̂܂܂̃R�[�h������.
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        HFONT hFont;
        // �t�H���g���쐬����
        hFont = CreateFont(
            32,
            0,
            0,
            0,
            FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            SHIFTJIS_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            NULL
        );
        SelectObject(hMemDC, hFont);
        SetTextColor(hMemDC, RGB(255, 255, 255));
        SetBkMode(hMemDC, TRANSPARENT);

        Graphics graphics(hMemDC);

        g_gameManager->Draw(&graphics);

        BitBlt(hdc, 0, 0, SCREEN_W, SCREEN_H, hMemDC, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
    }
    return 0;
    case WM_LBUTTONDOWN:
        g_gameManager->Next(hwnd);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

