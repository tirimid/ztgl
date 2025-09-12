// SPDX-License-Identifier: BSD-3-Clause

i32
z_readpack(OUT z_pack_t *p, u8 *pack, usize size)
{
	// TODO: implement.
}

i32
z_readpackfile(OUT z_pack_t *p, FILE *fp)
{
	// TODO: implement.
}

void
z_writepack(OUT u8 buf[], OUT usize *size, z_pack_t const *p)
{
	// TODO: implement.
}

i32
z_writepackfile(FILE *fp, z_pack_t const *p)
{
	// TODO: implement.
}

void
z_destroypack(z_pack_t *p)
{
	// TODO: implement.
}

i32
z_packadd(z_pack_t *p, z_res_t const *r)
{
	// TODO: implement.
}

i32
z_packinsert(z_pack_t *p, z_res_t const *r, u32 idx)
{
	// TODO: implement.
}

i32
z_packrm(z_pack_t *p, z_restype_t type, u32 idx)
{
	// TODO: implement.
}

bool
z_packfind(OUT u32 *idx, z_pack_t const *p, z_restype_t type, char const *name)
{
	// TODO: implement.
}

void
z_packranges(z_pack_t *p)
{
	// TODO: implement.
}
