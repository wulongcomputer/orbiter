// Copyright (c) Martin Schweiger
// Licensed under the MIT License

//=============================================================================
// Launchpad tab implementations
//=============================================================================

#define STRICT 1
#include <windows.h>
#include <commctrl.h>
#include "Launchpad.h"
#include "LpadTab.h"
#include "Log.h"
#include "resource.h"

//-----------------------------------------------------------------------------
// LaunchpadTab base class

LaunchpadTab::LaunchpadTab (const MainDialog *lp)
{
	pLp = lp;
	pCfg = lp->Cfg();
	hTab = NULL;
	bActive = false;
	nitem = 0;
	item = NULL;
	itempos = NULL;
}

//-----------------------------------------------------------------------------

LaunchpadTab::~LaunchpadTab ()
{
	if (hTab) DestroyWindow (hTab);
	if (nitem) {
		delete []item;
		delete []itempos;
	}
}

//-----------------------------------------------------------------------------

void LaunchpadTab::Show ()
{
	if (hTab) ShowWindow (hTab, SW_SHOW);
	bActive = true;
}

//-----------------------------------------------------------------------------

void LaunchpadTab::Hide ()
{
	if (hTab) ShowWindow (hTab, SW_HIDE);
	bActive = false;
}

//-----------------------------------------------------------------------------

BOOL LaunchpadTab::Size (int w, int h)
{
	if (nitem) {
		int dx = max (0, (w - (int)(pos0.right-pos0.left))/2);
		int dy = max (0, (h - (int)(pos0.bottom-pos0.top))/2);
		for (int i = 0; i < nitem; i++) {
			SetWindowPos (GetDlgItem (hTab, item[i]), NULL,
				itempos[i].x+dx, itempos[i].y+dy, 0, 0,
				SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOCOPYBITS);
		}
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------

HWND LaunchpadTab::CreateTab (int resid)
{
	HWND hT = CreateDialogParam (pLp->GetInstance(), MAKEINTRESOURCE(resid), pLp->GetWindow(), TabProcHook, (LPARAM)this);
	SetWindowLong (hT, DWL_USER, (LONG)this);

	POINT p0, p1;
	GetClientRect (hT, &pos0);
	p0.x = p0.y = 0; ClientToScreen (pLp->GetWindow(), &p0);
	p1.x = p1.y = 0; ClientToScreen (hT, &p1);
	int dx = p1.x-p0.x, dy = p1.y-p0.y;
	pos0.left += dx, pos0.right += dx;
	pos0.top += dy, pos0.bottom += dy;

	return hT;
}

//-----------------------------------------------------------------------------

void LaunchpadTab::RegisterItemPositions (int *_item, int _nitem)
{
	if (nitem) {
		delete []item;
		delete []itempos; // clear existing list
	}
	if (nitem = _nitem) {
		item = new int[nitem]; TRACENEW
		itempos = new POINT[nitem]; TRACENEW
		memcpy (item, _item, nitem*sizeof(int));
		RECT r;
		for (int i = 0; i < nitem; i++) {
			r = GetClientPos (hTab, GetDlgItem (hTab, item[i]));
			itempos[i].x = r.left;
			itempos[i].y = r.top;
		}
	}
}

//-----------------------------------------------------------------------------

BOOL LaunchpadTab::TabProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

//-----------------------------------------------------------------------------

BOOL CALLBACK LaunchpadTab::TabProcHook (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LaunchpadTab *lt = (LaunchpadTab*)GetWindowLong (hWnd, DWL_USER);
	switch (uMsg) {
	case WM_INITDIALOG:
		lt = (LaunchpadTab*)lParam;
		return lt->InitDialog (hWnd, wParam, lParam);
	case WM_SIZE:
		return lt->Size (LOWORD(lParam), HIWORD(lParam));
	default:
		if (lt) return lt->TabProc (hWnd, uMsg, wParam, lParam);
		break;
	}
	return FALSE;
}
