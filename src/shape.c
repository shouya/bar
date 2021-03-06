#include <stdlib.h>

#include <shape.h>

struct shape_t* g_shps = NULL;
int g_numshp = 0;

static void fillpixels(unsigned char pix[SHAPE_H][SHAPE_W],
                       int(*coord)[2], int num);

static int shp_pix[][4][2] = {
  /*O*/{{0,0},{0,1},{1,1},{1,0}},
  /*I*/{{0,0},{0,1},{0,2},{0,3}},
  /*L*/{{0,0},{0,1},{0,2},{1,2}},
  /*J*/{{1,0},{1,1},{1,2},{0,2}},
  /*Z*/{{0,0},{1,0},{1,1},{2,1}},
  /*S*/{{0,1},{1,1},{1,0},{2,0}},
  /*T*/{{0,0},{1,0},{2,0},{1,1}},
};

static int shp_sz[][2] = {
  {2,2}, {1,4}, {2,3}, {2,3}, {3,2}, {3,2}, {3,2}
};

static unsigned long shp_color[] = {
  /*O:Yello I:Cyan  L:Orange J:Blue Z: Red    S:Lime  T:Purple*/
  0xffff00, 0xffff, 0xff7f00, 0xff, 0xff0000, 0xff00, 0x7f007f
};

void default_init_shapes(void) {
  int i = 0;

  g_numshp = 7;
  g_shps = calloc(g_numshp, sizeof(struct shape_t));
  for (; i != g_numshp; ++i) {
    g_shps[i].no = i;
    fillpixels(g_shps[i].pix, shp_pix[i], 4);
    g_shps[i].w = shp_sz[i][0];
    g_shps[i].h = shp_sz[i][1];
    g_shps[i].color = shp_color[i];
  }
}

void fillpixels(unsigned char pix[SHAPE_H][SHAPE_W],
                int (*coord)[2], int num) {
  int j, i, n;
  
  for (j = 0; j != SHAPE_H; ++j) {
    for (i = 0; i != SHAPE_W; ++i) {
      pix[j][i] = 0;
      for (n = 0; n != num; ++n) {
        if (coord[n][0] == i && coord[n][1] == j) {
          pix[j][i] = 1;
        }
      } /* end of each coord */
    } /*  \  */
  } /*    / for each pixel */
}
