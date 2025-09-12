// SPDX-License-Identifier: BSD-3-Clause

i32
z_readpack(OUT z_pack_t *p, u8 *pack, usize size)
{
	// TODO: implement.
	(void)p, (void)pack, (void)size;
	__builtin_unreachable();
}

i32
z_readpackfile(OUT z_pack_t *p, FILE *fp)
{
	// TODO: implement.
	(void)p, (void)fp;
	__builtin_unreachable();
}

void
z_writepack(OUT u8 buf[], OUT usize *size, z_pack_t const *p)
{
	// TODO: implement.
	(void)buf, (void)size, (void)p;
	__builtin_unreachable();
}

i32
z_writepackfile(FILE *fp, z_pack_t const *p)
{
	// TODO: implement.
	(void)fp, (void)p;
	__builtin_unreachable();
}

void
z_destroypack(z_pack_t *p)
{
	// TODO: implement.
	(void)p;
	__builtin_unreachable();
}

i32
z_packadd(z_pack_t *p, z_res_t const *r)
{
	// TODO: implement.
	(void)p, (void)r;
	__builtin_unreachable();
}

i32
z_packinsert(z_pack_t *p, z_res_t const *r, u32 idx)
{
	// TODO: implement.
	(void)p, (void)r, (void)idx;
	__builtin_unreachable();
}

i32
z_packrm(z_pack_t *p, z_restype_t type, u32 idx)
{
	// TODO: implement.
	(void)p, (void)type, (void)idx;
	__builtin_unreachable();
}

bool
z_packfind(OUT u32 *idx, z_pack_t const *p, z_restype_t type, char const *name)
{
	// TODO: implement.
	(void)idx, (void)p, (void)type, (void)name;
	__builtin_unreachable();
}

void
z_packranges(z_pack_t *p)
{
	// TODO: implement.
	(void)p;
	__builtin_unreachable();
}
