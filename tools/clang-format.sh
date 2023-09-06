find $1 -name "*.cpp" -or -name "*.hpp" \
         -name "*.hxx" -or -name "*.cxx" \
         -name "*.h"  | \
    xargs -n 1 clang-format -i
