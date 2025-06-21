/**
 ** strtol - •¶š—ñ‚ğ long ®”’l‚É•ÏŠ·‚µ‚Ü‚·B
 **     [whitespace] [{+ | -}] [0 [{ x | X }]] [digits]
 **/

long strtol( const char *nptr, char **endptr, int base )
{
    long l;
    char c;
    char flag = 1;

    *endptr = (char *)nptr;

    /* skip space */
    while (c = *nptr++)
    {
        if (c != ' ' && c != '\t')
            break;
    }

    /* •„† */
    if (c == '-')
        flag = 0;
    else if (c != '+')
        --nptr;

    /* ”’l‚ÌŠî”‚Ì”»’è */
    if (nptr[0] == '0' && (nptr[1] == 'x' || nptr[1] == 'X') && (base == 0 || base == 16))
    {
        base = 16;
        nptr += 2;
    }
    else if (nptr[0] == '0' && (base == 0 || base == 8))
    {
        base = 8;
        nptr += 1;
    }
    else if (base == 0)
    {
        base = 10;
    }

    l = 0;
    for (;;)
    {
        c = *nptr++;
        if ('0' <= c && c <= '9')
            c -= '0';
        else if ('A' <= c && c <= 'Z')
            c -= 'A' - 10;
        else if ('a' <= c && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        l = l * base + c;
        *endptr = (char *)nptr;
    }
    return flag ? l : -l;
}
