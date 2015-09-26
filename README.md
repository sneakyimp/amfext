# amfext
The source in this directory contains E. Ruffaldi's original v0.9.2 source code (now years old)
as downloaded from the PECL website at php.net here: https://pecl.php.net/package/amfext
# amfext v0.10.0
The source in this directory is for AMFEXT.

Originally written by Emanuele Ruffaldi, this extension will serialize and unserialize
PHP data objects to Adobe/Macromedia's Actionscript Messsage Format 3 format. AMF3 is
supported. The original AMF (AMF0?) format is no longer supported at all.

For the published AMF3 spec, go here:
http://www.adobe.com/go/amfspec

This extension provides two functions useful for encoding and decoding objects into AMF
format:
amf_encode
amf_decode

This project currently just serializes objects. No deserialization is yet implemented.
Soon! Soon!

#Building
TODO: these instructions need to be tested.

Under Uni*/Linux: obtain the source into a directory. Then as any PHP extension perform:

    phpize
    ./configure --enable-amf
    make clean all
    make install
    
Make sure the extension is mentioned in the php.ini file
    extension=amf.so

#Authors
Emanuele Ruffaldi <emanuele.ruffaldi@gmail.com>
J. Adams
