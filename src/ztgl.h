// SPDX-License-Identifier: BSD-3-Clause

#ifndef Z_ZTGL_H
#define Z_ZTGL_H

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

#define Z_VERMAJOR 1
#define Z_VERMINOR 3
#define Z_VERPATCH 0

//--------//
// macros //
//--------//

#ifndef Z_NOGLOBAL
#define OUT
#define INOUT
#endif

// profiling.
#ifdef Z_PROFILE
#define Z_NEWTIMER(timer) u64 timer
#define Z_BEGINTIMER(timer) z_begintimer(name)
#define Z_ENDTIMER(timer, name) z_endtimer(timer, name)
#else
#define Z_NEWTIMER(timer)
#define Z_BEGINTIMER(timer)
#define Z_ENDTIMER(timer, name)
#endif

// util constants.
#define Z_PI 3.141592f
#define Z_TAU (2.0f * Z_PI)

// library data constants.
#define Z_MAXRESNAME 15
#define Z_BATCHALIGN 16
#define Z_MAXOPTIONKEY 128
#define Z_MAXOPTIONVALUE 128
#define Z_OPTIONSCAN "%127s = %127[^\r\n]"

//--------------//
// type aliases //
//--------------//

#ifndef Z_NOGLOBAL

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t usize;

typedef float f32;
typedef double f64;

#endif

//--------------------//
// enumeration values //
//--------------------//

typedef enum z_err
{
	Z_OK = 0,
	Z_NOTFOUND,
	Z_INVALIDFORMAT,
	Z_INVALIDCONVERSION
} z_err_t;

typedef enum z_color
{
	Z_PANELCOLOR = 0,
	Z_LABELTEXTCOLOR,
	Z_BUTTONCOLOR,
	Z_BUTTONPRESSCOLOR,
	Z_BUTTONHOVERCOLOR,
	Z_BUTTONTEXTCOLOR,
	Z_BUTTONTEXTPRESSCOLOR,
	Z_BUTTONTEXTHOVERCOLOR,
	Z_SLIDERCOLOR,
	Z_SLIDERPRESSCOLOR,
	Z_SLIDERHOVERCOLOR,
	Z_SLIDERBARCOLOR,
	Z_SLIDERBARPRESSCOLOR,
	Z_SLIDERBARHOVERCOLOR,
	Z_SLIDERTEXTCOLOR,
	Z_SLIDERTEXTPRESSCOLOR,
	Z_SLIDERTEXTHOVERCOLOR,
	Z_TEXTFIELDCOLOR,
	Z_TEXTFIELDPRESSCOLOR,
	Z_TEXTFIELDHOVERCOLOR,
	Z_TEXTFIELDTEXTCOLOR,
	Z_TEXTFIELDTEXTPRESSCOLOR,
	Z_TEXTFIELDTEXTHOVERCOLOR,
	Z_TEXTFIELDBARCOLOR,
	Z_TEXTFIELDBARPRESSCOLOR,
	Z_TEXTFIELDBARHOVERCOLOR,
	Z_TEXTFIELDPROMPTCOLOR,
	Z_TEXTFIELDPROMPTPRESSCOLOR,
	Z_TEXTFIELDPROMPTHOVERCOLOR,
	Z_INACTIVECOLOR
} z_color_t;

typedef enum z_uiflag
{
	Z_INACTIVE = 0x1
} z_uiflag_t;

typedef enum z_uitype
{
	Z_LABEL = 0,
	Z_BUTTON,
	Z_SLIDER,
	Z_TEXTFIELD
} z_uitype_t;

typedef enum z_restype
{
	Z_IMAGEFILE = 0,
	Z_SOUNDFILE,
	Z_MODELFILE,
	Z_FONTFILE,
	Z_MAPFILE
} z_restype_t;

//-----------------//
// data structures //
//-----------------//

typedef struct z_conf
{
	// library function config.
	FILE *log;
	char const *errtitle;
	u64 tickus;
	i32 uipad;
	i32 uitextfieldbar;
	
	// rendering call config.
	void (*renderrect)(i32, i32, i32, i32, z_color_t);
	void (*rendertext)(i32, i32, i32, i32, char const *, z_color_t);
} z_conf_t;

typedef struct z_tfdata
{
	char *buf;
	u32 len, cap;
	u32 csr, first;
	bool sel;
} z_tfdata_t;

typedef union z_uielem
{
	// all UI elements have at least these properties.
	struct
	{
		u8 type;
		u16 flags;
		i32 x, y;
		i32 w, h;
	} any;
	
	struct
	{
		u8 type;
		u16 flags;
		i32 x, y;
		i32 w, h;
		char const *text;
	} label;
	
	struct
	{
		u8 type;
		u16 flags;
		i32 x, y;
		i32 w, h;
		char const *text;
	} button;
	
	struct
	{
		u8 type;
		u16 flags;
		i32 x, y;
		i32 w, h;
		f32 val;
		char const *text;
	} slider;
	
	struct
	{
		u8 type;
		u16 flags;
		i32 x, y;
		i32 w, h;
		u32 ndraw;
		char const *text;
		z_tfdata_t const *tfdata;
	} textfield;
} z_uielem_t;

typedef struct z_ui
{
	z_uielem_t *elems;
	usize nelems, elemcap;
	i32 x, y;
	TTF_Font *font;
	SDL_Window const *wnd;
	bool active;
} z_ui_t;

typedef struct z_res
{
	u8 type;
	char name[Z_MAXRESNAME];
	u32 flags;
	u32 size;
	u8 *data; // invalidated upon pack update.
} z_res_t;

typedef struct z_pack
{
	void *buf;
	usize size;
	u8 *data;
	usize datasize;
	z_res_t *images, *sounds, *models, *fonts, *maps, *others;
	u32 nimages, nsounds, nmodels, nfonts, nmaps, nothers;
} z_pack_t;

typedef struct z_allocbatch
{
	void **ptr;
	usize n, size;
} z_allocbatch_t;

typedef struct z_reallocbatch
{
	void **ptr;
	usize nold, nnew, size;
} z_reallocbatch_t;

//-----------------------//
// library configuration //
//-----------------------//

extern z_conf_t z_conf; // should be set before library use.

//-------------//
// data tables //
//-------------//

extern u8 z_defaultcolors[][4];

//------------//
// procedures //
//------------//

// input.
void z_handleinput(SDL_Event const *e);
void z_prepareinput(void);
bool z_kdown(SDL_Keycode k);
bool z_kpressed(SDL_Keycode k);
bool z_kreleased(SDL_Keycode k);
void z_mpos(SDL_Window const *wnd, OUT i32 *x, OUT i32 *y);
bool z_mdown(i32 btn);
bool z_mpressed(i32 btn);
bool z_mreleased(i32 btn);
bool z_textinput(char ch);

// options.
z_err_t z_optraw(OUT char buf[], FILE *fp, char const *key);
z_err_t z_optkeycode(OUT SDL_Keycode *k, FILE *fp, char const *key);
z_err_t z_optfloat(OUT f64 *f, FILE *fp, char const *key);
z_err_t z_optint(OUT i64 *i, FILE *fp, char const *key);
z_err_t z_optbool(OUT bool *b, FILE *fp, char const *key);

// pack.
i32 z_readpack(OUT z_pack_t *p, u8 *pack, usize size);
i32 z_readpackfile(OUT z_pack_t *p, FILE *fp);
void z_writepack(OUT u8 buf[], OUT usize *size, z_pack_t const *p);
i32 z_writepackfile(FILE *fp, z_pack_t const *p);
void z_destroypack(z_pack_t *p);
i32 z_packadd(z_pack_t *p, z_res_t const *r);
i32 z_packinsert(z_pack_t *p, z_res_t const *r, u32 idx);
i32 z_packrm(z_pack_t *p, z_restype_t type, u32 idx);
bool z_packfind(OUT u32 *idx, z_pack_t const *p, z_restype_t type, char const *name);
void z_packranges(z_pack_t *p);

// ui.
z_ui_t z_beginui(z_uielem_t elems[], usize elemcap, i32 x, i32 y, TTF_Font *font, SDL_Window const *wnd);
void z_renderui(z_ui_t const *u);
void z_uiactive(z_ui_t *u, bool active);
void z_uipad(z_ui_t *u, i32 dx, i32 dy);
void z_uilabel(z_ui_t *u, char const *text);
bool z_uibutton(z_ui_t *u, char const *text);
bool z_uislider(z_ui_t *u, char const *text, INOUT f32 *val);
void z_uitextfield(z_ui_t *u, char const *text, INOUT z_tfdata_t *tfdata, u32 ndraw);

// util.
void z_err(char const *fmt, ...);
u64 z_unixus(void);
void z_begintick(void);
void z_endtick(void);
void z_begintimer(OUT u64 *timer);
void z_endtimer(u64 timer, char const *name);
f32 z_interpangle(f32 a, f32 b, f32 t);
f32 z_shortestangle(f32 a, f32 b);
f32 z_r2d(f32 r);
f32 z_d2r(f32 d);
void *z_allocbatch(INOUT z_allocbatch_t *allocs, usize nallocs);
void *z_reallocbatch(INOUT z_reallocbatch_t *reallocs, usize nreallocs);

#endif
