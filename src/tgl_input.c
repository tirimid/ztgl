// SPDX-License-Identifier: BSD-3-Clause

// assume that 1024 represents a big enough number of keystates.
static uint8_t tgl_kdownstates[1024 / 8];
static uint8_t tgl_kpressstates[1024 / 8], tgl_kreleasestates[1024 / 8];

// SDL2 supports 5 mouse buttons.
static bool tgl_mdownstates[5];
static bool tgl_mpressstates[5], tgl_mreleasestates[5];

void
tgl_handleinput(SDL_Event const *e)
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
		
		size_t byte = k / 8, bit = k % 8;
		
		if (state)
		{
			tgl_kdownstates[byte] |= 1 << bit;
			tgl_kpressstates[byte] |= 1 << bit;
		}
		else
		{
			tgl_kdownstates[byte] &= ~(1 << bit);
			tgl_kreleasestates[byte] |= 1 << bit;
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		bool state = e->type == SDL_MOUSEBUTTONDOWN;
		
		if (state)
		{
			tgl_mdownstates[e->button.button] = true;
			tgl_mpressstates[e->button.button] = true;
		}
		else
		{
			tgl_mdownstates[e->button.button] = false;
			tgl_mreleasestates[e->button.button] = true;
		}
	}
}

void
tgl_prepareinput(void)
{
	memset(tgl_kpressstates, 0, sizeof(tgl_kpressstates));
	memset(tgl_kreleasestates, 0, sizeof(tgl_kreleasestates));
	memset(tgl_mpressstates, 0, sizeof(tgl_mpressstates));
	memset(tgl_mreleasestates, 0, sizeof(tgl_mreleasestates));
}

bool
tgl_kdown(SDL_Keycode k)
{
	if (k & 1 << 30)
	{
		k &= ~(1 << 30);
		k += 128;
	}
	size_t byte = k / 8, bit = k % 8;
	return tgl_kdownstates[byte] & 1 << bit;
}

bool
tgl_kpressed(SDL_Keycode k)
{
	if (k & 1 << 30)
	{
		k &= ~(1 << 30);
		k += 128;
	}
	size_t byte = k / 8, bit = k % 8;
	return tgl_kpressstates[byte] & 1 << bit;
}

bool
tgl_kreleased(SDL_Keycode k)
{
	if (k & 1 << 30)
	{
		k &= ~(1 << 30);
		k += 128;
	}
	size_t byte = k / 8, bit = k % 8;
	return tgl_kreleasestates[byte] & 1 << bit;
}

void
tgl_mpos(SDL_Window const *wnd, int32_t *outx, int32_t *outy)
{
	if (SDL_GetMouseFocus() != wnd)
	{
		*outx = 0;
		*outy = 0;
		return;
	}
	
	SDL_GetMouseState(outx, outy);
}

bool
tgl_mdown(int32_t btn)
{
	return tgl_mdownstates[btn];
}

bool
tgl_mpressed(int32_t btn)
{
	return tgl_mpressstates[btn];
}

bool
tgl_mreleased(int32_t btn)
{
	return tgl_mreleasestates[btn];
}
