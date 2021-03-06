#include <stdio.h>
#include <dlfcn.h>

int main()
{
	void *handle;
	void (*my_str_copy)(char *, char *);
	void (*my_str_cat)(char *, char *);
	char *error;

	handle = dlopen("mystr.so", RTLD_LAZY);
	if(!handle)
	{
		printf("%s\n", dlerror());
		exit(1);
	}
	dlerror();
	my_str_copy = dlsym(handle, "my_strcpy");
	my_str_cat = dlsym(handle, "my_strcat");
	if ((error = dlerror()) != NULL)
	{
		printf("%s\n", error);
		exit(1);
	}

	char dest[100];
	char src[] = "Hello World!";
	char extra[] = "extra string";
	
	my_str_copy(dest, src);
	printf("%s\n", dest);

	my_str_cat(src, extra);
	printf("%s\n", src);
	dlclose(handle);
	return 0;
}
