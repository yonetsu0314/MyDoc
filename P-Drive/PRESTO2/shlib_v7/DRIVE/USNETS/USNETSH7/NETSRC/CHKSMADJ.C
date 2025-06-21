/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/chksmadj.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: chksmadj.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* CHKSMADJ.C  Edition: 1  Last changed: 25-Jan-99,12:05:02  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1998 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    CHKSMADJ.C   Checksum adjusting module (routine)

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by United
    States Software Corporation.

 * * * * * * * * * * * * * * * * * * * * * * * * *
   MODIFICATION HISTORY:
    25-JAN-1999  BSK   Add: New to USNET
 * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/** * * * * * *
 ** void ussChecksumAdjust()  Adjust the value of a checksum
 **
 ** void ussChecksumAdjust(unsigned char *, unsigned char *, int,
 **     unsigned char *, int);
 **
 ** PARAMETERS:
 **   (in/out) unsigned char *chksum    Pointer to checksum to change
 **   (in) unsigned char *optr          Old data
 **   (in) int olen                     Length of old data
 **   (in) unsigned char *nptr          New data
 **   (in) int nlen                     Length of new data
 **
 ** DESCRIPTION:
 **   Assuming: unsigned char is 8 bits, long is 32 bits.
 **   Adjust checksum where the old data at optr is replaced
 **   with the new data at nptr.
 **
 ** USAGE/COMMENTS:
 **   This function assumes that chars are 8 bits and longs are 32 bits.
 **   It will adjust TCP and UDP checksums.
 **
 ** * * * * * *
 ** MODIFICATION HISTORY:
 **   25-JAN-1999  BSK  Created comment
 ** * * * * * */
void ussChecksumAdjust(unsigned char *chksum, unsigned char *optr, int olen,
    unsigned char *nptr, int nlen)
{
    long x, old, new;

    x = chksum[0] * 256 + chksum[1];
    x = ~x & 0xFFFF;
    while (olen) {
        old = optr[0] * 256 + optr[1];
        optr += 2;
        x -= old & 0xffff;
        if (x <= 0) {
            x--;
            x &= 0xffff;
        }
        olen -= 2;
    }
    while (nlen) {
        new = nptr[0] * 256 + nptr[1];
        nptr += 2;
        x += new & 0xffff;
        if (x & 0x10000) {
            x++;
            x &= 0xffff;
        }
        nlen -= 2;
    }
    x = ~x & 0xFFFF;
    chksum[0] = x / 256;
    chksum[1] = x & 0xff;
}

