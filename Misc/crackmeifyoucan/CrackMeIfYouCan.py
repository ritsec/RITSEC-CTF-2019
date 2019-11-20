#!/bin/python3
import crypt, socket, sys
import hashlib,binascii, random
import linecache, threading, traceback

approved_password_lists = {"xato-net-10-million-passwords-1000000.txt":1000000, 'probable-v2-top12000.txt':12645, 'darkweb2017-top10000.txt':100000, 'Common-Credentials/10-million-password-list-top-1000000.txt':1000000, 'Common-Credentials/500-worst-passwords.txt':500}
users = ['root', 'rick', 'elliot', 'evilcorp', 'philswift', 'chaim', 'tiger']
Threads = []

comparison1 = ""
comparison2 = "" 

import signal

def handle_sigterm(*args):
    raise KeyboardInterrupt()

signal.signal(signal.SIGTERM, handle_sigterm)


#solution: sudo hashcat -a 0 -m 1800 this_value_in_a_txt.txt SecLists/Passwords/PasswordFile.txt  --force -o cracked.txt  --potfile-disable
def LinuxHashValue(value):
    hash = crypt.crypt(value)
    string = users[random.randint(0, len(users)-1)]
    return (crypt.crypt(value))

#solution: time sudo hashcat -a 0 -m 1000 This_value_in_a_txt.txt SecLists/Passwords/PasswordFile.txt  --force -o cracked.txt  --potfile-disable
def WindowsHashValue(value):
    hash = hashlib.new('md4', value.encode('utf-16le')).digest()
    return (hex(int(binascii.hexlify(hash), 16))[2:])

def getRandomHash(list_name):
    #random linux/windows hash
    password = ''
    while(password == '' or password == ' '):
        random_int = random.randint(0, approved_password_lists[list_name])
        password = linecache.getline("SecLists/Passwords/" + list_name, random_int).replace("\n", '')
    print("Password:" + password)
    if(random.getrandbits(1) == 0):
        return WindowsHashValue(password), password
    else:
        return LinuxHashValue(password), password

def clientThread(conn, address):
    try:
        passwordlist = random.sample(list(approved_password_lists), 3)
        print(passwordlist)
        quit = False
        print("Started thread")
        conn.settimeout(60 * 5)
        #remove Common-Credentials path before sending to user
        passwordlist_stripped = []
        for i in passwordlist:
            passwordlist_stripped.append(i.replace("Common-Credentials/", ""))
        banner = "Some moron just breached Meme Corp and decided to dump their passwords...  \nIn the meantime, prepare your GPUs, and get Ready... Set.... and go CRACK!\nHowever... We have a theory that the passwords might come from " + passwordlist_stripped[0] + ", " + passwordlist_stripped[1] + " or " + passwordlist_stripped[2]
        #print(banner)
        conn.send((banner + "\n").encode('utf-8'))
        for i in range(0,3):
            hash, password = getRandomHash(passwordlist[random.randint(0,2)])
            print("Providing user " + str(address) + " with hash " + str(hash) + ":" + str(password) + ': attempt:' + str(i))
            conn.send((hash + '\n').encode('utf-8'))
            try:
                data = conn.recv(4096)
                print(data.decode('utf-8').replace("\n", ''))
                comparison1 = str(data.decode('utf-8').replace("\n", ''))
                comparison2 = str(password)
                print("User provided: " + comparison1 + "against password: " + comparison2)
                if(str(data.decode('utf-8').replace("\n", '')) == str(password)):
                    conn.send("Good job.\n".encode('utf-8'))
                else: 
                    conn.send("Oof.\n".encode('utf-8'))
                    conn.shutdown(socket.SHUT_WR)
                    conn.close()
                    quit = True
                    break
            except socket.timeout:
                conn.send("\nThanks For All the Fish\n".encode('utf-8'))
                conn.send(b'\x04')
                conn.shutdown(socket.SHUT_WR)
                conn.close()
                quit = True
                break
        if(quit == False):
            conn.send("NICE JOB.  FLAG:RS{H@$HM31FY0UCAN}\n".encode('utf-8'))
            conn.send(b'\x04')
            conn.shutdown(socket.SHUT_WR)
            conn.close()

    except:
        traceback.print_exc()
        conn.send(b'\x04')
        conn.shutdown(socket.SHUT_WR)
        conn.close()
        


def __init__():    
    try:
        HOST = '0.0.0.0' 
        PORT = 5555
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((HOST,PORT))
        s.listen(1000)
        print("started.  listening.")
        while(True):
            connection, address = s.accept()
            print("Accepted connection from: " + str(address))
            t=threading.Thread(target=clientThread, args=(connection,address))
            t.daemon = True
            t.start()
            Threads.append(t)
    except socket.error as msg:
        print('Bind failed. Error Code : ' + str(msg))
        s.close()
    except KeyboardInterrupt:
        s.close()
        exit()
    except Exception:
        traceback.print_exc()
        s.close()


__init__()