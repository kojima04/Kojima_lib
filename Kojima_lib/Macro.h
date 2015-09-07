#pragma once

#define sqr(x) ( (x)*(x) )

#define SAFE_DELETE(x)		{delete (x);(x)=NULL;}

#define SAFE_DELETEARRAY(x)	{delete [] (x);(x)=NULL;}

#define SAFE_RELEASE(x)		{(x)->Release();(x)=NULL;}

#define	ERROR_MSG(x)		MessageBox(NULL, x,"ERROR", MB_OK)

#define	PI		3.14159265f

#define DEBUG
//#define SILENT
#define KEY
#define	FULLSCREEN 0
#define BUSITSU





