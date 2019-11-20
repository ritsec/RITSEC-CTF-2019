import socket

def encode(abort, query, channel, data):
    """Generate a compliant packet from the abort, query, channel, and data
    """
    # Base sig: 00... 01... 10... 11...
    pkt = [0x00, 0x40, 0x80, 0xc0]
    pkt[0] |= (abort & 1) << 5 # Set the A bit
    pkt[0] |= (query & 1) << 4 # Save the q bit

    # Set the channel
    pkt[0] |= (channel >> 3)
    pkt[1] |= ((channel &  7) << 3)

    # Set the data
    pkt[3] |= data & 63
    pkt[2] |= (data >> 6) & 63
    pkt[1] |= (data >> 12) & 7
    return bytes(pkt)

def decode(data):
    """Get A, Q, C, and D from the packet"""
    # Check the signature and size of the 4 bytes
    if len(data) != 4:
        raise ValueError("Invalid Size")
    if data[0] >> 6 != 0:
        raise ValueError("Invalid Signature")
    if data[1] >> 6 != 1:
        raise ValueError("Invalid Signature")
    if data[2] >> 6 != 2:
        raise ValueError("Invalid Signature")
    if data[3] >> 6 != 3:
        raise ValueError("Invalid Signature")

    # Get the integers from the data
    # We know the first two bits here are 0, so we dont need to mask it
    abort = data[0] >> 5
    query = (data[0] & 16) >> 4

    # Build channel, this is in 2 bytes so it will come in 2 parts
    c1 = data[0] & 15   # xxxxcccc & 00001111 = 0000cccc
    c1 = c1 << 3        # cccc000   - c is only 7 bits
    c2 = data[1] & 56   # xxcccxxx & 00111000 = 00ccc000
    c2 = c2 >> 3        # 00000ccc
    channel = c1 | c2         # 0ccc c000 | 0000 0ccc = 0ccc cccc

    # Build data
    d1 = data[1] & 7    # xxxxxddd & 00000111 = 0000 0ddd
    d1 = d1 << 12       # 0ddd 0000 0000 0000
    d2 = data[2] & 63   # xxdddddd & 00111111 = 00dd dddd
    d2 = d2 << 6        # 0000 dddd dd00 0000
    d3 = data[3] & 63   # 00dd dddd
    data = d1 | d2 | d3

    return (abort, query, channel, data)

def recv(sock, channel=None, timeout=5, debug=True):
    """Get packets from the socket until a timeout"""
    buffer = []
    sock.settimeout(timeout)
    while True:
        try:
            # Get a packet
            raw_packet = sock.recv(4)
            if debug:
                print(".", end="", flush=True)
            packet = decode(raw_packet)
            # If we are specifying a channel, and the packet isnt on that channel, skip it
            if channel is not None and packet[2] != channel:
                continue
            buffer.append(packet)
        except socket.timeout:
            break
    if debug:
        print("")
    return buffer

def wordsToBytes(words, wordsize=15):
    """Convert a list of words into 8bit bytes"""
    binary = ""
    for word in words:
        binary += "{:0{}b}".format(word, wordsize)

    buffer = bytearray()
    while binary:
        # Get 8 bits, padded with 0s
        byte = "{:<08}".format(binary[:8])
        binary = binary[8:]
        buffer.append(int(byte, 2))
    
    return buffer

def main():
    host, port = "uplink.ritsec.club:8001".split(":")
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, int(port)))

    # Build the query packet
    query_name = encode(0, 1, 0o001, 0)

    print("Sending the query for channel 0o001")
    sock.send(query_name)

    print("Receiving the results from channel 0o001")
    packets = recv(sock, 0o001)

    words = []
    for packet in packets:
        print("a = {}, q = {}, channel = 0o{:o}, data = 0o{:o}".format(*packet))
        # Get all the 15bit words from the packets, 'data' section
        words += [packet[3]]
    data = wordsToBytes(words)
    print(data)

main()