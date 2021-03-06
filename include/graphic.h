/* header guard generate by genheader.pl */
#ifndef _graphic_h_
#define _graphic_h_

#define PARAM_RGBA int r, int g, int b, int a

#define PIXR(rgba) (((rgba)>>16)&0xff)
#define PIXG(rgba) (((rgba)>>8)&0xff)
#define PIXB(rgba) ((rgba)&0xff)
#define PIXA(rgba) (((rgba)>>24)&0xff)
#define PACKR(r) (((r)&0xff)<<16)
#define PACKG(g) (((g)&0xff)<<8)
#define PACKB(b) ((b)&0xff)
#define PACKA(a) (((a)&0xff)<<24)
#define UNPACK_RGB(rgb) PIXR(rgb), PIXG(rgb), PIXB(rgb)
#define UNPACK_RGBA(rgba) UNPACK_RGB(rgba), PIXA(rgba)
#define PACK_RGBA(r,g,b,a) (PACKR(r)|PACKG(g)|PACKB(b)|PACKA(a))
#define PACK_RGB(r,g,b) PACK_RGBA(r,g,b,0xff)


#ifdef ALLOW_TTF
#define FONT_H 10
#endif /* allow ttf */

struct canvas_t;

void init_graphic(void);

void drawpixel(struct canvas_t* canvas, int x, int y, PARAM_RGBA);
void drawrect(struct canvas_t* canvas, int x, int y, int w, int h, PARAM_RGBA);
void fillrect(struct canvas_t* canvas, int x, int y, int w, int h, PARAM_RGBA);
/* returns width of the string drawn */
int drawstr(struct canvas_t* canvas, int x, int y, const char* s, PARAM_RGBA);

#endif /* _graphic_h_ */
