#include <iostream>
#include <optional>

std::optional<std::string> noblock_input(std::istream& in = std::cin)
{
    in.sync_with_stdio(false);

    if (in.rdbuf()->in_avail() > 0) {
        char buffer[1024];

        in.getline(buffer, sizeof(buffer));

        return buffer;
    }

    return {};
}