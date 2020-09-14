
cd asn1c

make clean -f converter-example.mk && make -f converter-example.mk

cp -f ./libasncodec.a ../

cd ..

gcc simucucp_message.c E1AP_message.c -std=c99  -fPIC -shared -o libasn1.so libasncodec.a -I asn1c/
