if(EXISTS "D:/momen fci 3/OOP/OOP mayar/build_gui/unit_tests[1]_tests.cmake")
  include("D:/momen fci 3/OOP/OOP mayar/build_gui/unit_tests[1]_tests.cmake")
else()
  add_test(unit_tests_NOT_BUILT unit_tests_NOT_BUILT)
endif()