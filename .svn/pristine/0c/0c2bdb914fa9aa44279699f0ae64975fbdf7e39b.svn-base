/*
 *	bookmarks.c
 *
 *	(C) Copyright 1999-2005 Jan van den Baard.
 *	    All Rights Reserved.
 *
 */

#include "defs.h"

/*
 *	Send a marker state changing/changed message.
 */
static BOOL SendStateMessage( LPCLASSDATA lpcd, int nLine, int nMarker, BOOL bNewState, BOOL bChanged )
{
	NMMARKERSTATE		nms;
	HWND			hParent;

	/*
	 *	Do we have a parent?
	 */
	if (( hParent = GetParent( lpcd->hWnd )) != NULL )
	{
		/*
		 *	Fill in the structure. 
		 */
		nms.hdr.code		= bChanged ? NMBC_MARKERSTATECHANGED : NMBC_MARKERSTATECHANGING;
		nms.hdr.hwndFrom	= lpcd->hWnd;
		nms.hdr.idFrom		= GetWindowLong( lpcd->hWnd, GWL_ID );
		nms.nLine		= nLine + 1;
		nms.nMarker		= nMarker;
		nms.bState		= bNewState;
		nms.bAllowState		= TRUE;

		/*
		 *	Send the notification  and return it's result.
		 */
		SendMessage( hParent, WM_NOTIFY, nms.hdr.idFrom, ( LPARAM )&nms );
 
		/*
		 *	TRUE if the marker state is allowed to change.
		 */
		return nms.bAllowState; 
	}
	/*
	 *	By default the marker state may always change.
	 */
	return TRUE; 
} 

BOOL SetMarker( LPCLASSDATA lpcd, int nMarker, int nLine, BOOL bSingleton )
{
	/*
	 *	Is the passed line number in range?
	 */
	if ( nLine >= 0 && nLine < ArrayGetSize( lpcd->lpLines ))
	{
		LPLINE		lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, nLine );

		/*
		 *	Marker already set?
		 */
		if (( lpLine->dwFlags & ( 1 << nMarker )) != ( 1 << nMarker ))
		{
			/*
			 *	Are we allowed to set the marker state?
			 */
			if ( nMarker && SendStateMessage( lpcd, nLine, nMarker, TRUE, FALSE ) == FALSE )
				return FALSE;

			/*
			 *	Is it a singleton marker?
			 */
			if ( bSingleton )
			{
				/*
				 *	Marker already set elsewhere?
				 */
				int nCLine = lpcd->aMarkers[ nMarker ].nCurrentLine;
				if ( nCLine != -1 )
				{
					/*
					 *	Get the line it is set on.
					 */
					LPLINE lpCurrent = ( LPLINE )ArrayGetAt( lpcd->lpLines, nCLine );
					if ( lpCurrent )
					{
						/*
						 *	Clear and re-render the line.
						 */
						lpCurrent->dwFlags &= ~( 1 << nMarker );
						RenderLine( lpcd, nCLine );
					}
				}
				else
				{
					/*
					 *	First run as a singleton. Clear all previously
					 *	set markers.
					 */
					ClearAllMarkers( lpcd, nMarker );
				}
				/*
				 *	Mark the current line.
				 */
				lpcd->aMarkers[ nMarker ].nCurrentLine = nLine;
			}

			/*
			 *	Set the bit.
			 */
			lpLine->dwFlags |= ( 1 << nMarker );

			/*
			 *	Increase the bookmark counter.
			 */
			lpcd->aMarkers[ nMarker ].nMarkers++;

			/*
			 *	Was it the bookmark?
			 */
			if ( nMarker == 0 )
			{
				/*
				 *	Do we need to send a status message?
				 */
				if ( lpcd->aMarkers[ 0 ].nMarkers == 1 )
					SendStatusMessage( lpcd );
			}

			/*
 			 *	Re-render the line.
			 */
			RenderLine( lpcd, nLine );

			/*
			 *	Tell'm we have changed the marker state.
			 */
			if ( nMarker ) SendStateMessage( lpcd, nLine, nMarker, TRUE, TRUE );
			return TRUE;
		}
	}
	return FALSE;
}
void SetBookmark( LPCLASSDATA lpcd )
{
	SetMarker( lpcd, 0, lpcd->ptCaretPos.y, FALSE );
}
LRESULT OnSetMarker( HWND hWnd, WPARAM wParam, LPARAM lParam, LPCLASSDATA lpcd )
{
	if ( HIWORD( wParam ) > 0 && HIWORD( wParam ) < MAXMARKERS && lParam <= ArrayGetSize( lpcd->lpLines ))
	{
		return SetMarker( lpcd, ( int )HIWORD( wParam ), ( int )( lParam == -1 ? lpcd->ptCaretPos.y : ( lParam - 1 )), ( BOOL )LOWORD( wParam ));
	}
	return FALSE;
}

BOOL ClearMarker( LPCLASSDATA lpcd, int nMarker, int nLine )
{
	/*
	 *	Is the passed line number in range?
	 */
	if ( nLine >= 0 && nLine < ArrayGetSize( lpcd->lpLines ))
	{
		LPLINE		lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, nLine );
		
		/*
		 *	Marker bit set?
		 */
		if (( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker ))
		{
			/*
			 *	Are we allowed to clear the marker state?
			 */
			if ( nMarker && SendStateMessage( lpcd, nLine, nMarker, FALSE, FALSE ) == FALSE )
				return FALSE;

			/*
 			 *	Clear the bit.
			 */
			lpLine->dwFlags &= ~( 1 << nMarker );

			/*
			 *	Decrease the bookmark counter.
			 */
			lpcd->aMarkers[ nMarker ].nMarkers--;

			/*
			 *	Is it the bookmarker?
			 */
			if ( nMarker == 0 )
			{
				/*
				 *	Do we need to send a status message?
				 */
				if ( lpcd->aMarkers[ 0 ].nMarkers == 0 )
					SendStatusMessage( lpcd );
			}

			/*
			 *	Re-render the line.
			 */
			RenderLine( lpcd, nLine );

			/*
			 *	Tell'm we have changed the marker state.
			 */
			if ( nMarker ) SendStateMessage( lpcd, nLine, nMarker, FALSE, TRUE );
			return TRUE;
		}
	}
	return FALSE;
}
void ClearBookmark( LPCLASSDATA lpcd )
{
	ClearMarker( lpcd, 0, lpcd->ptCaretPos.y );
}
LRESULT OnClearMarker( HWND hWnd, WPARAM wParam, LPARAM lParam, LPCLASSDATA lpcd )
{
	if ( wParam > 0 && wParam < MAXMARKERS && lParam <= ArrayGetSize( lpcd->lpLines ))
	{
		return ClearMarker( lpcd, ( int )wParam, ( int )( lParam == -1 ? lpcd->ptCaretPos.y : ( lParam - 1 )));
	}
	return FALSE;
}

BOOL ToggleMarker( LPCLASSDATA lpcd, int nMarker, int nLine )
{
	/*
	 *	Is the passed line number in range?
	 */
	if ( nLine >= 0 && nLine < ArrayGetSize( lpcd->lpLines ))
	{
		LPLINE		lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, nLine );
		BOOL		bStatus = FALSE;

		/*
		 *	Are we allowed to invert the marker state?
		 */
		if ( nMarker && SendStateMessage( lpcd, nLine, nMarker, ! ( lpLine->dwFlags & ( 1 << nMarker )), FALSE ) == FALSE )
			return FALSE;

		/*
		 *	Is the Marker set or cleared?
		 */
		if (( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker ))
		{
			/*
			 *	Is it the bookmark?
			 */
			if ( nMarker == 0 )
			{
				/*
				 *	Decrease the bookmark counter.
				 */
				lpcd->aMarkers[ 0 ].nMarkers--;
				if ( lpcd->aMarkers[ 0 ].nMarkers == 0 )
					bStatus = TRUE;
			}
		}
		else
		{
			/*
			 *	Is it the bookmark?
			 */
			if ( nMarker == 0 )
			{
				/*
				 *	Increase the bookmark counter.
				 */
				lpcd->aMarkers[ 0 ].nMarkers++;
				if ( lpcd->aMarkers[ 0 ].nMarkers == 1 )
					bStatus = TRUE;
			}
		}

		/*
		 *	Flip the marker
		 *	bit.
		 */
		lpLine->dwFlags ^= ( 1 << nMarker );

		/*
		 *	Send the status message.
		 */
		if ( bStatus )
			SendStatusMessage( lpcd );

		/*
		 *	Re-render the line.
		 */
		RenderLine( lpcd, nLine );

		/*
		 *	Tell'm we have changed the marker state.
		 */
		if ( nMarker ) SendStateMessage( lpcd, nLine, nMarker, ( BOOL )( lpLine->dwFlags & ( 1 << nMarker )), TRUE );
		return TRUE;
	}
	return FALSE;
}
void ToggleBookmark( LPCLASSDATA lpcd )
{
	ToggleMarker( lpcd, 0, lpcd->ptCaretPos.y );
}
LRESULT OnToggleMarker( HWND hWnd, WPARAM wParam, LPARAM lParam, LPCLASSDATA lpcd )
{
	if ( wParam > 0 && wParam < MAXMARKERS && lParam <= ArrayGetSize( lpcd->lpLines ))
	{
		return ToggleMarker( lpcd, ( int )wParam, ( int )( lParam == -1 ? lpcd->ptCaretPos.y : ( lParam - 1 )));
	}
	return FALSE;
}

BOOL ClearAllMarkers( LPCLASSDATA lpcd, int nMarker )
{
	LPLINE		lpLine;
	BOOL		bStatus = FALSE, bResult = TRUE;
	int		i;

	/*
	 *	Iterate lines.
	 */
	for ( i = 0; i < ArrayGetSize( lpcd->lpLines ); i++ )
	{
		/*
		 *	Get line pointer.
		 */
		lpLine = ArrayGetAt( lpcd->lpLines, i );

		/*
		 *	The marker?
		 */
		if (( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker ))
		{
			/*
			 *	Are we allowed to clear the marker state?
			 */
			if ( nMarker && SendStateMessage( lpcd, i, nMarker, FALSE, FALSE ) == FALSE )
			{
				bResult = FALSE;
				continue;
			}

			/*
			 *	We must send a status message
			 *	if it is the bookmark.
			 */
			if ( nMarker == 0 )
				bStatus = TRUE;

			/*
			 *	Clear the bit.
			 */
			lpLine->dwFlags &= ~( 1 << nMarker );

			/*
			 *	Is it visible?
			 */
			if ( i >= lpcd->ptViewPos.y && i < lpcd->ptViewPos.y + lpcd->szViewSize.cy )
				/*
				 *	Render the line.
				 */
				RenderLine( lpcd, i );

			/*
			 *	Tell'm we have changed the marker state.
			 */
			if ( nMarker ) SendStateMessage( lpcd, i, nMarker, FALSE, TRUE );
		}
	}

	/*
	 *	No more markers.
	 */
	lpcd->aMarkers[ nMarker ].nMarkers = 0;

	/*
	 *	Send status message if necessary.
	 */
	if ( bStatus )
		SendStatusMessage( lpcd );
	return bResult;
}
void ClearAllBookmarks( LPCLASSDATA lpcd )
{
	ClearAllMarkers( lpcd, 0 );
}
LRESULT OnClearMarkers( HWND hWnd, WPARAM wParam, LPARAM lParam, LPCLASSDATA lpcd )
{
	if ( wParam > 0 && wParam < MAXMARKERS )
	{
		return ClearAllMarkers( lpcd, ( int )wParam );
	}
	return FALSE;
}

void NextMarker( LPCLASSDATA lpcd, int nMarker )
{
	LPLINE		lpLine;
	int		i, nLines = ArrayGetSize( lpcd->lpLines ) - 1, nLine = lpcd->ptCaretPos.y;

	/*
	 *	Only one line?
	 */
	if ( nLines == 0 )
		return;

	/*
	 *	On the last line?
	 */
	if ( lpcd->ptCaretPos.y == nLines )
	{
		/*
		 *	Set anchor.
		 */
		lpcd->aMarkers[ nMarker ].nAnchor = lpcd->ptCaretPos.y;

		/*
		 *	Go to the start of the first line.
		 */
		lpcd->ptCaretPos.y = lpcd->ptCaretPos.x = lpcd->nLastColumnPos = 0;

		/*
		 *	Call ourselves.
		 */
		NextMarker( lpcd, nMarker );
	}

	/*
	 *	If we are located on a marker
	 *	and the anchor is not set yet
	 *	we start looking on the next line.
	 */
	lpLine = GETLINE( lpcd );
	if ((( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker )) && lpcd->aMarkers[ nMarker ].nAnchor == -1 )
		nLine++;

	/*
	 *	Iterate lines.
	 */
	for ( i = nLine; i <= nLines; i++ )
	{
		lpLine = ArrayGetAt( lpcd->lpLines, i );

		/*
		 *	Marker?
		 */
		if ((( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker )) || i == lpcd->aMarkers[ nMarker ].nAnchor )
		{
		 	/*
			 *	On the anchor?
			 */
			if ( i == lpcd->aMarkers[ nMarker ].nAnchor )
			{
				/*
				 *	Done...
				 */
				MessageBeep( 0xFFFFFFFF );
			}

			/*
			 *	Hide the caret.
			 */
			DisplayCaret( lpcd, FALSE );

			/*
			 *	Yes. Move the caret here.
			 */
			lpcd->ptCaretPos.y = i;
			lpcd->ptCaretPos.x = lpcd->nLastColumnPos = 0;

			/*
			 *	Make the caret visible.
			 */
			MakeCaretVisible( lpcd );

			/*
			 *	Reset anchor.
			 */
			lpcd->aMarkers[ nMarker ].nAnchor = -1;

			/*
			 *	Show the caret.
			 */
			DisplayCaret( lpcd, TRUE );
			return;
		}
	}

	/*
	 *	Reaching this means that we are
	 *	on the last line or that there
	 *	are no bookmarks.
	 */

	/*
	 *	Set anchor.
	 */
	lpcd->aMarkers[ nMarker ].nAnchor = nLine;

	/*
	 *	Go to the start of the file and try again.
	 */
	lpcd->ptCaretPos.y = lpcd->ptCaretPos.x = lpcd->nLastColumnPos = 0;

	/*
	 *	Call ourselves.
	 */
	NextMarker( lpcd, nMarker );
}
void NextBookmark( LPCLASSDATA lpcd )
{
	NextMarker( lpcd, 0 );
}

void PrevMarker( LPCLASSDATA lpcd, int nMarker )
{
	LPLINE		lpLine;
	int		i, nLines = ArrayGetSize( lpcd->lpLines ) - 1, nLine = lpcd->ptCaretPos.y;

	/*
	 *	Only one line?
	 */
	if ( nLines == 0 )
		return;

	/*
	 *	On the first line?
	 */
	if ( lpcd->ptCaretPos.y== 0 )
	{
		/*
		 *	Set anchor.
		 */
		lpcd->aMarkers[ nMarker ].nAnchor = lpcd->ptCaretPos.y;

		/*
		 *	Go to the start of the last line.
		 */
		lpcd->ptCaretPos.y = nLines;
		lpcd->ptCaretPos.x = lpcd->nLastColumnPos = 0;

		/*
		 *	Call ourselves.
		 */
		PrevMarker( lpcd, 0 );
	}

	/*
	 *	If we are located on a bookmark
	 *	and the anchor is not set yet
	 *	we start looking on the previous line.
	 */
	lpLine = GETLINE( lpcd );
	if ((( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker )) && lpcd->aMarkers[ nMarker ].nAnchor == -1 )
		nLine--;

	/*
	 *	Iterate lines.
	 */
	for ( i = nLine; i >= 0; i-- )
	{
		lpLine = ArrayGetAt( lpcd->lpLines, i );

		/*
		 *	Marker?
		 */
		if ((( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker )) || i == lpcd->aMarkers[ nMarker ].nAnchor )
		{
			/*
			 *	On the anchor?
			 */
			if ( i == lpcd->aMarkers[ nMarker ].nAnchor )
			{
				/*
				 *	Done...
				 */
				MessageBeep( 0xFFFFFFFF );
			}

			/*
			 *	Hide the caret.
			 */
			DisplayCaret( lpcd, FALSE );

			/*
			 *	Yes. Move the caret here.
			 */
			lpcd->ptCaretPos.y = i;
			lpcd->ptCaretPos.x=lpcd->nLastColumnPos = 0;

			/*
			 *	Make the caret visible.
			 */
			MakeCaretVisible( lpcd );

			/*
			 *	Reset anchor.
			 */
			lpcd->aMarkers[ nMarker ].nAnchor = -1;

			/*
			 *	Show the caret.
			 */
			DisplayCaret( lpcd, TRUE );
			return;
		}
	}

	/*
	 *	Reaching this means that we are
	 *	on the last line or that there
	 *	are no bookmarks.
	 */

	/*
	 *	Set anchor.
	 */
	lpcd->aMarkers[ nMarker ].nAnchor = nLine;

	/*
	 *	Go to the end of the file and try again.
	 */
	lpcd->ptCaretPos.y = nLines;
	lpcd->ptCaretPos.x = lpcd->nLastColumnPos = 0;

	/*
	 *	Call ourselves.
	 */
	PrevMarker( lpcd, nMarker );
}
void PrevBookmark( LPCLASSDATA lpcd )
{
	PrevMarker( lpcd, 0 );
}

void FirstMarker( LPCLASSDATA lpcd, int nMarker )
{
	LPLINE		lpLine;
	int		i;

	/*
	 *	Iterate...
	 */
	for ( i = 0; i < ArrayGetSize( lpcd->lpLines ); i++ )
	{
		lpLine = ArrayGetAt( lpcd->lpLines, i );

		/*
		 *	Marker?
		 */
		if (( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker ))
		{
			/*
			 *	Hide the caret.
			 */
			DisplayCaret( lpcd, FALSE );

			/*
			 *	Yes. Move the caret here.
			 */
			lpcd->ptCaretPos.y = i;
			lpcd->ptCaretPos.x = lpcd->nLastColumnPos = 0;

			/*
			 *	Make the caret visible.
			 */
			MakeCaretVisible( lpcd );

			/*
			 *	Show the caret.
			 */
			DisplayCaret( lpcd, TRUE );
			return;
		}
	}

	/*
	 *	No bookmark found.
	 */
	MessageBeep( 0xFFFFFFFF );
}
void FirstBookmark( LPCLASSDATA lpcd )
{
	FirstMarker( lpcd, 0 );
}

void LastMarker( LPCLASSDATA lpcd, int nMarker )
{
	LPLINE		lpLine;
	int		i;

	/*
	 *	Iterate...
	 */
	for ( i = ArrayGetSize( lpcd->lpLines ) - 1; i >= 0; i-- )
	{
		lpLine = ArrayGetAt( lpcd->lpLines, i );

		/*
		 *	Bookmark?
		 */
		if (( lpLine->dwFlags & ( 1 << nMarker )) == ( 1 << nMarker ))
		{
			/*
			 *	Hide the caret.
			 */
			DisplayCaret( lpcd, FALSE );

			/*
			 *	Yes. Move the caret here.
			 */
			lpcd->ptCaretPos.y = i;
			lpcd->ptCaretPos.x = lpcd->nLastColumnPos = 0;

			/*
			 *	Make the caret visible.
			 */
			MakeCaretVisible( lpcd );

			/*
			 *	Show the caret.
			 */
			DisplayCaret( lpcd, TRUE );
			return;
		}
	}

	/*
	 *	No bookmark found.
	 */
	MessageBeep( 0xFFFFFFFF );
}
void LastBookmark( LPCLASSDATA lpcd )
{
	LastMarker( lpcd, 0 );
}

LRESULT OnGotoMarker( HWND hWnd, WPARAM wParam, LPARAM lParam, LPCLASSDATA lpcd )
{
	if ( wParam > 0 && wParam < MAXMARKERS && lParam >= GMK_NEXT && lParam <= GMK_LAST )
	{
		switch ( lParam )
		{
			case	GMK_NEXT:
				NextMarker( lpcd, ( int )wParam);
				break;

			case	GMK_PREVIOUS:
				PrevMarker( lpcd, ( int )wParam);
				break;

			case	GMK_FIRST:
				FirstMarker( lpcd, ( int )wParam);
				break;

			case	GMK_LAST:
				LastMarker( lpcd, ( int )wParam );
				break;

			default:
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}