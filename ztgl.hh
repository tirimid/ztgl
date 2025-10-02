// SPDX-License-Identifier: BSD-3-Clause

#ifndef ZTGL_HH
#define ZTGL_HH

#include <cstdio>

// system dependencies.
extern "C"
{
#include <SDL.h>
#include <SDL_ttf.h>
}

//--------------------------//
// library meta-information //
//--------------------------//

#define ZTGL_VER_MAJOR 1
#define ZTGL_VER_MINOR 0
#define ZTGL_VER_PATCH 0

//--------//
// macros //
//--------//

#ifndef ZTGL_NO_GLOBAL
#define OUT
#define IN_OUT
#define PI 3.141592f
#define TAU (2.0f * PI)
#endif

// profiling.
#ifdef ZTGL_PROFILE
#define ZTGL_NEW_TIMER(Timer) u64 Timer
#define ZTGL_BEGIN_TIMER(Timer) z::BeginTimer(Timer)
#define ZTGL_END_TIMER(Timer) z::EndTimer(Timer, Name)
#else
#define ZTGL_NEW_TIMER(Timer)
#define ZTGL_BEGIN_TIMER(Timer)
#define ZTGL_END_TIMER(Timer, Name)
#endif

// library data constants.
#define ZTGL_MAX_RES_NAME 15
#define ZTGL_BATCH_ALIGN 16
#define ZTGL_MAX_OPTION_KEY 128
#define ZTGL_MAX_OPTION_VALUE 128
#define ZTGL_OPTION_SCAN "%127s = %127[^\r\n]"

// resource inclusion.
#define ZTGL_INC_XXD(Name) \
	extern u8  Name[]; \
	extern u32 Name##_len;

#define ZTGL_INC_RES(Name) \
	{ \
		.m_Data = Name, \
		.m_Size = &Name##_len \
	}

//--------------//
// type aliases //
//--------------//

#ifndef ZTGL_NO_GLOBAL

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

namespace ZTGL
{

//--------------------//
// enumeration values //
//--------------------//

enum ErrCode
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
ErrCode OptRaw(OUT char data[], FILE *f, char const *key);
ErrCode OptKeycode(OUT SDL_Keycode *data, FILE *f, char const *key);
ErrCode OptFloat(OUT f32 *data, FILE *f, char const *key);
ErrCode OptInt(OUT i64 *data, FILE *f, char const *key);
ErrCode OptBool(OUT bool *data, FILE *f, char const *key);

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

#ifdef ZTGL_IMPLEMENTATION
#ifndef ZTGL_IMPL_INCLUDED
#define ZTGL_IMPL_INCLUDED

// standard library.
#include <cstdarg>
#include <cstdlib>
#include <cstring>

// system dependencies.
extern "C"
{
#include <sys/time.h>
}

namespace ZTGL::Internal
{

// input.
u8 KDownStates[1024 / 8];
u8 KPressStates[1024 / 8];
u8 KReleaseStates[1024 / 8];
u8 MDownStates;
u8 MPressStates;
u8 MReleaseStates;
u8 TextInputStates[128 / 8];

// util.
u64 TickStart;

}

namespace ZTGL
{

//-------//
// input //
//-------//

void
HandleInput(SDL_Event const *e)
{
}

void
PrepareInput()
{
}

bool
KDown(SDL_Keycode k)
{
}

bool
KPressed(SDL_Keycode k)
{
}

bool
KReleased(SDL_Keycode k)
{
}

void
MPos(SDL_Window const *wnd, OUT i32 *x, OUT i32 *y)
{
}

bool
MDown(i32 btn)
{
}

bool
MPressed(i32 btn)
{
}

bool
MReleased(i32 btn)
{
}

bool
TextInput(char c)
{
}

//---------//
// options //
//---------//

ErrCode
OptRaw(OUT char data[], FILE *f, char const *key)
{
}

ErrCode
OptKeycode(OUT SDL_Keycode *data, FILE *f, char const *key)
{
}

ErrCode
OptFloat(OUT f32 *data, FILE *f, char const *key)
{
}

ErrCode
OptInt(OUT i64 *data, FILE *f, char const *key)
{
}

ErrCode
OptBool(OUT bool *data, FILE *f, char const *key)
{
}

//----//
// ui //
//----//

//------//
// util //
//------//

void
Err(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	char msg[512];
	vsnprintf(msg, sizeof(msg), fmt, args);
	
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, Conf.m_ErrTitle, msg, nullptr))
	{
		fprintf(Conf.m_Log, "\x1b[1;31merr\x1b[0m: %s\n", msg);
	}
	
	va_end(args);
}

u64
UnixUS()
{
	timeval tv;
	gettimeofday(&tv, nullptr);
	return (u64)tv.tv_sec * 1000000 + (u64)tv.tv_usec;
}

void
BeginTick()
{
	Internal::TickStart = UnixUS();
}

void
EndTick()
{
	u64 tickEnd = UnixUS();
	i64 timeLeft = Conf.m_TickUS - tickEnd + Internal::TickStart;
	timeLeft *= timeLeft > 0;
	
	SDL_Delay(timeLeft / 1000);
}

void
BeginTimer(OUT u64 *timer)
{
	*timer = UnixUS();
}

void
EndTimer(u64 timer, char const *name)
{
}

f32
InterpAngle(f32 a, f32 b, f32 t)
{
	return a + ShortestAngle(a, b) * t;
}

f32
ShortestAngle(f32 a, f32 b)
{
	f32 d = fmod(b - a, TAU);
	f32 shortest = fmod(2.0f * d, TAU) - d;
	return shortest;
}

f32
ToDeg(f32 r)
{
	return r / PI * 180.0f;
}

f32
ToRad(f32 d)
{
	return d / 180.0f * PI;
}

void *
AllocBatch(IN_OUT AllocBatchDesc *allocs, usize nAllocs)
{
}

void *
ReallocBatch(void *p, IN_OUT ReallocBatchDesc *reallocs, usize nReallocs)
{
	usize *oldOffsets = (usize *)calloc(nReallocs, sizeof(usize));
	usize *newOffsets = (usize *)calloc(nReallocs, sizeof(usize));
	
	usize newSize = 0, oldSize = 0;
	for (usize i = 0; i < nReallocs; ++i)
	{
		newOffsets[i] = newSize;
		newSize += reallocs[i].m_NNew * reallocs[i].m_Size;
		newSize = Align(newSize, ZTGL_BATCH_ALIGN);
		
		oldOffsets[i] = oldSize;
		oldSize += reallocs[i].m_NOld * reallocs[i].m_Size;
		oldSize = Align(oldSize, ZTGL_BATCH_ALIGN);
	}
	
	p = realloc(p, newSize);
	if (!p)
	{
		free(oldOffsets);
		free(newOffsets);
		return nullptr;
	}
	
	u8 *up = (u8 *)p;
	for (isize i = nReallocs - 1; i >= 0; --i)
	{
		usize newBytes = reallocs[i].m_NNew * reallocs[i].m_Size;
		usize oldBytes = reallocs[i].m_NOld * reallocs[i].m_Size;
		usize bytes = newBytes < oldBytes ? newBytes : oldBytes;
		
		memmove(&up[newOffsets[i]], &up[oldOffsets[i]], bytes);
		*reallocs[i].m_Ptr = &up[newOffsets[i]];
	}
	
	free(oldOffsets);
	free(newOffsets);
	return p;
}

u64
Align(u64 addr, u64 align)
{
	return addr + align - addr % align;
}

}

#endif
#endif
