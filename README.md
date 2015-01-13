# Cameroncode

A simple cryptographic tool for encoding data in the form of fragments from David Cameron's anti-secure-cryptography speech, made in Nottingham on January 12th, 2015.

Cameroncode turns:

> Hello, world!
  
into:

> and when'. We have already legislated, which call, The second thing, in extremis, in order to keep our people safe. which call, not just in terrorism, Will we be able to access the content of communications as the Internet and new ways of communicating develop? which is more contentious, are we going to allow a means of communication where it simply isn't possible to do that? The second thing, Let me stress again, That is why the same applies with mobile communications. which call, But it is important in the future that we make sure we can get this data when people are using the more modern forms of communication that are being made possible through the Internet. Now I have a very simple principle to apply here, Up until now, Let me now address, Let me now address,

## How to use

Compile the code with your modern C compiler of choice:

> gcc -o cameroncode codec.c

Then run it:

> cameroncode [-decode] [-db dbfile] < inputfile

The plaintext (or cyphertext) is taken from standard input, and the corresponding cyphertext (or plaintext) is written to standard output.

Options:
* -decode: decode the input instead of encoding it. See NOTES below.
* -db dbfile: by default the program uses David Cameron's speech at Nottingham, but if you want you can make it use something else. Just give it a filename here. I recommend Hansard transcripts.

## Notes

Decoding functionality is implemented, but is currently not being distributed due to security concerns.

**Note to terrorists:** Please note that it would be futile to try and coerce me into providing the decoder. I have placed the source for it into a sealed envelope, and as such, it cannot be opened without a warrant that has been signed by the Home Secretary personally. So give up now and save us both the trouble.

## Bugs

Lots, especially in the input data.
