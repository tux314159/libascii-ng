#include <dstring.h>

/* INIT/DEINIT */
enum lmds_status string_init(struct string *in)
{
	NULLCHK(in);

	in->len = 0;
	in->str = calloc(1, sizeof(char));

	if (in->str == NULL)
		return LMDS_ALLOC_FAIL;

	return LMDS_OK;
}

enum lmds_status string_deinit(struct string *in)
{
	NULLCHK(in);

	free(in->str);
	return LMDS_OK;
}

/* MISC */
enum lmds_status string_grow(struct string *in, const size_t add)
{
	char *t;

	NULLCHK(in);

	t = realloc(in->str, (in->len + add + 1) * sizeof(char));

	if (t == NULL)
		return LMDS_ALLOC_FAIL;

	in->str = t;
	in->len += add;
	in->str[in->len] = '\0';

	return LMDS_OK;
}

enum lmds_status string_append(struct string *in, const char *str)
{
	NULLCHK(in);
	NULLCHK(str);

	RETIFNOK(string_grow(in, strlen(str)));

	strcat(in->str, str);

	return LMDS_OK;
}

enum lmds_status string_insert(struct string *in, const size_t idx, const char *str)
{
	size_t oldlen;
	size_t len;

	NULLCHK(in);
	NULLCHK(str);

	len = strlen(str);
	oldlen = in->len;

	RETIFNOK(string_grow(in, len));
	memmove(in->str + idx + len, in->str + idx,
			(oldlen - idx + 1 /* the NULL byte */) * sizeof(char));
	memcpy(in->str + idx, str, len * sizeof(char));

	return LMDS_OK;
}

enum lmds_status string_delete(struct string *in, const size_t start, const size_t end)
{
	NULLCHK(in);
	if (in->len == 0 || end > in->len - 1)
		return LMDS_OOB;

	memmove(in->str + start, in->str + end + 1, (in->len - end) * sizeof(char)); /* Moves the NUL byte as well */
	in->len -= (end - start + 1);

	return LMDS_OK;
}
