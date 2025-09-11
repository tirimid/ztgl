// SPDX-License-Identifier: BSD-3-Clause

// standard library.
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

// system dependencies.
#include <sys/time.h>

tgl_conf_t tgl_conf;

uint8_t tgl_defaultcolors[][4] =
{
	{0, 0, 0, 128}, // panel.
	{255, 255, 255, 255}, // label text.
	{255, 255, 255, 128}, // button.
	{0, 0, 0, 255}, // button press.
	{128, 128, 128, 255}, // button hover.
	{255, 255, 255, 255}, // button text.
	{255, 255, 255, 255}, // button press text.
	{255, 255, 255, 255}, // button hover text.
	{0, 0, 0, 128}, // slider.
	{0, 0, 0, 255}, // slider press.
	{0, 0, 0, 255}, // slider hover.
	{128, 128, 128, 255}, // slider bar.
	{128, 128, 128, 255}, // slider press bar.
	{128, 128, 128, 255}, // slider hover bar.
	{255, 255, 255, 255}, // slider text.
	{255, 255, 255, 255}, // slider press text.
	{255, 255, 255, 255}, // slider hover text.
	{0, 0, 0, 128}, // textfield.
	{0, 0, 0, 255}, // textfield press.
	{0, 0, 0, 255}, // textfield hover.
	{255, 255, 255, 255}, // textfield text.
	{255, 255, 255, 255}, // textfield press text.
	{255, 255, 255, 255}, // textfield hover text.
	{255, 255, 255, 255}, // textfield bar.
	{255, 255, 255, 255}, // textfield press bar.
	{255, 255, 255, 255}, // textfield hover bar.
	{128, 128, 128, 255}, // textfield prompt.
	{128, 128, 128, 255}, // textfield press prompt.
	{128, 128, 128, 255} // textfield hover prompt.
};
