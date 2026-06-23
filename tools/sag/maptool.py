import sys, struct, collections
def load(path,w,h):
    data=bytearray(open(path,'rb').read())
    return data
def gi(data,w,x,y): return struct.unpack_from('<H',data,(y*w+x)*2)[0]
def si(data,w,x,y,v): struct.pack_into('<H',data,(y*w+x)*2,v)
def mid(v): return v&0x03FF
if __name__=='__main__':
    cmd=sys.argv[1]
    if cmd=='show':
        path,w,h=sys.argv[2],int(sys.argv[3]),int(sys.argv[4])
        d=load(path,w,h)
        for y in range(h):
            print(' '.join('%03X'%mid(gi(d,w,x,y)) for x in range(w)))
    elif cmd=='stamp':
        # stamp src rect (sx,sy,bw,bh) onto dest (dx,dy) within same map, full u16 (keeps collision)
        path,w,h,sx,sy,bw,bh,dx,dy=[sys.argv[2]]+[int(a) for a in sys.argv[3:11]]
        d=load(path,w,h)
        block=[[gi(d,w,sx+i,sy+j) for i in range(bw)] for j in range(bh)]
        for j in range(bh):
            for i in range(bw):
                si(d,w,dx+i,dy+j,block[j][i])
        open(path,'wb').write(d)
        print("stamped %dx%d from (%d,%d) to (%d,%d)"%(bw,bh,sx,sy,dx,dy))
