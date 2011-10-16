/* header guard generate by genhead.pl */
#ifndef _shape_h_
#define _shape_h_

#define SHAPE_W 3
#define SHAPE_H 4

struct shape_t {
  int no;
  unsigned char pix[SHAPE_H][SHAPE_W];
  unsigned long color;
};

extern struct shape_t* g_shps;
extern int g_numshp;

void default_init_shapes(void);


#endif /* _shape_h_ */
