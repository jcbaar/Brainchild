/*
 *	cursor.c
 *
 *	(C) Copyright 1999-2005 Jan van den Baard.
 *	    All Rights Reserved.
 *
 *	Cursor related routines.
 */

#include "defs.h"

LRESULT OnSetCursor( HWND hWnd, WPARAM wParam, LPARAM lParam, LPCLASSDATA lpcd )
{
	/*
	 *	Are we inside the client area?
	 */
	if ( LOWORD( lParam ) == HTCLIENT )
	{
		/*
		 *	Convert the current cursor position to
		 *	client coordinates.
		 */
		POINT		ptCursor;
		GetCursorPos( &ptCursor );
		ScreenToClient( hWnd, &ptCursor );

		/*
		 *	What cursor do we show?
		 */
		if ( lpcd->bIncrementalMode )						SetCursor( lpcd->hIncremental );
		else if ( ptCursor.x < GetMarginWidth( lpcd ))				SetCursor( lpcd->hSelection );
		else if ( MouseOnSelection( lpcd ) && Parser->bDnDEnabled == TRUE )	SetCursor( lpcd->hArrow );
		else									SetCursor( lpcd->hIBeam );

		return TRUE;
	}
	
	/*
	 *	Call the superclass.
	 */
	return DefWindowProc( hWnd, WM_SETCURSOR, wParam, lParam );
}
