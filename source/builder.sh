g++ -m32 -DCPPHTTPLIB_OPENSSL_SUPPORT -o Microsoft.exe BIT.cpp -lssl -lcrypto -lws2_32 -lcrypt32 -static
ldd Microsoft.exe