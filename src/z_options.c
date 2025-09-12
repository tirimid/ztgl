// SPDX-License-Identifier: BSD-3-Clause

z_err_t
z_optraw(OUT char buf[], FILE *fp, char const *key)
{
	fseek(fp, 0, SEEK_SET);
	
	for (usize line = 0; !feof(fp) && !ferror(fp); ++line)
	{
		i32 ch;
		while (ch = fgetc(fp), ch != EOF && isspace(ch))
		{
		}
		
		if (ch == '#')
		{
			while (ch = fgetc(fp), ch != EOF && ch != '\n')
			{
			}
		}
		
		if (ch == '\n' || feof(fp))
		{
			continue;
		}
		
		fseek(fp, -1, SEEK_CUR);
		char keybuf[Z_MAXOPTIONKEY] = {0};
		if (fscanf(fp, Z_OPTIONSCAN, keybuf, buf) != 2)
		{
			return Z_INVALIDFORMAT;
		}
		
		if (!strcmp(buf, "NONE"))
		{
			buf[0] = 0;
		}
		
		if (!strcmp(keybuf, key))
		{
			return Z_OK;
		}
	}
	
	return Z_NOTFOUND;
}

z_err_t
z_optkeycode(OUT SDL_Keycode *k, FILE *fp, char const *key)
{
	char buf[Z_MAXOPTIONVALUE] = {0};
	z_err_t err = z_optraw(buf, fp, key);
	if (err)
	{
		return err;
	}
	
	*k = SDL_GetKeyFromName(buf);
	if (*k == SDLK_UNKNOWN)
	{
		return Z_INVALIDCONVERSION;
	}
	
	return Z_OK;
}

z_err_t
z_optfloat(OUT f64 *f, FILE *fp, char const *key)
{
	char buf[Z_MAXOPTIONVALUE] = {0};
	z_err_t err = z_optraw(buf, fp, key);
	if (err)
	{
		return err;
	}
	
	errno = 0;
	*f = strtof(buf, NULL);
	if (errno)
	{
		return Z_INVALIDCONVERSION;
	}
	
	return Z_OK;
}

z_err_t
z_optint(OUT i64 *i, FILE *fp, char const *key)
{
	char buf[Z_MAXOPTIONVALUE] = {0};
	z_err_t err = z_optraw(buf, fp, key);
	if (err)
	{
		return err;
	}
	
	errno = 0;
	*i = (i64)strtoll(buf, NULL, 0);
	if (errno)
	{
		return Z_INVALIDCONVERSION;
	}
	
	return Z_OK;
}

z_err_t
z_optbool(OUT bool *b, FILE *fp, char const *key)
{
	char buf[Z_MAXOPTIONVALUE] = {0};
	z_err_t err = z_optraw(buf, fp, key);
	if (err)
	{
		return err;
	}
	
	if (!strcmp(buf, "true"))
	{
		*b = true;
		return Z_OK;
	}
	else if (!strcmp(buf, "false"))
	{
		*b = false;
		return Z_OK;
	}
	else
	{
		return Z_INVALIDCONVERSION;
	}
}
