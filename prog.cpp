#include <stdio.h>
#include <stdlib.h>

struct Base
{
    virtual ~Base() = default;
    virtual void foo()
    {
        printf("libfoo: inside Base::foo\n");
    }
};

struct Derived : Base
{
    int value;
};

extern "C" bool vptr_check()
{
    auto* d = new Derived();
    d->value = 42;
    printf("libfoo: created Derived at %p\n", d);
    Base* b = dynamic_cast<Base*>(d);
    b->foo(); // let's hope for UBSan's vptr to instrument this ...
    printf("libfoo: Base::foo virtual call completed with Base at %p\n", b);
    return true;
}

int main(int argc, char** argv)
{
    printf("calling vptr_check()\n");
    if (!vptr_check())
    {
        printf("vptr_check() failed\n");
        return EXIT_FAILURE;
    }
    printf("vptr_check() succeeded\n");
    return EXIT_SUCCESS;
}
