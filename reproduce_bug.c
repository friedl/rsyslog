#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef unsigned char uchar;

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
    uchar *line = (uchar*) "/var/log/hatestd.log, 50000, /usr/bin/scripts/log_rotation_script";
    uchar *p = line;
    char *template;
    long size;
    char *cmd;

    get_Field(&p, &template);
    printf("Template: '%s', rest: '%s'\n", template, p);

    if (*p) get_off_t(&p, &size);
    printf("Size: %ld, rest: '%s'\n", size, p);

    if (*p) get_restOfLine(&p, &cmd);
    printf("Cmd: '%s', rest: '%s'\n", cmd, p);

    return 0;
}
