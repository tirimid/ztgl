// SPDX-License-Identifier: BSD-3-Clause

tgl_ui_t
tgl_beginui(
	tgl_uielem_t elems[],
	size_t elemcap,
	int32_t x,
	int32_t y,
	TTF_Font *font,
	SDL_Window const *wnd
)
{
	return (tgl_ui_t)
	{
		.elems = elems,
		.elemcap = elemcap,
		.x = x,
		.y = y,
		.font = font,
		.wnd = wnd
	};
}

void
tgl_renderui(tgl_ui_t const *u)
{
	if (!u->nelems)
	{
		return;
	}
	
	// find render boundaries to draw panel.
	int32_t minx = INT32_MAX, miny = INT32_MAX;
	int32_t maxx = INT32_MIN, maxy = INT32_MIN;
	for (size_t i = 0; i < u->nelems; ++i)
	{
		int32_t x = u->elems[i].any.x, y = u->elems[i].any.y;
		int32_t w = u->elems[i].any.w, h = u->elems[i].any.h;
		
		minx = x < minx ? x : minx;
		miny = y < miny ? y : miny;
		maxx = x + w > maxx ? x + w : maxx;
		maxy = y + h > maxy ? y + h : maxy;
	}
	
	int32_t pad = tgl_conf.uipad;
	
	// draw panel.
	tgl_conf.renderrect(
		minx - pad,
		miny - pad,
		maxx - minx + 2 * pad,
		maxy - miny + 2 * pad,
		TGL_PANELCOLOR
	);
	
	// draw UI elements.
	int32_t mx, my;
	tgl_mpos(u->wnd, &mx, &my);
	for (size_t i = 0; i < u->nelems; ++i)
	{
		int32_t x = u->elems[i].any.x, y = u->elems[i].any.y;
		int32_t w = u->elems[i].any.w, h = u->elems[i].any.h;
		
		if (u->elems[i].any.type == TGL_LABEL)
		{
			tgl_conf.rendertext(
				x,
				y,
				w,
				h,
				u->elems[i].button.text,
				TGL_LABELTEXTCOLOR
			);
		}
		else if (u->elems[i].any.type == TGL_BUTTON)
		{
			tgl_color_t bcol = TGL_BUTTONCOLOR, tcol = TGL_BUTTONTEXTCOLOR;
			if (mx >= x && my >= y && mx < x + w && my < y + h)
			{
				if (tgl_mdown(SDL_BUTTON_LEFT))
				{
					bcol = TGL_BUTTONPRESSCOLOR;
					tcol = TGL_BUTTONTEXTPRESSCOLOR;
				}
				else
				{
					bcol = TGL_BUTTONHOVERCOLOR;
					tcol = TGL_BUTTONTEXTPRESSCOLOR;
				}
			}
			
			tgl_conf.renderrect(x, y, w, h, bcol);
			tgl_conf.rendertext(
				x + pad,
				y + pad,
				w - 2 * pad,
				h - 2 * pad,
				u->elems[i].button.text,
				tcol
			);
		}
		else // slider.
		{
			// TODO: implement.
		}
	}
}

void
tgl_uipad(tgl_ui_t *u, int32_t dx, int32_t dy)
{
	u->x += dx;
	u->y += dy;
}

void
tgl_uilabel(tgl_ui_t *u, char const *text)
{
	if (u->nelems >= u->elemcap)
	{
		return;
	}
	
	int32_t w, h;
	TTF_SizeText(u->font, text, &w, &h);
	
	u->elems[u->nelems++] = (tgl_uielem_t)
	{
		.label =
		{
			.type = TGL_LABEL,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text
		}
	};
	u->y += h;
}

bool
tgl_uibutton(tgl_ui_t *u, char const *text)
{
	if (u->nelems >= u->elemcap)
	{
		return false;
	}
	
	bool state = false;
	
	int32_t w, h;
	TTF_SizeText(u->font, text, &w, &h);
	w += 2 * tgl_conf.uipad;
	h += 2 * tgl_conf.uipad;
	
	int32_t mx, my;
	tgl_mpos(u->wnd, &mx, &my);
	
	if (tgl_mreleased(SDL_BUTTON_LEFT)
		&& mx >= u->x
		&& my >= u->y
		&& mx < u->x + w
		&& my < u->y + h)
	{
		state = true;
	}
	
	u->elems[u->nelems++] = (tgl_uielem_t)
	{
		.button =
		{
			.type = TGL_BUTTON,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text
		}
	};
	u->y += h;
	
	return state;
}

bool
tgl_uislider(tgl_ui_t *u, char const *text, float *inoutval)
{
	// TODO: implement.
}
