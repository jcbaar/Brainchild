// BrainChildEditView.h : interface of the CBrainChildEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRAINCHILDEDITVIEW_H__1FE7458F_0D01_441C_B465_B2C21408E28A__INCLUDED_)
#define AFX_BRAINCHILDEDITVIEW_H__1FE7458F_0D01_441C_B465_B2C21408E28A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "brainchildeditctrl.h"

typedef BOOL (*PFNLoadParserResource)( HGLOBAL );

class CBrainChildEditView : public CBrainChildEditCtrl
{
public:
	CBrainChildEditView()
	{
		HINSTANCE hInst = LoadLibrary("brainchild.dll");
		if (hInst == 0)
		{
			MessageBox(_T("Couldn't load <brainchild.dll>"));
		}

		AddParserFile(MAKEINTRESOURCE(IDR_CPLUSPLUS));
		AddParserFile(MAKEINTRESOURCE(IDR_JSCRIPT));
		AddParserFile(MAKEINTRESOURCE(IDR_VBSCRIPT));
	}
	~CBrainChildEditView()
	{
	}

	//DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BOOL AddParserFile(LPCTSTR lpszResource)
	{
		BOOL bResult = FALSE;
		HMODULE hModule = _Module.GetModuleInstance();
		HGLOBAL hResData = NULL, hMem = NULL;
		PFNLoadParserResource LoadParserResource = (PFNLoadParserResource) GetProcAddress(GetModuleHandle("brainchild.dll"), "LoadParserFromMemory");
		
		// Function not found...
		if ( LoadParserResource == 0 )
			return FALSE;

		HRSRC hsrc = FindResource(hModule, lpszResource, _T("BRAINCHILD"));
		if ( hsrc )
		{
			hResData = LoadResource(hModule, hsrc);
			hMem = GlobalAlloc(GHND, SizeofResource(hModule, hsrc));
			if ( hMem )
			{
				LPVOID vRes = LockResource( hResData );
				LPVOID vMem = GlobalLock( hMem );
				if (vMem && vRes)
				{
					memcpy(vMem, vRes, SizeofResource(hModule, hsrc));
				}
				UnlockResource( hResData );
				GlobalUnlock( hMem );
			}
		}

		if (hMem)
		{
			bResult = (LoadParserResource)(hMem);
			GlobalFree( hMem );
			hMem = NULL;
		}
		return bResult;
	}

	BEGIN_MSG_MAP(CBrainChildEditView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);

		//TODO: Add your drawing code here

		return 0;
	}
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRAINCHILDEDITVIEW_H__1FE7458F_0D01_441C_B465_B2C21408E28A__INCLUDED_)
