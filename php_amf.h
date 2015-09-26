/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifndef PHP_AMF_H
#define PHP_AMF_H

#define PHP_AMF_VERSION "0.9.4"
#include "ext/standard/php_smart_str.h"

extern zend_module_entry amf_module_entry;
#define phpext_amf_ptr &amf_module_entry

#ifdef PHP_WIN32
#define PHP_amf_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define PHP_AMF_API __attribute__ ((visibility("default")))
#else
#define PHP_AMF_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(amf);
PHP_MSHUTDOWN_FUNCTION(amf);
/* PHP_RINIT_FUNCTION(amf); */
/* PHP_RSHUTDOWN_FUNCTION(amf); */
static PHP_MINFO_FUNCTION(amf);

static PHP_FUNCTION(amf_encode);
static PHP_FUNCTION(amf_decode);
static PHP_FUNCTION(amf_last_error);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
*/
ZEND_BEGIN_MODULE_GLOBALS(amf)
	/* may not need this...following the example of json_encode */
	int error_code;
	/* the system endianness - I would like this to be a constant but not sure how? */
	int endianness;

	/* the following vars deprecated -- hoping to use approach suggested here instead
	 * http://news.php.net/php.pecl.dev/8820
	 */
	/* vars to track arrays/objects for serialization by reference */
	/* array containing memory addresses of arrays and objects encountered */
/*	zval **objects_and_arrays; */
	/* number of serialized arrays/objects stored in the array */
/*	int objects_and_arrays_count; */
	/* number of slots allocated for the array */
/*	int objects_and_arrays_allocated;*/
ZEND_END_MODULE_GLOBALS(amf)


/* In every utility function you add that needs to use variables 
   in php_amf_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as AMF_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define AMF_G(v) TSRMG(amf_globals_id, zend_amf_globals *, v)
#else
#define AMF_G(v) (amf_globals.v)
#endif


PHP_AMF_API void php_amf_encode(smart_str *buf, zval *val, int flags, HashTable *htArrays, HashTable *htObjects, HashTable *htStrings TSRMLS_DC);
PHP_AMF_API void php_amf_decode(zval *return_value, char *str, int str_len, long flags TSRMLS_DC);


/*
   A variety of constants that AMF will export into the global namespace
*/

/* flags */
/* whether to force amf0 serialization, not yet implemented */
#define PHP_AMF_AMF0	(1<<0)
/* whether to force utf8 translation, not yet implemented */
#define PHP_AMF_UTF8_XLATE	(1<<1)

/* type markers from amf3 spec */
#define PHP_AMF_AMF3_TYPE_UNDEFINED ((unsigned char)0x00)
#define PHP_AMF_AMF3_TYPE_NULL ((unsigned char)0x01)
#define PHP_AMF_AMF3_TYPE_FALSE ((unsigned char)0x02)
#define PHP_AMF_AMF3_TYPE_TRUE ((unsigned char)0x03)
#define PHP_AMF_AMF3_TYPE_INTEGER ((unsigned char)0x04)
#define PHP_AMF_AMF3_TYPE_DOUBLE ((unsigned char)0x05)
#define PHP_AMF_AMF3_TYPE_STRING ((unsigned char)0x06)
#define PHP_AMF_AMF3_TYPE_XML_DOC ((unsigned char)0x07)
#define PHP_AMF_AMF3_TYPE_DATE ((unsigned char)0x08)
#define PHP_AMF_AMF3_TYPE_ARRAY ((unsigned char)0x09)
#define PHP_AMF_AMF3_TYPE_OBJECT ((unsigned char)0x0A)
#define PHP_AMF_AMF3_TYPE_XML ((unsigned char)0x0B)
#define PHP_AMF_AMF3_TYPE_BYTEARRAY ((unsigned char)0x0C)

/* error codes */
#define PHP_AMF_ERROR_NONE (0x00)

/* misc boundary numbers, etc */
#define PHP_AMF_AMF3_INT_MAX 268435455
#define PHP_AMF_AMF3_INT_MIN -268435456
#define PHP_AMF_STRING_MAX_LENGTH PHP_AMF_AMF3_INT_MAX
#define PHP_AMF_STRING_MAX_REFERENCES PHP_AMF_AMF3_INT_MAX
#define PHP_AMF_ARRAY_CONTIG_LENGTH_MAX PHP_AMF_AMF3_INT_MAX
#define PHP_AMF_OBJECT_PROPERTIES_MAX 33554431
#define PHP_AMF_OBJECT_REFERENCES_MAX PHP_AMF_AMF3_INT_MAX

/* system endianness constants */
#define PHP_AMF_ENDIAN_LITTLE 0
#define PHP_AMF_ENDIAN_BIG 1

#endif	/* PHP_AMF_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
