AC_INIT([toyprogram], [0.1], [outerpasta@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CHECK_LIB([readline], [main])
AC_CHECK_LIB([SDL2], [main])
AC_CHECK_LIB([SDL2_image], [main])
AC_CHECK_LIB([xml2], [main])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
