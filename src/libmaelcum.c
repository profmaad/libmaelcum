/* libmaelcum, library to generate signed urls for Amazon Cloudfront
 * 
 * Copyright (C) 2012, Max Wolter aka Prof. MAAD
 *
 * libmaelcum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
	
 * libmaelcum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
	
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

# include <stdlib.h>
# include <stdio.h>
# include <errno.h>

# include <nettle/rsa.h>

# include "libmaelcum.h"

struct maelcum_ctx
{
	struct rsa_public_key public_key;
	struct rsa_private_key private_key;
};

struct maelcum_ctx* maelcum_init(void)
{
	struct maelcum_ctx *ctx = (struct maelcum_ctx*)malloc(sizeof(struct maelcum_ctx));
	if(!ctx) { return NULL; }

	rsa_public_key_init(&(ctx->public_key));
	rsa_private_key_init(&(ctx->private_key));

	return ctx;
}

long maelcum_get_file_size(FILE *file, int *error)
{
	long size = -1;

	if(fseek(file, 0, SEEK_END) != 0)
	{
		*error = errno;
		return -1;
	}
	size = ftell(file);
	if(size < 0)
	{
		*error = errno;
		return -1;
	}
	rewind(file);

	return size;
}

uint8_t* maelcum_read_file(FILE *file, long *size, int *error)
{
	uint8_t *buffer = NULL;
	long buffer_size = -1;

	*error = 0;

	buffer_size = maelcum_get_file_size(file, error);
	if(buffer_size < 0) { return NULL; }
	
	buffer = (uint8_t*)malloc(sizeof(uint8_t)*buffer_size);
	if(!buffer)
	{
		*error = errno;
		return NULL;
	}

	size_t bytes_read = fread((void*)buffer, sizeof(uint8_t), (size_t)buffer_size, file);	
	if(bytes_read < buffer_size)
	{
		*error = -1;
	}

	*size = buffer_size;
	return buffer;
}

int maelcum_load_key(struct maelcum_ctx* ctx, const char *filename)
{
	uint8_t *buffer = NULL;
	long buffer_size = -1;
	int error = -1;

	FILE *keyfile = fopen(filename, "rb");
	if(!keyfile) { return errno; }
	
	buffer = maelcum_read_file(keyfile, &buffer_size, &error);
	fclose(keyfile);
	
	if(error != 0)
	{
		if(buffer) { free(buffer); }
		return error;
	}
	
	rsa_public_key_clear(&(ctx->public_key));
	rsa_private_key_clear(&(ctx->private_key));

	error = rsa_keypair_from_sexp(&(ctx->public_key), &(ctx->private_key), 0, buffer_size, buffer);
	if(error != 1)
	{
		return -1;
	}

	return 0;
}
