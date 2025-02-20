# dumbUSD
Dumb rust wrapper around tinyUSDZ

- still wip, can only read polymeshes at the moment.

## How to use
1. clone
2. update submodules:
  `git submodule update --init --recursive`
3. check examples

## Development
- first build will be slow
- after first build/run you can comment out the =X= makred part in build.rs to have faster builds (by avoiding building on tinyUSDZ c++ lib every time time )
