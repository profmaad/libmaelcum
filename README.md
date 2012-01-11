# libmaelcum

## Description

libmaelcum is a library to create signed URLs for Amazons Cloudfront CDN service.

It uses libnettle for its crypto and uses RSA keys in nettle's S-Expression format. See openssl2nettle for a conversion utility.

## Requirements

 * autotools
 * libnettle (>= 2.4, although earlier versions might work)

## Build

If you cloned the git repository, first do

	./autogen.sh

Now, just use the autotools trinity:

     ./configure && make && make install


## Usage

TODO

## License

Copyright (C) 2012 *Prof. MAAD* aka Max Wolter

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
