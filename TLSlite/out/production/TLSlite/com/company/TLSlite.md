# TLS Lite

## Nonce Generation

-[x] generate 32 byte nonce


## hadshake Items
-[x] C&S certificates
-[x] DH public key
-[x] signed DH public key

### handshake process
-[x] nonce picked and stored at client
-[x] client sends nonce
-[x] server receives nonce (receiveNonce()) and stores
-[x] server & client calculates DHPK, and Signs(DHPK);
-[x] server sends three items
-[x] client sends three items
-[x] server & client verify
-[x] client and server store shared secret DH
-[x] client & server calculate 6 keys
-[ ] server MAC(all handshake messages so far)
-[ ] client receives
-[ ] client MAC(all THOSE messages);

### steps
-[x] get the certificate?
-[x] set the DH public key
-[x] set the signed DH public key
-[x] compute shared secret
-[ ] derive the 6 keys


## messages
-[ ] compute HMAC of the message using MAC key
-[ ] use cypher to encrypt the message concatenated with the MAC
-[ ] send/receive the resulting byte array using object stream classes


## Testing
-[x] two programs, client and server
-[ ] open connection (handshake)
-[ ] send large file (MBs in size);
-[ ] client should respond when it has the message


## Error resolution
