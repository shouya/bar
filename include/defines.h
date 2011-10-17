/* header guard generate by genheader.pl */
#ifndef _defines_h_
#define _defines_h_

/* the following keycode definition are copy & convert from sdl library. */
/* fully compatible to SDLK_* and KMOD_*  */

        /** @name ASCII mapped keysyms
         *  The keyboard syms have been cleverly chosen to map to ASCII
         */
        /*@{*/
# define BARK_UNKNOWN 0
# define BARK_FIRST 0
# define BARK_BACKSPACE 8
# define BARK_TAB 9
# define BARK_CLEAR 12
# define BARK_RETURN 13
# define BARK_PAUSE 19
# define BARK_ESCAPE 27
# define BARK_SPACE 32
# define BARK_EXCLAIM 33
# define BARK_QUOTEDBL 34
# define BARK_HASH 35
# define BARK_DOLLAR 36
# define BARK_AMPERSAND 38
# define BARK_QUOTE 39
# define BARK_LEFTPAREN 40
# define BARK_RIGHTPAREN 41
# define BARK_ASTERISK 42
# define BARK_PLUS 43
# define BARK_COMMA 44
# define BARK_MINUS 45
# define BARK_PERIOD 46
# define BARK_SLASH 47
# define BARK_0 48
# define BARK_1 49
# define BARK_2 50
# define BARK_3 51
# define BARK_4 52
# define BARK_5 53
# define BARK_6 54
# define BARK_7 55
# define BARK_8 56
# define BARK_9 57
# define BARK_COLON 58
# define BARK_SEMICOLON 59
# define BARK_LESS 60
# define BARK_EQUALS 61
# define BARK_GREATER 62
# define BARK_QUESTION 63
# define BARK_AT 64
 /* 
    Skip uppercase letters
  */
# define BARK_LEFTBRACKET 91
# define BARK_BACKSLASH 92
# define BARK_RIGHTBRACKET 93
# define BARK_CARET 94
# define BARK_UNDERSCORE 95
# define BARK_BACKQUOTE 96
# define BARK_a 97
# define BARK_b 98
# define BARK_c 99
# define BARK_d 100
# define BARK_e 101
# define BARK_f 102
# define BARK_g 103
# define BARK_h 104
# define BARK_i 105
# define BARK_j 106
# define BARK_k 107
# define BARK_l 108
# define BARK_m 109
# define BARK_n 110
# define BARK_o 111
# define BARK_p 112
# define BARK_q 113
# define BARK_r 114
# define BARK_s 115
# define BARK_t 116
# define BARK_u 117
# define BARK_v 118
# define BARK_w 119
# define BARK_x 120
# define BARK_y 121
# define BARK_z 122
# define BARK_DELETE 127
 /* End of ASCII mapped keysyms */
        /*@}*/

 /** @name Numeric keypad */
        /*@{*/
# define BARK_KP0 256
# define BARK_KP1 257
# define BARK_KP2 258
# define BARK_KP3 259
# define BARK_KP4 260
# define BARK_KP5 261
# define BARK_KP6 262
# define BARK_KP7 263
# define BARK_KP8 264
# define BARK_KP9 265
# define BARK_KP_PERIOD 266
# define BARK_KP_DIVIDE 267
# define BARK_KP_MULTIPLY 268
# define BARK_KP_MINUS 269
# define BARK_KP_PLUS 270
# define BARK_KP_ENTER 271
# define BARK_KP_EQUALS 272
        /*@}*/

 /** @name Arrows + Home/End pad */
        /*@{*/
# define BARK_UP 273
# define BARK_DOWN 274
# define BARK_RIGHT 275
# define BARK_LEFT 276
# define BARK_INSERT 277
# define BARK_HOME 278
# define BARK_END 279
# define BARK_PAGEUP 280
# define BARK_PAGEDOWN 281
        /*@}*/

 /** @name Function keys */
        /*@{*/
# define BARK_F1 282
# define BARK_F2 283
# define BARK_F3 284
# define BARK_F4 285
# define BARK_F5 286
# define BARK_F6 287
# define BARK_F7 288
# define BARK_F8 289
# define BARK_F9 290
# define BARK_F10 291
# define BARK_F11 292
# define BARK_F12 293
# define BARK_F13 294
# define BARK_F14 295
# define BARK_F15 296
        /*@}*/

 /** @name Key state modifier keys */
        /*@{*/
# define BARK_NUMLOCK 300
# define BARK_CAPSLOCK 301
# define BARK_SCROLLOCK 302
# define BARK_RSHIFT 303
# define BARK_LSHIFT 304
# define BARK_RCTRL 305
# define BARK_LCTRL 306
# define BARK_RALT 307
# define BARK_LALT 308
# define BARK_RMETA 309
# define BARK_LMETA 310
# define BARK_LSUPER 311 /**< Left "Windows" key */
# define BARK_RSUPER 312 /**< Right "Windows" key */
# define BARK_MODE 313 /**< "Alt Gr" key */
# define BARK_COMPOSE 314 /**< Multi-key compose key */
        /*@}*/


/* key mods */

# define BARKM_NONE   0x0000
# define BARKM_LSHIFT 0x0001
# define BARKM_RSHIFT 0x0002
# define BARKM_LCTRL  0x0040
# define BARKM_RCTRL  0x0080
# define BARKM_LALT   0x0100
# define BARKM_RALT   0x0200
# define BARKM_LMETA  0x0400
# define BARKM_RMETA  0x0800
# define BARKM_NUM    0x1000
# define BARKM_CAPS   0x2000
# define BARKM_MODE   0x4000
# define BARKM_RESERVED  0x8000

# define BARKM_CTRL	(BARKM_LCTRL|BARKM_RCTRL)
# define BARKM_SHIFT	(BARKM_LSHIFT|BARKM_RSHIFT)
# define BARKM_ALT	(BARKM_LALT|BARKM_RALT)
# define BARKM_META	(BARKM_LMETA|BARKM_RMETA)



#endif /* _defines_h_ */
