// SPDX-License-Identifier: BSD-3-Clause

int32_t
tgl_readpack(tgl_pack_t *out, uint8_t *pack, size_t size)
{
	// TODO: implement.
}

int32_t
tgl_readpackfile(tgl_pack_t *out, FILE *fp)
{
	// TODO: implement.
}

void
tgl_writepack(uint8_t outbuf[], size_t *outsize, tgl_pack_t const *p)
{
	// TODO: implement.
}

int32_t
tgl_writepackfile(FILE *fp, tgl_pack_t const *p)
{
	// TODO: implement.
}

void
tgl_destroypack(tgl_pack_t *p)
{
	// TODO: implement.
}

int32_t
tgl_packadd(tgl_pack_t *p, tgl_res_t const *r)
{
	// TODO: implement.
}

int32_t
tgl_packinsert(tgl_pack_t *p, tgl_res_t const *r, uint32_t idx)
{
	// TODO: implement.
}

int32_t
tgl_packrm(tgl_pack_t *p, tgl_restype_t type, uint32_t idx)
{
	// TODO: implement.
}

bool
tgl_packfind(
	uint32_t *out,
	tgl_pack_t const *p,
	tgl_restype_t type,
	char const *name
)
{
	// TODO: implement.
}

void
tgl_packranges(tgl_pack_t *p)
{
	// TODO: implement.
}
