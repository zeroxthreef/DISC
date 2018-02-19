# DisC
A discord library for C. You can use it with C++, but it doesn't have any nifty objects. DisC is planned to work on all platforms that support OpenSSL (which is basically anything that can connect to the internet).

Also, sorry for the hiatus. I had alot of things get in the way.

## Build instructions
* (optional) `mkdir build`
* (optional) `cd build`
* (use . if doing in tree build) `cmake ..`
* `make`
* `sudo make install`

## Status
Very broken and in early stages. I plan on completing this.
Also, I'll be changing the name from DISC to DisC in the future.

## TODO
* Add all REST functions
* Add ratelimiting
* Proper gateway handling
* Add everything to work with the gateway and use callbacks
* Add better thread support

## Plans at version 1.0
* At version 1.0, all of the REST and gateway functions are implemented.
* Voice connection comes after 1.0
* Expect the api to change drastically up until 1.0

## Thread saftey
currently, DisC is single threaded and doesnt support threading as far as I know. I will add this in the future.

## Libraries Needed
You're gonna need Jansson and OpenSSL (in the future, mbedTLS).

## Want to test your bot?
This is a guild where you can invite your bots and talk to me about features you want to add or bugs that need to be squashed.

[![Discord ](https://discordapp.com/api/guilds/263823960116953088/embed.png?style=banner1)](https://discord.gg/nSa7n8v)

## Author
* 0x3F(zeroxthreef)
