from ctypes import *
import time
import socket
from threading import Thread
import  struct

import pymysql


def unpack(buf):
    #unpack msg received from c++
    messgeType = struct.unpack('i', buf[0:4])
    buf = buf[4:]
    username = str(buf[:20].decode('gbk')).strip('\x00')
    targetname = str(buf[20:53].decode('gbk')).strip('\x00')
    message = str(buf[53:].decode('gbk')).strip('\x00')
    return (messgeType[0],username,targetname,message)

def combineStr(i,username,targetname,message):
    #format strs that should be sent to c++
    return struct.pack("i20s33s1024s", i, bytes(username.encode('gbk')),
                bytes(targetname.encode('gbk')),
                bytes(message.encode('gbk')))

def register(messgeType, username, targetname, message):
    cursor.execute('select userName from userinfo')
    try:
        #if user does not exist,add into database
        cursor.execute("""insert into userinfo(userName,passwd) values ('%s','%s')"""%(username,targetname))
        con.commit()
        return  'succ'

        #if user existed
    except Exception as msg:
        return 'failed register'
        print(msg)

def login(messgeType, username, targetname, message,newSocket):

    cursor.execute("""select userName,passwd from userinfo where userName='%s'""" % (username,))
    result = cursor.fetchall()
    #acct does not exist
    if len(result) == 0:
        return 'no existing account'
    # error account
    else:
        #check the pwd
        if targetname == result[0][1]:
            if not sockdict.has_key(username):
                sockdict[username]=newSocket
                return friendMess(username)
                #sockdict.update({username,newSocket})
            else:
                # if socket already existed
                return 'already logged in'
        else:
            return 'wrong password'
def friendMess(username):
    #send friens msg after registration
    cursor.execute("""select friend from userfriend where username='%s'""" % (username))
    result = cursor.fetchall()
    returnls = ''
    #combine strs
    for row in result:
        returnls += row[0]
        returnls += '|'
    cursor.execute("""select groupName from groupls where userName='%s'""" % (username))
    result = cursor.fetchall()
    returnls += '!'
    for row in result:
        returnls += row[0]
        returnls += '|'
    return returnls
def groupChat(messgeType, username, targetname, message):
    cursor.execute(
        """select userName from groupls where groupName='%s'""" % (targetname))
    result = cursor.fetchall()
    #message=username+' : '+message
    for row in result:
        sock = sockdict.get(row[0])
        if sock != None:
            username=row[0]
            sock.send(combineStr(messgeType, username, targetname, message))
def sendfriend(messgeType, username, targetname, message):
    #msg to add friend
    cursor.execute("""select userName from userinfo where userName='%s'""" % (targetname,))
    result = cursor.fetchall()
    if len(result) == 0:
        return 'no existing account'
    else:
        cursor.execute(
            #find user,frd
            """select userName,friend from userfriend where userName='%s' and friend='%s'""" % (username, targetname))
        result = cursor.fetchall()
        if len(result) != 0:
            return 'he/she is already ur friend'
        else:
            #if not friend but exist
            cursor.execute("""insert into userfriend(userName,friend) values ('%s','%s')""" % (username, targetname))
            cursor.execute("""insert into userfriend(userName,friend) values ('%s','%s')""" % (targetname, username))
            con.commit()
            sock = sockdict.get(targetname)
            if sock != None:
                #if that one is online, send msg to him
                sock.send(combineStr(7, username, targetname, message))
            return 'succ'
def joinGrp(messgeType, username, targetname, message):
    cursor.execute("""select userName from groupls where groupName='%s'""" % (targetname,))
    result = cursor.fetchall()
    #check if grp exist
    if len(result) == 0:
        return 'no existing group'
    else:
        for row in result:
            if row[0] == username:
                return "you r already in the group"
                break
        else:
            cursor.execute("""insert into groupls(userName,groupName) values ('%s','%s')""" % (username, targetname))
            con.commit()
            return 'succ'
def createGrp(messgeType, username, targetname, message):
    cursor.execute("""select userName from groupls where groupName='%s'""" % (targetname,))
    result = cursor.fetchall()
    #check grp existance
    if len(result) != 0:
        return "group has already existed"
    else:
        #doesn't exist
        cursor.execute("""insert into groupls(userName,groupName) values ('%s','%s')""" % (username, targetname))
        con.commit()
        return  'succ'
def fun(newSocket):
    try:
        while True:
            #depending on msg, send to different block
            buf = newSocket.recv(1068)
            if(len(buf)>0):
                messgeType, username, targetname, message=unpack(buf)
                if(messgeType==1):
                    message=register(messgeType, username, targetname, message)
                    newSocket.send(combineStr(messgeType, username, targetname, message))
                elif(messgeType==2):
                    message=login(messgeType, username, targetname, message,newSocket)
                    newSocket.send(combineStr(messgeType, username, targetname, message))
                elif(messgeType==3):
                    sock = sockdict.get(targetname)
                    if sock != None:#online
                        #message=username+' : '+message
                        sock.send(combineStr(messgeType, username, targetname, message))
                        newSocket.send(combineStr(messgeType, targetname, username, message))
                    else:#not online
                        newSocket.send(combineStr(messgeType, targetname, username, ''))
                        # loginbuf = struct.pack("i20s20s1024s", 3, bytes(username.encode('gbk')),
                        #                        bytes(targetname.encode('gbk')),
                        #                        bytes(message.encode('gbk')))
                        # sock.send(loginbuf)
                elif(messgeType == 4):
                    groupChat(messgeType, username, targetname, message)
                elif(messgeType==5):
                    message=sendfriend(messgeType, username, targetname, message)
                    newSocket.send(combineStr(messgeType, username, targetname, message))
                elif(messgeType==6):
                    message=createGrp(messgeType, username, targetname, message)
                    newSocket.send(combineStr(messgeType, username, targetname, message))
                elif(messgeType==8):
                    message=joinGrp(messgeType, username, targetname, message)
                    newSocket.send(combineStr(messgeType, username, targetname, message))
            else:
                for key,val in sockdict.items():
                    if val==newSocket:
                        del sockdict[key]
                        break
                newSocket.close()
                break
    except Exception as  Ex:#C++ file closed
        for key, val in sockdict.items():
            if val == newSocket:
                del sockdict[key]
                break
        newSocket.close()
        print(Ex)








# messgeType = struct.unpack('i', buf[0:4])
# buf = buf[4:]
# username = str(buf[:20].decode('gbk')).strip('\x00')
# targetname = str(buf[20:40].decode('gbk')).strip('\x00')
# message =  str(buf[40:].decode('gbk')).strip('\x00')

# print messgeType[0]
# print username
# print targetname
# print message

sockdict={}
con = pymysql.connect(
        host='127.0.0.1',
        user='root',
        password='root',
        database='chartboxdb',
        port=3306)

cursor = con.cursor()
server = socket.socket(socket.AF_INET,
                       socket.SOCK_STREAM,
                       socket.IPPROTO_TCP)
try:
    server.bind(('127.0.0.1', 12345))
except Exception as msg:
    print(msg)
server.listen(5)

while True:
    client, adrr = server.accept()
    print 'haha'
    hhh = Thread(target=fun, args=(client,))
    hhh.start()
    # buf=eval(raw_input())
    # username=raw_input()
    # targetname=raw_input()
    # message=raw_input()
    # client.send(combineStr(3,username,targetname,message))

# buf=client.recv(1024)
# buf=str(buf.decode('gbk'))
# print buf


