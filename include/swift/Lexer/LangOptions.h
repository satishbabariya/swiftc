//
// Created by Satish Babariya on 23/04/25.
//

#ifndef LANGOPTIONS_H
#define LANGOPTIONS_H

#include "llvm/TargetParser/Triple.h"

namespace swift {


    /// A collection of options that affect the language dialect and
    /// provide compiler debugging facilities.
    class LangOptions final {
        /// The target we are building for.
        ///
        /// This represents the minimum deployment target.
        llvm::Triple Target;

        /// \brief The second target for a zippered build
        ///
        /// This represents the target and minimum deployment version for the
        /// second ('variant') target when performing a zippered build.
        /// For example, if the target is x86_64-apple-macosx10.14 then
        /// a target-variant of x86_64-apple-ios12.0-macabi will produce
        /// a zippered binary that can be loaded into both macCatalyst and
        /// macOS processes. A value of 'None' means no zippering will be
        /// performed.
        std::optional<llvm::Triple> TargetVariant;
    };

    } // namespace swift

#endif //LANGOPTIONS_H
