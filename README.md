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

For license, see file

	COPYING
