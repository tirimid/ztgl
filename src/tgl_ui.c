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
		else if (u->elems[i].any.type == TGL_SLIDER)
		{
			tgl_color_t scol = TGL_SLIDERCOLOR;
			tgl_color_t tcol = TGL_SLIDERTEXTCOLOR;
			tgl_color_t bcol = TGL_SLIDERBARCOLOR;
			if (mx >= x && my >= y && mx < x + w && my < y + h)
			{
				if (tgl_mdown(SDL_BUTTON_LEFT))
				{
					scol = TGL_SLIDERPRESSCOLOR;
					tcol = TGL_SLIDERTEXTPRESSCOLOR;
					bcol = TGL_SLIDERBARPRESSCOLOR;
				}
				else
				{
					scol = TGL_SLIDERHOVERCOLOR;
					tcol = TGL_SLIDERTEXTPRESSCOLOR;
					bcol = TGL_SLIDERBARHOVERCOLOR;
				}
			}
			
			tgl_conf.renderrect(x, y, w, h, scol);
			tgl_conf.renderrect(x, y, u->elems[i].slider.val * w, h, bcol);
			tgl_conf.rendertext(
				x + pad,
				y + pad,
				w - 2 * pad,
				h - 2 * pad,
				u->elems[i].slider.text,
				tcol
		}
		else // textfield.
		{
			tgl_color_t tfcol = TGL_TEXTFIELDCOLOR;
			tgl_color_t tftcol = TGL_TEXTFIELDTEXTCOLOR;
			tgl_color_t tfbcol = TGL_TEXTFIELDBARCOLOR;
			tgl_color_t tfpcol = TGL_TEXTFIELDPROMPTCOLOR;
			if (mx >= x && my >= y && mx < x + w && my < y + h)
			{
				if (tgl_mdown(SDL_BUTTON_LEFT))
				{
					tfcol = TGLTEXTFIELDPRESSCOLOR;
					tftcol = TGLTEXTFIELDTEXTPRESSCOLOR;
					tfbcol = TGLTEXTFIELDBARPRESSCOLOR;
					tfpcol = TGLTEXTFIELDPROMPTPRESSCOLOR;
				}
				else
				{
					tfcol = TGLTEXTFIELDHOVERCOLOR;
					tftcol = TGLTEXTFIELDTEXTHOVERCOLOR;
					tfbcol = TGLTEXTFIELDBARHOVERCOLOR;
					tfpcol = TGLTEXTFIELDPROMPTHOVERCOLOR;
				}
			}
			
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
tgl_uislider(tgl_ui_t *u, char const *text, TGL_INOUT float *val)
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
		*val = (double)(mx - u->x) / w;
		state = true;
	}
	
	*val = *val < 0.0f ? 0.0f : *val;
	*val = *val > 1.0f ? 1.0f : *val;
	
	u->elems[u->nelems++] = (tgl_uielem_t)
	{
		.slider =
		{
			.type = TGL_SLIDER,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text,
			.val = *val
		}
	};
	u->y += h;
	
	return state;
}

// function assumes that the user is rendering UI with a monospacing font.
void
tgl_uitextfield(
	tgl_ui_t *u,
	char const *text,
	TGL_INOUT tgl_tfdata_t *tfdata,
	uint32_t ndraw
)
{
	if (u->nelems >= u->elemcap)
	{
		return false;
	}
	
	int32_t chw, chh;
	TTF_SizeText(u->font, " ", &chw, &chh);
	
	int32_t w = ndraw * chw + 2 * u->uipad;
	int32_t h = chh + 2 * u->uipad;
	
	if (tgl_mreleased(SDL_LEFT))
	{
		int32_t mx, my;
		tgl_mpos(u->wnd, &mx, &my);
		
		if (mx >= u->x && my >= u->y && mx < u->x + w && my < u->y + h)
		{
			tfdata->sel = true;
		}
		else
		{
			tfdata->sel = false;
		}
	}
	
	if (tfdata->sel)
	{
		if (tgl_kpressed(SDLK_LEFT))
		{
			tfdata->csr -= tfdata->csr > 0;
			tfdata->first -= tfdata->csr < tfdata->first;
		}
		
		if (tgl_kpressed(SDLK_RIGHT))
		{
			tfdata->csr += tfdata->csr < tfdata->len;
			tfdata->first += tfdata->csr - tfdata->first >= ndraw;
		}
		
		if (tgl_kpressed(SDLK_UP))
		{
			tfdata->csr = 0;
			tfdata->first = 0;
		}
		
		if (tgl_kpressed(SDLK_DOWN))
		{
			tfdata->csr = tfdata->len;
			tfdata->first = 0;
			while (tfdata->csr - tfdata->first > ndraw)
			{
				++tfdata->first;
			}
		}
		
		// TODO: handle text input.
	}
	
	u->elems[u->nelems++] = (tgl_uielem_t)
	{
		.textfield =
		{
			.type = TGL_TEXTFIELD,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text,
			.tfdata = &tfdata,
			.ndraw = ndraw
		}
	};
	u->y += h;
}
