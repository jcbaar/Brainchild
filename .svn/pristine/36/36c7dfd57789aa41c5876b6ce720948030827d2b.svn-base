/*
 *	edit.c
 *
 *	(C) Copyright 1993-2005 Jan van den Baard.
 *	    All Rights Reserved.
 *
 *	Text editing routines.
 */

#include "defs.h"

/*
 *	Remove blanks between the cursor
 *	position and the next non-blank.
 */
void Pull( LPCLASSDATA lpcd )
{
	LPLINE	lpLine = GETLINE( lpcd );
	POINT	ptStart, ptEnd;

	/*
	 *	Read-only.
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Clear markers.
	 */
	ClearMark( lpcd );

	/*
	 *	Are we on the end of the line?
	 */
	if ( lpcd->ptCaretPos.x == lpLine->nLength )
		return;

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Setup the points.
	 */
	ptStart = ptEnd = lpcd->ptCaretPos;

	/*
	 *	Find the first non-blank.
	 */
	while ( _istspace( lpLine->pcText[ ptEnd.x ] ))
	{
		/*
		 *	The end?
		 */
		if ( ptEnd.x == lpLine->nLength )
			break;

		/*
		 *	Next...
		 */
		ptEnd.x++;
	}

	/*
	 *	Delete the characters.
	 */
	DeleteText( lpcd, &ptStart, &ptEnd, TRUE );

	/*
	 *	Move caret.
	 */
	lpcd->ptCaretPos = ptStart;

	/*
	 *	Caret still inside the view?
	 */
	if ( CaretInView( lpcd ))
	{
		/*
		 *	Simply re-render the line.
		 */
		RenderLine( lpcd, lpcd->ptCaretPos.y );
	}
	else
		/*
		 *	Move the view.
		 */
		MakeCaretVisible( lpcd );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

/*
 *	Delete to the end of the line.
 */
void DeleteEol( LPCLASSDATA lpcd )
{
	LPLINE	lpLine = GETLINE( lpcd );
	POINT	ptStart, ptEnd;

	/*
	 *	Read-only.
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Clear markers.
	 */
	ClearMark( lpcd );

	/*
	 *	Are we alreay on the end of the line?
	 */
	if ( lpcd->ptCaretPos.x == lpLine->nLength )
		return;

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Setup the points.
	 */
	ptStart = ptEnd = lpcd->ptCaretPos;
	ptEnd.x = lpLine->nLength;

	/*
	 *	Delete the characters.
	 */
	DeleteText( lpcd, &ptStart, &ptEnd, TRUE );

	/*
	 *	Move caret.
	 */
	lpcd->ptCaretPos = ptStart;

	/*
	 *	Caret still inside the view?
	 */
	if ( CaretInView( lpcd ))
	{
		/*
		 *	Simply re-render the line.
		 */
		RenderLine( lpcd, lpcd->ptCaretPos.y );
	}
	else
		/*
		 *	Move the view.
		 */
		MakeCaretVisible( lpcd );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

/*
 *	Back space a character.
 */
void BackSpace( LPCLASSDATA lpcd )
{
	POINT	ptStart, ptEnd;
	BOOL	bChangeLines = FALSE;

	/*
	 *	Read-only.
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Delete only the text selection
	 *	if the markers are valid.
	 */
	if ( HasMark( lpcd ))
	{
		Delete( lpcd );
		return;
	}

	/*
	 *	If we are at the start of the text
	 *	we do nothing.
	 */
	if ( lpcd->ptCaretPos.y == 0 && lpcd->ptCaretPos.x == 0 )
		return;

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Setup the points.
	 */
	ptStart = ptEnd = lpcd->ptCaretPos;

	/*
	 *	Are we on the start of the line?
	 */
	if ( lpcd->ptCaretPos.x == 0 )
	{
		/*
		 *	Join the previous and this line.
		 */
		ptStart.y--;
		ptStart.x = (( LPLINE )ArrayGetAt( lpcd->lpLines, ptStart.y ))->nLength;

		/*
		 *	Re-render whole view.
		 */
		bChangeLines = TRUE;
	}
	else
		/*
		 *	One character.
		 */
		ptStart.x--;

	/*
	 *	Delete the character.
	 */
	DeleteText( lpcd, &ptStart, &ptEnd, TRUE );

	/*
	 *	Move caret.
	 */
	lpcd->ptCaretPos = ptStart;

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Caret still inside the view?
	 */
	if ( ! CaretInView( lpcd ))
	{
		/*
		 *	Full redraw...
		 */
		bChangeLines = TRUE;
		MakeCaretVisibleNoRedraw( lpcd );
	}

	/*
	 *	Any changes in the line count?
	 */
	if ( bChangeLines )
	{
		/*
		 *	Re-render and setup the scrollers.
		 */
		InvalidateRect( lpcd->hWnd, NULL, TRUE );
		SetupHScroller( lpcd );
		SetupVScroller( lpcd );
	}
	else
		/*
		 *	Simply re-render the line.
		 */
		RenderLine( lpcd, lpcd->ptCaretPos.y );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

/*
 *	Delete a character from the text.
 */
void DeleteChar( LPCLASSDATA lpcd )
{
	LPLINE	lpLine = GETLINE( lpcd );
	POINT	ptStart, ptEnd;
	BOOL	bChangeLines = FALSE;

	/*
	 *	Read-only.
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Delete only the text selection
	 *	if the markers are valid.
	 */
	if ( HasMark( lpcd ))
	{
		Delete( lpcd );
		return;
	}

	/*
	 *	If we are at the end of the text
	 *	we do nothing.
	 */
	if ( lpcd->ptCaretPos.y == ArrayGetSize( lpcd->lpLines ) - 1 && lpcd->ptCaretPos.x == lpLine->nLength )
		return;

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Setup the points.
	 */
	ptStart = ptEnd = lpcd->ptCaretPos;

	/*
	 *	Are we on the end of the line?
	 */
	if ( lpcd->ptCaretPos.x == lpLine->nLength )
	{
		/*
		 *	Join this and the next lines.
		 */
		ptEnd.x = 0;
		ptEnd.y++;

		/*
		 *	Re-render whole view.
		 */
		bChangeLines = TRUE;
	}
	else
		/*
		 *	One character.
		 */
		ptEnd.x++;

	/*
	 *	Delete the character.
	 */
	DeleteText( lpcd, &ptStart, &ptEnd, TRUE );

	/*
	 *	Any changes in the line count?
	 */
	if ( bChangeLines )
	{
		/*
		 *	Re-render and setup the scrollers.
		 */
		InvalidateRect( lpcd->hWnd, NULL, TRUE );
		SetupHScroller( lpcd );
		SetupVScroller( lpcd );
	}
	else
		RenderLine( lpcd, lpcd->ptCaretPos.y );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

/*
 *	Delete current line.
 */
void DeleteLine( LPCLASSDATA lpcd )
{
	LPLINE	lpLine = GETLINE( lpcd );
	POINT	ptStart, ptEnd;

	/*
	 *	Read-only.
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Clear markers.
	 */
	ClearMark( lpcd );

	/*
	 *	Any text to delete?
	 */
	if ( ArrayGetSize( lpcd->lpLines ) == 1 && lpLine->nLength == 0 )
		return;

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Setup the points.
	 */
	ptStart = ptEnd = lpcd->ptCaretPos;

	/*
	 *	Are we on the last line?
	 */
	if ( lpcd->ptCaretPos.y == ArrayGetSize( lpcd->lpLines ) - 1 )
	{
		/*
		 *	If this is no the first
		 *	line we go one up.
		 */
		if ( lpcd->ptCaretPos.y > 0 )
		{
			/*
			 *	Go to the end.
			 */
			ptStart.y--;
			ptStart.x = (( LPLINE )ArrayGetAt( lpcd->lpLines, ptStart.y ))->nLength;
		}
		else
			/*
			 *	Move to the start of the line.
			 */
			ptStart.x = 0;

		/*
		 *	End at the end of the current line.
		 */
		ptEnd.x = lpLine->nLength;
	}
	else
	{
		/*
		 *	Move to the start of the next line.
		 */
		ptEnd.y++;
		ptEnd.x = 0;

		/*
		 *	And the start of the current.
		 */
		ptStart.x = 0;
	}

	/*
	 *	Delete the characters.
	 */
	DeleteText( lpcd, &ptStart, &ptEnd, TRUE );

	/*
	 *	Set caret at the start position.
	 */
	lpcd->ptCaretPos = ptStart;

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Caret still inside the view?
	 */
	if ( ! CaretInView( lpcd ))
		MakeCaretVisibleNoRedraw( lpcd );

	/*
	 *	Re-render and setup the scrollers.
	 */
	InvalidateRect( lpcd->hWnd, NULL, TRUE );
	SetupHScroller( lpcd );
	SetupVScroller( lpcd );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

/*
 *      Automatic bracket matching.
 */
static void AutoMatch( LPCLASSDATA lpcd, TCHAR cChar )
{
	/*
	 *	Is it a closing bracket?
	 */
	if ( cChar == _T( ')' ) || cChar == _T( ']' ) || cChar == _T( '}' ) || cChar == _T( '>' ) || cChar == _T( '»' ))
	{
		/*
		 *	Save caret position.
		 */
		POINT	ptCaret = lpcd->ptCaretPos;

		/*
		 *	Go back one.
		 */
		lpcd->ptCaretPos.x--;

		/*
		 *	No dialogs, no visual
		 *	updates...
		 */
		lpcd->bSilentMode = TRUE;

		/*
		 *      Match it.
		 */
		if ( ! MatchBracket( lpcd ))
		{
			/*
			 *	Visible alarm?
			 */
			if ( Parser->bVisibleMatchError )
				/*
				 *	Display error dialog.
				 */
				OkError( lpcd, lpcd->hWnd, GetString( IDS_AUTOMATCH_FAULT ));
			else
				/*
				 *	Sound the alarm...
				 */
				MessageBeep( 0xFFFFFFFF );
		}

		/*
		 *	Silent mode off.
		 */
		lpcd->bSilentMode = FALSE;

		/*
		 *	Restore caret position.
		 */
		lpcd->ptCaretPos = ptCaret;
        }
}

/*
 *	Insert a character into the text.
 */
void InsertChar( LPCLASSDATA lpcd, TCHAR cChar )
{
	TCHAR	szBuf[ 2 ] = { 0 };
	BOOL	bDeleted = FALSE;
	
	/*
	 *	Read-only.
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Delete selection
	 */
	if ( HasMark( lpcd ))
		bDeleted = Delete( lpcd );

	/*
	 *	Construct text to insert.
	 */
	szBuf[ 0 ] = cChar;

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Overwrite?
	 */
	if ( lpcd->bOverwrite )
	{
		LPLINE		lpLine = GETLINE( lpcd );

		/*
		 *	Are we at the end of the line?
		 */
		if ( lpcd->ptCaretPos.x < lpLine->nLength )
		{
			/*
			 *	No. Delete the next character.
			 */
			POINT		ptStart, ptEnd;

			/*
			 *	Setup points.
			 */
			ptStart = ptEnd = lpcd->ptCaretPos;
			ptEnd.x++;

			/*
			 *	Delete the text.
			 */
			if ( DeleteText( lpcd, &ptStart, &ptEnd, ! bDeleted ) == FALSE )
				return;

			/*
			 *	Make sure the undo group is continued.
			 */
			bDeleted = TRUE;
		}
	}

	/*
	 *	Insert the text.
	 */
	InsertText( lpcd, lpcd->ptCaretPos.y, lpcd->ptCaretPos.x, szBuf, &lpcd->ptCaretPos, ! bDeleted );

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Caret still inside the view?
	 */
	if ( CaretInView( lpcd ))
		/*
		 *	Yes, simply re-render the line.
		 */
		RenderLine( lpcd, lpcd->ptCaretPos.y );
	else
		/*
		 *	Move the view.
		 */
		MakeCaretVisible( lpcd );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );

	/*
	 *	Auto-match?
	 */
	if ( Parser->bAutoMatch )
		AutoMatch( lpcd, cChar );
}

void InsertCharacters( LPCLASSDATA lpcd, LPCTSTR lpszText )
{
	/*
	 *	Read only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Clear markers.
	 */
	ClearMark( lpcd );

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Insert the text.
	 */
	InsertText( lpcd, lpcd->ptCaretPos.y, lpcd->ptCaretPos.x, lpszText, &lpcd->ptCaretPos, TRUE );

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Caret in the view?
	 */
	if ( ! CaretInView( lpcd ))
		MakeCaretVisibleNoRedraw( lpcd );

	/*
	 *	Re-render and setup the scrollers.
	 */
	InvalidateRect( lpcd->hWnd, NULL, TRUE );
	SetupHScroller( lpcd );
	SetupVScroller( lpcd );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

/*
 *	Check if the line we have just
 *	split has only blanks left on
 *	it.
 */
static void CheckPreviousLine( LPCLASSDATA lpcd )
{
	/*
	 *	Is there a previous line?
	 */
	if ( lpcd->ptCaretPos.y > 0 )
	{
		/*
		 *	Get a pointer to it.
		 */
		LPLINE	lpPrev = ArrayGetAt( lpcd->lpLines, lpcd->ptCaretPos.y - 1 );
		int	nBlanks = 0;

		/*
		 *	Valid text?
		 */
		if ( lpPrev->pcText )
		{
			/*
			 *	Count spaces.
			 */
			while ( _istspace( lpPrev->pcText[ nBlanks ] ) && nBlanks < lpPrev->nLength ) nBlanks++;

			/*
			 *	Does the whole line consist
			 *	out of blanks?
			 */
			if ( nBlanks == lpPrev->nLength )
				/*
				 *	Yes. Clear it.
				 */
				 lpPrev->nLength = 0;
		}
	}
}

/*
 *	Split up the current line.
 */
void SplitLine( LPCLASSDATA lpcd )
{
	TCHAR	*szText = _T("\r\n"), *pcBlanks = NULL;
	int	nPosX = 0;
	BOOL	bDeleted = FALSE;

	/*
	 *	Read-only.
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Delete selection
	 */
	if ( HasMark( lpcd ))
		bDeleted = Delete( lpcd );

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Overwrite mode?
	 */
	if ( lpcd->bOverwrite )
	{
		/*
		 *	Are we before the last line?
		 */
		if ( lpcd->ptCaretPos.y < ArrayGetSize( lpcd->lpLines ) - 1 )
		{
			/*
			 *	One down.
			 */
			lpcd->ptCaretPos.y++;
			lpcd->ptCaretPos.x = 0;

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
		else
			/*
			 *	Go to the end of the last line.
			 */
			lpcd->ptCaretPos.x = (( LPLINE )GETLINE( lpcd ))->nLength;
	}

	/*
	 *	Auto indent?
	 */
	if ( Parser->bAutoIndent )
		/*
		 *	Get leading blanks.
		 */
		pcBlanks = CopyLeadingBlanks( lpcd, &nPosX );

	/*
	 *	Insert a line-break.
	 */
	if ( InsertText( lpcd, lpcd->ptCaretPos.y, lpcd->ptCaretPos.x, szText, &lpcd->ptCaretPos, ! bDeleted ) == FALSE )
		return;

	/*
	 *	Indent?
	 */
	if ( pcBlanks )
	{
		/*
		 *	Insert blanks.
		 */
		InsertText( lpcd, lpcd->ptCaretPos.y, lpcd->ptCaretPos.x, pcBlanks, &lpcd->ptCaretPos, FALSE );

		/*
		 *	Put the caret at the
		 *	correct position.
		 */
		lpcd->ptCaretPos.x = nPosX;

		/*
		 *	Check if the line we split has
		 *	only blanks left.
		 */
		CheckPreviousLine( lpcd );

		/*
		 *	Free the string.
		 */
		FreePooled( lpcd->pMemPool, pcBlanks );
	}

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Caret in the view?
	 */
	if ( ! CaretInView( lpcd ))
		MakeCaretVisibleNoRedraw( lpcd );

	/*
	 *	Re-render and setup the scrollers.
	 */
	InvalidateRect( lpcd->hWnd, NULL, TRUE );
	SetupHScroller( lpcd );
	SetupVScroller( lpcd );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

BOOL GetWordPoints( LPCLASSDATA lpcd, LPPOINT lpStart, LPPOINT lpEnd )
{
	LPLINE	lpLine = GETLINE( lpcd );
	int	nIndex = lpcd->ptCaretPos.x;

	/*
	 *	Any text?
	 */
	if ( AnyText( lpcd ) == FALSE || lpLine->pcText == NULL )
		return FALSE;

	/*
	 *	Are we on a word?
	 */
	if ( _istspace( lpLine->pcText[ nIndex ] ))
		return FALSE;

	/*
	 *	Setup line.
	 */
	lpStart->y = lpEnd->y = lpcd->ptCaretPos.y;

	/*
	 *	Are we on a delimiter?
	 */
	if ( IsDelimiter( lpcd, lpLine->pcText[ nIndex ] ))
	{
		/*
		 *	The delimiter is the whole word.
		 */
		lpStart->x = nIndex;
		lpEnd->x   = nIndex + 1;

		return TRUE;
	}

	/*
	 *	Find the start of the word.
	 */
	while ( nIndex > 0 && ! _istspace( lpLine->pcText[ nIndex ] ) && ! IsDelimiter( lpcd, lpLine->pcText[ nIndex ] ))
		nIndex--;

	/*
	 *	Did we stop on a space or a delimiter?
	 */
	if ( _istspace( lpLine->pcText[ nIndex ] ) || IsDelimiter( lpcd, lpLine->pcText[ nIndex ] ))
	{
		if ( nIndex < lpLine->nLength )
			nIndex++;
	}

	/*
	 *	Start column.
	 */
	lpStart->x = nIndex;

	/*
	 *	Find the end of the word.
	 */
	while ( nIndex < lpLine->nLength && ! _istspace( lpLine->pcText[ nIndex ] ) && ! IsDelimiter( lpcd, lpLine->pcText[ nIndex ] ))
		nIndex++;

	/*
	 *	End column.
	 */
	lpEnd->x = nIndex;

	return TRUE;
}

/*
 *	Convert casing between two
 *	points.
 */
void ConvertCase( LPCLASSDATA lpcd, LPPOINT lpStart, LPPOINT lpEnd, int nType )
{
	TCHAR  *pcText;
	int	i = 0;

	/*
	 *	Read-only.
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Get text to delete.
	 */
	pcText = GetText( lpcd, lpStart, lpEnd );

	/*
	 *	Text retrieved?
	 */
	if ( pcText == NULL )
		return;

	/*
	 *	Delete the text.
	 */
	if ( DeleteText( lpcd, lpStart, lpEnd, TRUE ) == FALSE )
	{
		/*
		 *	Free the text.
		 */
		ArrayFreeMem( lpcd->lpUndoRedo, pcText );
		return;
	}

	/*
	 *	Convert the text.
	 */
	for ( i = 0; ; i++ )
	{
		/*
		 *	Stop at the terminator.
		 */
		if ( pcText[ i ] == _T( '\0' ))
			break;

		/*
		 *	Convert how?
		 */
		switch ( nType )
		{
			case	NTYPE_TOUPPER:
				/*
				 *	Convert to upper case.
				 */
				pcText[ i ] = ( TCHAR )_totupper( pcText[ i ] );
				break;

			case	NTYPE_TOLOWER:
				/*
				 *	Convert to lower case.
				 */
				pcText[ i ] = ( TCHAR )_totlower( pcText[ i ] );
				break;

			case	NTYPE_SWAP:
				/*
				 *	Swap case.
				 */
				if (	  _istlower( pcText[ i ] )) pcText[ i ] = ( TCHAR )_totupper( pcText[ i ] );
				else if ( _istupper( pcText[ i ] )) pcText[ i ] = ( TCHAR )_totlower( pcText[ i ] );
		}
	}

	/*
	 *	Insert the converted text.
	 */
	InsertText( lpcd, lpStart->y, lpStart->x, pcText, &lpcd->ptCaretPos, FALSE );

	/*
	 *	Free the text.
	 */
	ArrayFreeMem( lpcd->lpUndoRedo, pcText );

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Caret still inside the view?
	 */
	if ( CaretInView( lpcd ) == FALSE )
		/*
		 *	Move the view.
		 */
		MakeCaretVisibleNoRedraw( lpcd );

	/*
	 *	Re-render.
	 */
	InvalidateRect( lpcd->hWnd, NULL, FALSE );

	/*
	 *	We are modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Show the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

static void WordToXXX( LPCLASSDATA lpcd, int nType )
{
	POINT	ptStart, ptEnd;

	/*
	 *	Clear marker.
	 */
	ClearMark( lpcd );

	/*
	 *	Get word points.
	 */
	if ( GetWordPoints( lpcd, &ptStart, &ptEnd ))
		/*
		 *	Convert word casing.
		 */
		ConvertCase( lpcd, &ptStart, &ptEnd, nType );
}

void WordToUpper( LPCLASSDATA lpcd )
{
	WordToXXX( lpcd, NTYPE_TOUPPER );
}

void WordToLower( LPCLASSDATA lpcd )
{
	WordToXXX( lpcd, NTYPE_TOLOWER );
}

void WordSwapCase( LPCLASSDATA lpcd )
{
	WordToXXX( lpcd, NTYPE_SWAP );
}

void CopyLine( LPCLASSDATA lpcd )
{
	POINT	ptStart = lpcd->ptCaretPos, ptEnd;
	LPLINE	lpLine = GETLINE( lpcd );
	LPTSTR	lpszText;
	int	nLength = lpLine->nLength + 3;

	/*
	 *	Read only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Clear markers.
	 */
	ClearMark( lpcd );

	/*
	 *	Allocate text buffer.
	 */
	if (( lpszText = AllocPooled( lpcd->pMemPool, REAL_SIZE( nLength ))) != NULL )
	{
		/*
		 *	Hide the caret.
		 */
		DisplayCaret( lpcd, FALSE );

		/*
		 *	Copy the text.
		 */
		if ( lpLine->pcText )
			_tcsncpy_s( lpszText, REAL_SIZE( nLength ), lpLine->pcText, lpLine->nLength );

		/*
		 *	Append line terminator.
		 */
		_tcscat_s( lpszText, REAL_SIZE( nLength ), _T("\r\n"));

		/*
		 *	Insert at the start of the line.
		 */
		ptStart.x = 0;

		/*
		 *	Insert the text.
		 */
		InsertText( lpcd, ptStart.y, ptStart.x, lpszText, &ptEnd, TRUE );

		/*
		 *	Copy position.
		 */
		lpcd->ptCaretPos = ptEnd;

		/*
		 *	Free the text.
		 */
		FreePooled( lpcd->pMemPool, lpszText );

		/*
		 *	Caret in the view?
		 */
		if ( ! CaretInView( lpcd ))
			MakeCaretVisibleNoRedraw( lpcd );

		/*
		 *	Re-render and setup the scrollers.
		 */
		InvalidateRect( lpcd->hWnd, NULL, TRUE );
		SetupHScroller( lpcd );
		SetupVScroller( lpcd );

		/*
		 *	We are modified.
		 */
		SetModified( lpcd, TRUE );

		/*
		 *	Show the caret.
		 */
		DisplayCaret( lpcd, TRUE );
	}
}

void DeleteWord( LPCLASSDATA lpcd )
{
	POINT	ptStart, ptEnd;

	/*
	 *	Read-only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Clear markers.
	 */
	if ( HasMark( lpcd ))
		ClearMark( lpcd );

	/*
	 *	Get the word points.
	 */
	if ( GetWordPoints( lpcd, &ptStart, &ptEnd ))
	{
		/*
		 *	Set these points up as a
		 *	text selection.
		 */
		lpcd->ptSelStart = ptStart;
		lpcd->ptSelEnd   = ptEnd;

		/*
		 *	Delete it.
		 */
		Delete( lpcd );
	}
}

void DeleteEow( LPCLASSDATA lpcd )
{
	POINT	ptStart, ptEnd;

	/*
	 *	Read-only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Clear markers.
	 */
	if ( HasMark( lpcd ))
		ClearMark( lpcd );

	/*
	 *	Get the word points.
	 */
	if ( GetWordPoints( lpcd, &ptStart, &ptEnd ))
	{
		/*
		 *	Set the caret position and
		 *	the end of the word as the
		 *	text selection.
		 */
		lpcd->ptSelStart = lpcd->ptCaretPos;
		lpcd->ptSelEnd   = ptEnd;

		/*
		 *	Delete it.
		 */
		Delete( lpcd );
	}
}

void DeleteSow( LPCLASSDATA lpcd )
{
	POINT	ptStart, ptEnd;

	/*
	 *	Read-only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Clear markers.
	 */
	if ( HasMark( lpcd ))
		ClearMark( lpcd );

	/*
	 *	Get the word points.
	 */
	if ( GetWordPoints( lpcd, &ptStart, &ptEnd ))
	{
		/*
		 *	Set the caret position and
		 *	the start of the word as the
		 *	text selection.
		 */
		lpcd->ptSelStart = ptStart;
		lpcd->ptSelEnd   = lpcd->ptCaretPos;

		/*
		 *	Delete it.
		 */
		Delete( lpcd );
	}
}

void SwapLines( LPCLASSDATA lpcd )
{
	LPLINE		lpLine1 = GETLINE( lpcd ), lpLine2;
	LINE		lnTmp;
	POINT		ptStart, ptEnd;

	/*
	 *	Are we read-only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Are we on the first line?
	 */
	if ( lpcd->ptCaretPos.y == 0 )
	{
		/*
		 *	Beep the speaker.
		 */
		MessageBeep( 0xFFFFFFFF );
		return;
	}

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	The start POINT will contain the lines which
	 *	are swapped.
	 */
	ptStart.x = lpcd->ptCaretPos.y;
	ptStart.y = lpcd->ptCaretPos.y - 1;

	/*
	 *	For this undo record the end POINT is not
	 *	used.
	 */
	ptEnd.x = 0;
	ptEnd.y = 0;

	/*
	 *	Swap the contents of the two LINE structures
	 *	in the array.
	 */
	lnTmp = *lpLine1;
	lpLine2 = ( LPLINE )ArrayGetAt( lpcd->lpLines, lpcd->ptCaretPos.y - 1 );
	*lpLine1 = *lpLine2;
	*lpLine2 = lnTmp;

	/*
	 *	Add an undo record.
	 */
	AddUndoRecord( lpcd, URF_GROUP | URF_LINESWAP, &ptStart, &ptEnd, NULL );

	/*
	 *	Clear the marker.
	 */
	if ( HasMark( lpcd ))
		ClearMark( lpcd );

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Re-render lines.
	 */
	RenderLines( lpcd, ptStart.y - 1, ptStart.y );

	/*
	 *	Were modifieed.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Display the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

/*
 *	The line sorting callback.
 */
int CompareLines( const void *lpElemA, const void *lpElemB )
{
	LPSORTLINE lpLine1 = ( LPSORTLINE )lpElemA, lpLine2 = ( LPSORTLINE )lpElemB;

	/*
	 *	If both lines are empty do not
	 *	bother swapping them.
	 */
	if ( lpLine1->lnLine.pcText == NULL && lpLine2->lnLine.pcText == NULL ) 
	{
		return 0;
	}
	/*
	 *	If the first line is empty and the second is
	 *	not we add a swap record and return -1.
	 */
	else if ( lpLine1->lnLine.pcText == NULL && lpLine2->lnLine.pcText ) 
	{
		return -1;
	}
	/*
	 *	If the second line is empty and the first is
	 *	not we add a swap record and return 1.
	 */
	else if ( lpLine1->lnLine.pcText && lpLine2->lnLine.pcText == NULL ) 
	{
		return 1;
	}
	/*
	 *	Do a case insensitive string compare on the two lines. If this
	 *	returns non-zero we add a swap record and
	 *	return the result of the compare.
	 */
	return _tcsicmp( lpLine1->lnLine.pcText, lpLine2->lnLine.pcText );
}

void InternalUnsortLines( LPCLASSDATA lpcd, LPSORTLINE lpLines, POINT ptLines )
{
	LPLINE pLine;
	int i;

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Loop through the array of sorted lines.
	 */
	for ( i = 0; i < ( ptLines.y - ptLines.x ); i++ )
	{
		/*
		 *	Get the line at the position the first
		 *	sorted line used to be on.
		 */
		pLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, lpLines[ i ].nLine );
		if ( pLine )
		{
			/*
			 *	Copy the data back at its
			 *	original position.
			 */
			*pLine = lpLines[ i ].lnLine;
		}
	}

	/*
	 *	Display the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

void InternalSortLines( LPCLASSDATA lpcd, LPPOINT lpLines, BOOL bInternalMode )
{
	LPARRAY  lpTempLines;
	SORTLINE lpLine, *lpSrc;
	LPLINE   lpDest;
	POINT    ptLines;
	int      nFirstLine, nLastLine, i;

	/*
	 *	Are we read-only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Are we in internal mode?
	 */
	if ( bInternalMode == FALSE )
	{
		/*
		*	Do we have a marker and are the first and last line
		*	in the selection different?
		*/
		if ( ! HasMark( lpcd ) || ( lpcd->ptSelStart.y == lpcd->ptSelEnd.y ))
			return;
	}

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Get this first and last line numbers.
	 */
	if ( ! bInternalMode )
	{
		nFirstLine = lpcd->ptSelStart.y;
		nLastLine = lpcd->ptSelEnd.y;
	}
	else
	{
		nFirstLine = lpLines->x;
		nLastLine = lpLines->y;
	}

	/*
	 *	Allocate an array which can hold 
	 *	the lines.
	 */
	if (( lpTempLines = ArrayCreate( 0, nLastLine - nFirstLine, sizeof( SORTLINE ))) != NULL )
	{
		/*
		 *	Copy the lines into the temporary array.
		 */
		for ( i = nFirstLine; i <= nLastLine; i++ )
		{
			lpLine.lnLine = *( LPLINE )ArrayGetAt( lpcd->lpLines, i );
			lpLine.nLine  = i;
			ArrayAdd( lpTempLines, &lpLine, 1 );
		}

		/*
		 *	Note the first and last sorted line.
		 */
		ptLines.x = nFirstLine;
		ptLines.y = nLastLine;

		/*
		 *	Add the undo record containing the
		 *	lines if necessary.
		 */
		if ( bInternalMode == TRUE || AddUndoRecord( lpcd, URF_LINESORT | URF_GROUP, &ptLines, &ptLines, ( LPCTSTR )lpTempLines ) == TRUE )
		{
			/*
			 *	Sort the array.
			 */
			ArraySort( lpTempLines, CompareLines );

			/*
			 *	Copy them back again.
			 */
			for ( i = nFirstLine; i <= nLastLine; i++ )
			{
				lpSrc = ( LPSORTLINE )ArrayGetAt( lpTempLines, i - nFirstLine );
				lpDest = ( LPLINE )ArrayGetAt( lpcd->lpLines, i );
				*lpDest = lpSrc->lnLine;
			}
		}

		/*
		 *	Free the temporary array.
		 */
		ArrayDelete( lpTempLines );
	}
 
	/*
	 *	Skip some stuff when we are in
	 *	internal mode.
	 */
	if ( ! bInternalMode )
	{
		/*
		 *	Clear the marker.
		 */
		if ( HasMark( lpcd ))
			ClearMark( lpcd );

		/*
		 *	Update column position.
		 */
		lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

		/*
		 *	Re-render.
		 */
		InvalidateRect( lpcd->hWnd, NULL, FALSE );

		/*
		 *	Were modifieed.
		 */
		SetModified( lpcd, TRUE );
	}

	/*
	 *	Display the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}
void SortLines( LPCLASSDATA lpcd )
{
	InternalSortLines( lpcd, NULL, FALSE );
}

/*
 *	This function will try to locate a comment initiator
 *	or terminator inside lines the selection covers.
 */
static BOOL ContainsComment( LPCLASSDATA lpcd )
{
	LPLINE	lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, lpcd->ptSelEnd.y );
	BOOL	bFound = FALSE;

	/*
	 *	Do we have a comment initiator and terminator
	 *	configured?
	 */
	if ( Parser->pszCommentInit && Parser->pszCommentTerm )
	{
		/*
		 *	Save current caret position.
		 */
		POINT ptCaretPos = lpcd->ptCaretPos;

		/*
		 *	Save the current find string.
		 */
		LPTSTR pszCurrent = lpcd->bmSkipTable.lpszSearchString;

		/*
		 *	Setup the comment initiator as find
		 *	string and look for it from the selection
		 *	start down.
		 */
		SetBoyerMoore( lpcd, Parser->pszCommentInit );
		lpcd->ptCaretPos = lpcd->ptSelStart;
		lpcd->ptCaretPos.x = 0;
		if ( FindString( lpcd, NULL, 1, TRUE, TRUE, TRUE ))
		{
			/*
			 *	Is it located inside the lines in the selection? The 
			 *	find will set the caret at the end of the found string. 
			 *	If this is located in the selection it is inside the selection.
			 */
			if (( lpcd->ptCaretPos.y < lpcd->ptSelEnd.y ) ||
			    ( lpcd->ptCaretPos.y == lpcd->ptSelEnd.y && lpcd->ptCaretPos.x <= lpLine->nLength ))
				bFound = TRUE;
		}

		/*
		 *	Initiator not found. Try the same for the
		 *	terminator.
		 */
		if ( ! bFound )
		{
			/*
			 *	Setup the comment terminator find
			 *	string and look for it from the selection
			 *	start down.
			 */
			SetBoyerMoore( lpcd, Parser->pszCommentTerm );
			lpcd->ptCaretPos = lpcd->ptSelStart;
			lpcd->ptCaretPos.x = 0;
			if ( FindString( lpcd, NULL, 1, TRUE, TRUE, TRUE ))
			{
				/*
				 *	Is it located inside the lines in the selection? The 
				 *	find will set the caret at the end of the found string. 
				 *	If this is located in the selection it is inside the selection.
				 */
				if (( lpcd->ptCaretPos.y < lpcd->ptSelEnd.y ) ||
				    ( lpcd->ptCaretPos.y == lpcd->ptSelEnd.y && lpcd->ptCaretPos.x <= lpLine->nLength ))
					bFound = TRUE;
			}
		}

		/*
		 *	Restore the caret position.
		 */
		lpcd->ptCaretPos = ptCaretPos;

		/*
		 *	Restore the find string.
		 */
		if ( pszCurrent ) SetBoyerMoore( lpcd, pszCurrent );
		else lpcd->bmSkipTable.lpszSearchString = NULL;

		/*
		 *	Return the found state.
		 */
		return bFound;
	}
	/*
	 *	Can't find what is not configured.
	 */
	return FALSE;
}

void CommentSelection( LPCLASSDATA lpcd )
{
	LPLINE	lpLine;
	POINT	ptDummy;
	BOOL	bPartial = TRUE, bGroup = TRUE, bCaretAtStart;
	int	nLine, nFirstLine, nLastLine;

	/*
	 *	Are we read-only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Do we have a selection?
	 */
	if ( ! HasMark( lpcd ))
		return;

	/*
	 *	If we do not have comment strings configured
	 *	we can bail here.
	 */
	if ( Parser->pszCommentSingle == NULL && ( Parser->pszCommentTerm == NULL || Parser->pszCommentInit == NULL ))
	{
		OkInformation( lpcd, lpcd->hWnd, GetString( IDS_NO_COMMENTS ));
		return;
	}

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );

	/*
	 *	Is the caret located at the start of the selection?
	 */
	bCaretAtStart = ( lpcd->ptCaretPos.x == lpcd->ptSelStart.x && lpcd->ptCaretPos.y == lpcd->ptSelStart.y ) ? TRUE : FALSE;

	/*
	 *	Mark the first and last lines.
	 */
	nFirstLine = lpcd->ptSelStart.y;
	nLastLine  = lpcd->ptSelEnd.y;

	/*
	 *	Do we support partial comments?
	 */
	if ( Parser->pszCommentInit == NULL && Parser->pszCommentTerm == NULL )
	{
		/*
		 *	No. Always interped as a whole lines
		 *	selection.
		 */
		bPartial = FALSE;
	}
	else
	{
		/*
		 *	Does the selection only cover whole
		 *	lines?
		 */
		lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, lpcd->ptSelEnd.y );
		if ( lpcd->ptSelStart.x == 0 && lpcd->ptSelEnd.x == lpLine->nLength && lpLine->nLength )
		{
			/*
			 *	Whole lines including the last line.
			 */
			bPartial = FALSE;
		}
		else if ( lpcd->ptSelStart.x == 0 && lpcd->ptSelEnd.x == 0 )
		{
			/*
			 *	Whole lines excluding the
			 *	last one.
			 */
			nLastLine--;
			bPartial = FALSE;
		}
	}

	/*
	 *	If the selection only contains whole lines
	 *	and we have a single line comment configured we
	 *	insert the single line comment at the start of each line.
	 *
	 *	If we do have a partial selection but this selection contains
	 *	either a comment initiator or a comment terminator we single
	 *	line comment all lines.
	 */
	if ( Parser->pszCommentSingle && ( bPartial == FALSE || ContainsComment( lpcd )))
	{
		/*
		 *	Iterate the lines to comment.
		 */
		for ( nLine = nFirstLine; nLine <= nLastLine; nLine++ )
		{
			/*
			 *	Insert the single line comment.
			 */
			InsertText( lpcd, nLine, 0, Parser->pszCommentSingle, &ptDummy, bGroup );

			/*
			 *	All will go into one group.
			 */
			bGroup = FALSE;
		}

		/*
		 *	Was it a partial selection?
		 */
		if ( bPartial )
		{
			/*
			 *	Get the line comment length.
			 */
			int nLength = _tcslen( Parser->pszCommentSingle );

			/*
			 *	Adjust marker and caret position.
			 */
			lpcd->ptCaretPos.x += nLength;
			lpcd->ptSelStart.x += nLength;
			lpcd->ptSelEnd.x   += nLength;
		}
	}
	/*
	 *	If the selection contains partial lines and we
	 *	have a comment initiator and terminator configured
	 *	we enclose the selection inside the comment strings.
	 */
	else if ( Parser->pszCommentInit && Parser->pszCommentTerm )
	{
		/*
		 *	Insert the comment initiator.
		 */
		if ( InsertText( lpcd, lpcd->ptSelStart.y, lpcd->ptSelStart.x, Parser->pszCommentInit, &ptDummy, TRUE ))
		{
			/*
			 *	Get the comment initiator length.
			 */
			int nLength = _tcslen( Parser->pszCommentInit );

			/*
			 *	Is the caret locate at the start of the
			 *	selection?
			 */
			if ( bCaretAtStart )
			{
				/* 
				 *	Adjust caret and the selection positions.
				 */
				lpcd->ptCaretPos.x += nLength;
				lpcd->ptSelStart.x += nLength;
				if ( lpcd->ptSelStart.y == lpcd->ptSelEnd.y ) lpcd->ptSelEnd.x += nLength;
			}
			else
			{
				/*
				 *	Adjusting the selection positions
				 *	will do.
				 */
				lpcd->ptSelStart.x += nLength;
				if ( lpcd->ptSelStart.y == lpcd->ptSelEnd.y ) 
				{
					lpcd->ptCaretPos.x += nLength;
					lpcd->ptSelEnd.x += nLength;
				}
			}

			/*
			 *	Insert the comment terminator.
			 */
			InsertText( lpcd, lpcd->ptSelEnd.y, lpcd->ptSelEnd.x, Parser->pszCommentTerm, &ptDummy, FALSE );	
		}
	}
	else
		/*
		 *	This should never happen.
		 */
		MessageBeep( 0xFFFFFFFF );

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Re-render.
	 */
	InvalidateRect( lpcd->hWnd, NULL, TRUE );

	/* 
	 *	Were modified.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Display the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}

void UncommentSelection( LPCLASSDATA lpcd )
{
	LPLINE	lpLine;
	BOOL	bPartial = TRUE, bGroup = TRUE, bDone = FALSE, bCaretAtStart;
	POINT	ptFrom, ptTo;
	TCHAR  *pcText;
	int	nLine, nFirstLine, nLastLine, nPos;

	/*
	 *	Are we read-only?
	 */
	if ( ISREADONLY )
		return;

	/*
	 *	Do we have a selection?
	 */
	if ( ! HasMark( lpcd ))
		return;

	/*
	 *	If we do not have comment strings configured
	 *	we can bail here.
	 */
	if ( Parser->pszCommentSingle == NULL && ( Parser->pszCommentTerm == NULL || Parser->pszCommentInit == NULL ))
	{
		OkInformation( lpcd, lpcd->hWnd, GetString( IDS_NO_COMMENTS ));
		return;
	}

	/*
	 *	Hide the caret.
	 */
	DisplayCaret( lpcd, FALSE );
 
	/*
	 *	Is the caret located at the start of the selection?
	 */
	bCaretAtStart = ( lpcd->ptCaretPos.x == lpcd->ptSelStart.x && lpcd->ptCaretPos.y == lpcd->ptSelStart.y ) ? TRUE : FALSE;

	/*
	 *	Mark the first and last lines.
	 */
	nFirstLine = lpcd->ptSelStart.y;
	nLastLine  = lpcd->ptSelEnd.y;

	/*
	 *	Does the selection only cover whole
	 *	lines?
	 */
	lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, lpcd->ptSelEnd.y );
	if ( lpcd->ptSelStart.x == 0 && lpcd->ptSelEnd.x == lpLine->nLength && lpLine->nLength )
	{
		/*
		 * Whole lines including the last line.
		 */
		bPartial = FALSE;
	}
	else if ( lpcd->ptSelStart.x == 0 && lpcd->ptSelEnd.x == 0 )
	{
		/*
		 *	Whole lines excluding the
		 *	last one.
		 */
		nLastLine--;
		bPartial = FALSE;
	}

	/*
	 *	Are we a partial selection and are the comment initiator and
	 *	terminators configured?
	 */
	if ( bPartial == TRUE && Parser->pszCommentInit && Parser->pszCommentTerm )
	{
		/*
		 *	Get the length of the comment initiator
		 *	and terminator.
		 */
		int nInitLen = _tcslen( Parser->pszCommentInit ), nTermLen = _tcslen( Parser->pszCommentTerm );

		/*
		 *	Get a pointer to the top line.
		 */
		lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, nFirstLine );

		/*
		 *	Is there enough room before the selection marker
		 *	to contain the initiator?
		 */
		if ( lpcd->ptSelStart.x >= nInitLen - 1 )
		{
			/*
			 *	Is the comment initiator located directly before
			 *	the selection?
			 */
			if ( ! ( Parser->bCaseOn ? _tcsncmp : _tcsnicmp )( &lpLine->pcText[ lpcd->ptSelStart.x - nInitLen ], Parser->pszCommentInit, nInitLen ))
			{
				/*
				 *	Get a pointer to the last line.
				 */
				lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, nLastLine );

				/*
				 *	Is there enough text left in the line after the 
				 *	selection to fit the comment terminator?
				 */
				if (( lpLine->nLength - lpcd->ptSelEnd.x ) >= nTermLen )
				{
					/*
					 *	Is the comment terminator located directly after
					 *	the selection.
					 */
					if ( ! ( Parser->bCaseOn ? _tcsncmp : _tcsnicmp )( &lpLine->pcText[ lpcd->ptSelEnd.x ], Parser->pszCommentTerm, nTermLen ))
					{
						/*
						 *	Setup the points and delete the
						 *	comment terminator.
						 */
						ptFrom = ptTo = lpcd->ptSelEnd;
						ptTo.x += nTermLen;
						if ( DeleteText( lpcd, &ptFrom, &ptTo, TRUE ))
						{
							/*
							 *	Setup the points and delete the
							 *	comment initiator.
							 */
							ptFrom = ptTo = lpcd->ptSelStart;
							ptFrom.x -= nInitLen;
							DeleteText( lpcd, &ptFrom, &ptTo, FALSE );

							/*
							 *	Adjust the caret position and
							 *	selection position.
							 */
							lpcd->ptSelStart.x -= nInitLen;
							if ( nFirstLine == nLastLine ) 
							{
								lpcd->ptSelEnd.x -= nInitLen;
								lpcd->ptCaretPos.x -= nInitLen;
							}
							else if ( bCaretAtStart ) 
								lpcd->ptCaretPos.x -= nInitLen;

							/*
							 *	Uncommenting has been done.
							 */
							bDone = TRUE;
						}
					}
				}
			}
		}
	}

	/*
	 *	Nothing uncommented yet and do we have a single
	 *	line comment configured?
	 */
	if ( bDone == FALSE && Parser->pszCommentSingle )
	{
		/*
		 *	Determine the length of the single line comment.
		 */
		int nSingleLen = _tcslen( Parser->pszCommentSingle );

		/*
		 *	Iterate the lines.
		 */
		for ( nLine = nFirstLine; nLine <= nLastLine; nLine++ )
		{
			/*
			 *	Get a pointer to the line.
			 */
			lpLine = ( LPLINE )ArrayGetAt( lpcd->lpLines, nLine );

			/*
			 *	Any text on this line?
			 */
			if (( pcText = lpLine->pcText ) != NULL )
			{
				/*
				 *	Skip blanks.
				 */
				nPos = 0;
				while ( nPos < lpLine->nLength && _istspace( pcText[ nPos ] )) nPos++;
			}

			/*
			 *	Are we at the end of the line?
			 */
			if ( nPos < lpLine->nLength )
			{
				/*
				 *	No. Check if the single line comment is located
				 *	at this position.
				 */
				if ( ! ( Parser->bCaseOn ? _tcsncmp : _tcsnicmp )( &pcText[ nPos ], Parser->pszCommentSingle, nSingleLen ))
				{
					/*
					 *	Setup the points and delete the text.
					 */
					ptFrom.y = ptTo.y = nLine;
					ptFrom.x = nPos;
					ptTo.x = nPos + nSingleLen;
					DeleteText( lpcd, &ptFrom, &ptTo, bGroup );

					/*
					 *	No more group marker.
					 */
					bGroup = FALSE;
				}
			}
		}
	}

	/*
	 *	Update column position.
	 */
	lpcd->nLastColumnPos = GetCaretOffset( lpcd, lpcd->ptCaretPos.x );

	/*
	 *	Re-render.
	 */
	InvalidateRect( lpcd->hWnd, NULL, TRUE );

	/*
	 *	Were modifieed.
	 */
	SetModified( lpcd, TRUE );

	/*
	 *	Display the caret.
	 */
	DisplayCaret( lpcd, TRUE );
}