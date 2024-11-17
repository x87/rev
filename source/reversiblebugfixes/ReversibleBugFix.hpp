#pragma once
#include <source_location>
#include <string>
#include "KeyGen.h"


namespace notsa::bugfixes {
struct ReversibleBugFix {
    static inline ReversibleBugFix* Tail{};
    
    //ReversibleBugFix(
    //    std::string Name,
    //    std::string Description,
    //    std::string Credit,
    //    bool        Enabled = true,
    //    bool        Locked = false
    //):
    //    Name{ Name },
    //    Description{ Description },
    //    Credit{ Credit },
    //    Enabled{ Enabled },
    //    Locked{ Locked },
    //    Prev{std::exchange(Tail, this)}
    //{
    //}

    std::string       Name{};                            //!< User-defined name
    std::string       Description{};                     //!< User defined description
    std::string       Credit{};                          //!< Credits for this bugfix
    bool              Enabled{ true };                   //!< Is this bugfix enabled? (If not, the bug is present)
    bool              Locked{ false };                   //!< Can this bugfix's enabled state be changed?
    ReversibleBugFix* Prev{ std::exchange(Tail, this) }; //!< Linked list of bugfixes (Maintained for the debug tool)

    constexpr operator bool() const { return Enabled; }
};
};
