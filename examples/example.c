# include <stdio.h>

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

	return 0;
}
