exec(open('wpa3_etamax.py').read().split("out={}")[0])
sig={l:L(l) for l in C.DIFF8}
def signal(kind,cells,labels): return sum(hist(sig[l],kind,'signed',cells) for l in labels)
def sc(d,m,ia_,ib_,tol,start):
    m=m*d[ia_:ib_].sum()/m[ia_:ib_].sum()
    for i in range(start,120):
        if d[i]>=20 and abs(m[i]/d[i]-1)>tol: return round(E[i],2),m
    return 5.0,m
def idx(x): return int(round((x+1)/0.05))
for s,kind in [('H4','h4'),('cr_c08','cutremoved_c08'),('cr_c04','cutremoved_c04')]:
 for sel,cells in [('ALL',CELLS),('D0',['D0']),('D1',['D1']),('D2',['D2'])]:
  d=sum(hist(data[l],kind,'raw',cells) for l in C.PERIODS)
  S6=signal(kind,cells,C.SIGNAL); S8=signal(kind,cells,C.DIFF8)
  for ch in ['7071','7091','nd4cat']:
    nd=cocktail(ch,kind,cells)
    r=[]
    # B: ND only, norm 2.0-3.0 ; start scan 2.0
    xb10,_=sc(d,nd,idx(2.0),idx(3.0),.10,idx(2.0)); xb15,_=sc(d,nd,idx(2.0),idx(3.0),.15,idx(2.0))
    # C: ND+DIFF8 with relative signed weights, norm 1.0-2.5, scan from 2.5
    xc10,mc_=sc(d,nd+S8,idx(1.0),idx(2.5),.10,idx(2.5)); xc15,_=sc(d,nd+S8,idx(1.0),idx(2.5),.15,idx(2.5))
    # D: ND+DIFF8 no rescale at all (absolute): ratio data/MC
    ab=(d[idx(2.5):idx(3.3)].sum(), (nd+S8)[idx(2.5):idx(3.3)].sum())
    print(f'{s:7s}{sel:4s}{ch:7s} B[ND,ref2.0-3.0,scan>=2.0]X10={xb10} X15={xb15} | C[ND+DIFF8,ref1.0-2.5]X10={xc10} X15={xc15} | abs sum[2.5,3.3) data={ab[0]:.0f} (ND+DIFF8)={ab[1]:.0f} ND={nd[idx(2.5):idx(3.3)].sum():.0f}')
    if sel=='ALL' and ch=='nd4cat' and s!='cr_c04':
      m=nd+S8; m=m*d[idx(1.0):idx(2.5)].sum()/m[idx(1.0):idx(2.5)].sum()
      print('   C profile D/MC:',' '.join(f'{E[i]:.2f}:{d[i:i+5].sum()/max(m[i:i+5].sum(),1e-9):.2f}' for i in range(idx(1.5),idx(3.6),5)))
      m=nd*d[idx(2.0):idx(3.0)].sum()/nd[idx(2.0):idx(3.0)].sum()
      print('   B profile D/MC:',' '.join(f'{E[i]:.2f}:{d[i:i+5].sum()/max(m[i:i+5].sum(),1e-9):.2f}' for i in range(idx(1.5),idx(3.6),5)))
