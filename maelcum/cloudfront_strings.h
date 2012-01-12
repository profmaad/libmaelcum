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

# ifndef MAELCUM_CLOUDFRONT_STRINGS_H
# define MAELCUM_CLOUDFRONT_STRINGS_H

const char *MAELCUM_CLOUDFRONT_COND_IPADDRESS = "\"IpAddress\":{\"AWS:SourceIp\":\"%s\"}";
const char *MAELCUM_CLOUDFRONT_COND_DATELESSTHAN = "\"DateLessThan\":{\"AWS:EpochTime\":%s}";
const char *MAELCUM_CLOUDFRONT_COND_DATEGREATERTHAN = "\"DateGreaterThan\":{\"AWS:EpochTime\":%s}";
const char *MAELCUM_CLOUDFRONT_COND_RESOURCE = "\"Resource\":\"%s\"";

const char *MAELCUM_CLOUDFRONT_CONDITION = "\"Condition\":{%s}";
const char *MAELCUM_CLOUDFRONT_POLICY = "{\"Statement\":[{%s,%s}]}";
const char *MAELCUM_CLOUDFRONT_POLICY_NO_RESOURCE = "{\"Statement\":[{%s}]}";

# endif /*MAELCUM_CLOUDFRONT_STRINGS_H*/
