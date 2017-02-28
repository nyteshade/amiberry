 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Interface to the graphics system (X, SVGAlib)
  *
  * Copyright 1995-1997 Bernd Schmidt
  */

#pragma once
#include "machdep/rpt.h"

typedef uae_u32 xcolnr;

typedef int(*allocfunc_type)(int, int, int, xcolnr *);

extern xcolnr xcolors[4096];
extern xcolnr xcolors_16[4096];
extern xcolnr xcolors_32[4096];
extern uae_u32 p96_rgbx16[65536];

extern int graphics_setup (void);
extern int graphics_init (bool);
extern void graphics_leave (void);
extern void graphics_reset(void);
extern bool handle_events(void);
extern int handle_msgpump (void);
extern void setup_brkhandler (void);
extern bool vsync_switchmode (int);
extern void flush_screen (void);
extern int isvsync_chipset(void);
extern int isvsync_rtg(void);
extern int isvsync(void);

extern int lockscr (void);
extern void unlockscr (void);
extern bool target_graphics_buffer_update (void);

extern void screenshot (int);

extern int bits_in_mask (unsigned long mask);
extern int mask_shift (unsigned long mask);
extern unsigned int doMask (int p, int bits, int shift);
extern unsigned int doMask256 (int p, int bits, int shift);
extern void alloc_colors64k (int, int, int, int, int, int, int);
extern void alloc_colors_picasso (int rw, int gw, int bw, int rs, int gs, int bs, int rgbfmt);
extern double getvsyncrate(double hz, int *mult);

/* The graphics code has a choice whether it wants to use a large buffer
* for the whole display, or only a small buffer for a single line.
* If you use a large buffer:
*   - set bufmem to point at it
*   - set linemem to 0
*   - if memcpy within bufmem would be very slow, i.e. because bufmem is
*     in graphics card memory, also set emergmem to point to a buffer
*     that is large enough to hold a single line.
*   - implement flush_line to be a no-op.
* If you use a single line buffer:
*   - set bufmem and emergmem to 0
*   - set linemem to point at your buffer
*   - implement flush_line to copy a single line to the screen
*/
struct vidbuffer
{
	/* Function implemented by graphics driver */
	void(*flush_line)         (struct vidbuf_description *gfxinfo, struct vidbuffer *vb, int line_no);
	void(*flush_block)        (struct vidbuf_description *gfxinfo, struct vidbuffer *vb, int first_line, int end_line);
	void(*flush_screen)       (struct vidbuf_description *gfxinfo, struct vidbuffer *vb, int first_line, int end_line);
	void(*flush_clear_screen) (struct vidbuf_description *gfxinfo, struct vidbuffer *vb);
	int(*lockscr)            (struct vidbuf_description *gfxinfo, struct vidbuffer *vb);
	void(*unlockscr)          (struct vidbuf_description *gfxinfo, struct vidbuffer *vb);
	uae_u8 *linemem;
	uae_u8 *emergmem;

	uae_u8 *bufmem, *bufmemend;
	uae_u8 *realbufmem;
	uae_u8 *bufmem_allocated;
	bool bufmem_lockable;
	int rowbytes; /* Bytes per row in the memory pointed at by bufmem. */
	int pixbytes; /* Bytes per pixel. */
				  /* size of this buffer */
	int width_allocated;
	int height_allocated;
	/* size of max visible image */
	int outwidth;
	int outheight;
	/* nominal size of image for centering */
	int inwidth;
	int inheight;
	/* same but doublescan multiplier included */
	int inwidth2;
	int inheight2;
	/* use drawbuffer instead */
	bool nativepositioning;
	/* tempbuffer in use */
	bool tempbufferinuse;
	/* extra width, chipset hpos extra in right border */
	int extrawidth;

	int xoffset; /* superhires pixels from left edge */
	int yoffset; /* lines from top edge */

	int inxoffset; /* positive if sync positioning */
	int inyoffset;
};

extern int max_uae_width, max_uae_height;

struct vidbuf_description
{
  uae_u8 *bufmem;
  int rowbytes; /* Bytes per row in the memory pointed at by bufmem. */
  int pixbytes; /* Bytes per pixel. */
  int outwidth;
  int outheight;
};

extern struct vidbuf_description gfxvidinfo;

/* For ports using tui.c, this should be built by graphics_setup(). */
extern struct bstring *video_mode_menu;
extern void vidmode_menu_selected(int);
