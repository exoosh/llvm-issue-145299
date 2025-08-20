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
        static const char* (*pfn_get_executable_name)() = nullptr;
        static const char* (*pfn_get_arg)(size_t) = nullptr;
        if (!pfn_get_executable_name)
        {
            pfn_get_executable_name = (decltype(pfn_get_executable_name))dlsym(dso, "get_executable_name");
            if (!pfn_get_executable_name)
            {
                fprintf(stderr, "dlsym(\"%s\") -> failed: %p: %s\n", "get_executable_name", pfn_get_executable_name, dlerror());
                dlclose(dso);
                return EXIT_FAILURE;
            }
            printf("Executable name: %s\n", pfn_get_executable_name());
        }
        if (!pfn_get_arg)
        {
            pfn_get_arg = (decltype(pfn_get_arg))dlsym(dso, "get_arg");
            if (!pfn_get_arg)
            {
                fprintf(stderr, "dlsym(\"%s\") -> failed: %p: %s\n", "get_arg", pfn_get_arg, dlerror());
                dlclose(dso);
                return EXIT_FAILURE;
            }
            printf("Argument 1: %s\n", pfn_get_arg(1));
        }
        dlclose(dso);
    }
    int x = -1;
    int y = x << 1;
    printf("%d ... UBSan should have triggered on the previous line\n", y);
    return EXIT_SUCCESS;
}
