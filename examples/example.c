# include <stdio.h>
# include <stdlib.h>

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

	printf("key id is: %s\n", maelcum_get_key_id(ctx));

	char *policy = maelcum_create_policy("http://example.com/test.html", 123, 321, "127.0.0.1");
	printf("policy 0: %s\n", policy);
	free(policy);

	policy = maelcum_create_policy("http://example.com/test.html", 123, -1, "127.0.0.1");
	printf("policy 1: %s\n", policy);
	free(policy);

	policy = maelcum_create_policy("http://example.com/test.html", 123, 321, NULL);
	printf("policy 2: %s\n", policy);
	free(policy);

	policy = maelcum_create_policy(NULL, 123, 321, "127.0.0.1");
	printf("policy 3: %s\n", policy);
	free(policy);

	maelcum_free(ctx);

	return 0;
}
