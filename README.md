# amfext v0.10.0
This is the source code for the PECL extension of AMFEXT and its currently in a state of flux and upgrade.

Please be patient.

Note that this version of amfext constitutes a complete rewrite of E. Riffaldi's original code.

Originally written by Emanuele Ruffaldi, this PHP extension will serialize and unserialize
PHP data objects to Adobe/Macromedia's Actionscript Messsage Format 3 format. AMF3 is
supported. The original AMF (AMF0?) format is no longer supported at all.

E. Ruffaldi's original v0.9.2 source code (now years old) is tagged and available via the
0.9.2 tag (and branch). This is the exact code as downloaded from the PECL website 
here: https://pecl.php.net/package/amfext

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
