// SPDX-License-Identifier: BSD-3-Clause

typedef enum z_uiflag
{
	Z_INACTIVE = 0x1
} z_uiflag_t;

typedef enum z_uitype
{
	Z_LABEL = 0,
	Z_BUTTON,
	Z_SLIDER,
	Z_TEXTFIELD,
	Z_HOLDBUTTON
} z_uitype_t;

z_ui_t
z_beginui(
	z_uielem_t elems[],
	usize elemcap,
	i32 x,
	i32 y,
	TTF_Font *font,
	SDL_Window const *wnd
)
{
	return (z_ui_t)
	{
		.elems = elems,
		.elemcap = elemcap,
		.x = x,
		.y = y,
		.font = font,
		.wnd = wnd,
		.active = true
	};
}

void
z_renderui(z_ui_t const *u)
{
	if (!u->nelems)
	{
		return;
	}
	
	// find render boundaries to draw panel.
	i32 minx = INT32_MAX, miny = INT32_MAX;
	i32 maxx = INT32_MIN, maxy = INT32_MIN;
	for (usize i = 0; i < u->nelems; ++i)
	{
		i32 x = u->elems[i].any.x, y = u->elems[i].any.y;
		i32 w = u->elems[i].any.w, h = u->elems[i].any.h;
		
		minx = x < minx ? x : minx;
		miny = y < miny ? y : miny;
		maxx = x + w > maxx ? x + w : maxx;
		maxy = y + h > maxy ? y + h : maxy;
	}
	
	i32 pad = z_conf.uipad;
	
	// draw panel.
	z_conf.renderrect(
		minx - pad,
		miny - pad,
		maxx - minx + 2 * pad,
		maxy - miny + 2 * pad,
		Z_PANELCOLOR
	);
	
	// draw UI elements.
	i32 mx, my;
	z_mpos(u->wnd, &mx, &my);
	for (usize i = 0; i < u->nelems; ++i)
	{
		i32 x = u->elems[i].any.x, y = u->elems[i].any.y;
		i32 w = u->elems[i].any.w, h = u->elems[i].any.h;
		
		if (u->elems[i].any.flags & Z_INACTIVE)
		{
			z_conf.renderrect(x, y, w, h, Z_INACTIVECOLOR);
			continue;
		}
		
		if (u->elems[i].any.type == Z_LABEL)
		{
			z_conf.rendertext(
				x,
				y,
				w,
				h,
				u->elems[i].button.text,
				Z_LABELTEXTCOLOR
			);
		}
		else if (u->elems[i].any.type == Z_BUTTON)
		{
			z_color_t bcol = Z_BUTTONCOLOR;
			z_color_t tcol = Z_BUTTONTEXTCOLOR;
			if (mx >= x && my >= y && mx < x + w && my < y + h)
			{
				if (z_mdown(SDL_BUTTON_LEFT))
				{
					bcol = Z_BUTTONPRESSCOLOR;
					tcol = Z_BUTTONTEXTPRESSCOLOR;
				}
				else
				{
					bcol = Z_BUTTONHOVERCOLOR;
					tcol = Z_BUTTONTEXTPRESSCOLOR;
				}
			}
			
			z_conf.renderrect(x, y, w, h, bcol);
			z_conf.rendertext(
				x + pad,
				y + pad,
				w - 2 * pad,
				h - 2 * pad,
				u->elems[i].button.text,
				tcol
			);
		}
		else if (u->elems[i].any.type == Z_SLIDER)
		{
			z_color_t scol = Z_SLIDERCOLOR;
			z_color_t tcol = Z_SLIDERTEXTCOLOR;
			z_color_t bcol = Z_SLIDERBARCOLOR;
			if (mx >= x && my >= y && mx < x + w && my < y + h)
			{
				if (z_mdown(SDL_BUTTON_LEFT))
				{
					scol = Z_SLIDERPRESSCOLOR;
					tcol = Z_SLIDERTEXTPRESSCOLOR;
					bcol = Z_SLIDERBARPRESSCOLOR;
				}
				else
				{
					scol = Z_SLIDERHOVERCOLOR;
					tcol = Z_SLIDERTEXTPRESSCOLOR;
					bcol = Z_SLIDERBARHOVERCOLOR;
				}
			}
			
			z_conf.renderrect(x, y, w, h, scol);
			z_conf.renderrect(x, y, u->elems[i].slider.val * w, h, bcol);
			z_conf.rendertext(
				x + pad,
				y + pad,
				w - 2 * pad,
				h - 2 * pad,
				u->elems[i].slider.text,
				tcol
			);
		}
		else if (u->elems[i].any.type == Z_TEXTFIELD)
		{
			z_color_t tfcol = Z_TEXTFIELDCOLOR;
			z_color_t tftcol = Z_TEXTFIELDTEXTCOLOR;
			z_color_t tfbcol = Z_TEXTFIELDBARCOLOR;
			z_color_t tfpcol = Z_TEXTFIELDPROMPTCOLOR;
			if (mx >= x && my >= y && mx < x + w && my < y + h)
			{
				if (z_mdown(SDL_BUTTON_LEFT))
				{
					tfcol = Z_TEXTFIELDPRESSCOLOR;
					tftcol = Z_TEXTFIELDTEXTPRESSCOLOR;
					tfbcol = Z_TEXTFIELDBARPRESSCOLOR;
					tfpcol = Z_TEXTFIELDPROMPTPRESSCOLOR;
				}
				else
				{
					tfcol = Z_TEXTFIELDHOVERCOLOR;
					tftcol = Z_TEXTFIELDTEXTHOVERCOLOR;
					tfbcol = Z_TEXTFIELDBARHOVERCOLOR;
					tfpcol = Z_TEXTFIELDPROMPTHOVERCOLOR;
				}
			}
			
			z_conf.renderrect(x, y, w, h, tfcol);
			
			z_tfdata_t const *t = u->elems[i].textfield.tfdata;
			i32 chw = (w - 2 * pad) / u->elems[i].textfield.ndraw;
			i32 chh = h - 2 * pad;
			
			char const *text = t->len ? t->buf : u->elems[i].textfield.text;
			u32 textfirst = t->len ? t->first : 0;
			u32 textlen = t->len ? t->len : strlen(text);
			z_color_t textcol = t->len ? tftcol : tfpcol;
			
			i32 dx = 0;
			for (u32 j = textfirst; j < textlen; ++j)
			{
				if (dx >= w - 2 * pad)
				{
					break;
				}
				char render[] = {text[j], 0};
				z_conf.rendertext(x + pad + dx, y + pad, chw, chh, render, textcol);
				dx += chw;
			}
			
			if (t->sel)
			{
				z_conf.renderrect(
					x + pad + (t->csr - t->first) * chw,
					y + pad,
					z_conf.uitextfieldbar,
					chh,
					tfbcol
				);
			}
		}
		else // holdbutton.
		{
			z_color_t bcol = Z_BUTTONCOLOR;
			z_color_t tcol = Z_BUTTONTEXTCOLOR;
			if (mx >= x && my >= y && mx < x + w && my < y + h)
			{
				if (z_mdown(SDL_BUTTON_LEFT))
				{
					bcol = Z_BUTTONPRESSCOLOR;
					tcol = Z_BUTTONTEXTPRESSCOLOR;
				}
				else
				{
					bcol = Z_BUTTONHOVERCOLOR;
					tcol = Z_BUTTONTEXTHOVERCOLOR;
				}
			}
			
			z_conf.renderrect(x, y, w, h, bcol);
			z_conf.rendertext(
				x + pad,
				y + pad,
				w - 2 * pad,
				h - 2 * pad,
				u->elems[i].holdbutton.text,
				tcol
			);
		}
	}
}

void
z_uilabel(z_ui_t *u, char const *text)
{
	if (u->nelems >= u->elemcap)
	{
		return;
	}
	
	i32 w, h;
	TTF_SizeText(u->font, text, &w, &h);
	
	u->elems[u->nelems++] = (z_uielem_t)
	{
		.label =
		{
			.type = Z_LABEL,
			.flags = Z_INACTIVE * !u->active,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text
		}
	};
	if (u->horizontal)
	{
		u->x += w;
	}
	else
	{
		u->y += h;
	}
}

bool
z_uibutton(z_ui_t *u, char const *text)
{
	if (u->nelems >= u->elemcap)
	{
		return false;
	}
	
	bool state = false;
	
	i32 w, h;
	TTF_SizeText(u->font, text, &w, &h);
	w += 2 * z_conf.uipad;
	h += 2 * z_conf.uipad;
	
	if (u->active)
	{
		i32 mx, my;
		z_mpos(u->wnd, &mx, &my);
		
		if (z_mreleased(SDL_BUTTON_LEFT)
			&& mx >= u->x
			&& my >= u->y
			&& mx < u->x + w
			&& my < u->y + h)
		{
			state = true;
		}
	}
	
	u->elems[u->nelems++] = (z_uielem_t)
	{
		.button =
		{
			.type = Z_BUTTON,
			.flags = Z_INACTIVE * !u->active,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text
		}
	};
	if (u->horizontal)
	{
		u->x += w;
	}
	else
	{
		u->y += h;
	}
	
	return state;
}

bool
z_uislider(z_ui_t *u, char const *text, INOUT f32 *val)
{
	if (u->nelems >= u->elemcap)
	{
		return false;
	}
	
	bool state = false;
	
	i32 w, h;
	TTF_SizeText(u->font, text, &w, &h);
	w += 2 * z_conf.uipad;
	h += 2 * z_conf.uipad;
	
	if (u->active)
	{
		i32 mx, my;
		z_mpos(u->wnd, &mx, &my);
		
		if (z_mreleased(SDL_BUTTON_LEFT)
			&& mx >= u->x
			&& my >= u->y
			&& mx < u->x + w
			&& my < u->y + h)
		{
			*val = (f64)(mx - u->x) / w;
			state = true;
		}
		
		*val = *val < 0.0f ? 0.0f : *val;
		*val = *val > 1.0f ? 1.0f : *val;
	}
	
	u->elems[u->nelems++] = (z_uielem_t)
	{
		.slider =
		{
			.type = Z_SLIDER,
			.flags = Z_INACTIVE * !u->active,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text,
			.val = *val
		}
	};
	if (u->horizontal)
	{
		u->x += w;
	}
	else
	{
		u->y += h;
	}
	
	return state;
}

// function assumes that the user is rendering UI with a monospacing font.
void
z_uitextfield(z_ui_t *u, char const *text, INOUT z_tfdata_t *tfdata, u32 ndraw)
{
	if (u->nelems >= u->elemcap)
	{
		return;
	}
	
	i32 chw, chh;
	TTF_SizeText(u->font, " ", &chw, &chh);
	
	i32 w = ndraw * chw + 2 * z_conf.uipad;
	i32 h = chh + 2 * z_conf.uipad;
	
	if (u->active)
	{
		if (z_mreleased(SDL_BUTTON_LEFT))
		{
			i32 mx, my;
			z_mpos(u->wnd, &mx, &my);
			
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
			if (z_kpressed(SDLK_LEFT))
			{
				tfdata->csr -= tfdata->csr > 0;
				tfdata->first -= tfdata->csr < tfdata->first;
			}
			
			if (z_kpressed(SDLK_RIGHT))
			{
				tfdata->csr += tfdata->csr < tfdata->len;
				tfdata->first += tfdata->csr - tfdata->first >= ndraw;
			}
			
			if (z_kpressed(SDLK_UP))
			{
				tfdata->csr = 0;
				tfdata->first = 0;
			}
			
			if (z_kpressed(SDLK_DOWN))
			{
				tfdata->csr = tfdata->len;
				tfdata->first = 0;
				while (tfdata->csr - tfdata->first > ndraw)
				{
					++tfdata->first;
				}
			}
			
			for (u8 i = 0; i < 128; ++i)
			{
				if (tfdata->len + 1 >= tfdata->cap)
				{
					break;
				}
				
				if (!isprint(i))
				{
					continue;
				}
				
				if (z_textinput(i))
				{
					memmove(
						&tfdata->buf[tfdata->csr + 1],
						&tfdata->buf[tfdata->csr],
						tfdata->len - tfdata->csr
					);
					
					++tfdata->csr;
					tfdata->first += tfdata->csr - tfdata->first >= ndraw;
					tfdata->buf[tfdata->csr - 1] = i;
					++tfdata->len;
					tfdata->buf[tfdata->len] = 0;
				}
			}
			
			if (z_kpressed(SDLK_BACKSPACE) && tfdata->csr)
			{
				memmove(
					&tfdata->buf[tfdata->csr - 1],
					&tfdata->buf[tfdata->csr],
					tfdata->len - tfdata->csr
				);
				
				--tfdata->csr;
				tfdata->first -= tfdata->csr < tfdata->first;
				
				--tfdata->len;
				tfdata->buf[tfdata->len] = 0;
			}
		}
	}
	
	u->elems[u->nelems++] = (z_uielem_t)
	{
		.textfield =
		{
			.type = Z_TEXTFIELD,
			.flags = Z_INACTIVE * !u->active,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text,
			.tfdata = tfdata,
			.ndraw = ndraw
		}
	};
	if (u->horizontal)
	{
		u->x += w;
	}
	else
	{
		u->y += h;
	}
}

bool
z_holdbutton(z_ui_t *u, char const *text)
{
	if (u->nelems >= u->elemcap)
	{
		return false;
	}
	
	bool state = false;
	
	i32 w, h;
	TTF_SizeText(u->font, text, &w, &h);
	w += 2 * z_conf.uipad;
	h += 2 * z_conf.uipad;
	
	if (u->active)
	{
		i32 mx, my;
		z_mpos(u->wnd, &mx, &my);
		
		if (z_mdown(SDL_BUTTON_LEFT)
			&& mx >= u->x
			&& my >= u->y
			&& mx < u->x + w
			&& my < u->y + h)
		{
			state = true;
		}
	}
	
	u->elems[u->nelems++] = (z_uielem_t)
	{
		.holdbutton =
		{
			.type = Z_HOLDBUTTON,
			.flags = Z_INACTIVE * !u->active,
			.x = u->x,
			.y = u->y,
			.w = w,
			.h = h,
			.text = text
		}
	};
	if (u->horizontal)
	{
		u->x += w;
	}
	else
	{
		u->y += h;
	}
	
	return state;
}
