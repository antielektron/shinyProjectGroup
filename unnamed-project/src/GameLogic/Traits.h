#ifndef UNNAMED_PROJECT_GAME_LOGIC_TRAITS_H
#define UNNAMED_PROJECT_GAME_LOGIC_TRAITS_H

namespace traits
{
    template <typename T>
    struct action_name
    {
        static_assert(sizeof(T) == 0, "This type is not an action");
    };

    template <typename T>
    struct animation_name
    {
        static_assert(sizeof(T) == 0, "This type is not an animation");
    };

    template <typename T>
    struct precondition_name
    {
        static_assert(sizeof(T) == 0, "This type is not a precondition");
    };

}

#endif // UNNAMED_PROJECT_GAME_LOGIC_TRAITS_H
