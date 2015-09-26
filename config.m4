dnl $Id$
dnl config.m4 for extension amfg

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(amf, for amf support,
dnl Make sure that the comment is aligned:
dnl [  --with-amf             Include amf support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(amf, whether to enable amf support,
[  --enable-amf           Enable amf support])

if test "$PHP_AMF" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-amf -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/amf.h"  # you most likely want to change this
  dnl if test -r $PHP_AMF/$SEARCH_FOR; then # path given as parameter
  dnl   AMF_DIR=$PHP_AMF
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for amf files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       AMF_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$AMF_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the amf distribution])
  dnl fi

  dnl # --with-amf -> add include path
  dnl PHP_ADD_INCLUDE($AMF_DIR/include)

  dnl # --with-amf -> check for lib and symbol presence
  dnl LIBNAME=amf # you may want to change this
  dnl LIBSYMBOL=amf # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $AMF_DIR/lib, AMF_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_AMFLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong amf lib version or lib not found])
  dnl ],[
  dnl   -L$AMF_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(AMF_SHARED_LIBADD)

  AC_DEFINE(HAVE_AMF, 1, [Whether you have AMF])
  PHP_NEW_EXTENSION(amf, amf.c, $ext_shared)
fi
