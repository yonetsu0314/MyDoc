/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/hitachi/RCS/stddef.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: stddef.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/
/* STDDEF.H */
/* Replacement for standard Hitachi stddef.h file.  The treatment of errno 
   caused us difficulties. */
/*
    Copyright (C) 1997 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com
*/

#ifndef size_t
#define size_t unsigned long
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef errno
#define errno _errno
extern volatile int _errno;
#endif
#undef EBADF
