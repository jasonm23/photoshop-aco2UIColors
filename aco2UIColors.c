/* aco2UIcolors.c - .aco photoshop palette file to list of UIColor declarations
 *
 * based on aco2HTMLcolors - Copyright(C) 2006 Salvatore Sanfilippo 
 * http://www.hping.org/aco2html/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct acoentry {
    unsigned char r, g, b;
    char *name; /* NULL if no name is available */
};

struct aco {
    int ver;    /* ACO file version, 1 or 2 */
    int len;    /* number of colors */
    struct acoentry *color; /* array of colors as acoentry structures */
};

/* proper error handling is not for lazy people ;) */
void *acomalloc(size_t len)
{
    void *ptr = malloc(len);
    if (ptr == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(1);
    }
    return ptr;
}

/* Read a 16bit word in big endian from 'fp' and return it
 * converted in the host byte order as usigned int.
 * On end of file -1 is returned. */
int readword(FILE *fp)
{
    unsigned char buf[2];
    unsigned int w;
    int retval;

    retval = fread(buf, 1, 2, fp);
    if (retval == 0)
        return -1;
    w = buf[1] | buf[0] << 8;
    return w;
}

/* Version of readword() that exists with an error message
 * if an EOF occurs */
int mustreadword(FILE *fp) {
    int w;

    w = readword(fp);
    if (w == -1) {
        fprintf(stderr, "Unexpected end of file!\n");
        exit(1);
    }
    return w;
}

void genUIColors(struct aco *aco)
{
    int j;
    int cols = 4;

    for (j = 0; j < aco->len; j += cols) {
        int i;
        for (i = 0; i < cols; i++) {

            int r, g, b;
            char *class, *name;

            if (j+i == aco->len) break;

            r = aco->color[j+i].r;
            g = aco->color[j+i].g;
            b = aco->color[j+i].b;

            float rf = (float) r / 255;
            float gf = (float) g / 255;
            float bf = (float) b / 255;

            printf("[UIColor colorWithRed:%f green:%f blue:%f alpha:1.0f], // #%02x%02x%02x  rgb(%i,%i,%i) \n",
                   rf, gf, bf, r, g, b, r, g, b);
        }
    }

}

int convertcolor(FILE *fp, int ver, int *r, int *g, int *b, int buflen)
{
    int cspace = mustreadword(fp);
    int skiplen;

    if (cspace != 0) {
        int j;
        for (j = 0; j < 4; j++) mustreadword(fp);
        if (ver == 2) {
            mustreadword(fp);
            skiplen = mustreadword(fp);
            for (j = 0; j < skiplen; j++)
                mustreadword(fp);
        }
        fprintf(stderr, "Non RGB color (colorspace %d) skipped - use RGB colorspace\n", cspace);
        return 1;
    }
    *r = mustreadword(fp)/256;
    *g = mustreadword(fp)/256;
    *b = mustreadword(fp)/256;
    mustreadword(fp); /* just skip this word, (Z not used for RGB) */
    if (ver == 1) return 0;
    mustreadword(fp); /* just skip this word, don't know what it's used for */
    skiplen = mustreadword(fp);
    skiplen--;
    while(skiplen > 0) {
        int c = mustreadword(fp);        

        if (c > 0xff) /* just ascii */
            c = ' ';

        if (buflen > 1)
            buflen--;

        skiplen--;
    }
    mustreadword(fp);
    return 0;
}

/* Read an ACO file from 'infp' FILE and return
 * the structure describing the palette.
 *
 * On initial end of file NULL is returned.
 * That's not a real library to read this format but just
 * an hack in order to write this convertion utility, so
 * on error we just exit(1) brutally after priting an error. */
struct aco *readaco(FILE *infp)
{
    int ver;
    int colors;
    int j;
    struct aco *aco;

    ver = readword(infp);
    if (ver < 1 || ver > 2) {
        fprintf(stderr, "Unknown ACO file version %d, unable to read, exiting...\n", ver);
        exit(1);
    }

    colors = readword(infp);
    aco = acomalloc(sizeof(*aco));
    aco->len = colors;
    aco->color = acomalloc(sizeof(struct acoentry)*aco->len);
    aco->ver = ver;

    for(j=0; j < colors; j++) {
        int r,g,b;
        if (convertcolor(infp, ver, &r, &g, &b, 256)) continue;
        aco->color[j].r = r;
        aco->color[j].g = g;
        aco->color[j].b = b;
    }
    return aco;
}

int main(void)
{
    struct aco *aco;
    aco = readaco(stdin);
    fprintf(stderr, "Read ACO : Generating UIColors (Hex & RGB Triplets @c77-85 & c86-)\n\n");
    if(aco)
        genUIColors(aco);
    else
        fprintf(stderr, "No Colors found\n");
    return 0;
}
