import sys,os,struct,collections
import numpy as np
from PIL import Image, ImageDraw
sys.path.insert(0,'tools/sag'); import render as R
mapbin,W,H,primdir,secdir,out=sys.argv[1],int(sys.argv[2]),int(sys.argv[3]),sys.argv[4],sys.argv[5],sys.argv[6]
pals=R.load_pals(primdir); pals.update(R.load_pals(secdir))
pt=R.load_tiles(primdir); st=R.load_tiles(secdir)
pm=R.load_mts(os.path.join(primdir,'metatiles.bin')); sm=R.load_mts(os.path.join(secdir,'metatiles.bin'))
def tile(tid):
    if tid<512: return pt[tid] if tid<len(pt) else np.zeros((8,8),int)
    return st[tid-512] if tid-512<len(st) else np.zeros((8,8),int)
def mt(mid): return (pm[mid] if mid<len(pm) else pm[0]) if mid<512 else (sm[mid-512] if mid-512<len(sm) else pm[0])
def render_mt(mid):
    im=Image.new('RGB',(16,16),(0,0,0)); px=im.load(); ent=mt(mid)
    for layer in range(2):
        for sub in range(4):
            v=ent[layer*4+sub]; tid=v&0x3FF; hf=(v>>10)&1; vf=(v>>11)&1; pal=(v>>12)&0xF
            t=tile(tid)
            if hf: t=t[:,::-1]
            if vf: t=t[::-1,:]
            pc=pals.get(pal) or pals.get(0) or [(0,0,0)]*16
            ox=(sub%2)*8; oy=(sub//2)*8
            for yy in range(8):
                for xx in range(8):
                    idx=int(t[yy,xx])
                    if layer==1 and idx==0: continue
                    px[ox+xx,oy+yy]=pc[idx] if idx<len(pc) else (0,0,0)
    return im
# also report collision per metatile id (from the map cells where it appears)
cells=struct.unpack('<%dH'%(W*H),open(mapbin,'rb').read()[:W*H*2])
coll={}
for c in cells: coll.setdefault(c&0x3FF,(c>>10)&3)
freq=collections.Counter(c&0x3FF for c in cells)
ids=[i for i,_ in freq.most_common(24)]
cols=6; S=4; cw=16*S+8; ch=16*S+20; rows=(len(ids)+cols-1)//cols
img=Image.new('RGB',(cols*cw, rows*ch),(25,25,25)); d=ImageDraw.Draw(img)
for k,mid in enumerate(ids):
    r,c=divmod(k,cols); x=c*cw+4; y=r*ch+2
    img.paste(render_mt(mid).resize((16*S,16*S),Image.NEAREST),(x,y))
    d.text((x, y+16*S+4), "%03X c%d"%(mid,coll.get(mid,0)), fill=(230,230,230))
img.save(out); print("swatch ->",out, "ids:", " ".join("%03X"%i for i in ids))
