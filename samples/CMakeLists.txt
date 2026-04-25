file(GLOB hdrs "*.h*" "../include/*.h")
set(srcs
    main_arithmetic.cpp
    ../src/parser.cpp
    ../src/validator.cpp
    ../src/calculator.cpp
    ../src/token_traits.cpp
    ../src/string_lib.cpp
)
add_executable(postfix ${srcs} ${hdrs})
