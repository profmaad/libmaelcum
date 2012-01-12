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

# ifndef MAELCUM_H
# define MAELCUM_H

# include <nettle/nettle-types.h>

struct maelcum_ctx;

struct maelcum_ctx* maelcum_init(void);
void maelcum_free(struct maelcum_ctx* ctx);

int maelcum_load_key(
struct maelcum_ctx* ctx, const char *filename);
void maelcum_set_key_id(struct maelcum_ctx* ctx, const char *key_id);
const char* maelcum_get_key_id(struct maelcum_ctx* ctx);

char* maelcum_create_policy(const char *resource, long date_less_than, long date_greather_than, const char *ip_address);

void maelcum_base64_to_url(char *str);
void maelcum_url_to_base64(char *str);

char* maelcum_base64_encode(const uint8_t *src, unsigned int length);
uint8_t* maelcum_base64_decode(const char *src, unsigned int *result_length);

uint8_t* maelcum_sign(struct maelcum_ctx *ctx, const uint8_t *data, unsigned int length, size_t *signature_length);

# endif /*MAELCUM_H*/
