// SPDX-License-Identifier: BSD-3-Clause

#ifndef TGL_TGL_H
#define TGL_TGL_H

// standard library.
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// system dependencies.
#include <SDL.h>
#include <SDL_ttf.h>

//--------------------------//
// library meta-information //
//--------------------------//

#define TGL_VERMAJOR 1
#define TGL_VERMINOR 1
#define TGL_VERPATCH 0

//--------//
// macros //
//--------//

// profiling.
#ifdef TGL_PROFILE
#define TGL_NEWTIMER(timer) uint64_t timer
#define TGL_BEGINTIMER(timer) tgl_begintimer(name)
#define TGL_ENDTIMER(timer, name) tgl_endtimer(timer, name)
#else
#define TGL_NEWTIMER(timer)
#define TGL_BEGINTIMER(timer)
#define TGL_ENDTIMER(timer, name)
#endif

// util constants.
#define TGL_PI 3.141592f
#define TGL_TAU (2.0f * TGL_PI)

// library data constants.
#define TGL_MAXRESNAME 15
#define TGL_BATCHALIGN 16
#define TGL_MAXOPTIONKEY 127
#define TGL_MAXOPTIONVALUE 127
#define TGL_OPTIONSCAN "%127s = %127[^\r\n]"

//--------------------//
// enumeration values //
//--------------------//

typedef enum tgl_color
{
	TGL_PANELCOLOR = 0,
	TGL_LABELTEXTCOLOR,
	TGL_BUTTONCOLOR,
	TGL_BUTTONPRESSCOLOR,
	TGL_BUTTONHOVERCOLOR,
	TGL_BUTTONTEXTCOLOR,
	TGL_BUTTONTEXTPRESSCOLOR,
	TGL_BUTTONTEXTHOVERCOLOR,
	TGL_SLIDERCOLOR,
	TGL_SLIDERPRESSCOLOR,
	TGL_SLIDERHOVERCOLOR,
	TGL_SLIDERBARCOLOR,
	TGL_SLIDERBARPRESSCOLOR,
	TGL_SLIDERBARHOVERCOLOR,
	TGL_SLIDERTEXTCOLOR,
	TGL_SLIDERTEXTPRESSCOLOR,
	TGL_SLIDERTEXTHOVERCOLOR
} tgl_color_t;

typedef enum tgl_uitype
{
	TGL_LABEL = 0,
	TGL_BUTTON,
	TGL_SLIDER
} tgl_uitype_t;

typedef enum tgl_restype
{
	TGL_IMAGEFILE = 0,
	TGL_SOUNDFILE,
	TGL_MODELFILE,
	TGL_FONTFILE,
	TGL_MAPFILE
} tgl_restype_t;

//-----------------//
// data structures //
//-----------------//

typedef struct tgl_conf
{
	// library function config.
	FILE *log;
	char const *errtitle;
	uint64_t tickus;
	int32_t uipad;
	
	// rendering call config.
	void (*renderrect)(int32_t, int32_t, int32_t, int32_t, tgl_color_t);
	void (*rendertext)(int32_t, int32_t, int32_t, int32_t, char const *, tgl_color_t);
} tgl_conf_t;

typedef union tgl_uielem
{
	// all UI elements have at least these properties.
	struct
	{
		uint8_t type;
		int32_t x, y;
		int32_t w, h;
	} any;
	
	struct
	{
		uint8_t type;
		int32_t x, y;
		int32_t w, h;
		char const *text;
	} label;
	
	struct
	{
		uint8_t type;
		int32_t x, y;
		int32_t w, h;
		char const *text;
	} button;
	
	struct
	{
		uint8_t type;
		int32_t x, y;
		int32_t w, h;
		char const *text;
		float *val;
	} slider;
} tgl_uielem_t;

typedef struct tgl_ui
{
	tgl_uielem_t *elems;
	size_t nelems, elemcap;
	int32_t x, y;
	TTF_Font *font;
	SDL_Window const *wnd;
} tgl_ui_t;

typedef struct tgl_res
{
	uint8_t type;
	char name[TGL_MAXRESNAME];
	uint32_t flags;
	uint32_t size;
	uint8_t *data; // invalidated upon pack update.
} tgl_res_t;

typedef struct tgl_pack
{
	void *buf;
	size_t size;
	uint8_t *data;
	size_t datasize;
	tgl_res_t *images, *sounds, *models, *fonts, *maps, *others;
	uint32_t nimages, nsounds, nmodels, nfonts, nmaps, nothers;
} tgl_pack_t;

typedef struct tgl_allocbatch
{
	void **ptr;
	size_t n, size;
} tgl_allocbatch_t;

typedef struct tgl_reallocbatch
{
	void **ptr;
	size_t nold, nnew, size;
} tgl_reallocbatch_t;

//-----------------------//
// library configuration //
//-----------------------//

extern tgl_conf_t tgl_conf; // should be set before library use.

//------------//
// procedures //
//------------//

// input.
void tgl_handleinput(SDL_Event const *e);
void tgl_prepareinput(void);
bool tgl_kdown(SDL_Keycode k);
bool tgl_kpressed(SDL_Keycode k);
bool tgl_kreleased(SDL_Keycode k);
void tgl_mpos(SDL_Window const *wnd, int32_t *outx, int32_t *outy);
bool tgl_mdown(int32_t btn);
bool tgl_mpressed(int32_t btn);
bool tgl_mreleased(int32_t btn);

// options.
int32_t tgl_optraw(char out[], FILE *fp, char const *key);
int32_t tgl_optkeycode(SDL_Keycode *out, FILE *fp, char const *key);
int32_t tgl_optfloat(double *out, FILE *fp, char const *key);
int32_t tgl_optint(int64_t *out, FILE *fp, char const *key);
int32_t tgl_optbool(bool *out, FILE *fp, char const *key);

// pack.
int32_t tgl_readpack(tgl_pack_t *out, uint8_t *pack, size_t size);
int32_t tgl_readpackfile(tgl_pack_t *out, FILE *fp);
void tgl_writepack(uint8_t outbuf[], size_t *outsize, tgl_pack_t const *p);
int32_t tgl_writepackfile(FILE *fp, tgl_pack_t const *p);
void tgl_destroypack(tgl_pack_t *p);
int32_t tgl_packadd(tgl_pack_t *p, tgl_res_t const *r);
int32_t tgl_packinsert(tgl_pack_t *p, tgl_res_t const *r, uint32_t idx);
int32_t tgl_packrm(tgl_pack_t *p, tgl_restype_t type, uint32_t idx);
bool tgl_packfind(uint32_t *out, tgl_pack_t const *p, tgl_restype_t type, char const *name);
void tgl_packranges(tgl_pack_t *p);

// ui.
tgl_ui_t tgl_beginui(tgl_uielem_t elems[], size_t elemcap, int32_t x, int32_t y, TTF_Font *font, SDL_Window const *wnd);
void tgl_renderui(tgl_ui_t const *u);
void tgl_uipad(tgl_ui_t *u, int32_t dx, int32_t dy);
void tgl_uilabel(tgl_ui_t *u, char const *text);
bool tgl_uibutton(tgl_ui_t *u, char const *text);
bool tgl_uislider(tgl_ui_t *u, char const *text, float *inoutval);

// util.
void tgl_err(char const *fmt, ...);
uint64_t tgl_unixus(void);
void tgl_begintick(void);
void tgl_endtick(void);
void tgl_begintimer(uint64_t *outtimer);
void tgl_endtimer(uint64_t timer, char const *name);
float tgl_interpangle(float a, float b, float t);
float tgl_shortestangle(float a, float b);
float tgl_r2d(float r);
float tgl_d2r(float d);
void *tgl_allocbatch(tgl_allocbatch_t *allocs, size_t nallocs);
void *tgl_reallocbatch(tgl_reallocbatch_t *reallocs, size_t nreallocs);

#endif
