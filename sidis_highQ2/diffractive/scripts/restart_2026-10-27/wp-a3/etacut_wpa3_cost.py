exec(open('wpa3_etamax.py').read().split("out={}")[0])
sig={l:L(l) for l in C.DIFF8}
def idx(x): return int(round((x+1)/0.05))
def H(l,cell,key,basis='cutremoved'):
    return np.array(sig[l]['my_tables']['per_cell'][cell]['eta_hist_by_my'][basis]['hists'][key],float) # [my17, eta120]
GR={'SIGNAL6':C.SIGNAL,'DIFF8':C.DIFF8,'ELASTIC3':['9012','9024','9033'],'PDISS3':['9155','9157','9158']}
cs=[3.3,3.2,3.0,2.8,2.6]
print('label, cell: signed in-grid cutremoved total, raw')
for g,labs in GR.items():
  for cell in ['D0','D1','D2']:
    s=sum(H(l,cell,'signed') for l in labs); r=sum(H(l,cell,'raw') for l in labs)
    tot=s[:,:idx(3.3)].sum(); 
    row=[]
    for c in cs:
        sel=s[:,:idx(c)]; sr=r[:,:idx(c)]
        eff=sel.sum()/tot; fm=sel[6:].sum()/sel.sum()
        row.append(f'c={c}: eff={eff:.3f} My>1.6={fm:.3f} (rawN={sr.sum():.0f})')
    print(g,cell,f'N(<3.3) signed={tot:.1f} raw={r[:,:idx(3.3)].sum():.0f}')
    print('   ',' | '.join(row))
# eff by cell D0..D2 summed
print('DATA cutremoved_c08 even raw, in-grid:')
for cell in ['D0','D1','D2','outside_D_cells']:
  d=sum(np.array(data[l]['wpa2']['dcell_eta_variants']['cutremoved_c08'][cell]['raw'],float) for l in C.PERIODS)
  print(cell,' '.join(f'c={c}:{d[:idx(c)].sum():.0f}(x2={2*d[:idx(c)].sum():.0f})' for c in cs))
# per period D total
for l in C.PERIODS:
  print(l,[ (cell, int(sum(np.array(data[l]['wpa2']['dcell_eta_variants']['cutremoved_c08'][cell]['raw'])[:idx(3.3)]))) for cell in ['D0','D1','D2']])
# DIFF8 label-level MY>1.6 at 3.3 for context
for l in C.DIFF8:
  s=sum(H(l,c,'signed') for c in ['D0','D1','D2']); print(l,'tot',round(s[:,:idx(3.3)].sum(),1),'My>1.6 frac@3.3',round(s[6:,:idx(3.3)].sum()/s[:,:idx(3.3)].sum(),3),'@2.6',round(s[6:,:idx(2.6)].sum()/s[:,:idx(2.6)].sum(),3),'eff2.6',round(s[:,:idx(2.6)].sum()/s[:,:idx(3.3)].sum(),3))
