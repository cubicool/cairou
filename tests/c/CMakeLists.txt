MACRO(ADD_TEST _example)
	ADD_EXECUTABLE("test-${_example}" "test-${_example}.c" common.c common.h)
	TARGET_LINK_LIBRARIES("test-${_example}" cairou)
ENDMACRO()

ADD_TEST("grid")

