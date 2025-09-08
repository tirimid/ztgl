// SPDX-License-Identifier: BSD-3-Clause

static uint64_t tgl_tickstart;

void
tgl_err(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	char msg[512];
	vsnprintf(msg, sizeof(msg), fmt, args);
	
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, tgl_conf.errtitle, msg, NULL))
	{
		fprintf(tgl_conf.log, "\x1b[1;31merr\x1b[0m: %s\n", msg);
	}
	
	va_end(args);
}

uint64_t
tgl_unixus(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
}

void
tgl_begintick(void)
{
	tgl_tickstart = tgl_unixus();
}

void
tgl_endtick(void)
{
	uint64_t tickend = tgl_unixus();
	int64_t timeleft = tgl_conf.tickus - tickend + tgl_tickstart;
	timeleft *= (timeleft > 0);
	
	struct timespec ts =
	{
		.tv_nsec = 1000 * timeleft
	};
	nanosleep(&ts, NULL);
}

void
tgl_begintimer(uint64_t *outtimer)
{
	*outtimer = tgl_unixus();
}

void
tgl_endtimer(uint64_t timer, char const *name)
{
	uint64_t d = tgl_unixus() - timer;
	fprintf(
		tgl_conf.log,
		"\x1b[1;33mprofile\x1b[0m: %s: %llu\n",
		name,
		(unsigned long long)d
	);
}

float
tgl_interpangle(float a, float b, float t)
{
	return a + tgl_shortestangle(a, b) * t;
}

float
tgl_shortestangle(float a, float b)
{
	float d = fmod(b - a, TGL_TAU);
	float shortest = fmod(2.0f * d, TGL_TAU) - d;
	return shortest;
}

float
tgl_r2d(float r)
{
	return r / TGL_PI * 180.0f;
}

float
tgl_d2r(float d)
{
	return d / 180.0f * TGL_PI;
}

void *
tgl_allocbatch(tgl_allocbatch_t *allocs, size_t nallocs)
{
	// TODO: implement.
}

void *
tgl_reallocbatch(tgl_reallocbatch_t *reallocs, size_t nreallocs)
{
	// TODO: implement.
}
