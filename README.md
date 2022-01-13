# Crypto.com Implementation

This application is meant to implement the crypto.com websocket API. It's also a means for me to learn more about C++.

## How to setup

install websocketpp and boost (using yay or paru for example)
```bash
git clone https://github.com/Yimura/Crypto.com-Implementation.git --recursive CryptoCom
cd CryptoCom
```

### building

```bash
premake5 gmake2

# to build
make
# release build
make config=release
```

## State Of Project

Unusable in production, only a few basic websocket endpoints have been implemented
