# napi-snap7

[![Build status](https://ci.appveyor.com/api/projects/status/github/fabioriva/napi-snap7?svg=true)](https://ci.appveyor.com/project/fabioriva/napi-snap7/branch/master)

Asyncronous, non-blocking [Snap7](http://snap7.sourceforge.net) bindings for Node.js

```
npm install napi-snap7
```

## About Snap7

Snap7 is an open source, 32/64 bit, multi-platform Ethernet communication suite for interfacing natively with **Siemens S7 PLCs** (See [compatibility](http://snap7.sourceforge.net/snap7_client.html#target_compatibility)).

## Motivation

I have been using the [node-snap7](https://github.com/mathiask88/node-snap7) module since the first releases.  
So this writing was inspired by the great work done previously by the author of the node-snap7 module.  
As a member of the open source community I decided to contribute writing my own addon using the modern [node-addon-api](https://github.com/nodejs/node-addon-api) module.  
node-addon-api is based on [Node-API](https://nodejs.org/api/n-api.html) and supports using different Node-API versions.

## Features

All the I/O functions in the module have synchronous, callback, and promise-based forms, and are accessible using both CommonJS syntax and ES6 Modules (ESM).

Prebuilt binaries shipped with [prebuildify](https://github.com/prebuild/prebuildify).

## Acknowledgements

Davide Nardella for creating Snap7 and Mathias Küsel for creating node-snap7

## License

MIT
