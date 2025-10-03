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
#define ZTGL_NEW_TIMER(timer) u64 timer
#define ZTGL_BEGIN_TIMER(timer) ZTGL::BeginTimer(timer)
#define ZTGL_END_TIMER(timer) ZTGL::EndTimer(timer, name)
#else
#define ZTGL_NEW_TIMER(timer)
#define ZTGL_BEGIN_TIMER(timer)
#define ZTGL_END_TIMER(timer, name)
#endif

// library data constants.
#define ZTGL_BATCH_ALIGN 16
#define ZTGL_MAX_OPTION_KEY 128
#define ZTGL_MAX_OPTION_VALUE 128
#define ZTGL_OPTION_SCAN "%127s = %127[^\r\n]"

// resource inclusion.
#define ZTGL_INC_XXD(name) \
	extern u8  name[]; \
	extern u32 name##_len;

#define ZTGL_INC_RES(name) \
	{ \
		.m_Data = name, \
		.m_Size = name##_len \
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

enum ErrorCode
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

//----------------------------//
// standalone data structures //
//----------------------------//

struct Conf
{
	// library function config.
	char const* m_ErrorTitle;
	FILE* m_Log;
	u64   m_TickMicro;
	i32   m_UIPad;
	i32   m_UITextFieldBar;
	
	// rendering call config.
	void (*m_RenderRect)(i32, i32, i32, i32, Color);
	void (*m_RenderText)(i32, i32, i32, i32, char const*, Color);
};

struct Res
{
	u8*  m_Data;
	u32& m_Size;
};

struct TFData
{
	char* m_Buffer;
	u32   m_Length;
	u32   m_Capacity;
	u32   m_Cursor;
	u32   m_First;
	bool  m_Selected;
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
		char const* m_Text;
	} m_Label;
	
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
		char const* m_Text;
	} m_Button;
	
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
		f32 m_Value;
		char const* m_Text;
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
		char const*   m_Text;
		TFData const& m_TFData;
	} m_TextField;
	
	struct
	{
		u8  m_Type;
		u16 m_Flags;
		i32 m_X;
		i32 m_Y;
		i32 m_W;
		i32 m_H;
		char const* m_Text;
	} m_HoldButton;
};

struct AllocBatchDesc
{
	void*& m_Pointer;
	usize  m_Count;
	usize  m_Size;
};

struct ReallocBatchDesc
{
	void*& m_Pointer;
	usize  m_OldCount;
	usize  m_NewCount;
	usize  m_Size;
};

//------------------------------//
// data structures with methods //
//------------------------------//

// ui.
struct UIPanel
{
	UIElem*   m_Elems;
	usize     m_ElemsLength;
	usize     m_ElemsCapacity;
	TTF_Font* m_Font;
	SDL_Window const* m_Window;
	
	// can safely be modified by end user.
	i32  m_X;
	i32  m_Y;
	bool m_Active;
	bool m_Horizontal;
	
	UIPanel(UIElem elems[], usize elemsCapacity, TTF_Font* font, SDL_Window const* window);
	
	void Render();
	void Label(char const* text);
	bool Button(char const* text);
	bool Slider(char const* text, IN_OUT f32& value);
	void TextField(char const* text, IN_OUT TFData& data, u32 nDraw);
	bool HoldButton(char const* text);
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
void HandleInput(SDL_Event const& event);
void PrepareInput();
bool KeyDown(SDL_Keycode key);
bool KeyPressed(SDL_Keycode key);
bool KeyReleased(SDL_Keycode key);
SDL_Point MousePos(SDL_Window const* window);
bool MouseDown(i32 button);
bool MousePressed(i32 button);
bool MouseReleased(i32 button);
bool TextInput(char c);

// options.
ErrorCode OptionRaw(OUT char data[], FILE* file, char const* key);
ErrorCode OptionKeycode(OUT SDL_Keycode& data, FILE* file, char const* key);
ErrorCode OptionFloat(OUT f32& data, FILE* file, char const* key);
ErrorCode OptionInt(OUT i64& data, FILE* file, char const* key);
ErrorCode OptionBool(OUT bool& data, FILE* file, char const* key);

// util.
void  Error(char const* format, ...);
u64   UnixMicro();
void  BeginTick();
void  EndTick();
void  BeginTimer(OUT u64& timer);
void  EndTimer(u64 timer, char const* name);
f32   InterpAngle(f32 a, f32 b, f32 t);
f32   ShortestAngle(f32 a, f32 b);
f32   Degrees(f32 rad);
f32   Radians(f32 deg);
void* AllocBatch(IN_OUT AllocBatchDesc allocs[], usize nAllocs);
void* ReallocBatch(void* p, IN_OUT ReallocBatchDesc reallocs[], usize nReallocs);
u64   Align(u64 addr, u64 align);

}

#endif

#ifdef ZTGL_IMPLEMENTATION
#ifndef ZTGL_IMPL_INCLUDED
#define ZTGL_IMPL_INCLUDED

// standard library.
#include <cctype>
#include <cerrno>
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

enum UIFlag
{
	INACTIVE = 0x1
};

enum UIType
{
	LABEL = 0,
	BUTTON,
	SLIDER,
	TEXT_FIELD,
	HOLD_BUTTON
};

// input.
u8 KeyDownStates[1024 / 8];
u8 KeyPressStates[1024 / 8];
u8 KeyReleaseStates[1024 / 8];
u8 MouseDownStates;
u8 MousePressStates;
u8 MouseReleaseStates;
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
HandleInput(SDL_Event const& event)
{
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		if (event.key.repeat)
		{
			return;
		}
		
		bool state = event.type == SDL_KEYDOWN;
		
		SDL_Keycode key = event.key.keysym.sym;
		if (key & 1 << 30)
		{
			key &= ~(1 << 30);
			key += 12;
		}
		
		usize byte = key / 8;
		usize bit = key % 8;
		
		if (state)
		{
			Internal::KeyDownStates[byte] |= 1 << bit;
			Internal::KeyPressStates[byte] |= 1 << bit;
		}
		else
		{
			Internal::KeyDownStates[byte] &= ~(1 << bit);
			Internal::KeyReleaseStates[byte] |= 1 << bit;
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
	{
		bool state = event.type == SDL_MOUSEBUTTONDOWN;
		
		if (state)
		{
			Internal::MouseDownStates |= 1 << event.button.button;
			Internal::MousePressStates |= 1 << event.button.button;
		}
		else
		{
			Internal::MouseDownStates &= ~(1 << event.button.button);
			Internal::MouseReleaseStates |= 1 << event.button.button;
		}
	}
	else if (event.type == SDL_TEXTINPUT)
	{
		u8 c = event.text.text[0];
		
		// disregard non-ASCII input.
		if (c & 0x80)
		{
			return;
		}
		
		usize byte = c / 8;
		usize bit = c % 8;
		Internal::TextInputStates[byte] |= 1 << bit;
	}
}

void
PrepareInput()
{
	memset(Internal::KeyPressStates, 0, sizeof(Internal::KeyPressStates));
	memset(Internal::KeyReleaseStates, 0, sizeof(Internal::KeyReleaseStates));
	
	Internal::MousePressStates = 0;
	Internal::MouseReleaseStates = 0;
	
	memset(Internal::TextInputStates, 0, sizeof(Internal::TextInputStates));
}

bool
KeyDown(SDL_Keycode key)
{
	if (key & 1 << 30)
	{
		key &= ~(1 << 30);
		key += 128;
	}
	usize byte = key / 8;
	usize bit = key % 8;
	return Internal::KeyDownStates[byte] & 1 << bit;
}

bool
KeyPressed(SDL_Keycode key)
{
	if (key & 1 << 30)
	{
		key &= ~(1 << 30);
		key += 128;
	}
	usize byte = key / 8;
	usize bit = key % 8;
	return Internal::KeyPressStates[byte] & 1 << bit;
}

bool
KeyReleased(SDL_Keycode key)
{
	if (key & 1 << 30)
	{
		key &= ~(1 << 30);
		key += 128;
	}
	usize byte = key / 8;
	usize bit = key % 8;
	return Internal::KeyReleaseStates[byte] & 1 << bit;
}

SDL_Point
MousePos(SDL_Window const* window)
{
	if (SDL_GetMouseFocus() != window)
	{
		return {0, 0};
	}
	
	i32 x;
	i32 y;
	SDL_GetMouseState(&x, &y);
	
	return {x, y};
}

bool
MouseDown(i32 button)
{
	return !!(Internal::MouseDownStates & 1 << button);
}

bool
MousePressed(i32 button)
{
	return !!(Internal::MousePressStates & 1 << button);
}

bool
MouseReleased(i32 button)
{
	return !!(Internal::MouseReleaseStates & 1 << button);
}

bool
TextInput(char c)
{
	usize byte = c / 8;
	usize bit = c % 8;
	return Internal::TextInputStates[byte] & 1 << bit;
}

//---------//
// options //
//---------//

ErrorCode
OptionRaw(OUT char data[], FILE* file, char const* key)
{
	fseek(file, 0, SEEK_SET);
	
	for (usize line = 0; !feof(file) && !ferror(file); ++line)
	{
		i32 c;
		while (c = fgetc(file), c != EOF && isspace(c))
		{
		}
		
		if (c == '#')
		{
			while (c = fgetc(file), c != EOF && c != '\n')
			{
			}
		}
		
		if (c == '\n' || feof(file))
		{
			continue;
		}
		
		fseek(file, -1, SEEK_CUR);
		char keyBuffer[ZTGL_MAX_OPTION_KEY] = {0};
		if (fscanf(file, ZTGL_OPTION_SCAN, keyBuffer, data) != 2)
		{
			return INVALID_FORMAT;
		}
		
		if (!strcmp(data, "NONE"))
		{
			data[0] = 0;
		}
		
		if (!strcmp(keyBuffer, key))
		{
			return OK;
		}
	}
	
	return NOT_FOUND;
}

ErrorCode
OptionKeycode(OUT SDL_Keycode& data, FILE* file, char const* key)
{
	char buffer[ZTGL_MAX_OPTION_VALUE] = {0};
	ErrorCode err = OptionRaw(buffer, file, key);
	if (err)
	{
		return err;
	}
	
	data = SDL_GetKeyFromName(buffer);
	if (data == SDLK_UNKNOWN)
	{
		return INVALID_CONVERSION;
	}
	
	return OK;
}

ErrorCode
OptionFloat(OUT f32& data, FILE* file, char const* key)
{
	char buffer[ZTGL_MAX_OPTION_VALUE] = {0};
	ErrorCode err = OptionRaw(buffer, file, key);
	if (err)
	{
		return err;
	}
	
	errno = 0;
	data = strtof(buffer, nullptr);
	if (errno)
	{
		return INVALID_CONVERSION;
	}
	
	return OK;
}

ErrorCode
OptionInt(OUT i64& data, FILE* file, char const* key)
{
	char buffer[ZTGL_MAX_OPTION_VALUE] = {0};
	ErrorCode err = OptionRaw(buffer, file, key);
	if (err)
	{
		return err;
	}
	
	errno = 0;
	data = (i64)strtoll(buffer, nullptr, 0);
	if (errno)
	{
		return INVALID_CONVERSION;
	}
	
	return OK;
}

ErrorCode
OptionBool(OUT bool& data, FILE* file, char const* key)
{
	char buffer[ZTGL_MAX_OPTION_VALUE] = {0};
	ErrorCode err = OptionRaw(buffer, file, key);
	if (err)
	{
		return err;
	}
	
	if (!strcmp(buffer, "true"))
	{
		data = true;
		return OK;
	}
	else if (!strcmp(buffer, "false"))
	{
		data = false;
		return OK;
	}
	else
	{
		return INVALID_CONVERSION;
	}
}

//----//
// ui //
//----//

UIPanel::UIPanel(
	UIElem elems[],
	usize elemsCapacity,
	TTF_Font* font,
	SDL_Window const* window
)
	: m_Elems(elems),
	m_ElemsCapacity(elemsCapacity),
	m_Font(font),
	m_Window(window),
	m_Active(true)
{
}

void
UIPanel::Render()
{
}

void
UIPanel::Label(char const* text)
{
	if (m_ElemsLength >= m_ElemsCapacity)
	{
		return;
	}
	
	i32 w;
	i32 h;
	TTF_SizeText(m_Font, text, &w, &h);
	
	m_Elems[m_ElemsLength].m_Label.m_Type = Internal::LABEL;
	m_Elems[m_ElemsLength].m_Label.m_Flags = Internal::INACTIVE * !m_Active;
	m_Elems[m_ElemsLength].m_Label.m_X = m_X;
	m_Elems[m_ElemsLength].m_Label.m_Y = m_X;
	m_Elems[m_ElemsLength].m_Label.m_W = w;
	m_Elems[m_ElemsLength].m_Label.m_H = h;
	m_Elems[m_ElemsLength].m_Label.m_Text = text;
	++m_ElemsLength;
	
	if (m_Horizontal)
	{
		m_X += w;
	}
	else
	{
		m_Y += h;
	}
}

bool
UIPanel::Button(char const* text)
{
	if (m_ElemsLength >= m_ElemsCapacity)
	{
		return false;
	}
	
	bool state = false;
	
	i32 w;
	i32 h;
	TTF_SizeText(m_Font, text, &w, &h);
	w += 2 * Conf.m_UIPad;
	h += 2 * Conf.m_UIPad;
	
	if (m_Active)
	{
		SDL_Point m = MousePos(m_Window);
		if (MouseReleased(SDL_BUTTON_LEFT)
			&& m.x >= m_X
			&& m.y >= m_Y
			&& m.x < m_X + w
			&& m.y < m_Y + h)
		{
			state = true;
		}
	}
	
	m_Elems[m_ElemsLength].m_Button.m_Type = Internal::BUTTON;
	m_Elems[m_ElemsLength].m_Button.m_Flags = Internal::INACTIVE * !m_Active;
	m_Elems[m_ElemsLength].m_Button.m_X = m_X;
	m_Elems[m_ElemsLength].m_Button.m_Y = m_Y;
	m_Elems[m_ElemsLength].m_Button.m_W = w;
	m_Elems[m_ElemsLength].m_Button.m_H = h;
	m_Elems[m_ElemsLength].m_Button.m_Text = text;
	++m_ElemsLength;
	
	if (m_Horizontal)
	{
		m_X += w;
	}
	else
	{
		m_Y += h;
	}
	
	return state;
}

bool
UIPanel::Slider(char const* text, IN_OUT f32& value)
{
}

void
UIPanel::TextField(char const* text, IN_OUT TFData& data, u32 nDraw)
{
}

bool
UIPanel::HoldButton(char const* text)
{
}

//------//
// util //
//------//

void
Error(char const* format, ...)
{
	va_list args;
	va_start(args, format);
	
	char msg[512];
	vsnprintf(msg, sizeof(msg), format, args);
	
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, Conf.m_ErrorTitle, msg, nullptr))
	{
		fprintf(Conf.m_Log, "\x1b[1;31merr\x1b[0m: %s\n", msg);
	}
	
	va_end(args);
}

u64
UnixMicro()
{
	timeval tv;
	gettimeofday(&tv, nullptr);
	return (u64)tv.tv_sec * 1000000 + (u64)tv.tv_usec;
}

void
BeginTick()
{
	Internal::TickStart = UnixMicro();
}

void
EndTick()
{
	u64 tickEnd = UnixMicro();
	i64 timeLeft = Conf.m_TickMicro - tickEnd + Internal::TickStart;
	timeLeft *= timeLeft > 0;
	
	SDL_Delay(timeLeft / 1000);
}

void
BeginTimer(OUT u64& timer)
{
	timer = UnixMicro();
}

void
EndTimer(u64 timer, char const* name)
{
	u64 d = UnixMicro() - timer;
	fprintf(
		Conf.m_Log,
		"\x1b[1;33mprofile\x1b[0m: %s: %llu\n",
		name,
		(unsigned long long)d
	);
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
Degrees(f32 rad)
{
	return rad / PI * 180.0f;
}

f32
Radians(f32 deg)
{
	return deg / 180.0f * PI;
}

void*
AllocBatch(IN_OUT AllocBatchDesc allocs[], usize nAllocs)
{
	usize size = 0;
	for (usize i = 0; i < nAllocs; ++i)
	{
		size += allocs[i].m_Count * allocs[i].m_Size;
		size = Align(size, ZTGL_BATCH_ALIGN);
	}
	
	u8* p = (u8*)malloc(size);
	if (!p)
	{
		return nullptr;
	}
	
	usize offset = 0;
	for (usize i = 0; i < nAllocs; ++i)
	{
		allocs[i].m_Pointer = &p[offset];
		offset += allocs[i].m_Count * allocs[i].m_Size;
		offset = Align(offset, ZTGL_BATCH_ALIGN);
	}
	
	return p;
}

void*
ReallocBatch(void* p, IN_OUT ReallocBatchDesc reallocs[], usize nReallocs)
{
	usize* oldOffsets = (usize*)calloc(nReallocs, sizeof(usize));
	usize* newOffsets = (usize*)calloc(nReallocs, sizeof(usize));
	
	usize newSize = 0, oldSize = 0;
	for (usize i = 0; i < nReallocs; ++i)
	{
		newOffsets[i] = newSize;
		newSize += reallocs[i].m_NewCount * reallocs[i].m_Size;
		newSize = Align(newSize, ZTGL_BATCH_ALIGN);
		
		oldOffsets[i] = oldSize;
		oldSize += reallocs[i].m_OldCount * reallocs[i].m_Size;
		oldSize = Align(oldSize, ZTGL_BATCH_ALIGN);
	}
	
	p = realloc(p, newSize);
	if (!p)
	{
		free(oldOffsets);
		free(newOffsets);
		return nullptr;
	}
	
	u8* up = (u8*)p;
	for (isize i = nReallocs - 1; i >= 0; --i)
	{
		usize newBytes = reallocs[i].m_NewCount * reallocs[i].m_Size;
		usize oldBytes = reallocs[i].m_OldCount * reallocs[i].m_Size;
		usize bytes = newBytes < oldBytes ? newBytes : oldBytes;
		
		memmove(&up[newOffsets[i]], &up[oldOffsets[i]], bytes);
		reallocs[i].m_Pointer = &up[newOffsets[i]];
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
