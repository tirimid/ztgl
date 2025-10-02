// SPDX-License-Identifier: BSD-3-Clause

#ifndef Z_ZTGL_HH
#define Z_ZTGL_HH

#include <iostream>

// system dependencies.
extern "C"
{
#include <SDL.h>
#include <SDL_ttf.h>
}

//--------------------------//
// library meta-information //
//--------------------------//

#define Z_VER_MAJOR 1
#define Z_VER_MINOR 0
#define Z_VER_PATCH 0

//--------//
// macros //
//--------//

#ifndef Z_NO_GLOBAL
#define OUT
#define IN_OUT
#endif

// profiling.
#ifdef Z_PROFILE
#define Z_NEW_TIMER(Timer) u64 Timer
#define Z_BEGIN_TIMER(Timer) z::BeginTimer(Timer)
#define Z_END_TIMER(Timer) z::EndTimer(Timer, Name)
#else
#define Z_NEW_TIMER(Timer)
#define Z_BEGIN_TIMER(Timer)
#define Z_END_TIMER(Timer, Name)
#endif

// util constants.
#define Z_PI 3.141592f
#define Z_TAU (2.0f * Z_PI)

// library data constants.
#define Z_MAX_RES_NAME 15
#define Z_BATCH_ALIGN 16
#define Z_MAX_OPTION_KEY 128
#define Z_MAX_OPTION_VALUE 128
#define Z_OPTION_SCAN "%127s = %127[^\r\n]"

// resource inclusion.
#define Z_INC_XXD(Name) \
	extern u8  Name[]; \
	extern u32 Name##_len;

#define Z_INC_RES(Name) \
	{ \
		.m_Data = Name, \
		.m_Size = &Name##_len \
	}

//--------------//
// type aliases //
//--------------//

#ifndef Z_NO_GLOBAL

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using isize = ssize_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = size_t;

using f32 = float;
using f64 = double;

#endif

namespace z
{

//--------------------//
// enumeration values //
//--------------------//

enum Err
{
	OK = 0,
	NOT_FOUND,
	INVALID_FORMAT,
	INVALID_CONVERSION
};

enum Color
{
	PANEL_COLOR = 0,
	LABEL_TEXT_COLOR,
	BUTTON_COLOR,
	BUTTON_PRESS_COLOR,
	BUTTON_HOVER_COLOR,
	BUTTON_TEXT_COLOR,
	BUTTON_TEXT_PRESS_COLOR,
	BUTTON_TEXT_HOVER_COLOR,
	SLIDER_COLOR,
	SLIDER_PRESS_COLOR,
	SLIDER_HOVER_COLOR,
	SLIDER_BAR_COLOR,
	SLIDER_BAR_PRESS_COLOR,
	SLIDER_BAR_HOVER_COLOR,
	SLIDER_TEXT_COLOR,
	SLIDER_TEXT_PRESS_COLOR,
	SLIDER_TEXT_HOVER_COLOR,
	TEXT_FIELD_COLOR,
	TEXT_FIELD_PRESS_COLOR,
	TEXT_FIELD_HOVER_COLOR,
	TEXT_FIELD_TEXT_COLOR,
	TEXT_FIELD_TEXT_PRESS_COLOR,
	TEXT_FIELD_TEXT_HOVER_COLOR,
	TEXT_FIELD_BAR_COLOR,
	TEXT_FIELD_BAR_PRESS_COLOR,
	TEXT_FIELD_BAR_HOVER_COLOR,
	TEXT_FIELD_PROMPT_COLOR,
	TEXT_FIELD_PROMPT_PRESS_COLOR,
	TEXT_FIELD_PROMPT_HOVER_COLOR,
	INACTIVE_COLOR
};

//-----------------//
// data structures //
//-----------------//

struct Conf
{
	// library function config.
	char const *m_ErrTitle;
	FILE  *m_Log;
	u64    m_TickUS;
	i32    m_UIPad;
	i32    m_UITextFieldBar;
	
	// rendering call config.
	void (*m_RenderRect)(i32, i32, i32, i32, Color);
	void (*m_RenderText)(i32, i32, i32, i32, char const *, Color);
};

struct Res
{
	u8  *m_Data;
	u32 *m_Size;
};

struct TFData
{
	char *m_Buf;
	u32   m_Len;
	u32   m_Cap;
	u32   m_Csr;
	u32   m_First;
	bool  m_Sel;
};

union UIElem
{
	// all UI elements have at least there properties.
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
	} m_Any;
	
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
		char const *m_Text;
	} m_Label;
	
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
		char const *m_Text;
	} m_Button;
	
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
		f32 m_Val;
		char const *m_Text;
	} m_Slider;
	
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
		u32 m_NDraw;
		char const   *m_Text;
		TFData const *m_TFData;
	} m_TextField;
	
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
		char const *m_Text;
	} m_HoldButton;
};

struct UIPanel
{
	UIElem   *m_Elems;
	usize     m_NElems;
	usize     m_ElemCap;
	TTF_Font *m_Font;
	SDL_Window const *m_Wnd;
	
	// can safely be modified by end user.
	i32  m_X;
	i32  m_Y;
	bool m_Active;
	bool m_Horizontal;
};

struct AllocBatchDesc
{
	void **m_Ptr;
	usize  m_N;
	usize  m_Size;
};

struct ReallocBatchDesc
{
	void **m_Ptr;
	usize  m_NOld;
	usize  m_NNew;
	usize  m_Size;
};

//-----------------------//
// library configuration //
//-----------------------//

extern Conf Conf;

//-------------//
// data tables //
//-------------//

extern u8 DefaultColors[][4];

//-----------------------//
// standalone procedures //
//-----------------------//

// input.
void HandleInput(SDL_Event const *e);
void PrepareInput();
bool KDown(SDL_Keycode k);
bool KPressed(SDL_Keycode k);
bool KReleased(SDL_Keycode k);
void MPos(SDL_Window const *wnd, OUT i32 *x, OUT i32 *y);
bool MDown(i32 btn);
bool MPressed(i32 btn);
bool MReleased(i32 btn);
bool TextInput(char c);

// options.
Err OptRaw(OUT char data[], FILE *f, char const *key);
Err OptKeycode(OUT SDL_Keycode *data, FILE *f, char const *key);
Err OptFloat(OUT f32 *data, FILE *f, char const *key);
Err OptInt(OUT i64 *data, FILE *f, char const *key);
Err OptBool(OUT bool *data, FILE *f, char const *key);

// util.
void  Err(char const *fmt, ...);
u64   UnixUS();
void  BeginTick();
void  EndTick();
void  BeginTimer(OUT u64 *timer);
void  EndTimer(u64 timer, char const *name);
f32   InterpAngle(f32 a, f32 b, f32 t);
f32   ShortestAngle(f32 a, f32 b);
f32   ToDeg(f32 r);
f32   ToRad(f32 d);
void *AllocBatch(IN_OUT AllocBatchDesc *allocs, usize nAllocs);
void *ReallocBatch(void *p, IN_OUT ReallocBatchDesc *reallocs, usize nReallocs);
u64   Align(u64 addr, u64 align);

}

#endif

#ifdef Z_IMPLEMENTATION
#ifndef Z_IMPL_INCLUDED
#define Z_IMPL_INCLUDED

#endif
#endif
