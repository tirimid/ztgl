// SPDX-License-Identifier: BSD-3-Clause

#ifndef TGL_TGL_H
#define TGL_TGL_H

// standard library.
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// system dependencies.
#include <SDL.h>

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

//-----------------//
// data structures //
//-----------------//

typedef struct tgl_conf
{
	FILE *log;
	char const *errtitle;
	uint64_t tickus;
} tgl_conf_t;

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
void tgl_mpos(int32_t *outx, int32_t *outy);
bool tgl_mdown(int32_t btn);
bool tgl_mpressed(int32_t btn);
bool tgl_mreleased(int32_t btn);

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
