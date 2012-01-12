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
# include <string.h>

# include <nettle/rsa.h>

# include "../config.h"

# include "maelcum.h"
# include "cloudfront_strings.h"

# define MAELCUM_LONG_STRING_MAX_LENGTH 50
# define MAELCUM_CONDITION_MAX_LENGTH 512

struct maelcum_ctx
{
	struct rsa_public_key public_key;
	struct rsa_private_key private_key;
	char *key_id;
};

struct maelcum_ctx* maelcum_init(void)
{
	struct maelcum_ctx *ctx = (struct maelcum_ctx*)malloc(sizeof(struct maelcum_ctx));
	if(!ctx) { return NULL; }

	rsa_public_key_init(&(ctx->public_key));
	rsa_private_key_init(&(ctx->private_key));
	ctx->key_id = NULL;

	return ctx;
}
void maelcum_free(struct maelcum_ctx* ctx)
{
	rsa_public_key_clear(&(ctx->public_key));
	rsa_private_key_clear(&(ctx->private_key));

	if(ctx->key_id) { free(ctx->key_id); }

	free(ctx);
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

	error = rsa_keypair_from_sexp(&(ctx->public_key), &(ctx->private_key), 0, buffer_size, buffer);
	free(buffer);
	if(error != 1)
	{
		return -1;
	}

	return 0;
}

void maelcum_set_key_id(struct maelcum_ctx* ctx, const char *key_id)
{
# if HAVE_STRDUP      
	ctx->key_id = strdup(key_id);
# else
	size_t buffer_size = strlen(key_id)+1;
	ctx->key_id = (char*)malloc(buffer_size);
	strncpy(ctx->key_id, key_id, buffer_size);
# endif
}
const char* maelcum_get_key_id(struct maelcum_ctx* ctx)
{
	return ctx->key_id;
}

void maelcum_base64_to_url(char *str)
{
	char *str_ptr = str;

	while(*str_ptr != '\0')
	{
		switch(*str_ptr)
		{
		case '+':
			*str_ptr = '-';
			break;
		case '=':
			*str_ptr = '_';
			break;
		case '/':
			*str_ptr = '~';
			break;
		}

		str_ptr++;
	}
}
void maelcum_url_to_base64(char *str)
{
	char *str_ptr = str;

	while(*str_ptr != '\0')
	{
		switch(*str_ptr)
		{
		case '-':
			*str_ptr = '+';
			break;
		case '_':
			*str_ptr = '=';
			break;
		case '~':
			*str_ptr = '/';
			break;
		}

		str_ptr++;
	}
}

char* maelcum_long_to_string(long value)
{
	char *buffer = (char*)malloc(MAELCUM_LONG_STRING_MAX_LENGTH);
	if(!buffer) { return NULL; }

	int success = snprintf(buffer, MAELCUM_LONG_STRING_MAX_LENGTH, "%li", value);
	if(success < 0)
	{
		free(buffer);
		return NULL;
	}

	return buffer;
}

char* maelcum_create_condition(const char *format, const char *value)
{
	size_t buffer_size = strlen(value)+1+strlen(format)-2;
	char *buffer = (char*)malloc(buffer_size);
	if(!buffer) { return NULL; }

	int success = snprintf(buffer, buffer_size, format, value);
	if(success < 0)
	{
		free(buffer);
		return NULL;
	}

	return buffer;
}
char* maelcum_create_condition_ipaddress(const char *ip_address)
{
	return maelcum_create_condition(MAELCUM_CLOUDFRONT_COND_IPADDRESS, ip_address);
}
char* maelcum_create_condition_datelessthan(long date_less_than)
{
	char *value_string = maelcum_long_to_string(date_less_than);
	if(value_string == NULL) { return NULL; }

	char *cond = maelcum_create_condition(MAELCUM_CLOUDFRONT_COND_DATELESSTHAN, value_string);
	free(value_string);

	return cond;
}
char* maelcum_create_condition_dategreaterthan(long date_greater_than)
{
	char *value_string = maelcum_long_to_string(date_greater_than);
	if(value_string == NULL) { return NULL; }

	char *cond = maelcum_create_condition(MAELCUM_CLOUDFRONT_COND_DATELESSTHAN, value_string);
	free(value_string);

	return cond;
}
char* maelcum_create_condition_resource(const char *resource)
{
	return maelcum_create_condition(MAELCUM_CLOUDFRONT_COND_RESOURCE, resource);
}

char* maelcum_string_append(char *dest, const char *src, size_t *n)
{
	if(!src || strlen(src)+1 > *n) { return NULL; }

	strncpy(dest, src, *n);
	*n = (*n-strlen(src));

	return dest+strlen(src);
}
char* maelcum_string_append_and_free(char *dest, char *src, size_t *n)
{
	char *result = maelcum_string_append(dest, src, n);
	free(src);

	return result;
}

int maelcum_create_condition_string(char *dest, size_t n, long date_less_than, long date_greater_than, const char *ip_address)
{
	if(date_less_than < 0) { return -1; }
	if(!dest) { return -1; }

	char *cond_ptr = dest;
	char *tmp = NULL;
	size_t size_left = n;

	cond_ptr = maelcum_string_append_and_free(cond_ptr, maelcum_create_condition_datelessthan(date_less_than), &size_left);
	if(!cond_ptr) { return -1; }

	if(date_greater_than > 0)
	{
		cond_ptr = maelcum_string_append(cond_ptr, ",", &size_left);
		if(!cond_ptr) { return -1; }

		cond_ptr = maelcum_string_append_and_free(cond_ptr, maelcum_create_condition_dategreaterthan(date_greater_than), &size_left);
		if(!cond_ptr) { return -1; }
	}
	
	if(ip_address)
	{
		cond_ptr = maelcum_string_append(cond_ptr, ",", &size_left);
		if(!cond_ptr) { return -1; }

		cond_ptr = maelcum_string_append_and_free(cond_ptr, maelcum_create_condition_ipaddress(ip_address), &size_left);
		if(!cond_ptr) { return -1; }
	}

	return n-size_left;
}

char* maelcum_create_policy(const char *resource, long date_less_than, long date_greater_than, const char *ip_address)
{
	char *resource_str = NULL;
	char *inner_condition = NULL;
	char *condition = NULL;
	char *policy = NULL;
	size_t policy_length = strlen(MAELCUM_CLOUDFRONT_POLICY)-2+1;
	int bytes_written = -1;

	inner_condition = (char*)malloc(MAELCUM_CONDITION_MAX_LENGTH);
	if(!inner_condition) { return NULL; }
	size_t cond_size = maelcum_create_condition_string(inner_condition, MAELCUM_CONDITION_MAX_LENGTH, date_less_than, date_greater_than, ip_address);
	if(cond_size < 0)
	{
		free(inner_condition);
		return NULL;
	}

	condition = (char*)malloc(strlen(MAELCUM_CLOUDFRONT_CONDITION)+cond_size-2+1);
	if(!condition)
	{
		free(inner_condition);
		return NULL;
	}
	bytes_written = snprintf(condition, strlen(MAELCUM_CLOUDFRONT_CONDITION)+cond_size-2+1, MAELCUM_CLOUDFRONT_CONDITION, inner_condition);
	free(inner_condition);
	if(bytes_written < 0 || bytes_written >= strlen(MAELCUM_CLOUDFRONT_CONDITION)+cond_size-2+1)
	{
		free(condition);
		return NULL;
	}
	policy_length += strlen(condition);

	if(resource)
	{
		resource_str = maelcum_create_condition_resource(resource);
		if(!resource_str)
		{
			free(condition);
			return NULL;
		}

		policy_length += strlen(resource_str);
	}

	policy = (char*)malloc(policy_length);
	if(!policy)
	{
		free(condition);
		free(resource_str);
		return NULL;
	}

	if(resource)
	{
		bytes_written = snprintf(policy, policy_length, MAELCUM_CLOUDFRONT_POLICY, resource_str, condition);
	}
	else
	{
		bytes_written = snprintf(policy, policy_length, MAELCUM_CLOUDFRONT_POLICY_NO_RESOURCE, condition);
	}
	free(condition);
	free(resource_str);
	
	if(bytes_written < 0 || bytes_written >= policy_length)
	{
		free(policy);
		
		return NULL;
	}

	return policy;
}
