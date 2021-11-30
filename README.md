# Crypto.com Implementation

This application is meant to implement the crypto.com websocket API. It's also a means for me to learn more about C++.

## How to setup

```bash
git clone https://github.com/Yimura/Crypto.com-Implementation.git --recursive CryptoCom
cd CryptoCom
```

### Visual Studio 2019 or higher

```bash
premake5 vs2019
```

### Other environments

```bash
premake5 gmake2

# to build
make
# release build
make config=release
```

## State Of Project

Unusable in production, only the authentication and reponse to heartbeat have been implemented.
