/*
 *	messagebox.c
 *
 *	(C) Copyright 1999-2005 Jan van den Baard.
 *	    All Rights Reserved.
 */

#include "defs.h"

static int MsgBox( LPCLASSDATA lpcd, HWND hParent, DWORD dwType, LPCTSTR lpszBody, va_list args )
{
	TCHAR		szBuffer[ 1024 ];
	
	/*
	 *	Clear qualifiers.
	 */
	lpcd->cQualifier = 0;

	/*
	 *	Format the arguments.
	 */
	_vstprintf_s( szBuffer, 1024, lpszBody, args );
	return MessageBox( hParent ? hParent : lpcd->hWnd, szBuffer, lpcd->szFileName, dwType );
}

int OkWarning( LPCLASSDATA lpcd, HWND hParent, LPCTSTR lpszBody, ... )
{
	va_list		args;
	int		rc;

	va_start( args, lpszBody );
	rc = MsgBox( lpcd, hParent, MB_OK | MB_APPLMODAL | MB_ICONWARNING, lpszBody, args );
	va_end( args );

	return rc;
}

int OkInformation( LPCLASSDATA lpcd, HWND hParent, LPCTSTR lpszBody, ... )
{
	va_list		args;
	int		rc;

	va_start( args, lpszBody );
	rc = MsgBox( lpcd, hParent, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION, lpszBody, args );
	va_end( args );

	return rc;
}

int YesNoWarning( LPCLASSDATA lpcd, HWND hParent, LPCTSTR lpszBody, ... )
{
	va_list		args;
	int		rc;

	va_start( args, lpszBody );
	rc = MsgBox( lpcd, hParent, MB_YESNO | MB_APPLMODAL | MB_ICONWARNING, lpszBody, args );
	va_end( args );

	return rc;
}

int YesNoCancelWarning( LPCLASSDATA lpcd, HWND hParent, LPCTSTR lpszBody, ... )
{
	va_list		args;
	int		rc;
	
	va_start( args, lpszBody );
	rc = MsgBox( lpcd, hParent, MB_YESNOCANCEL | MB_APPLMODAL | MB_ICONWARNING, lpszBody, args );
	va_end( args );

	return rc;
}

int OkIOError( LPCLASSDATA lpcd, HWND hParent, LPCTSTR lpszBody, ... )
{
	TCHAR		sz[ 1024 ];
	va_list		args;
	LPVOID		lpMsgBuf;
	int		rc = 0;
	
	va_start( args, lpszBody );

	if ( FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
			    FORMAT_MESSAGE_FROM_SYSTEM |
			    FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
			    ( LPTSTR )&lpMsgBuf, 0, NULL))
	{
		/*
		 *	Build the message.
		 */
		_tcsncpy_s( sz, 1024, lpszBody, 1024 );
		_tcsncat_s( sz, 1024, _T( "\n\n" ), 1024 );
		_tcsncat_s( sz, 1024, lpMsgBuf, 1024 );
		
		/*
		 *	Free the formatted output.
		 */
		LocalFree( lpMsgBuf );

		/*
		 *	Put up the message box.
		 */
		rc = MsgBox( lpcd, hParent, MB_OK | MB_APPLMODAL | MB_ICONERROR, sz, args );
	}
	va_end( args );
	return rc;
}

int OkError( LPCLASSDATA lpcd, HWND hParent, LPCTSTR lpszBody, ... )
{
	va_list		args;
	int		rc;
	
	va_start( args, lpszBody );
	rc = MsgBox( lpcd, hParent, MB_OK | MB_APPLMODAL | MB_ICONERROR, lpszBody, args );
	va_end( args );

	return rc; 
}

void ErrorMsg( LPCTSTR lpszBody, ... ) 
{
	va_list		args;
	TCHAR		szBuffer[ 1024 ];
	
	/*
	 *	Format the arguments.
	 */
	va_start( args, lpszBody );
	_vstprintf_s( szBuffer, 1024, lpszBody, args );
	MessageBox( NULL, szBuffer, _T("Brainchild DLL"), MB_OK | MB_APPLMODAL | MB_ICONERROR );
	va_end( args );
}

void AboutControl( LPCLASSDATA lpcd )
{
	TCHAR		szBuffer[ 1024 ];
	
	/*
	 *	Clear qualifiers.
	 */ 
	lpcd->cQualifier = 0;

	/*
	 *	Show the messagebox.
	 *
	 *	These strings are not translated.
	 */
	_stprintf_s( szBuffer, 1024, VERSION_MESSAGE, BCVERSION, BCREVISION, __DATE__, __TIME__ );
	MessageBox( lpcd->hWnd, szBuffer, lpcd->szFileName, MB_OK | MB_APPLMODAL | MB_ICONINFORMATION );
}