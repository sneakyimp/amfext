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
 | Author: J. Adams <zardozrocks@gmail.com>                             |
 +----------------------------------------------------------------------+
 */

// JTA - for some godforsaken reason Eclipse CDT is complaining about NULL not being defined
#ifndef NULL
#define NULL   ((void *) 0)
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "ext/standard/php_var.h"
#include <stddef.h>
#include "php_amf.h"

ZEND_DECLARE_MODULE_GLOBALS(amf)

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_amf_encode, 0, 0, 1)
	ZEND_ARG_INFO(0, arg)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_amf_decode, 0, 0, 1)
	ZEND_ARG_INFO(0, amf_string)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_amf_last_error, 0)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO(arginfo_amf_last_error_msg, 0)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ amf_functions[] */
static const zend_function_entry amf_functions[] = {
		PHP_FE(amf_encode, arginfo_amf_encode)
		PHP_FE(amf_decode, arginfo_amf_decode)
		PHP_FE(amf_last_error, arginfo_amf_last_error)
		PHP_FE(amf_last_error_msg, arginfo_amf_last_error_msg) { NULL, NULL, NULL } /* Must be the last line in amf_functions[] */
};
/* }}} */

/* {{{ amf_module_entry
 */
zend_module_entry amf_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
		STANDARD_MODULE_HEADER,
#endif
		"amf", amf_functions, PHP_MINIT(amf), PHP_MSHUTDOWN(amf), NULL, /* Replace with PHP_RINIT(amf) if there's anything to do at request start */
NULL		, /* Replace with PHP_RSHUTDOWN(amf) if there's anything to do at request end */
		PHP_MINFO(amf),
#if ZEND_MODULE_API_NO >= 20010901
		PHP_AMF_VERSION,
#endif
		STANDARD_MODULE_PROPERTIES
	};
/* }}} */

#ifdef COMPILE_DL_AMF
ZEND_GET_MODULE(amf)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
 PHP_INI_BEGIN()
 STD_PHP_INI_ENTRY("amf.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_amf_globals, amf_globals)
 STD_PHP_INI_ENTRY("amf.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_amf_globals, amf_globals)
 PHP_INI_END()
 */
/* }}} */

/* {{{ php_amf_init_globals
 */
/* Uncomment this function if you have INI entries
 static void php_amf_init_globals(zend_amf_globals *amf_globals)
 {
 amf_globals->global_value = 0;
 amf_globals->global_string = NULL;
 }
 */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(amf) {
	/* flags */
	REGISTER_LONG_CONSTANT("AMF_AMF0", PHP_AMF_AMF0,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_UTF8_XLATE", PHP_AMF_UTF8_XLATE,
			CONST_CS | CONST_PERSISTENT);

	/* type markers from amf3 spec */
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_UNDEFINED",
			PHP_AMF_AMF3_TYPE_UNDEFINED, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_NULL", PHP_AMF_AMF3_TYPE_NULL,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_FALSE", PHP_AMF_AMF3_TYPE_FALSE,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_TRUE", PHP_AMF_AMF3_TYPE_TRUE,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_INTEGER", PHP_AMF_AMF3_TYPE_INTEGER,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_DOUBLE", PHP_AMF_AMF3_TYPE_DOUBLE,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_STRING", PHP_AMF_AMF3_TYPE_STRING,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_XML_DOC", PHP_AMF_AMF3_TYPE_XML_DOC,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_DATE", PHP_AMF_AMF3_TYPE_DATE,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_ARRAY", PHP_AMF_AMF3_TYPE_ARRAY,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_OBJECT", PHP_AMF_AMF3_TYPE_OBJECT,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_XML", PHP_AMF_AMF3_TYPE_XML,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_TYPE_BYTEARRAY",
			PHP_AMF_AMF3_TYPE_BYTEARRAY, CONST_CS | CONST_PERSISTENT);

	/* error constants */
	REGISTER_LONG_CONSTANT("AMF_ERROR_NONE", PHP_AMF_ERROR_NONE,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_ERROR_TOO_MANY_REFERENCES", PHP_AMF_ERROR_TOO_MANY_REFERENCES,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_ERROR_TOO_MANY_CLASSES", PHP_AMF_ERROR_TOO_MANY_CLASSES,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_ERROR_TOO_MANY_OBJECT_PROPERTIES", PHP_AMF_ERROR_TOO_MANY_OBJECT_PROPERTIES,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("PHP_AMF_ERROR_TYPE_SERIALIZATION_NOT_SUPPORTED", PHP_AMF_ERROR_TYPE_SERIALIZATION_NOT_SUPPORTED,
			CONST_CS | CONST_PERSISTENT);

	/* miscellaneous boundary numbers, etc. */
	REGISTER_LONG_CONSTANT("AMF_AMF3_INT_MAX", PHP_AMF_AMF3_INT_MAX,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_AMF3_INT_MIN", PHP_AMF_AMF3_INT_MIN,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_STRING_MAX_LENGTH", PHP_AMF_STRING_MAX_LENGTH,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_STRING_MAX_REFERENCES",
			PHP_AMF_STRING_MAX_REFERENCES, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_ARRAY_CONTIG_LENGTH_MAX",
			PHP_AMF_ARRAY_CONTIG_LENGTH_MAX, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_OBJECT_PROPERTIES_MAX",
			PHP_AMF_OBJECT_PROPERTIES_MAX, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_OBJECT_REFERENCES_MAX",
			PHP_AMF_OBJECT_REFERENCES_MAX, CONST_CS | CONST_PERSISTENT);

	/* system endian constants */
	REGISTER_LONG_CONSTANT("AMF_ENDIAN_LITTLE", PHP_AMF_ENDIAN_LITTLE,
			CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AMF_ENDIAN_BIG", PHP_AMF_ENDIAN_BIG,
			CONST_CS | CONST_PERSISTENT);

	/* check system endianness */
	int endian_check = 1;
	char *p = (char *) &endian_check;
	if (p[0] == 1) { // Lowest address contains the least significant byte
		REGISTER_LONG_CONSTANT("AMF_SYSTEM_ENDIANNESS", PHP_AMF_ENDIAN_LITTLE,
				CONST_CS | CONST_PERSISTENT);
		AMF_G(endianness) = PHP_AMF_ENDIAN_LITTLE;
	} else {
		REGISTER_LONG_CONSTANT("AMF_SYSTEM_ENDIANNESS", PHP_AMF_ENDIAN_BIG,
				CONST_CS | CONST_PERSISTENT);
		AMF_G(endianness) = PHP_AMF_ENDIAN_BIG;
	}

	/* If you have INI entries, uncomment these lines
	 REGISTER_INI_ENTRIES();
	 */
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */PHP_MSHUTDOWN_FUNCTION(amf) {
	/* uncomment this line if you have INI entries
	 UNREGISTER_INI_ENTRIES();
	 */
	return SUCCESS;
}
/* }}} */

/* Uncomment if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
/*
 PHP_RINIT_FUNCTION(amf)
 {
 return SUCCESS;
 }
 */
/* }}} */

/* Uncomment if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
/*
 PHP_RSHUTDOWN_FUNCTION(amf)
 {
 return SUCCESS;
 }
 */
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
static PHP_MINFO_FUNCTION(amf) {
	php_info_print_table_start();
	php_info_print_table_row(2, "amf support", "enabled");
	php_info_print_table_row(2, "amf version", PHP_AMF_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	 DISPLAY_INI_ENTRIES();
	 */
}
/* }}} */

/* todo - might want to make this a macro instead */
static inline void amf_write_byte(smart_str *buf, unsigned char c) /* {{{ */
{
	smart_str_appendc(buf, c);
}
/* }}} */

/* todo - might want to make this a macro instead */
/* todo - reverse byte order on big-endian machines?  this works for little-endian only methinks */
/* todo - this func appears to assume 32-bits...is there a problem with 64-bit longs? */
static void amf_write_int(smart_str *buf, long num TSRMLS_DC) /* {{{ */
{
	if (num < PHP_AMF_AMF3_INT_MIN || num > PHP_AMF_AMF3_INT_MAX) {
		php_error_docref(
				NULL TSRMLS_CC,
				E_NOTICE,
				"Could not serialize out-of-bounds integer %ld. Serialized as zero.",
				num);
		amf_write_byte(buf, 0);
		return;
	}

	if ((num & 0xffffff80) == 0) {
		amf_write_byte(buf, num);
	} else if ((num & 0xffffc000) == 0) {
		amf_write_byte(buf, (num >> 7) | 0x80);
		amf_write_byte(buf, num & 0x7f);
	} else if ((num & 0xffe00000) == 0) {
		amf_write_byte(buf, (num >> 14) | 0x80);
		amf_write_byte(buf, (num >> 7) | 0x80);
		amf_write_byte(buf, num & 0x7f);
	} else {
		amf_write_byte(buf, (num >> 22) | 0x80);
		amf_write_byte(buf, (num >> 15) | 0x80);
		amf_write_byte(buf, (num >> 8) | 0x80);
		amf_write_byte(buf, num & 0xff);
	}
}
/* }}} */

/* this function assumes a double is precisely 8 bytes and does take into
 * account system endianness. Deprecated because:
 *
 */
static void amf_write_doubleOLD(smart_str *buf, double dbl TSRMLS_DC) /* {{{ */
{
	if (zend_isinf(dbl) || zend_isnan(dbl)) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR,
				"double %.9g is not a valid double, serialization failed.",
				dbl);
		return;
	}
	if (AMF_G(endianness) == PHP_AMF_ENDIAN_LITTLE) {
		/*
		 * the system is little endian so we must
		 * reverse the byte order from little endian
		 * to big endian
		 */
		union aligned {
			double dval;
			char cval[sizeof(double)];
		} d;
		const char * number = d.cval;
		d.dval = dbl;

		char byte_arr[sizeof(double)];
		size_t i;
		for (i = 0; i < sizeof(double); ++i) {
			byte_arr[i] = number[sizeof(double) - 1 - i];
		}
		/* append byte_arr to the buffer! */
		smart_str_appendl(buf, byte_arr, (sizeof(double)));

	} else {
		smart_str_appendl(buf, (char *)&dbl, (sizeof(double)));
	}
} /* amf_write_double() */
/* }}} */

/* this function assumes a double is precisely 8 bytes and does take into
 * account system endianness
 */
static void amf_write_double(smart_str *buf, double dbl TSRMLS_DC) /* {{{ */
{
	if (zend_isinf(dbl) || zend_isnan(dbl)) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR,
				"double %.9g is not a valid double, serialization failed.",
				dbl);
		return;
	}


	// temp var to hold bytes of double
	char *number;

	if (AMF_G(endianness) == PHP_AMF_ENDIAN_LITTLE) {
		/*
		 * the system is little endian so we must
		 * reverse the byte order from little endian
		 * to big endian
		 */

		number = (char *)&dbl;

		char byte_arr[sizeof(double)];
		size_t i;
		for (i = 0; i < sizeof(double); ++i) {
			byte_arr[i] = number[sizeof(double) - 1 - i];
		}




		/* append byte_arr to the buffer! */
		smart_str_appendl(buf, byte_arr, (sizeof(double)));

	} else {
		smart_str_appendl(buf, (char *)&dbl, (sizeof(double)));
	}
} /* amf_write_double() */


static void amf_write_string(smart_str *buf, char *str, long len, long flags, HashTable *htStrings  TSRMLS_DC) /* {{{ */
{
	ulong *prev_serialized_ref;

	/* check for empty string first, these are common */
	if (len == 0) {
		amf_write_byte(buf, 0x01);
		return;
	}

	if (len > PHP_AMF_STRING_MAX_LENGTH) {
		amf_write_byte(buf, 0x01);
		php_error_docref(
				NULL TSRMLS_CC,
				E_WARNING,
				"string length of %ld exceeded maximum. serialized as empty string",
				len);
		return;
	}

	// AMF3 maintains a list of all objects, arrays, and STRINGS! encountered during serialization
	// add another object reference for this array to htStrings

	if (htStrings && amf_add_var_hash(htStrings, str, (void *) &prev_serialized_ref TSRMLS_CC) == FAILURE) {
		/* if we are unable to add val's hash to htStrings, this means
		 * we have already encountered this array before and wrote it
		 * previously to htStrings and its key in that array is prev_serialized_ref
		 * just write prev_serialized_ref shifted left by one bit, leaving the lowest
		 * bit as zero (to indicate an object reference rather than a value reference)
		 * as an amf-style 29 digit int and return */
		amf_write_int(buf, (((long)*prev_serialized_ref - 1) << 1));
		return;
	}

	if (flags & PHP_AMF_UTF8_XLATE) {
		/* todo - implement utf8 translation */
		amf_write_byte(buf, 0x01);
		php_error_docref(
				NULL TSRMLS_CC,
				E_WARNING,
				"UTF8 translation not yet implemented. serialized as empty string",
				len);
		return;
	}

	// write a length reference which is the length left-shifted by one bit
	// and the low bit set to one to indicate a value
	amf_write_int(buf, ((len << 1) | 0x01));

	// now write the string to the buffer
	smart_str_appendl(buf, str, len);


} /* amf_write_string() */
/* }}} */


inline int amf_add_var_hash(HashTable *htVars, zval *var, void *var_old TSRMLS_DC) /* {{{ */
{
	ulong var_no;
	char id[32], *p;
	register int len;

	/* relies on "(long)" being a perfect hash function for data pointers,
	 * however the actual identity of an object has had to be determined
	 * by its object handle since 5.0. */
	if ((Z_TYPE_P(var) == IS_OBJECT) && Z_OBJ_HT_P(var)->get_class_entry) {
		p = smart_str_print_long(id + sizeof(id) - 1, (long) Z_OBJ_HANDLE_P(var));
		*(--p) = 'O';
		len = id + sizeof(id) - 1 - p;
	} else {
		p = smart_str_print_long(id + sizeof(id) - 1, (long) var);
		len = id + sizeof(id) - 1 - p;
	}

	if (var_old && zend_hash_find(htVars, p, len, var_old) == SUCCESS) {
//		if (!Z_ISREF_P(var)) {
//			/* we still need to bump up the counter, since non-refs will
//			 * be counted separately by unserializer */
//			var_no = -1;
//			zend_hash_next_index_insert(htVars, &var_no, sizeof(var_no), NULL);
//		}
		return FAILURE;
	}

	/* +1 because otherwise hash will think we are trying to store NULL pointer */
	var_no = zend_hash_num_elements(htVars) + 1;
	zend_hash_add(htVars, p, len, &var_no, sizeof(var_no), NULL);
	return SUCCESS;
}
/* }}} */

/*
 * When this function gets called, the AMF3 code for an object has already been written
 * to the buffer.  Our task in this routine is to either serialize a reference number if the
 * object has previously been encountered OR to serialize (possibly recursively) the contents
 * of the object
 */
static void amf_write_object(smart_str *buf, zval **val, int flags, HashTable *htComplexObjects, HashTable *htObjectTypeTraits, HashTable *htStrings TSRMLS_DC) /* {{{ */
{
	ulong *prev_serialized_ref;
	long prevReferences;
	prevReferences = zend_hash_num_elements(htObjectTypeTraits);
	// if we've already serialized our upper limit of arrays/objects, then this object is one too many
	if (prevReferences >= PHP_AMF_OBJECT_REFERENCES_MAX) {
		// set the global last_error value and return
		AMF_G(error_code) = PHP_AMF_ERROR_TOO_MANY_REFERENCES;
//		php_error_docref(
//				NULL TSRMLS_CC,
//				E_ERROR,
//				"number of objects exceeded maximum of %d. serialized as empty string",
//				PHP_AMF_OBJECT_REFERENCES_MAX);
		return;
	}



	// AMF3 maintains a list of all objects and arrays encountered during serialization
	// add another object reference for this array to htComplexObjects
	if (htComplexObjects && amf_add_var_hash(htComplexObjects, *val, (void *) &prev_serialized_ref TSRMLS_CC) == FAILURE) {
		/* if we are unable to add val's hash to htComplexObjects, this means
		 * we have already encountered this array before and wrote it
		 * previously to htComplexObjects and its key in that array is prev_serialized_ref
		 * just write prev_serialized_ref shifted left by one bit, leaving the lowest
		 * bit as zero (to indicate an object reference rather than a value reference)
		 * as an amf-style 29 digit int and return */
		amf_write_int(buf, (((long)*prev_serialized_ref - 1) << 1));
		return;
	}

	// OBJECT HAS NOT BEEN ENCOUNTERED IN THIS SERIALIZATION CALL AND IS NOT a reference!
	// We'll need to sniff out some stuff in order to generate our first integer token which
	// is your standard 29-bit integer (U29).

	// NOTE: the first 4 bytes, written to the buffer as an int, communicate a few bits of information about the object serialized
	// lowest bit: 0 to indicate a reference (handled above) and 1 to indicate that a serialized object follows.
	// NOTE: if you reach this point in code, lowest bit will always be 1 because object is not serialized by reference
	int firstU29 = 1; // first U29 will have a low bit of 1, we will && other vars with this once we figure stuff out

	// 2nd lowest bit: a flag (representing whether a trait reference follows). 0 means traits are being sent by reference, 1 means we'll be serializing a list of the traits
	// NOTE: set to 1 for the first instance of some class, but zero for every subsequent instance of that class because the first instance listed all the traits
	// 3rd lowest bit: set this bit to 1 if this is an externalizable class, zero otherwise TODO: figure out how to detect externalizable objects (based on inheritance?)
	// 4th lowest bit: set to 1 if object includes dynamic traits, 0 if there are no dynamic traits

	// details from the AMF3 spec:
	// http://wwwimages.adobe.com/www.adobe.com/content/dam/Adobe/en/devnet/amf/pdf/amf-file-format-spec.pdf
	// There are a few possibilities for the least significant bits:
	// - U29O-traits-ref, least bits are xxxxxx01. This would be for an object if we had previously serialized
	// an instance of the same class and the 27 significant bits would specify an index for
	// the traits that we had serialized the first time around so we don't have to list them
	// again.
	// - U29O-traits-ext, least bits are xxxxx111. For objects that implement iExternalizable.
	// we don't need to list their traits and so on because their serialization protocol is
	// out of scope of AMF3 and designed in user-code. I.e., there's an understanding between
	// the client and server. The remaining 26 significant bits are ignored and should be zero.
	// - U29O-traits, least bits are xxxxx011. Applies to both generic Objects (stdClass) and
	// also to custom classes that are not iExternalizable. Additional bits are also meaningful:
	// 4th bit of 1 means it has dynamic properties (in addition to those normal to the class) and
	// 4th bit of zero means there are no additional dynamic properties. Remaining 25 bits specify
	// an integer that tell us how many 'sealed' traits exist (i.e., non-dynamic traits defined
	// by the class).


	// object representing the class of the object to be serialized
	zend_class_entry *classEntry = Z_TYPE_PP(val) == IS_OBJECT ? Z_OBJCE_PP(val) : zend_standard_class_def;

	// values used to set bits in firstU29
	zend_bool serializeSealedTraits=0; // "sealed" object traits should serialized and sent along. zero if this is note the first instance of this class we've encountered
	zend_bool objectIsExternalizable=0; // set this to true (1) if object being serialized (val) is externalizable
	zend_bool objectIsDynamic=0; // set this to true (1) if the object being serialized contains dynamic traits (i.e., not part of the class definition)


	// determine the index of the class-name in htObjectTypeTraits.
	int *existingClassEntryIndex; // existing index of this class in htObjectTypeTraits, if it's in there
	int currentObjectClassEntryIndex; // the ultimate index of the object being serialized in htObjectTypeTraits -- either an existing one or a new one if this class has not been seen before


	zend_object *zobj; // zend_object for checking property access (make sure props are visible from current scope)
	// this fetches a zend_object for the zval
	zobj = zend_objects_get_address(*val TSRMLS_CC);

	if (zobj->properties) {
		objectIsDynamic = 1;
	}


	if (classEntry == zend_standard_class_def) {
		// ANONYMOUS CLASS (i.e., AS3 Object class or PHP stdClass object)
		serializeSealedTraits = 1; // somewhat strangely, the 2nd bit is a 1 for anonymous objects
		// this is necessary so that the unserializers don't go looking at the subsequent bytes assuming they are referenced traits
	} else {
		// determine whether "sealed" traits can reference previously serialized traits
		// OR, if this is the first encounter we've had with this class, then let's serialize
		// all the sealed traits


		// if the class of this object (i.e., classEntry) exists in htObjectTypeTraits, find its index
		// TODO: make sure this works...i'm not sure we should be using zend_hash_find this way...i.e., supplying classEntry instead of some key
		if (zend_hash_find(htObjectTypeTraits, (char *)&classEntry, sizeof(classEntry), (void **)&existingClassEntryIndex) == SUCCESS) {

			// A INSTANCE OF THIS CLASS HAS PREVIOUSLY BEEN SERIALIZED
			// so we do not need to send it's class-name or "sealed" traits -- i.e., the non-dynamic traits defined by the object's class definition
			// we just need to encode a reference to the index of this class's entry in htObjectTypeTraits
			currentObjectClassEntryIndex = (*existingClassEntryIndex);
//			php_printf("found class definition, currentObjectClassEntryIndex=%ld\n", currentObjectClassEntryIndex);

			// this flag tells us that the traits were previously serialized when we encountered this class before
			// we now we simply specify a reference
			// it used to set the 2nd lowest bit of the first byte (a U29 actually) we serialize for this object
			// TODO: IMPORTANT...there are fundamental differences between PHP classes and AS3 classes
			// and the AMF3 spec reflects the way AS3 handles dynamic classes. "dynamic" is a keyword
			// when declaring classes in AS3 and the absence or presence of that keyword in the class
			// declaration dictates for all instances of a class whether or not they are dynamic.
			// Hard to explain, but the basic idea is that an AS3 class can be serialized with
			// its as U29O-traits-ref but also have dynamic properties without having to set the
			// 4th lowest bit to indicate dynamic properties.....kind of a mind boggler

			// NOTE: if 2nd lowest bit is zero because serializeSealedTraits = 0 then
			// we simply cannot specify any flags in 3rd and 4th-lowest bits because these are supposed
			// to contain bits that are expressing which traits-by-ref....derp derp derp derp
			// 3rd-lowest bit NOT A FLAG - bit is pre-empted by 27-bit trait-by-ref specifier
			// 4th lowest bit NOT A FLAG - bit is pre-empted by 27-bit trait-by-ref specifier
			// need to test this out. Basic idea is that a 2nd bit of zero means remaining 27 digits are interpreted as a trait-ref and 3rd/4th bits are NOT flags
			serializeSealedTraits = 0; // 2nd lowest bit of first U29 will be 0
			// TODO: possible solution? If object has dynamic properties
//			if (zobj->properties) {
//				// DYNAMIC PROPERTIES EXIST!
//				serializeSealedTraits = 1; // will this cause problems? Or will we just painlessly re-serialize all the name-value pairs for this class?
//				// probably depends on unserialize function
//			}


		} else {


			// THIS IS THE FIRST TIME WE HAVE ENCOUNTERED THIS CLASS
			// so we need to send the "sealed" traits as defined by the class
			// and also its class-name

			// otherwise, we must encode a new entry in htObjectTypeTraits
			currentObjectClassEntryIndex = zend_hash_num_elements(htObjectTypeTraits);
			//php_printf("remembering class definition, currentObjectClassEntryIndex=%ld\n", currentObjectClassEntryIndex);
			if (currentObjectClassEntryIndex >= PHP_AMF_OBJECT_CLASSES_MAX) {
				// we've exceeded the number of classes that can be specified
				// i believe this is constrained by 27 bits in U29O-traits-ref
				AMF_G(error_code) = PHP_AMF_ERROR_TOO_MANY_CLASSES;
				return;
			}

			// append the class of this object (i.e., classEntry) to the end of htObjectTypeTraits
			// TODO: perhaps we should just use classEntry->name as the key?
			zend_hash_add(htObjectTypeTraits, (char *)&classEntry, sizeof(classEntry), &currentObjectClassEntryIndex, sizeof(currentObjectClassEntryIndex), NULL);

			serializeSealedTraits = 1; // 2nd lowest bit of first U29 will be 1 AND we must send all the property names along




		} // if an object of this type has been serialized before

	} // if object is anonymous class



	HashPosition pos; // used to iterate through HashTables below
	HashTable *defaultProperties;
	zend_property_info **propinfo;
	int sealedTraitCount = 0; // number of "sealed" properties, aka "default properties" explicitly defined by classes
	char *key; // key/name of each object's property
	uint key_len; // strlen of key/name...usually looks to be one larger than length of string, i think keys are terminated with null chars or something
	ulong num_index; // need this var for call to zend_hashget_current_key_ex in case we need to return numeric keys instead of string keys


	// at this point, we have enough information to write the first U29 of the serialized object
	// which consists of a bunch of flags. first bit has already been set above
	firstU29 = firstU29 | (serializeSealedTraits << 1); // 2nd bit
	firstU29 = firstU29 | (objectIsExternalizable << 2); // 3rd bit
	firstU29 = firstU29 | (objectIsDynamic << 3); // 4th bit

	// there are two cases where we add additional data to the first U29
	if (serializeSealedTraits) {
		if (classEntry == zend_standard_class_def) {
			sealedTraitCount = 0; // anonymous class objects have no sealed traits / default properties
		} else {
			// if we are serializing the sealed trait names,
			// we must know how many sealed traits exist to complete firstU29.
			// sadly, we must loop through the default traits and try zend_check_property_access

//			php_printf("counting up accessible default properties\n");
			defaultProperties = &classEntry->properties_info;
			zend_hash_internal_pointer_reset_ex(defaultProperties, &pos);
			while (zend_hash_get_current_data_ex(defaultProperties, (void **) &propinfo, &pos) == SUCCESS) {
				if (zend_hash_get_current_key_ex(defaultProperties, &key, &key_len, &num_index, 0, &pos) == HASH_KEY_IS_STRING) {
					if (zend_check_property_access(zobj, key, key_len-1 TSRMLS_CC) == SUCCESS) {
//						php_printf("%s has been counted because it is accessible\n", key);
						sealedTraitCount++;
					} else {
//						php_printf("%s excluded\n", key);
					}
				}
				zend_hash_move_forward_ex(defaultProperties, &pos);
			}
			if (sealedTraitCount > PHP_AMF_OBJECT_PROPERTIES_MAX){
				// maximum number of sealed traits exceeded. I believe this is constrained by the 25-bit specifier in U29O-traits
				AMF_G(error_code) = PHP_AMF_ERROR_TOO_MANY_OBJECT_PROPERTIES;
				return;
			}
		}
//		php_printf("%d sealed traits / default properties found\n", sealedTraitCount);

		firstU29 = firstU29 | (sealedTraitCount << 4);

	} else {
		// if we are NOT sending the serialized trait names
		// we are sending a reference to traits we serialized when we encountered this class before
//		php_printf("seen this class before, sending traits by ref, ref=%d", currentObjectClassEntryIndex);
		firstU29 = firstU29 | (currentObjectClassEntryIndex << 2);
	}


	// write the first token that follows the object-byte-marker
	amf_write_int(buf, firstU29);


	// write class name (if needed)
	if (classEntry == zend_standard_class_def) {
		// ANONYMOUS CLASS (i.e., AS3 Object class or PHP stdClass object)
		// for anonymous classes, we always write the class-name which is an empty string, 0x01
//		php_printf("writing empty string byte because this class is anonymous\n");
		amf_write_byte(buf, 0x01); // our class name is the empty string, meaning anonymous Object/stdClass
	} else {
		// If the object is some non-anonymous, named class then we only write the class name if we
		// are sending all the serialized traits. If we aren't sending the traits, the class
		// name is NOT sent because the object's class and traits will be identified by reference
		if (serializeSealedTraits) {
			// this function automatically writes length indicator for the string and then the string itself
//			php_printf("writing class name %s because we must send serialized traits\n", classEntry->name);
			amf_write_string(buf, classEntry->name, classEntry->name_length, flags, htStrings TSRMLS_CC); // some class
		}
	}

	// *** NOW ITERATE THROUGH THE PROPERTIES AND SERIALIZE THEM

	HashTable *properties; // HT of key-value pairs for this object
	zval **property_value; // the value of each property
	const char *prop_name, *class_name; // need these to "unmangle" key/name, see http://www.phpinternalsbook.com/classes_objects/internal_structures_and_implementation.html
	uint prop_len; // length of the unmangled property name. seems to be the correct length (i.e., it lacks null chars that exist in mangled names)
	zend_bool dynamicTraitsHaveBegun = 0;


//    if (Z_OBJ_HT_PP(val)->get_properties == NULL) {
//    	// TODO: what to we do? fatal error?
//    	amf_write_byte(buf, 0x01); // serialize as null value
//    }
//
//    properties = Z_OBJ_HT_PP(val)->get_properties(*val TSRMLS_CC);
//    if (properties == NULL) {
//    	// TODO: what to we do? fatal error?
//    	amf_write_byte(buf, 0x01); // serialize as null value
//    }
	// one way to get properties directly from zval **val. this apparently grabs all sealed and dynamic properties and values
    properties = Z_OBJPROP_PP(val);

    // VERY IMPORTANT: this logic assumes that:
    // a) traits for a given class are always iterated in the exact same order for a given serialization operation
    // b) dynamic traits are always iterated after all the sealed ones have been iterated
    // if either is not true, this code must be fixed

    // first, if we must send serialized traits, serialize them first
    dynamicTraitsHaveBegun = 0;
	if (dynamicTraitsHaveBegun || serializeSealedTraits) {
		zend_hash_internal_pointer_reset_ex(properties, &pos);
		// TODO: can this be made more efficient?
		while (zend_hash_get_current_data_ex(properties, &property_value, &pos) == SUCCESS) {
			if (zend_hash_get_current_key_ex(properties, &key, &key_len, &num_index, 0, &pos) == HASH_KEY_IS_STRING) {
				if (zend_check_property_access(zobj, key, key_len-1 TSRMLS_CC) == SUCCESS) {

					// check to see if this property is a sealed property or dynamic one
					// dynamicTraitsHaveBegun lets us skip this check -- once iteration reaches
					// the dynamic properties, we expect there will be no more sealed ones
					if (zend_hash_find(&classEntry->properties_info, key, key_len, (void **) &propinfo) == SUCCESS) {
						// a sealed trait!
						//php_printf("property %s was found in the classEntry->propertries_info\n", key);
						zend_unmangle_property_name_ex(key, key_len - 1, &class_name, &prop_name, &prop_len);
//						php_printf("writing sealed trait unmangled property name %s, key_len is %d\n", prop_name, prop_len);
						amf_write_string(buf, prop_name, prop_len, flags, htStrings TSRMLS_CC);

					} else {
						// this trait is dynamic! we should be able to skip the rest of the properties
						//php_printf("property %s must be dynamic!", key);
						break; // exit the loop, we have output all sealed properties
					}
				} // if accessible
			} // if key is string
			zend_hash_move_forward_ex(properties, &pos);
		} // while getting data
	} // if we need to serialize traits

	// now output the values!
	dynamicTraitsHaveBegun = 0;
	zend_hash_internal_pointer_reset_ex(properties, &pos);
	while (zend_hash_get_current_data_ex(properties, &property_value, &pos) == SUCCESS) {
		if (zend_hash_get_current_key_ex(properties, &key, &key_len, &num_index, 0, &pos) == HASH_KEY_IS_STRING) {
			if (zend_check_property_access(zobj, key, key_len-1 TSRMLS_CC) == SUCCESS) {

				// check to see if this property is a sealed property or dynamic one
				// dynamicTraitsHaveBegun lets us skip this check -- once iteration reaches
				// the dynamic properties, we expect there will be no more sealed ones
				if (!dynamicTraitsHaveBegun && zend_hash_find(&classEntry->properties_info, key, key_len, (void **) &propinfo) == SUCCESS) {
					// a sealed trait!
					//php_printf("property %s was found in the classEntry->propertries_info\n", key);

				} else {
					// this trait is dynamic!
					//php_printf("property %s must be dynamic!", key);
					dynamicTraitsHaveBegun = 1; // this lets us skip the check to search the default properties
				}

				if (dynamicTraitsHaveBegun) {
					// if we are serializing the class traits OR we have reached the dynamic traits
					// then we need to serialize the property name
					zend_unmangle_property_name_ex(key, key_len - 1, &class_name, &prop_name, &prop_len);
					// don't need to add a reference count because we will not be returning this value
//					php_printf("unmangled property name is %s, key_len is %d\n", prop_name, prop_len);
					amf_write_string(buf, prop_name, prop_len, flags, htStrings TSRMLS_CC);
				}

//				php_printf("serializing value of %s\n", prop_name);
				// finally, serialize the property value itself
				php_amf_encode(buf, *property_value, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);


			} // if property is accessible
		} else {
//			php_printf("Could not retrieve key\n");
		}
		zend_hash_move_forward_ex(properties, &pos);
	}

	// and finally, if the object being serialized is dynamic, we need to serialize an empty string
	// to signify that dynamic properties have been completed
	if (objectIsDynamic) {
		amf_write_byte(buf, 0x01);
	}




	// once any class definition has been encoded, we should then encode the object:
	// encodeHash(ss, HASH_OF(val), opts, sht, oht, tht, 1 TSRMLS_CC);


} // amf_write_object

/*
 * When this function gets called, the AMF3 code for an array has already been written
 * to the buffer.  Our task in this routine is to either serialize a reference number if the
 * array has previously been encountered OR to serialize (possibly recursively) the contents
 * of the array, taking care to distinguish numeric and associative indices
 */
static void amf_write_array(smart_str *buf, zval **val, int flags, HashTable *htComplexObjects, HashTable *htObjectTypeTraits, HashTable *htStrings TSRMLS_DC) /* {{{ */
{
	/* holds the reference number if this array previously serialized */
	ulong *prev_serialized_ref;
	long i, j, numElements, prevReferences;
	HashTable *myHashTable;

	// check to make sure less than PHP_AMF_OBJECT_REFERENCES_MAX before trying
	prevReferences = zend_hash_num_elements(htComplexObjects);
	if (prevReferences > PHP_AMF_OBJECT_REFERENCES_MAX) {
		// set the global last_error value and return
		AMF_G(error_code) = PHP_AMF_ERROR_TOO_MANY_REFERENCES;
//		php_error_docref(
//				NULL TSRMLS_CC,
//				E_ERROR,
//				"number of objects exceeded maximum of %d. serialized as empty string",
//				PHP_AMF_OBJECT_REFERENCES_MAX);
		return;
	}


	// AMF3 maintains a list of all objects and arrays encountered during serialization
	// add another object reference for this array to htComplexObjects
	if (htComplexObjects && amf_add_var_hash(htComplexObjects, *val, (void *) &prev_serialized_ref TSRMLS_CC) == FAILURE) {
		/* if we are unable to add val's hash to htComplexObjects, this means
		 * we have already encountered this array before and wrote it
		 * previously to htComplexObjects and its key in that array is prev_serialized_ref
		 * just write prev_serialized_ref shifted left by one bit, leaving the lowest
		 * bit as zero (to indicate an object reference rather than a value reference)
		 * as an amf-style 29 digit int and return */
		amf_write_int(buf, (((long)*prev_serialized_ref - 1) << 1));
		return;
	}

	/* if amf_add_var_hash returned success then we need to iterate through the members
	 * of val and serialize them
	 * see http://flashmog.net/server_source_docs/current/__filesource/fsource_FlashMOG_FlashMOG_Server_fm_includesclassesSerializer_AMF3.php.html#a540
	 */

	// these vars needed to iterate through the array's HashTable object
	char *key;
	zval **data, **keyData;
	long index;
	uint key_len;
	HashPosition pos;

	ulong contigLength;

	myHashTable = Z_ARRVAL_PP(val);
	numElements = myHashTable ? zend_hash_num_elements(myHashTable) : 0;

	if (numElements > 0) {
		// if the array to be serialized has any elements...


		long hiKey=-1; // highest numeric key in the array
		long prevKey; //previous numeric key

		// reset the array HashTable and loop through all of its keys
		// to find the highest numeric one
		zend_hash_internal_pointer_reset_ex(myHashTable, &pos);
		for (;; zend_hash_move_forward_ex(myHashTable, &pos)) {
			i = zend_hash_get_current_key_ex(myHashTable, &key, &key_len, &index, 0, &pos);
			if (i == HASH_KEY_NON_EXISTANT) {
				// out of members! skip the rest of the loop
				break;
			}

			if (i == HASH_KEY_IS_LONG) {
				// var index holds the numeric key of the next element
				if (index > hiKey) {
					hiKey = index;
				}
			}
		}


//		php_printf("The highest numeric key is %ld\n", hiKey);


		// assume by default that the entire array is contiguous
		// note that contigLength will be (-1 + 1) = 0 if the array has no numeric indices
		contigLength = hiKey+1;
		// loop to determine how much (if any) of the array to be serialized is actually contiguous
		// note we don't have to check hiKey because we already know this key is the highest defined
		for(j=0; j<hiKey; j++){
            if (!zend_hash_index_exists(myHashTable, j)) {
            	// if the index does not exist, we have already reached the
            	// last contiguous numeric index or there are none is none at all
            	contigLength = j;
                break;
            }
        }

//				php_printf("contigLength is %ld\n", contigLength);

		if (contigLength > PHP_AMF_ARRAY_CONTIG_LENGTH_MAX){
			php_error_docref(
					NULL TSRMLS_CC,
					E_ERROR,
					"Array exceeds maximum allowed contiguous length of  %d",
					PHP_AMF_ARRAY_CONTIG_LENGTH_MAX
			);
			return;

		}

	} else {
		// array to be serialized has no elements at all!
		contigLength = 0;
	} // if array to be serialized has any elements


	// WRITE the appropriate contiguous length int
	ulong contigIntToWrite; // this is the value we'll write to the serialization buffer, the 0x01 mask indicates a value rather than a reference
	contigIntToWrite = (contigLength << 1) | 0x01;
	amf_write_int(buf, contigIntToWrite);


	if (numElements > 0) {

		// reset the ht to be serialized and loop thru its members, serializing both
		// associative and NON-contiguous numeric indices as associative keys
		zend_hash_internal_pointer_reset_ex(myHashTable, &pos);
		for (;; zend_hash_move_forward_ex(myHashTable, &pos)) {
			i = zend_hash_get_current_key_ex(myHashTable, &key, &key_len, &index, 0, &pos);
			if (i == HASH_KEY_NON_EXISTANT) {
				// out of members! skip the rest of the loop
				break;
			}

			// the following logic might be necessary for objects to avoid serializing certain things
			/*
			if (incomplete_class && strcmp(key, MAGIC_MEMBER) == 0) {
				continue;
			}
			*/

			if (i == HASH_KEY_IS_LONG) {
				// var index holds the numeric key of the next element
				// if it's outside the contiguous range, serialize it as a string
				if (index < 0 || index >= contigLength) {

					// this is a negative or non-contiguous index so we output it as a name-value pair
					// first, we retrieve the data from the main array we are serializing
					if (zend_hash_index_find(myHashTable, index, (void **) &data) != SUCCESS) {
						php_error_docref(
								NULL TSRMLS_CC,
								E_ERROR,
								"Unable to retrieve data for numeric key %ld while serializing an array",
								index
						);
						return;

					}
					// write the numeric index as a STRING
					// TODO: figure out how to convert a long to a char* and get its length
					char* indexAsString;
					spprintf(&indexAsString, 10, "%ld", index);
					if (!indexAsString){
						php_error_docref(
								NULL TSRMLS_CC,
								E_ERROR,
								"Error encountered trying to convert key %ld to a string",
								index
						);
						return;
					}
					amf_write_string(buf, indexAsString, strlen(indexAsString), flags, htStrings TSRMLS_CC);
					efree(indexAsString);

					// write the value which requires a recursive call
					php_amf_encode(buf, *data, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);


				} else {
					// this numeric index falls within the contiguous range and will be
					// serialized in the next loop
				}


			} else if (i == HASH_KEY_IS_STRING) {
				// var key holds an associative key of length key_len

				// retrieve the associated value from the array ht we are serializing
				if (zend_hash_find(myHashTable, key, key_len, (void **) &data) != SUCCESS) {
					php_error_docref(
							NULL TSRMLS_CC,
							E_ERROR,
							"Unable to retrieve data for associative key %s while serializing an array",
							key
					);
					return;

				}
				// write the key as a STRING
				// NOTE: we are using key_len-1 as length to shave off the null char terminating the string
				// TODO: find out if this the correct behavior or if null chars in keys will be a problem
				amf_write_string(buf, key, key_len-1, flags, htStrings TSRMLS_CC);
				// write the value which requires a recursive call
				php_amf_encode(buf, *data, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);

			} else {
				// A fatal error. This behavior and weird and unexpected and should not be due to any problem with user input.
				php_error_docref(
						NULL TSRMLS_CC,
						E_ERROR,
						"Unrecognized hash key type. i is %ld",
						i);
				return;
			}


		} // loop through all assoc and NON-contiguous numeric indices in the array to be serialized
	} // if array has elements

	// write an empty string to delineate associative keys from contiguous numeric ones
	amf_write_byte(buf, 0x01);

	if (numElements > 0) {

		// WRITE contiguous numeric indices
		for(j=0; j< contigLength; j++) {
			if (zend_hash_index_find(myHashTable, j, (void **) &data) != SUCCESS) {
				php_error_docref(
						NULL TSRMLS_CC,
						E_ERROR,
						"Unable to retrieve data for numeric key %ld while serializing contiguous numeric indices",
						j
				);

			}
			// write the value which requires a recursive call
			php_amf_encode(buf, *data, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);

		}
	}

} /* amf_write_array() */
/* }}} */


PHP_AMF_API void php_amf_encode(smart_str *buf, zval *val, int flags, HashTable *htComplexObjects, HashTable *htObjectTypeTraits, HashTable *htStrings TSRMLS_DC) /* {{{ */
{
	if (AMF_G(error_code) != PHP_AMF_ERROR_NONE) {
		// skip any additional serialization if we've encountered an error
		return;
	}


	switch (Z_TYPE_P(val)) {
	case IS_NULL:
//		php_printf("serializing NULL!\n");
		amf_write_byte(buf, PHP_AMF_AMF3_TYPE_NULL);
		break;

	case IS_BOOL:
		if (Z_BVAL_P(val)) {
			amf_write_byte(buf, PHP_AMF_AMF3_TYPE_TRUE);
		} else {
			amf_write_byte(buf, PHP_AMF_AMF3_TYPE_FALSE);
		}
		break;

	case IS_LONG: {
		long num = Z_LVAL_P(val);

		// this over flow check is also performed
		// in amf_write_int for safety.  we have it
		// here for clarity and to avoid putting a call
		// to the double-writing function inside the int-writing one
		if (num >= PHP_AMF_AMF3_INT_MIN && num <= PHP_AMF_AMF3_INT_MAX) {
			amf_write_byte(buf, PHP_AMF_AMF3_TYPE_INTEGER);
			amf_write_int(buf, num TSRMLS_CC);
		} else {
			amf_write_byte(buf, PHP_AMF_AMF3_TYPE_DOUBLE);
			amf_write_double(buf, num TSRMLS_CC);
		}
		break;
	}

	case IS_DOUBLE: {
		double dbl = Z_DVAL_P(val);
		if (!zend_isinf(dbl) && !zend_isnan(dbl)) {
			amf_write_byte(buf, PHP_AMF_AMF3_TYPE_DOUBLE);
			amf_write_double(buf, dbl TSRMLS_CC);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING,
					"double %.9g is not a valid double, encoded as NULL", dbl);
			amf_write_byte(buf, PHP_AMF_AMF3_TYPE_NULL);
		}
		break;
	}

	case IS_STRING:
		amf_write_byte(buf, PHP_AMF_AMF3_TYPE_STRING);
		amf_write_string(buf, Z_STRVAL_P(val), Z_STRLEN_P(val), flags, htStrings TSRMLS_CC);
		break;

	case IS_ARRAY:
		amf_write_byte(buf, PHP_AMF_AMF3_TYPE_ARRAY);
		amf_write_array(buf, &val, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);
		break;

	case IS_OBJECT:
		amf_write_byte(buf, PHP_AMF_AMF3_TYPE_OBJECT);
		amf_write_object(buf, &val, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);
		break;

	default:
		// set the global last_error value and return
		AMF_G(error_code) = PHP_AMF_ERROR_TYPE_SERIALIZATION_NOT_SUPPORTED;
		return;

		// this is never reached
		break;
	}

	return;
} /* php_amf_encode() */
/* }}} */

// reads one byte (a char) from the buffer, increments the cursor
char amf_read_byte(char *buf, size_t buf_len, size_t *buf_cursor)
{

	if ((unsigned int)*buf_cursor >= (unsigned int)buf_len) {
		// do I need to free up any memory here?
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Attempted to read byte %d when buffer only has length %d", (int)*buf_cursor, (int)buf_len);
	}


	char byte;
	byte = buf[*buf_cursor];
	(*buf_cursor)++;


	return byte;
} // amf_read_byte()

// Reads a variable-length U29 integer from the buffer, increments the cursor
// TODO: need to look at endianness??
// TODO: what about 64-bit ints? these probably OK due to limits of U29 ints
int amf_read_int(char *buf, size_t buf_len, size_t *buf_cursor) {
	char byte;
	int bytes_read=0; // number of bytes read
	int result=0; // the integer result of this function

	byte = amf_read_byte(buf, buf_len, buf_cursor);
	bytes_read=1;

    // keep reading bytes as long as the high bit is 1 and we haven't read 4 yet
    while(((byte & 0x80) != 0) && bytes_read < 4) {
        result <<= 7;
        result |= (byte & 0x7f);

        byte = amf_read_byte(buf, buf_len, buf_cursor);
        bytes_read++;
    }

    if (bytes_read < 4) {
        result <<= 7;
        result |= (byte & 0x7f);

    } else {
        // if the last byte was the 4th,
        // Use all 8 bits
        result <<= 8;
        result |= (byte & 0xff); // wtf is this for? not sure, but without the & 0xff we were getting too many bits set which was resulting in -1 values
//php_printf("result: %x\n", result);
        // Check the 29th bit to see if int should be negative
        if ((result & 0x10000000) != 0) {
//php_printf("29th bit is set\n");
            // if so, extend the sign bit to full-32 bit rep of negative
            result |= 0xe0000000;
        }
    }
//php_printf("final result: %x\n", result);

	return result;

} // amf_read_int

// deprecated because: "Reading from any field of a union other than the most recent one written to results in undefined behavior, according to the international C standard."
double amf_read_doubleOLD(char *buf, size_t buf_len, size_t *buf_cursor) {

	size_t last_byte_read = (*buf_cursor + sizeof(double));

	if ((unsigned long)last_byte_read > (unsigned long)buf_len){
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Attempt to read %ld bytes when buffer contains only %ld bytes", last_byte_read, buf_len);
	}

	union {
		double dval;
		char cval[sizeof(double)];
	} d;

	double result;
	int i;

	if (AMF_G(endianness) == PHP_AMF_ENDIAN_LITTLE) {
		// reverse the bytes?
		for (i=0; i< sizeof(double); i++) {
			d.cval[7-i] = 0xff & buf[*buf_cursor+i];
		}
	} else {
		for (i=0; i< sizeof(double); i++) {
			d.cval[i] = 0xff & buf[*buf_cursor+i];
		}

	}

	result = d.dval;
	*buf_cursor = last_byte_read;
	return result;

} // amf_read_double()

// modified version of function suggested by laserlight
// assumes sizeof(double) is EVEN and also alters the buffer if the system is little-endian
double amf_read_double(char *buf, size_t buf_len, size_t *buf_cursor) {
	size_t last_byte_read = (*buf_cursor + sizeof(double));

	if ((unsigned int)last_byte_read > (unsigned int)buf_len){
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Attempt to read %ld bytes when buffer contains only %ld bytes", (unsigned long)last_byte_read, (unsigned long)buf_len);
	}

    double result;
    size_t i;
    const size_t buf_pos = *buf_cursor;

    if (AMF_G(endianness) == PHP_AMF_ENDIAN_LITTLE) {
        // reverse the bytes
        char temp[sizeof(double)]; // temp var so we don't have to alter the original buffer
        for (i=0; i < sizeof(result); i++) {
            temp[(sizeof(result)-1)-i] = buf[buf_pos + i];
        }
        memcpy(&result, &temp, sizeof(result));
    } else {
    	// copy bytes as-is
        memcpy(&result, buf + buf_pos, sizeof(result));
    }

    *buf_cursor = last_byte_read;

    return result;
} // amf_read_double()
void amf_read_array(char *buf, size_t buf_len, size_t *buf_cursor, zval *return_value, long flags, HashTable *htComplexObjects, HashTable *htObjectTypeTraits, HashTable *htStrings TSRMLS_DC)
{

    int info;

    info = amf_read_int(buf, buf_len, buf_cursor);

    if ((info & 0x01) == 0) {
    	// this array has been passed by reference to a previously encountered object

    	size_t arr_index = (unsigned int)(info >> 1);
    	size_t obj_count = (unsigned int)zend_hash_next_free_element(htComplexObjects);

    	zval *arrz;
    	if (zend_hash_index_find(htComplexObjects, arr_index, (void**)&arrz) != SUCCESS) {
        	php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to locate array sent by reference #%d", (unsigned int)arr_index);
    	}

    	// make sure it's a string
    	if (Z_TYPE_P(arrz) != IS_ARRAY) {
        	php_error_docref(NULL TSRMLS_CC, E_ERROR, "array ZVAL stored at index %d is not a string", (unsigned int)arr_index);
    	}

    	// return the array (or a copy of it?)
    	array_init(return_value);
    	// TODO: RETURN_ZVAL(); ?? WHAT IS DTOR?

    	// no need to increase buf_cursor as that has already been handled by amf_read_int for info above

    	return;
    } else {
    	// value not passed by reference!
    	// first time this object encountered, serialized data follows

    	// length of contiguous numerically indexed elements
    	size_t contig_length = (unsigned int)(info >> 1);

    	// initialize return value as an array
    	array_init(return_value);

    	zval *assoc_key;
    	MAKE_STD_ZVAL(assoc_key);
    	amf_read_string(buf, buf_len, buf_cursor, assoc_key, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);

    	// had to remove this check because it was failing on empty strings for some reason
//    	if (Z_TYPE_P(assoc_key) == IS_STRING) {
//    		 //this seems to run even if the key is the empty string??
//    		// the baffling thing is it returns "String expected, but returned object type is 6"
//    		php_error_docref(NULL TSRMLS_CC, E_ERROR, "String expected, but returned object type is %d", (unsigned int)Z_TYPE_P(assoc_key));
//    	}
    	while (Z_STRLEN_P(assoc_key) > 0) {
    		zval *assoc_val;
    		php_amf_decode(assoc_val, buf, buf_len, buf_cursor, flags, htComplexObjects, htObjectTypeTraits, htStrings  TSRMLS_CC);
    		// add the key-value pair to the return_value array
    		if (add_assoc_string(return_value, Z_STRVAL_P(assoc_key), assoc_val, 1) != SUCCESS) {
    			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Could not add element %s to array\n", Z_STRVAL_P(assoc_key));
    		}

    		amf_read_string(buf, buf_len, buf_cursor, assoc_key, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);
    	}
    	// associative key-value pairs complete

    	// start reading the numeric indices
    	int i;
		for(i=0; i<contig_length; i++) {
			zval *next_value;
			MAKE_STD_ZVAL(next_value);
			php_amf_decode(next_value, buf, buf_len, buf_cursor, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);
			add_next_index_zval(return_value, next_value);
		}
    	return;
    }

}
void amf_read_string(char *buf, size_t buf_len, size_t *buf_cursor, zval *return_value, long flags, HashTable *htComplexObjects, HashTable *htObjectTypeTraits, HashTable *htStrings TSRMLS_DC) {
    char *str;
    size_t str_len;
    int info;
    size_t str_index;

    // first int read contains special info
    info = amf_read_int(buf, buf_len, buf_cursor);

    if ((info & 0x01) == 0) {
    	// unserialize as a reference!
    	// extract string's key from info
    	str_index = (unsigned int)(info >> 1);
    	// fetch zval from the string table and return a copy of the string it contains
    	// check if str_index (a size_t) is larger than the number of elements in the string hash table. if so, return error or return false or something
    	size_t string_count;
    	string_count = (unsigned int)zend_hash_next_free_element(htStrings);

    	if (str_index >= string_count) {
    		php_error_docref(NULL TSRMLS_CC, E_ERROR, "String reference index %d out of bounds in array of size %d", (unsigned int)str_index, (unsigned int)string_count);
    	}

    	// no need to increase buf_cursor as that has already been handled by amf_read_int
    	zval *strz;
    	if (zend_hash_index_find(htStrings, str_index, (void**)&strz) != SUCCESS) {
        	php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to locate string sent by reference #%d", (unsigned int)str_index);
    	}
    	// make sure it's a string
    	// WTF? Z_TYPE_P(strz) is never IS_STRING and Z_STRVAL_P returns messed up values
    	// e.g., "ZVAL stored at index 0 is not a string, type=-30420640"
//    	if (Z_TYPE_P(strz) != IS_STRING) {
//        	php_error_docref(NULL TSRMLS_CC, E_ERROR, "ZVAL stored at index %d is not a string, type=%d\n", (unsigned int)str_index, Z_STRVAL_P(strz));
//    	}
    	// return the string or a copy of it $this->string_table[$str_key];
    	RETURN_STRING(Z_STRVAL_P(strz), 1);

    } else {
    	// unserialize the following bytes as a string

    	// TODO: what about endianness?
    	// extract string length from info by dropping the low bit
    	str_len = (unsigned int)(info >> 1);
    	if (str_len > 0) {
			size_t last_byte_read = (*buf_cursor + str_len);
			if (last_byte_read > buf_len) {
				php_error_docref(NULL TSRMLS_CC, E_ERROR, "Attempt to read %ld bytes when buffer contains only %ld bytes", (unsigned long)last_byte_read, (unsigned long)buf_len);
			}
			// set str to point to the current location in the buffer
			str = (buf + *buf_cursor);
    		// create string zval to store so each string has an associated length when we pop it off as a reference
    		zval *strz;
    		MAKE_STD_ZVAL(strz);
    		ZVAL_STRINGL(strz, str, str_len, 1);
    		if (zend_hash_next_index_insert(htStrings, strz, sizeof(strz), NULL) != SUCCESS) {
    			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Error adding string to strings hash table");
    		}
        	*buf_cursor += str_len;
        	RETURN_STRINGL(str, str_len, 1);
    	} else {
    		//string length is zero
    		RETURN_EMPTY_STRING();
    	}
    }
}

PHP_AMF_API void php_amf_decode(zval *return_value, char *buf, size_t buf_len, size_t *buf_cursor, long flags, HashTable *htComplexObjects, HashTable *htObjectTypeTraits, HashTable *htStrings TSRMLS_DC) /* {{{ */
{

//	php_printf("buf_cursor is %d\n", *buf_cursor);
//	php_printf("buf_len is %d\n", buf_len);

	if (AMF_G(error_code) != PHP_AMF_ERROR_NONE) {
		// skip any additional deserialization and return
		return;
	}


	char byte;
	byte = amf_read_byte(buf, buf_len, buf_cursor);

//	php_printf("Byte read is %x\n", byte);
//	php_printf("buf_cursor is %d\n", *buf_cursor);

	int int_result;
	double double_result;

	switch(byte) {
		case PHP_AMF_AMF3_TYPE_UNDEFINED:
		case PHP_AMF_AMF3_TYPE_NULL:
			RETURN_NULL();

		case PHP_AMF_AMF3_TYPE_FALSE:
			RETURN_BOOL(0); // false

		case PHP_AMF_AMF3_TYPE_TRUE:
			RETURN_BOOL(1); // true

		case PHP_AMF_AMF3_TYPE_INTEGER:
			int_result = amf_read_int(buf, buf_len, buf_cursor);
			RETURN_LONG(int_result);
			break;
		case PHP_AMF_AMF3_TYPE_DOUBLE:
			double_result = amf_read_double(buf, buf_len, buf_cursor);
			RETURN_DOUBLE(double_result);
			break;
		case PHP_AMF_AMF3_TYPE_STRING:
			amf_read_string(buf, buf_len, buf_cursor, return_value, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);
			return; // although return_value is set in amf_read_string by RETURN_STRINGL, we must return here to avoid any further changes to return_value
			break;
		case PHP_AMF_AMF3_TYPE_ARRAY:
			amf_read_array(buf, buf_len, buf_cursor, return_value, flags, htComplexObjects, htObjectTypeTraits, htStrings TSRMLS_CC);
			return; // although return_value is set in amf_read_string by RETURN_STRINGL, we must return here to avoid any further changes to return_value
			break;

		case PHP_AMF_AMF3_TYPE_XML_DOC:
		case PHP_AMF_AMF3_TYPE_DATE:
		case PHP_AMF_AMF3_TYPE_OBJECT:
		case PHP_AMF_AMF3_TYPE_XML:
		case PHP_AMF_AMF3_TYPE_BYTEARRAY:
		default:
			php_error_docref(NULL TSRMLS_CC, E_ERROR, "Byte marker %x is not supported", byte);
	}

	/* some tricks?
	 zval *z;
	 ALLOC_INIT_ZVAL(z);
	 *return_value = *z;
	 RETVAL_NULL();
	 RETVAL_NULL();
	 RETVAL_BOOL(1); // true
	 RETVAL_BOOL(0); // false

	 if (type == IS_LONG) {
	 RETVAL_LONG(p);
	 } else if (type == IS_DOUBLE) {
	 RETVAL_DOUBLE(d);
	 }

	 if (Z_TYPE_P(return_value) != IS_NULL) {
	 }

	 zval_dtor(z);
	 FREE_ZVAL(z);
	 */

	//

	// TOOD: finish this biotch. for now, just return TRUE
	RETVAL_BOOL(1);
	// see http://flashmog.net/server_source_docs/current/__filesource/fsource_FlashMOG_FlashMOG_Server_fm_includesclassesSerializer_AMF3.php.html#a788

	// will probably need to construct HashTables for strings, objects, and traits
	// will probably also need to pass a buffer offset through decode functions so that
	// each can update the buffer cursor as they unserialize each object encountered.


} // php_amf_decode()
/* }}} */

/* {{{ proto string amf_encode(mixed arg[, int options])
 Return a string containing the AMF serialization of arg */
static PHP_FUNCTION(amf_encode) {
	/* params */
	/* the arg to be serialized */
	zval *ptrZvalToEncode;
	 /* encoding flags */
	long flags = 0;

	/* vars */
	/* the buffer to hold the string result */
	smart_str buf = { 0 };
	/* hash tables to track arrays, objects, and strings we encounter,
	 * a concept borrowed from the native PHP serialize() function
	 */
	HashTable htComplexObjects;
	HashTable htObjectTypeTraits;
	HashTable htStrings;


	/* initialize globals */
	AMF_G(error_code) = PHP_AMF_ERROR_NONE;

	/* sanity check on args */
	if ((ZEND_NUM_ARGS() < 1) || (ZEND_NUM_ARGS() > 2)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid argument count");
	}

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|l", &ptrZvalToEncode, &flags) == FAILURE) {
		return;
	}

	zend_hash_init(&(htComplexObjects), 10, NULL, NULL, 0);
	zend_hash_init(&(htObjectTypeTraits), 10, NULL, NULL, 0);
	zend_hash_init(&(htStrings), 10, NULL, NULL, 0);
	php_amf_encode(&buf, ptrZvalToEncode, flags, &htComplexObjects, &htObjectTypeTraits, &htStrings TSRMLS_CC);
	zend_hash_destroy(&(htComplexObjects));
	zend_hash_destroy(&(htObjectTypeTraits));
	zend_hash_destroy(&(htStrings));


	// I borrowed this trick from the json_encode function. NOTE that function also lets you specify whether you want partial output
//	if (JSON_G(error_code) != PHP_JSON_ERROR_NONE && !(options & PHP_JSON_PARTIAL_OUTPUT_ON_ERROR)) {
//		ZVAL_FALSE(return_value);
//	} else {
//		ZVAL_STRINGL(return_value, buf.c, buf.len, 1);
//	}

	if (AMF_G(error_code) != PHP_AMF_ERROR_NONE) {
		ZVAL_FALSE(return_value);
	} else {
		ZVAL_STRINGL(return_value, buf.c, buf.len, 1);
	}

	smart_str_free(&buf);

} /* amf_encode() */
/* }}} */

/* {{{ proto mixed amf_decode(string amf_string [, int options])
 Decodes amf_string from its AMF representation into a PHP data object */
static PHP_FUNCTION(amf_decode) {
	char *buf; // char buffer, contains serialize data string
	size_t buf_len; // the length of the buffer
	long tmp_buf_len;
	long flags = 0;

	/* sanity check on args */
	if ((ZEND_NUM_ARGS() < 1) || (ZEND_NUM_ARGS() > 2)) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Invalid argument count");
	}

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &buf, &tmp_buf_len, &flags) == FAILURE) {
		return;
	}
	// check per laserlight's suggestion
	if (tmp_buf_len < 0) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Input buffer length (%ld) is negative!", tmp_buf_len);
	}
	buf_len = (size_t)tmp_buf_len;


	AMF_G(error_code) = PHP_AMF_ERROR_NONE;

	if (!buf_len) {
		RETURN_NULL();
	}

	HashTable htComplexObjects;
	HashTable htObjectTypeTraits;
	HashTable htStrings;

	size_t buf_cursor=0; //current cursor in the buffer, should be passed by reference
	zend_hash_init(&(htComplexObjects), 10, NULL, NULL, 0);
	zend_hash_init(&(htObjectTypeTraits), 10, NULL, NULL, 0);
	zend_hash_init(&(htStrings), 10, NULL, NULL, 0);
	php_amf_decode(return_value, buf, buf_len, &buf_cursor, flags, &htComplexObjects, &htObjectTypeTraits, &htStrings TSRMLS_CC);
	zend_hash_destroy(&(htComplexObjects));
	zend_hash_destroy(&(htObjectTypeTraits));
	zend_hash_destroy(&(htStrings));

} // amf_decode()
/* }}} */

/* {{{ proto int amf_last_error()
 Returns the error code of the last amf_encode or amf_decode operation. */
static PHP_FUNCTION(amf_last_error) {
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(AMF_G(error_code));
}
/* }}} */


/* {{{ proto string json_last_error_msg()
   Returns the error string of the last json_encode() or json_decode() call. */
static PHP_FUNCTION(amf_last_error_msg)
{
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	switch(AMF_G(error_code)) {
		case PHP_AMF_ERROR_NONE:
			RETURN_STRING("No error", 1);
		case PHP_AMF_ERROR_TOO_MANY_REFERENCES:
			RETURN_STRING("Maximum number of objects exceeded", 1);
		case PHP_AMF_ERROR_TOO_MANY_CLASSES:
			RETURN_STRING("Maxiumum number of distinct classes exceeded", 1);
		case PHP_AMF_ERROR_TOO_MANY_OBJECT_PROPERTIES:
			RETURN_STRING("Maximum number of properties on one object exceeded", 1);
		case PHP_AMF_ERROR_TYPE_SERIALIZATION_NOT_SUPPORTED:
			RETURN_STRING("Data type encountered for which serialization is not supported", 1);
		default:
			RETURN_STRING("Unknown error", 1);
	}

}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
