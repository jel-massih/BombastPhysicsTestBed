#include "Core.h"
#include <assert.h>

uint32_t hashString(const char string[], uint32_t hash)
{
	assert(string);
	while (string != '\0')
	{
		char c = *string++;
		if ((c >= 'a') && (c <= 'z'))
		{
			c = c - 'a' + 'A';
		}
		hash += c;
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	return hash;
}