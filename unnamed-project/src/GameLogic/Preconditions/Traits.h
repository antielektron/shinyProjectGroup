#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_TRAITS_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_TRAITS_H

namespace traits
{
    template <typename T>
    struct precondition_name
    {
        static_assert(sizeof(T) == 0, "This type is not a precondition");
    };
}

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_TRAITS_H
