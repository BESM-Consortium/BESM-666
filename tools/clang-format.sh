if [ ! -d "$1" ]; then
    exit 0
fi

find $1 -name '*.cpp' -or -name '*.hpp' -or \
         -name '*.hxx' -or -name '*.cxx' -or \
         -name '*.h'  | \
    xargs -n 1 clang-format -i
