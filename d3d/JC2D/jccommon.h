#ifndef __JCCOMMON_H__
#define __JCCOMMON_H__

#include <Windows.h>
#include <cassert>

#define jccommon_assertM(c) { assert(c); }

#define jccommon_releaseComM(lp) { if(lp != NULL){ lp->Release(); lp = NULL; } }
#define jccommon_newM(lp, NewType) { assert(lp == NULL); lp = new NewType(); assert(lp != NULL); }
#define jccommon_deleteM(lp) { if(lp != NULL){ delete lp; lp = NULL; } }
#define jccommon_deleteArrayM(lp) { if(lp != NULL){ delete[] lp; lp = NULL; } }

#define jccommon_mallocM(bytes, lpDest, lpDestType) { assert(lpDest == NULL); lpDest = (lpDestType)malloc(bytes); assert(lpDest != NULL); }
#define jccommon_mallocCleanM(bytes, lpDest, lpDestType) { assert(lpDest == NULL); lpDest = (lpDestType)malloc(bytes); assert(lpDest != NULL); memset(lpDest, 0, bytes); }
#define jccommon_memsetM(lpDest, value, destBytes) { assert(lpDest != NULL); memset(lpDest, value, destBytes); }
#define jccommon_memcpyM(lpDest, lpSrc, srcBytes) { assert(lpDest != NULL); assert(lpSrc != NULL); memcpy(lpDest, lpSrc, srcBytes); }
#define jccommon_zeromem(lpDest, destBytes) { assert(lpDest != NULL) ; memset(lpDest, 0, destBytes); }
#define jccommon_memcpyM(lpDest, lpSrc, srcBytes) { assert(lpDest != NULL); assert(lpSrc != NULL); memcpy(lpDest, lpSrc, srcBytes); }

#define jccommon_hResultVerifyM(hResult) { if(FAILED(hResult)) { assert(FALSE); } }

#define jccommon_stdIterForEachM(collectionType, collection, iter) for(collectionType::iterator iter = collection.begin(); iter != collection.end(); ++iter)
#define jccommon_stdRIterForEachM(collectionType, collection, iter) for(collectionType::reverse_iterator iter = collection.rbegin(); iter != collection.rend(); ++iter)

#endif