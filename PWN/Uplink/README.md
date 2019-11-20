# Uplink
CTF Challenge for RITSEC CTF 2019


__Category:__ PWN  
__Difficulty:__ 200  
__Author:__ knif3  
__Description:__  

We have gotten some intelligence about a newly deployed enemy satellite. And it just so happens that we have some old documents laying around regarding the protocol, but they may be incomplete. See what information you can gather ASAP.

You can find the spacecraft at uplink.ritsec.club:8001

Flag format: `RITSEC{}`

__Files:__ `UPLINK_SPEC_V1.pdf`


## Writeup
So we know that we have a remote device, and we know that it uses a custom protocol. We just need to start implementing it.

Based on the spec, we know that each packet will be 4 bytes. Each byte has a distinct signature where the first 2 bits are hard coded.

```
00xx xxxx 01xx xxxx 10xx xxxx 11xx xxxx
```

Further on in the spec we learn that 3rd bit is the ABORT bit (A bit), and the 4th bit is the QUERY bit (Q bit). We can update the spec:

```
00aq xxxx 01xx xxxx 10xx xxxx 11xx xxxx
```

Lets connect to the satellite and see what output we get. Without even sending any data, we can start to see some results:
```
# nc uplink.ritsec.club 8001 | xxd
00000000: 0847 bfff 0847 bfff 2040 80c4 0847 bfff  .G...G.. @...G..
00000010: 2040 80c4 0847 bfff 2040 80c4 0847 bfff   @...G.. @...G..
00000020: 2040 80c4 0847 bfff 2040 80c4 0847 bfff   @...G.. @...G..
```

Lets disect the 1st packet:

```
00001000 01000111 10111111 11111111
00aqxxxx 01xxxxxx 10xxxxxx 11xxxxxx

Unknown bits:
    1000   000111   111111   111111

a = 0
q = 1
```

We can see that it matches the signature so its a valid packet. We have now have 22 bits that are unaccounted for. The spec tells us that the channel is 7 bits long. Looking at the first 7 bits (of the unaccounted for bits), we see the following:

```
1000 000 = 64 = 0o100
```

The spec tells us that a channel of 0o100 is the "Heartbeat" channel. It also mentions that the data in a heartbeat is usually `0x7fff` = `0o77777` = `111 111 111 111 111`.

So it looks like our packet is a "Heartbeat" packet. This makes sense because the it is sent every so often. Breaking down the heartbeat packet we get the following:

```
00001000 01000111 10111111 11111111
00aqcccc 01cccxxx 10xxxxxx 11xxxxxx

A bit = 0
Q bit = 0
Channel = 0o100
Data = 0o77777
```

We now know the full packet specification:
  - A is 1 bit
  - Q is 1 bit
  - Channel is 7 bits
  - Data is 15 bits (or 5 digits in octal)

So now we know how to encode and decode to meet spec.

The spec says the following about memory:

> I/O channels directly map to the segment of memory that holds the value. You can get and set this memory by toggling the query bit when sending data to the remote device. The query bit (q) is a single bit that indicates whether the sender is requesting the memory or trying to set the memory on the given channel.  
> ...  
> The Q bit is set to 1 for queries and set to 0 for write attempts.


Lets try to query some of the different channels and see if we can get the information back.
We need to build a packet to do this.
```
a = 0
q = 1   # 1 if we are querying
channel = 0o001 # Satellite Name channel
data = 0 # Since we dont know what to send when querying, lets just try 0
```

Plugging this into the known protocol format, and we get the following bytes:
```
00010000 01001000 10000000 11000000

\x10 \x48 \x80 \xc0
```

We can write some functions to encode the values and decode the values in python. You can see these function in `solve.py`. We also write a quick `recv` function that will read data in decode them.

So lets send a query to one of the channels and see what we get back
```
# Build the query packet
query_name = encode(0, 1, 0o001, 0)

print("Sending the query for channel 0o001")
sock.send(query_name)

print("Receiving the results for channel 0o001")
packets = recv(sock, 0o001)
```

We slowly start seeing data trickle back once we have sent the query. We get the following feedback

```
a = 0, q = 0, channel = 0o1, data = 0o24444
a = 0, q = 0, channel = 0o1, data = 0o52424
a = 0, q = 0, channel = 0o1, data = 0o64250
a = 0, q = 0, channel = 0o1, data = 0o33664
a = 0, q = 0, channel = 0o1, data = 0o21513
a = 0, q = 0, channel = 0o1, data = 0o10575
a = 0, q = 0, channel = 0o1, data = 0o31336
a = 0, q = 0, channel = 0o1, data = 0o72537
a = 0, q = 0, channel = 0o1, data = 0o33064
a = 0, q = 0, channel = 0o1, data = 0o55314

...
```

Now we have to build the 15bit words back into usable data. This can be achieved with bitshifting, but I found the simplest way is just to make a massive binary string and turn that into bytes.

You can see the implementation of this in `wordsTobytes()` in the python code.

Now all we need to do is query for the the "Name" channel, recreate the data into bytes, and we have the flag.

```
print("Receiving the results from channel 0o001")
packets = recv(sock, 0o001)

words = []
for packet in packets:
    # Get all the 15bit words from the packets, AKA 'data' section
    words += [packet[3]]
data = wordsToBytes(words)
print(data)
```

The output from the channels contains the flag:

```
bytearray(b'RITSEC{Did_You_lik3_that_latency?}             \x00\x80')
```
