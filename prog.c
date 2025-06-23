#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        fprintf(stderr, "No DSO paths given, give at least one\n");
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i++)
    {
        void* dso = dlopen(argv[i], RTLD_NOW);
        if (!dso)
        {
            fprintf(stderr, "dlopen(\"%s\", RTLD_NOW) failed: %s\n", argv[i], dlerror());
            return EXIT_FAILURE;
        }
        dlclose(dso);
    }
    return EXIT_SUCCESS;
}
