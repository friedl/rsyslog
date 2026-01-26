#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef unsigned char uchar;

int getSubString(uchar **ppSrc, char *pDst, size_t DstSize, char cSep) {
    uchar *pSrc = *ppSrc;
    int iErr = 0; /* 0 = no error, >0 = error */
    while ((cSep == ' ' ? !isspace(*pSrc) : *pSrc != cSep) && *pSrc != '\n' && *pSrc != '\0' && DstSize > 1) {
        *pDst++ = *(pSrc)++;
        DstSize--;
    }
    if (*pSrc == '\0' || *pSrc == '\n')
        *ppSrc = pSrc;
    else
        *ppSrc = pSrc + 1;
    *pDst = '\0';
    return iErr;
}

static void skip_Comma(uchar **pp) {
    register uchar *p;
    p = *pp;
    while (isspace(*p)) ++p;
    if (*p == ',') ++p;
    while (isspace(*p)) ++p;
    *pp = p;
}

static int get_off_t(uchar **pp, long *pOff_t) {
    register uchar *p;
    long val;
    skip_Comma(pp);
    p = *pp;
    val = 0;
    while (*p && isdigit((int)*p)) {
        val = val * 10 + (*p - '0');
        ++p;
    }
    *pp = p;
    *pOff_t = val;
    return 0;
}

static void get_Field(uchar **pp, char **pField) {
    skip_Comma(pp);
    uchar *p = *pp;
    char buf[1024];
    int i = 0;
    while (*p && *p != ' ' && *p != ',') {
        buf[i++] = *p++;
    }
    buf[i] = '\0';
    *pp = p;
    *pField = strdup(buf);
}

static void get_restOfLine(uchar **pp, char **pBuf) {
    skip_Comma(pp);
    uchar *p = *pp;
    *pBuf = strdup((char*)p);
    while (*p) p++;
    *pp = p;
}

int main() {
    uchar line[] = "hatestd_log,/var/log/hatestd.log, 50000, /usr/bin/scripts/log_rotation_script";
    uchar *p = line;
    char szName[128];
    char *template;
    long size = -1;
    char *cmd = NULL;

    getSubString(&p, szName, sizeof(szName), ',');
    printf("Name: '%s', rest: '%s'\n", szName, p);

    if (*p == ',') ++p;
    printf("After potential second comma eat: rest: '%s'\n", p);

    get_Field(&p, &template);
    printf("Template: '%s', rest: '%s'\n", template, p);

    if (*p) get_off_t(&p, &size);
    printf("Size: %ld, rest: '%s'\n", size, p);

    if (*p) get_restOfLine(&p, &cmd);
    printf("Cmd: '%s', rest: '%s'\n", cmd ? cmd : "NULL", p);

    return 0;
}
