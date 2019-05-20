#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <cstring>

#include <zlib.h>

#include "compress_decompress.h"
#include "compress_decompress_t.h" /* print_string */

static z_const char hello[] = "Juntos e shallow now!";

/* 
 * printf: 
 *   Invokes OCALL to display the enclave buffer to the terminal.
 */
void printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_printf(buf);
}

static char* strcpy(char* destination, const char* source)
{
	// return if no memory is allocated to the destination
	if (destination == NULL)
		return NULL;

	// take a pointer pointing to the beginning of destination string
	char *ptr = destination;
	
	// copy the C-string pointed by source into the array
	// pointed by destination
	while (*source != '\0')
	{
		*destination = *source;
		destination++;
		source++;
	}

	// include the terminating null character
	*destination = '\0';

	// destination is returned by standard strcpy()
	return ptr;
}

int test_compress(Byte *p_compr, uLong *p_comprLen, Byte *p_uncompr, uLong *p_uncomprLen)
{
    Byte *compr, *uncompr;
    uLong comprLen = 10000 * sizeof(int); /* don't overflow on MSDOS */
    uLong uncomprLen = comprLen;

    compr = (Byte *)calloc((uInt)comprLen, 1);
    uncompr = (Byte *)calloc((uInt)uncomprLen, 1);

    int err;
    uLong len = (uLong)strlen(hello) + 1;

    err = compress(compr, &comprLen, (const Bytef *)hello, len);
    CHECK_ERR(err, "compress");

    strcpy((char *)uncompr, "garbage");

    err = uncompress(uncompr, &uncomprLen, compr, comprLen);
    CHECK_ERR(err, "uncompress");

    if (strcmp((char *)uncompr, hello))
    {
        printf("bad uncompress\n");
        return -1;
    }
    else
    {
        printf("uncompress(): %s\n", (char *)uncompr);
    }

    return 0;
}
