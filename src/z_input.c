// SPDX-License-Identifier: BSD-3-Clause

// assume that 1024 represents a big enough number of keystates.
static u8 z_kdownstates[1024 / 8];
static u8 z_kpressstates[1024 / 8], z_kreleasestates[1024 / 8];

// SDL2 supports 5 mouse buttons.
static u8 z_mdownstates;
static u8 z_mpressstates, z_mreleasestates;

// ZTGL supports ASCII text input.
static u8 z_textinputstates[128 / 8];

void
z_handleinput(SDL_Event const *e)
{
	if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP)
	{
		if (e->key.repeat)
		{
			return;
		}
		
		bool state = e->type == SDL_KEYDOWN;
		
		SDL_Keycode k = e->key.keysym.sym;
		if (k & 1 << 30)
		{
			k &= ~(1 << 30);
			k += 128;
		}
		
		usize byte = k / 8, bit = k % 8;
		
		if (state)
		{
			z_kdownstates[byte] |= 1 << bit;
			z_kpressstates[byte] |= 1 << bit;
		}
		else
		{
			z_kdownstates[byte] &= ~(1 << bit);
			z_kreleasestates[byte] |= 1 << bit;
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		bool state = e->type == SDL_MOUSEBUTTONDOWN;
		
		if (state)
		{
			z_mdownstates |= 1 << e->button.button;
			z_mpressstates |= 1 << e->button.button;
		}
		else
		{
			z_mdownstates &= ~(1 << e->button.button);
			z_mreleasestates |= 1 << e->button.button;
		}
	}
	else if (e->type == SDL_TEXTINPUT)
	{
		u8 ch = e->text.text[0];
		
		// disregard non-ASCII input.
		if (ch & 0x80)
		{
			return;
		}
		
		usize byte = ch / 8, bit = ch % 8;
		z_textinputstates[byte] |= 1 << bit;
	}
}

void
z_prepareinput(void)
{
	memset(z_kpressstates, 0, sizeof(z_kpressstates));
	memset(z_kreleasestates, 0, sizeof(z_kreleasestates));
	
	z_mpressstates = 0;
	z_mreleasestates = 0;
	
	memset(z_textinputstates, 0, sizeof(z_textinputstates));
}

bool
z_kdown(SDL_Keycode k)
{
	if (k & 1 << 30)
	{
		k &= ~(1 << 30);
		k += 128;
	}
	usize byte = k / 8, bit = k % 8;
	return z_kdownstates[byte] & 1 << bit;
}

bool
z_kpressed(SDL_Keycode k)
{
	if (k & 1 << 30)
	{
		k &= ~(1 << 30);
		k += 128;
	}
	usize byte = k / 8, bit = k % 8;
	return z_kpressstates[byte] & 1 << bit;
}

bool
z_kreleased(SDL_Keycode k)
{
	if (k & 1 << 30)
	{
		k &= ~(1 << 30);
		k += 128;
	}
	usize byte = k / 8, bit = k % 8;
	return z_kreleasestates[byte] & 1 << bit;
}

void
z_mpos(SDL_Window const *wnd, OUT i32 *x, OUT i32 *y)
{
	if (SDL_GetMouseFocus() != wnd)
	{
		*x = 0;
		*y = 0;
		return;
	}
	
	SDL_GetMouseState(x, y);
}

bool
z_mdown(i32 btn)
{
	return !!(z_mdownstates & 1 << btn);
}

bool
z_mpressed(i32 btn)
{
	return !!(z_mpressstates & 1 << btn);
}

bool
z_mreleased(i32 btn)
{
	return !!(z_mreleasestates & 1 << btn);
}

bool
z_textinput(char ch)
{
	usize byte = ch / 8, bit = ch % 8;
	return z_textinputstates[byte] & 1 << bit;
}
