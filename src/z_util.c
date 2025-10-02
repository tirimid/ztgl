// SPDX-License-Identifier: BSD-3-Clause

static u64 z_tickstart;

void
z_err(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	char msg[512];
	vsnprintf(msg, sizeof(msg), fmt, args);
	
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, z_conf.errtitle, msg, NULL))
	{
		fprintf(z_conf.log, "\x1b[1;31merr\x1b[0m: %s\n", msg);
	}
	
	va_end(args);
}

u64
z_unixus(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (u64)tv.tv_sec * 1000000 + (u64)tv.tv_usec;
}

void
z_begintick(void)
{
	z_tickstart = z_unixus();
}

void
z_endtick(void)
{
	u64 tickend = z_unixus();
	i64 timeleft = z_conf.tickus - tickend + z_tickstart;
	timeleft *= (timeleft > 0);
	
	struct timespec ts =
	{
		.tv_nsec = 1000 * timeleft
	};
	nanosleep(&ts, NULL);
}

void
z_begintimer(OUT u64 *timer)
{
	*timer = z_unixus();
}

void
z_endtimer(u64 timer, char const *name)
{
	u64 d = z_unixus() - timer;
	fprintf(
		z_conf.log,
		"\x1b[1;33mprofile\x1b[0m: %s: %llu\n",
		name,
		(unsigned long long)d
	);
}

f32
z_interpangle(f32 a, f32 b, f32 t)
{
	return a + z_shortestangle(a, b) * t;
}

f32
z_shortestangle(f32 a, f32 b)
{
	f32 d = fmod(b - a, Z_TAU);
	f32 shortest = fmod(2.0f * d, Z_TAU) - d;
	return shortest;
}

f32
z_r2d(f32 r)
{
	return r / Z_PI * 180.0f;
}

f32
z_d2r(f32 d)
{
	return d / 180.0f * Z_PI;
}

void *
z_allocbatch(INOUT z_allocbatch_t *allocs, usize nallocs)
{
	usize size = 0;
	for (usize i = 0; i < nallocs; ++i)
	{
		size += allocs[i].n * allocs[i].size;
		size = z_align(size, 16);
	}
	
	u8 *ptr = malloc(size);
	if (!ptr)
	{
		return NULL;
	}
	
	usize offset = 0;
	for (usize i = 0; i < nallocs; ++i)
	{
		*allocs[i].ptr = &ptr[offset];
		offset += allocs[i].n * allocs[i].size;
		offset = z_align(offset, 16);
	}
	
	return ptr;
}

void *
z_reallocbatch(void *p, INOUT z_reallocbatch_t *reallocs, usize nreallocs)
{
	usize *oldoffsets = calloc(nreallocs, sizeof(usize));
	usize *newoffsets = calloc(nreallocs, sizeof(usize));
	
	usize newsize = 0, oldsize = 0;
	for (usize i = 0; i < nreallocs; ++i)
	{
		newoffsets[i] = newsize;
		newsize += reallocs[i].nnew * reallocs[i].size;
		newsize = z_align(newsize, 16);
		
		oldoffsets[i] = oldsize;
		oldsize += reallocs[i].nold * reallocs[i].size;
		oldsize = z_align(oldsize, 16);
	}
	
	p = realloc(p, newsize);
	if (!p)
	{
		free(oldoffsets);
		free(newoffsets);
		return NULL;
	}
	
	u8 *up = p;
	for (isize i = nreallocs - 1; i >= 0; --i)
	{
		usize newbytes = reallocs[i].nnew * reallocs[i].size;
		usize oldbytes = reallocs[i].nold * reallocs[i].size;
		usize bytes = newbytes < oldbytes ? newbytes : oldbytes;
		
		memmove(&up[newoffsets[i]], &up[oldoffsets[i]], bytes);
		*reallocs[i].ptr = &up[newoffsets[i]];
	}
	
	free(oldoffsets);
	free(newoffsets);
	return p;
}

u64
z_align(u64 addr, u64 align)
{
	return addr + align - addr % align;
}
