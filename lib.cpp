#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

auto trigger_dynamic_cast_in_ubsanrt()
{
    constexpr char cmdline[] = "/proc/self/cmdline";
    std::ifstream cmdlineFile(cmdline);
    if (!cmdlineFile)
    {
        fprintf(stderr, "Failed to read %s\n", cmdline);
        return std::vector<std::string>{};
    }

    std::string component;
    std::vector<std::string> components;
    while (std::getline(cmdlineFile, component, '\0'))
    {
        components.push_back(component);
        fprintf(stderr, "\t%s\n", component.c_str());
    }

    return components;
}

static auto const hold_args = trigger_dynamic_cast_in_ubsanrt();

extern "C" __attribute__((visibility("default"))) const char* get_executable_name()
{
    if (hold_args.size())
    {
        return hold_args[0].c_str();
    }
    return "/proc/self/exe";
}

extern "C" __attribute__((visibility("default"))) const char* get_arg(size_t idx)
{
    if (hold_args.size() && (idx > 0) && (idx < hold_args.size()))
    {
        return hold_args[idx].c_str();
    }
	return nullptr;
}
