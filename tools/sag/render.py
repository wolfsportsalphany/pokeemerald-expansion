import sys, os, glob, struct
import numpy as np
from PIL import Image
def load_pals(tsdir):
    pals={}
    for f in sorted(glob.glob(os.path.join(tsdir,'palettes','*.pal'))):
        nm=os.path.splitext(os.path.basename(f))[0]
        try: num=int(nm)
        except: num=int(nm,16)
        lines=open(f,encoding='utf-8',errors='ignore').read().splitlines()
        cols=[]
        for ln in lines[3:]:
            p=ln.split()
            if len(p)>=3: cols.append((int(p[0]),int(p[1]),int(p[2])))
        if cols: pals[num]=cols
    return pals
def load_tiles(tsdir):
    img=Image.open(os.path.join(tsdir,'tiles.png')).convert('P')
    a=np.array(img); h,w=a.shape; tpr=w//8
    return [a[(i//tpr)*8:(i//tpr)*8+8, (i%tpr)*8:(i%tpr)*8+8] for i in range((w//8)*(h//8))]
def load_mts(path):
    d=open(path,'rb').read()
    return [struct.unpack_from('<8H',d,i*16) for i in range(len(d)//16)]
def render(mapbin,w,h,primdir,secdir,out):
    pals=load_pals(primdir); pals.update(load_pals(secdir))
    pt=load_tiles(primdir); st=load_tiles(secdir)
    pm=load_mts(os.path.join(primdir,'metatiles.bin')); sm=load_mts(os.path.join(secdir,'metatiles.bin'))
    def tile(tid):
        if tid<512: return pt[tid] if tid<len(pt) else np.zeros((8,8),int)
        return st[tid-512] if tid-512<len(st) else np.zeros((8,8),int)
    def mt(mid):
        if mid<512: return pm[mid] if mid<len(pm) else pm[0]
        return sm[mid-512] if mid-512<len(sm) else pm[0]
    cells=struct.unpack('<%dH'%(w*h), open(mapbin,'rb').read()[:w*h*2])
    im=Image.new('RGB',(w*16,h*16),(40,40,40)); px=im.load()
    for cy in range(h):
        for cx in range(w):
            ent=mt(cells[cy*w+cx]&0x3FF)
            for layer in range(2):
                for sub in range(4):
                    v=ent[layer*4+sub]; tid=v&0x3FF; hf=(v>>10)&1; vf=(v>>11)&1; pal=(v>>12)&0xF
                    t=tile(tid)
                    if hf: t=t[:,::-1]
                    if vf: t=t[::-1,:]
                    pc=pals.get(pal) or pals.get(0) or [(0,0,0)]*16
                    ox=cx*16+(sub%2)*8; oy=cy*16+(sub//2)*8
                    for yy in range(8):
                        for xx in range(8):
                            idx=int(t[yy,xx])
                            if layer==1 and idx==0: continue
                            c=pc[idx] if idx<len(pc) else (0,0,0)
                            px[ox+xx,oy+yy]=c
    im=im.resize((im.width*2,im.height*2),Image.NEAREST)  # 2x for clarity
    im.save(out); print("rendered",out,im.size)
if __name__=='__main__':
    render(sys.argv[1],int(sys.argv[2]),int(sys.argv[3]),sys.argv[4],sys.argv[5],sys.argv[6])
