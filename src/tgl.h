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
#define TGL_VERMINOR 0
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
	TGL_BUTTONTEXTHOVERCOLOR
} tgl_color_t;

typedef enum tgl_uitype
{
	TGL_LABEL = 0,
	TGL_BUTTON
} tgl_uitype_t;

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
} tgl_uielem_t;

typedef struct tgl_ui
{
	tgl_uielem_t *elems;
	size_t nelems, elemcap;
	int32_t x, y;
	TTF_Font *font;
	SDL_Window const *wnd;
} tgl_ui_t;

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

// ui.
tgl_ui_t tgl_beginui(tgl_uielem_t elems[], size_t elemcap, int32_t x, int32_t y, TTF_Font *font, SDL_Window const *wnd);
void tgl_renderui(tgl_ui_t const *u);
void tgl_uipad(tgl_ui_t *u, int32_t dx, int32_t dy);
void tgl_uilabel(tgl_ui_t *u, char const *text);
bool tgl_uibutton(tgl_ui_t *u, char const *text);

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

#endif
