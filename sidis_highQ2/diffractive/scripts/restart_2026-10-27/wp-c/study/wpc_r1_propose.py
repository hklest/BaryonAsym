"""Round-1: G2 gen proxy; D7 and D7+ rules; reco x_IP variants. usage: wpc_r1_propose.py <rule D7|D7plus|start> <frame A|B> <xipcut> <outname>"""
import sys, json, csv, numpy as np
sys.path.insert(0,"/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
from wpc_lib import *
rule,frame,xipcut,outname=sys.argv[1],sys.argv[2],float(sys.argv[3]),sys.argv[4]
T0,E=load(); 
keep=(E["e_xIp"][T0["t_eglob"]]<xipcut)&(E["e_xIp"][T0["t_eglob"]]>0)
T={k:v[keep] for k,v in T0.items()}
w=T["t_w"]; mxr=T["t_mx"]; mxg=T["t_gmx"]; ch=T["t_charge"]
gv=T["t_gv"]&np.isfinite(T["t_xf2"])&np.isfinite(T["t_pt22"])
OBS={"xF":("t_xf"+frame,"t_xf2"),"pt2":("t_pt2"+frame,"t_pt22")}
FINE={"xF":np.array([-1,-.7,-.5,-.4,-.3,-.2,-.15,-.1,-.05,0,.05,.1,.15,.2,.3,.4,.5,.7,1.]),
      "pt2":np.array([0,.05,.1,.15,.2,.3,.4,.5,.75,1.,1.5,2,3,5,10.])}
CAP={"xF":6,"pt2":4}
MX6=np.array([4.,7,10,15,20,30,40])
D7P=(rule=="D7plus")
def halves(e): return np.sort(np.concatenate([e,(e[:-1]+e[1:])/2]))
def offs(el):
    o=[0]
    for e in el: o.append(o[-1]+len(e)-1)
    return o
def metrics(ax,el):
    rc,gc=OBS[ax]; R=[]; 
    el6=[]
    for i in range(3): h=halves(el[i]); el6+= [h]*2
    for c in (1,-1):
        m=ch==c; g=m&gv
        r=pur_stab(mxr[m],T[rc][m],mxg[m],T[gc][m],gv[m],w[m],MX_EDGES,el)
        r2=pur_stab(mxr[g],T[rc][g],mxg[g],T[gc][g],gv[g],w[g],MX_EDGES,el)
        cid,_,_,nc=cell_id(mxr[g],T[rc][g],MX6,el6)
        raw6=np.bincount(cid[cid>=0],minlength=nc)
        o6=offs(el6); o=offs(el); mn=np.zeros(o[-1])
        for i in range(3):
            for j in range(len(el[i])-1):
                vals=[raw6[o6[2*i+hh]+2*j+k] for hh in (0,1) for k in (0,1)]
                mn[o[i]+j]=min(vals)
        R.append((r,r2,mn))
    return R
def sig_local(ax,el,i):
    rc,gc=OBS[ax]; e=el[i]; res=[]
    m0=gv&(mxg>=MX_EDGES[i])&(mxg<MX_EDGES[i+1]+(1e-9 if i==2 else 0))
    for a,b in zip(e[:-1],e[1:]):
        m=m0&(T[gc]>=a)&(T[gc]<b); res.append(wstats(T[rc][m]-T[gc][m],w[m])["core"] if m.sum()>20 else np.nan)
    return np.array(res)
def table(ax,el,i):
    M=metrics(ax,el); o=offs(el); sl=slice(o[i],o[i+1])
    f=lambda g: np.nan_to_num(np.minimum(g(M[0])[sl],g(M[1])[sl]),nan=0.0)
    d=dict(n=f(lambda x:x[1]["w"]["nreco"]),pur=f(lambda x:x[0]["w"]["purity"]),stab=f(lambda x:x[0]["w"]["stab"]),
           pmig=f(lambda x:x[1]["w"]["purity"]),rawmin=f(lambda x:x[2]),
           fake=np.nan_to_num(np.maximum(M[0][0]["w"]["fake"][sl],M[1][0]["w"]["fake"][sl]),nan=1.0),
           nraw=f(lambda x:x[1]["raw"]["nreco"]))
    d["sig"]=sig_local(ax,el,i); d["wd"]=np.diff(el[i]); return d
def bad(d):
    b=[1-d["n"]/50,1-d["pur"]/.35,1-d["stab"]/.35,np.where(np.isfinite(d["sig"]),1-d["wd"]/(2*d["sig"]),0)]
    if D7P: b+= [1-d["pmig"]/.40,1-d["rawmin"]/100]
    return np.maximum(0,np.maximum.reduce(b))
def passes(d):
    ok=(d["n"]>=50)&(d["pur"]>=.35)&(d["stab"]>=.35)&(~(d["wd"]<2*d["sig"]))
    if D7P: ok&=(d["pmig"]>=.40)&(d["fake"]<=.35)&(d["rawmin"]>=100)
    return ok
res={}; tabs={}
for ax in ("xF","pt2"):
    if rule=="start":
        el=[(XF_EDGES if ax=="xF" else PT2_EDGES).copy() for _ in range(3)]
    else:
        el=[FINE[ax].copy() for _ in range(3)]
        for i in range(3):
            while len(el[i])>2:
                b=bad(table(ax,el,i)); cur=b.sum()
                if cur<=1e-9: break
                e=el[i]; cand=set()
                for j in np.nonzero(b>1e-9)[0]:
                    cand|={k for k in (j,j+1) if 0<k<len(e)-1}
                best=None
                for k in sorted(cand):
                    el2=list(el); el2[i]=np.delete(e,k); sc=bad(table(ax,el2,i)).sum()
                    if best is None or sc<best[0]: best=(sc,k)
                if best is None or best[0]>=cur-1e-9: break
                el[i]=np.delete(e,best[1])
            if D7P:
                while len(el[i])-1>CAP[ax]:
                    d=table(ax,el,i); pair=d["n"][:-1]+d["n"][1:]; k=int(np.argmin(pair))+1; el[i]=np.delete(el[i],k)
    res[ax]=[np.round(e,4).tolist() for e in el]
    tabs[ax]=[table(ax,el,i) for i in range(3)]
# CSV
HDR=["charge","mx_bin","axis","bin","lo","hi","n_data","n_sig_pred","n_mc_raw","f_nd","f_reg","f_fake","purity","stability","masked","p_mig","n_mc_raw_minreco","pass_D7plus"]
MXN=["4-10","10-20","20-40"]; rows=[]; npass={}
for ax in ("xF","pt2"):
    el=[np.array(e) for e in res[ax]]; rc,gc=OBS[ax]
    for c in (1,-1):
        m=ch==c; g=m&gv
        r=pur_stab(mxr[m],T[rc][m],mxg[m],T[gc][m],gv[m],w[m],MX_EDGES,el); r2=pur_stab(mxr[g],T[rc][g],mxg[g],T[gc][g],gv[g],w[g],MX_EDGES,el)
        M=metrics(ax,el); idx=0
        for i in range(3):
            d=tabs[ax][i]
            for j in range(len(el[i])-1):
                ok=bool((d["n"][j]>=50)&(d["pur"][j]>=.35)&(d["stab"][j]>=.35)&(d["pmig"][j]>=.40)&(d["fake"][j]<=.35)&(d["rawmin"][j]>=100))
                npass[(c,ax,i,j)]=ok
                rows.append(["h+" if c>0 else "h-",MXN[i],ax,j,el[i][j],el[i][j+1],"",round(r2["w"]["nreco"][idx],2),int(r2["raw"]["nreco"][idx]),"","",round(r["w"]["fake"][idx],4),round(r["w"]["purity"][idx],4),round(r["w"]["stab"][idx],4),"",round(r2["w"]["purity"][idx],4),int(M[0 if c>0 else 1][2][idx]),int(ok)])
                idx+=1
with open(BASE+outname,"w",newline="") as f:
    cw=csv.writer(f); cw.writerow(HDR); cw.writerows(rows)
json.dump(res,open(BASE+outname.replace(".csv",".json"),"w"))
nb=len(rows)//2
ffk=[r[11] for r in rows]; sel=[(ch==c)&gv for c in (1,-1)]
print("%s | rule %s frame %s xip<%.3f | gen-proxy G2"%(outname,rule,frame,xipcut))
print("  selected reco tracks (all) h+ raw %d w %.0f ; gen-valid signal h+ raw %d w %.0f | h- raw %d w %.0f ; gen-valid h- raw %d w %.0f"%((ch>0).sum(),w[ch>0].sum(),(sel[0]).sum(),w[sel[0]].sum(),(ch<0).sum(),w[ch<0].sum(),sel[1].sum(),w[sel[1]].sum()))
print("  bins/charge %d ; f_fake range %.2f-%.2f ; min purity %.3f min stab %.3f ; D7plus-pass bins/charge h+ %d h- %d"%(nb,min(ffk),max(ffk),min(r[12] for r in rows),min(r[13] for r in rows),sum(r[17] for r in rows[:nb]),sum(r[17] for r in rows[nb:])))
print("  edges",json.dumps(res))
