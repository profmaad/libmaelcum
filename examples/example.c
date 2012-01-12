# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <maelcum/maelcum.h>

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Usage: %s <keyfile>\n", argv[0]);
		return 1;
	}

	const char *keyfile = argv[1];
	struct maelcum_ctx *ctx;

	ctx = maelcum_init();
	if(!ctx)
	{
		printf("maelcum_init failed\n");
		return 1;
	}

	int success = maelcum_load_key(ctx, keyfile);
	if(success != 0)
	{
		printf("maelcum_load_key('%s') failed: %i\n", keyfile, success);
		return success;
	}

	maelcum_set_key_id(ctx, "abc123");

	char *url = maelcum_create_signed_url(ctx, "http://d604721fxaaqy9.cloudfront.net/training/video.mp4", "http://d604721fxaaqy9.cloudfront.net/training/*", 1258237200, -1, "145.168.143.0/24");
	printf("url: %s\n", url);
	free(url);

	maelcum_free(ctx);

	return 0;
}
