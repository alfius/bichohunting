#define WINVER    0x0500
#include <windows.h>
#include <commctrl.h>

#include "ControllerFactory.h"
#include "Ids.h"

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void CheckAndShowHSDialog(HWND);
BOOL CALLBACK HSDialogProc(HWND, UINT, WPARAM, LPARAM);

char szClassName[ ] = "BichoHunting";

HINSTANCE hInstance;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    hInstance = hThisInstance;

    InitCommonControls();

    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(ID_ICON), IMAGE_ICON, 32, 32, LR_SHARED);
    wincl.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(ID_ICON), IMAGE_ICON, 16, 16, LR_SHARED);
    wincl.hCursor = LoadCursor (0, IDC_CROSS);
    wincl.lpszMenuName = 0;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Bicho Hunting",     /* Title Text */
           WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    ShowWindow (hwnd, SW_SHOWMAXIMIZED);

    RECT rect;
    GetClientRect(hwnd, &rect);

    ControllerFactory::Current()->ScreenSize(rect.right, rect.bottom);
    SetTimer(hwnd, 1001, 25, 0);

    while (GetMessage (&messages, 0, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

HANDLE sndLaser;
HANDLE sndLaugh;
HANDLE sndBoing;
HANDLE sndError;
HBITMAP bmsplash;
HBITMAP bmballgreenr;
HBITMAP bmballgreenl;
HBITMAP bmballbluer;
HBITMAP bmballbluel;
HBITMAP bmballredr;
HBITMAP bmballredl;
HBITMAP bmballyellowr;
HBITMAP bmballyellowl;
HBITMAP bmballorange1;
HBITMAP bmballorange2;
HBITMAP bmballorange3;
HBITMAP bmballexploted;
HBITMAP bmballskull;
HBITMAP bmballexplotedsmall;
HBITMAP bmstatusscore;
HBITMAP bmstatuslives;
HFONT fontLocalScore;
HFONT fontStatus;
int _boingSoundCount = 0;
char _currentHitScore[35];

int bgColorR = 0, bgColorG = 255, bgColorB = 255;

int randseed = 0;
bool isStandBy = true;
int hitX, hitY;
int frozenCount = 0, nextLevelCount = 0, gameoverCount = 0;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
        {
            HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCE(ID_SOUNDLASER), "WAVE");
            sndLaser = LoadResource(hInstance, hResInfo);
            hResInfo = FindResource(hInstance, MAKEINTRESOURCE(ID_SOUNDBOING), "WAVE");
            sndBoing = LoadResource(hInstance, hResInfo);
            hResInfo = FindResource(hInstance, MAKEINTRESOURCE(ID_SOUNDLAUGH), "WAVE");
            sndLaugh = LoadResource(hInstance, hResInfo);
            hResInfo = FindResource(hInstance, MAKEINTRESOURCE(ID_SOUNDERROR), "WAVE");
            sndError = LoadResource(hInstance, hResInfo);

            bmsplash = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_SPLASH), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballgreenr = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLGREENR), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballgreenl = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLGREENL), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballbluer = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLBLUER), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballbluel = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLBLUEL), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballredr = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLREDR), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballredl = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLREDL), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballyellowr = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLYELLOWR), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballyellowl = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLYELLOWL), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballorange1 = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLORANGE1), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballorange2 = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLORANGE2), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballorange3 = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLORANGE3), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballexploted = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLEXPLOTED), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballskull = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLSKULL), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmballexplotedsmall = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_BALLSMALLEXPLOTED), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmstatusscore = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_STATUSSCORE), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
            bmstatuslives = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(ID_STATUSLIVES), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);

            fontLocalScore = CreateFont(-40, 0, 0, 0, 500, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Microsoft Sans Serif");
            fontStatus = CreateFont(-25, 0, 0, 0, 500, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Microsoft Sans Serif");
            break;
        }
        case WM_CLOSE:
#ifdef LANGUAGE_SPANISH
            if (isStandBy || (IDYES == MessageBox(hwnd, "Seguro que quieres salir?", "Salir",  MB_YESNO|MB_ICONQUESTION)))
#else
            if (isStandBy || (IDYES == MessageBox(hwnd, "Are you sure you want to exit?", "Exit",  MB_YESNO|MB_ICONQUESTION)))
#endif
            {
                FreeResource(sndLaser);
                FreeResource(sndBoing);
                FreeResource(sndLaugh);
                FreeResource(sndError);
                DeleteObject(bmsplash);
                DeleteObject(bmballgreenr);
                DeleteObject(bmballgreenl);
                DeleteObject(bmballbluer);
                DeleteObject(bmballbluel);
                DeleteObject(bmballredr);
                DeleteObject(bmballredl);
                DeleteObject(bmballyellowr);
                DeleteObject(bmballyellowl);
                DeleteObject(bmballorange1);
                DeleteObject(bmballorange2);
                DeleteObject(bmballorange3);
                DeleteObject(bmballexploted);
                DeleteObject(bmballskull);
                DeleteObject(bmballexplotedsmall);
                DeleteObject(bmstatusscore);
                DeleteObject(bmstatuslives);
                DeleteObject(fontLocalScore);
                DeleteObject(fontStatus);
                ControllerFactory::Delete();
                DestroyWindow(hwnd);
            }
            break;
        case WM_TIMER:
            if (isStandBy)
                randseed++;
            else
            {
                // Update counters and switch state if necessary
                if (frozenCount)
                    frozenCount--;
                if (nextLevelCount)
                    if (--nextLevelCount == 0)
                    {
                        ControllerFactory::Current()->PrepareLevel(ControllerFactory::Current()->Level()+1);
                        bgColorR = rand()%150;
                        bgColorG = 50 + rand()%150;
                        bgColorB = 50 + rand()%150;
                    }
                if (gameoverCount)
                    if (--gameoverCount == 0)
                    {
                        CheckAndShowHSDialog(hwnd);
                        bgColorR = 0;
                        bgColorB = bgColorG = 255;
                        isStandBy = true;
                        InvalidateRect(hwnd, 0, 0);
                    }
                _boingSoundCount = (_boingSoundCount + 1)%(1 + ControllerFactory::Current()->RemainingBalls()/20);
                if (ControllerFactory::Current()->Move() && frozenCount == 0 && _boingSoundCount == 0)
                {
                    const char* sndRes = (const char*)LockResource(sndBoing);
                    if (sndRes)
                    {
                        sndPlaySound(sndRes, SND_MEMORY|SND_ASYNC|SND_NODEFAULT);
                        UnlockResource(sndRes);
                    }
                }
                InvalidateRect(hwnd, 0, 0);
            }
            break;
        case WM_LBUTTONDOWN:
            if (isStandBy)
            {
                isStandBy = false;
                srand(randseed);
                ControllerFactory::Current()->PrepareLevel(1);
            }
            else
            {
                if ((frozenCount == 0) && (gameoverCount == 0))
                {
                    const char* sndRes = 0;
                    switch (ControllerFactory::Current()->Hit(LOWORD(lParam),HIWORD(lParam), _currentHitScore))
                    {
                        case CH_HIT:
                            sndRes = (const char*)LockResource(sndLaser);
                            break;
                        case CH_HITNONE:
                            sndRes = (const char*)LockResource(sndError);
                            break;
                        case CH_SKULL:
                            sndRes = (const char*)LockResource(sndLaugh);
                            break;
                    }
                    if (sndRes)
                    {
                        sndPlaySound(sndRes, SND_MEMORY|SND_ASYNC|SND_NODEFAULT);
                        UnlockResource(sndRes);
                    }
                    hitX = LOWORD(lParam);
                    hitY = HIWORD(lParam);
                    frozenCount = 25;
                    if (ControllerFactory::Current()->RemainingBalls() == 0)
                        nextLevelCount = 50;
                    if (ControllerFactory::Current()->Lives() == -1)
                        gameoverCount = 50;
                    InvalidateRect(hwnd, 0, 0);
                }
            }
            break;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
            {
                frozenCount = 0;
                nextLevelCount = 0;
                gameoverCount = 0;
                bgColorR = 0;
                bgColorB = bgColorG = 255;
                isStandBy = true;
                CheckAndShowHSDialog(hwnd);
                InvalidateRect(hwnd, 0, 0);
            }
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);

                RECT rect;
                GetClientRect(hwnd, &rect);

                HDC hdcDoubleBuffering = CreateCompatibleDC(hdc);
                HBITMAP bmDoubleBuffering = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
                SelectObject(hdcDoubleBuffering, bmDoubleBuffering);

                SetBkMode(hdcDoubleBuffering, TRANSPARENT);

                // Draw background
                {
                    const COLORREF color1 = RGB(
                        bgColorR - 200 >= 0 ? bgColorR - 200 : 0,
                        bgColorG - 200 >= 0 ? bgColorG - 200 : 0,
                        bgColorB - 200 >= 0 ? bgColorB - 200 : 0);
                    const COLORREF color2 = RGB(bgColorR, bgColorG, bgColorB);

                    MoveToEx(hdcDoubleBuffering, 0, 0, 0);
                    for (int i = 0; i < rect.bottom; i++)
                    {
                        int deltaR = (int)((double)i)/((double)rect.bottom/(double)(GetRValue(color2)-GetRValue(color1)));
                        int deltaG = (int)((double)i)/((double)rect.bottom/(double)(GetGValue(color2)-GetGValue(color1)));
                        int deltaB = (int)((double)i)/((double)rect.bottom/(double)(GetBValue(color2)-GetBValue(color1)));

                        COLORREF currentColor = RGB(
                            GetRValue(color1)+deltaR,
                            GetGValue(color1)+deltaG,
                            GetBValue(color1)+deltaB);

                        HPEN pen = CreatePen(PS_SOLID, 1, currentColor);
                        HPEN oldPen = (HPEN)SelectObject(hdcDoubleBuffering, pen);

                        LineTo(hdcDoubleBuffering, rect.right, i);
                        MoveToEx(hdcDoubleBuffering, 0, i+1, 0);

                        DeleteObject((HPEN)SelectObject(hdcDoubleBuffering, oldPen));
                    }
                }

                if (isStandBy)
                {
                    // Draw splash
                    {
                        HDC memdc = CreateCompatibleDC(hdcDoubleBuffering);
                        SelectObject(memdc, bmsplash);
                        TransparentBlt(hdcDoubleBuffering, rect.right/2 - 215, rect.bottom/2 - 125, 431, 250, memdc, 0, 0, 431, 250, RGB(255,0,255));
                        DeleteDC(memdc);
                    }

                    // Draw highscores
                    {
                        HGDIOBJ prevFont = SelectObject(hdcDoubleBuffering, fontLocalScore);
                        SetTextColor(hdcDoubleBuffering, RGB(255,255,255));
                        char fullText[500] = "";

                        list<HighScore*>* highscores = ControllerFactory::Current()->HighScoresList();
                        list<HighScore*>::iterator it = highscores->begin();
                        while (it != highscores->end() && strlen((*it)->Name()))
                        {
                            strcat(fullText, (*it)->Name());
                            strcat(fullText, " ---> ");
                            char score[20];
                            itoa((*it)->Score(), score, 10);
                            strcat(fullText, score);
                            strcat(fullText, "\n");
                            it++;
                        }

                        RECT rectHighScores;
                        rectHighScores.bottom = rect.bottom;
                        rectHighScores.left = 0;
                        rectHighScores.right = rect.right;
                        rectHighScores.top = 40;

                        DrawText(hdcDoubleBuffering, fullText, strlen(fullText), &rectHighScores, DT_CENTER);

                        SelectObject(hdcDoubleBuffering, prevFont);
                    }
                }
                else
                {
                    // Draw balls
                    for (int i = 0; i < ControllerFactory::Current()->BallCount(); i++)
                    {
                        Ball* currentBall = ControllerFactory::Current()->BallAt(i);
                        if (frozenCount == 0 && currentBall->Status() == BS_FROZEN)
                            currentBall->Remove();

                        HBITMAP currentBitmap = 0;
                        switch (currentBall->Size())
                        {
                            case BSIZEBIG:
                                switch (currentBall->Status())
                                {
                                    case BS_MOVING_RIGHT:
                                        switch (currentBall->Color())
                                        {
                                            case BC_GREEN:
                                                currentBitmap = bmballgreenr;
                                                break;
                                            case BC_BLUE:
                                                currentBitmap = bmballbluer;
                                                break;
                                            case BC_RED:
                                                currentBitmap = bmballredr;
                                                break;
                                            case BC_YELLOW:
                                                currentBitmap = bmballyellowr;
                                                break;
                                            case BC_SKULL:
                                                currentBitmap = bmballskull;
                                                break;
                                        }
                                        break;
                                    case BS_MOVING_LEFT:
                                        switch (currentBall->Color())
                                        {
                                            case BC_GREEN:
                                                currentBitmap = bmballgreenl;
                                                break;
                                            case BC_BLUE:
                                                currentBitmap = bmballbluel;
                                                break;
                                            case BC_RED:
                                                currentBitmap = bmballredl;
                                                break;
                                            case BC_YELLOW:
                                                currentBitmap = bmballyellowl;
                                                break;
                                            case BC_SKULL:
                                                currentBitmap = bmballskull;
                                                break;
                                        }
                                        break;
                                    case BS_FROZEN:
                                        currentBitmap = bmballexploted;
                                        break;
                                }
                                break;
                            case BSIZESMALL:
                                switch (currentBall->Status())
                                {
                                    case BS_MOVING_LEFT:
                                    case BS_MOVING_RIGHT:
                                        switch (currentBall->AnimationStep())
                                        {
                                            case 1:
                                                currentBitmap = bmballorange1;
                                                break;
                                            case 2:
                                                currentBitmap = bmballorange2;
                                                break;
                                            case 3:
                                                currentBitmap = bmballorange3;
                                                break;
                                        }
                                        break;
                                    case BS_FROZEN:
                                        currentBitmap = bmballexplotedsmall;
                                        break;
                                }
                        }

                        if (currentBitmap)
                        {
                            HDC memdc = CreateCompatibleDC(hdcDoubleBuffering);
                            SelectObject(memdc, currentBitmap);

                            int x = ControllerFactory::Current()->BallAt(i)->X1();
                            int y = ControllerFactory::Current()->BallAt(i)->Y1();
                            int xs = ControllerFactory::Current()->BallAt(i)->X2();
                            int ys = ControllerFactory::Current()->BallAt(i)->Y2();
                            TransparentBlt(hdcDoubleBuffering, x, y, xs-x, ys-y, memdc, 0, 0, xs-x, ys-y, (UINT)RGB(255, 0, 255));

                            DeleteDC(memdc);
                        }
                    }

                    // Draw axes and local score
                    if (frozenCount)
                    {
                        MoveToEx(hdcDoubleBuffering, 0, hitY, 0);
                        LineTo(hdcDoubleBuffering, rect.right, hitY);
                        MoveToEx(hdcDoubleBuffering, hitX, 0, 0);
                        LineTo(hdcDoubleBuffering, hitX, rect.bottom);

                        RECT rect;
                        rect.bottom = hitY - 15 + frozenCount;
                        rect.left = hitX + 15;
                        rect.top = rect.bottom - 50;
                        rect.right = hitX + 300;

                        HGDIOBJ prevFont = SelectObject(hdcDoubleBuffering, fontLocalScore);
                        SetTextColor(hdcDoubleBuffering, RGB(255,255,255));

                        DrawText(hdcDoubleBuffering, _currentHitScore, strlen(_currentHitScore), &rect, DT_LEFT|DT_BOTTOM);

                        SelectObject(hdcDoubleBuffering, prevFont);
                    }

                    // Draw life and global score status
                    {
                        HGDIOBJ prevFont = SelectObject(hdcDoubleBuffering, fontStatus);
                        SetTextColor(hdcDoubleBuffering, RGB(200,200,200));

                        char livesAsString[20];
                        if (ControllerFactory::Current()->Lives()>=0)
                            itoa(ControllerFactory::Current()->Lives(), livesAsString, 10);
                        else strcpy(livesAsString, "-");
                        RECT rectLives;
                        rectLives.top = 8;
                        rectLives.left = 40;
                        rectLives.bottom = rectLives.top + 50;
                        rectLives.right = rectLives.left + 100;
                        DrawText(hdcDoubleBuffering, livesAsString, strlen(livesAsString), &rectLives, DT_LEFT|DT_TOP);

                        char scoreAsString[20];
                        itoa(ControllerFactory::Current()->Score(), scoreAsString, 10);
                        RECT rectScore;
                        rectScore.top = 8;
                        rectScore.right = rect.right - 40;
                        rectScore.bottom = rectScore.top + 50;
                        rectScore.left = rectScore.right - 100;
                        DrawText(hdcDoubleBuffering, scoreAsString, strlen(scoreAsString), &rectScore, DT_RIGHT|DT_TOP);

                        SelectObject(hdcDoubleBuffering, prevFont);

                        HDC memdc = CreateCompatibleDC(hdcDoubleBuffering);
                        if (ControllerFactory::Current()->Lives()>=0)
                            SelectObject(memdc, bmstatuslives);
                        else SelectObject(memdc, bmstatusscore);
                        TransparentBlt(hdcDoubleBuffering, 10, 10, 24, 24, memdc, 0, 0, 24, 24, RGB(255,0,255));

                        SelectObject(memdc, bmstatusscore);
                        TransparentBlt(hdcDoubleBuffering, rect.right-10-24, 10, 24, 24, memdc, 0, 0, 24, 24, RGB(255,0,255));

                        DeleteDC(memdc);
                    }
                }

                // Copy memory dc to original dc
                BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcDoubleBuffering, 0, 0, SRCCOPY);
                DeleteDC(hdcDoubleBuffering);
                DeleteObject(bmDoubleBuffering);

                EndPaint(hwnd, &ps);
                break;
            }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            if (wParam == SIZE_RESTORED)
                ShowWindow(hwnd, SW_MAXIMIZE);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

char _hsname[25];

void CheckAndShowHSDialog(HWND hwnd)
{
    if (ControllerFactory::Current()->CheckLastScore())
    {
        DialogBox(hInstance, MAKEINTRESOURCE(ID_HIGHSCOREDIALOG), hwnd, &HSDialogProc);
        ControllerFactory::Current()->CommitLastScore(_hsname);
    }
}

BOOL CALLBACK HSDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND edit;
    switch (message)
    {
        case WM_INITDIALOG:
            edit = GetDlgItem(hwnd, ID_HIGHSCOREDIALOGEDIT);
            SendMessage(edit, EM_SETLIMITTEXT, (WPARAM)20, 0);
            SetFocus(edit);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                if (SendMessage(edit, WM_GETTEXT, (WPARAM)25, (LPARAM)_hsname))
                    EndDialog(hwnd, 0);
            }
            break;
    }
    return 0;
}

