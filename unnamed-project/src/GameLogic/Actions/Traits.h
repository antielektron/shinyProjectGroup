#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_TRAITS_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_TRAITS_H

namespace traits
{
    template <typename T>
    struct action_name
    {
        static_assert(sizeof(T) == 0, "This type is not an action");
    };
}

#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_TRAITS_H
