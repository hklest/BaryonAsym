import json, sys, numpy as np
sys.path.insert(0,'/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-a')
import wpa_common as C
B='/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a2/s2_diag/partials'
def L(l): return json.load(open(f'{B}/{l}/even/partial.json'))
E=np.linspace(-1,5,121)
CELLS=['D0','D1','D2','outside_D_cells']
def blk(p,kind):
    w=p['wpa2']
    return w['h4']['incl_eta_c08'] if kind=='h4' else w['dcell_eta_variants'][kind]
def hist(p,kind,key,cells):
    return sum(np.array(blk(p,kind)[c][key],float) for c in cells)
data={l:L(l) for l in C.PERIODS}
SAMPLES={'H4incl_c08':'h4','cr_c08':'cutremoved_c08','cr_c04':'cutremoved_c04'}
SEL={'ALL':CELLS,'D0':['D0'],'D1':['D1'],'D2':['D2']}
mc={l:L(l) for l in ['7071','7091','7094','7096','7097']}
def cocktail(chain,kind,cells):
    tot=0
    for l in C.CHAINS[chain]['nd']:
        rs=C.fac_used(l,chain)/C.table_fac(l)
        tot=tot+rs*hist(mc[l],kind,'signed',cells)
    return tot
ia=np.searchsorted(E,1.0-1e-9); ib=np.searchsorted(E,2.5-1e-9)  # ref window bins [ia,ib)
i25=ib; i36=int(round((3.6+1)/0.05))
def scan(d,m,tol):
    for i in range(i25,120):
        if d[i]>=20 and abs(m[i]/d[i]-1)>tol: return E[i],i
    return 5.0,None
out={}
for s,kind in SAMPLES.items():
  for sel,cells in SEL.items():
    d=sum(hist(data[l],kind,'raw',cells) for l in C.PERIODS)
    for ch in ['7071','7091','nd4cat']:
        m=cocktail(ch,kind,cells)
        n=m[ia:ib].sum()
        if n<=0: continue
        m=m*d[ia:ib].sum()/n
        x10,i10=scan(d,m,0.10); x15,i15=scan(d,m,0.15)
        out[(s,sel,ch)]=(x10,x15)
        print(f'{s:11s} {sel:4s} {ch:7s} X10={x10:.2f} (fail bin D={d[i10] if i10 is not None else "-"}, M/D={m[i10]/d[i10]:.2f})' if i10 is not None else f'{s:11s} {sel:4s} {ch:7s} X10=none<5', f' X15={x15:.2f}', f'dataRef={d[ia:ib].sum():.0f} data[2.5,3.3)={d[i25:86].sum():.0f}')
        if sel=='ALL' and s=='cr_c08' or (sel=='ALL' and s=='H4incl_c08' and ch=='nd4cat'):
            print('   bin  data  MC   D/MC')
            for i in range(i25,i36): print(f'   {E[i]:.2f} {d[i]:6.0f} {m[i]:8.1f} {d[i]/m[i] if m[i]>0 else float("nan"):.3f}')
print('=== PROFILE ALL cr_c08, ref1.0-2.5, D/MC per 0.25 coarse from -1')
for s,kind in [('cr_c08','cutremoved_c08'),('H4','h4')]:
  d=sum(hist(data[l],kind,'raw',CELLS) for l in C.PERIODS)
  for ch in ['7071','7091','nd4cat']:
    m=cocktail(ch,kind,CELLS); m=m*d[ia:ib].sum()/m[ia:ib].sum()
    print(s,ch,' '.join(f'{E[i]:.1f}:{d[i:i+5].sum()/max(m[i:i+5].sum(),1e-9):.2f}' for i in range(0,120,5) if d[i:i+5].sum()>0))
  print(s,'data coarse',' '.join(f'{E[i]:.1f}:{d[i:i+5].sum():.0f}' for i in range(0,120,5)))
