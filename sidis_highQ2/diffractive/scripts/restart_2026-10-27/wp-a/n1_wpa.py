#!/usr/bin/env python3
"""WP-A (restart 2026-10-27) fork of
scripts/t4/build_localization_n1_mytbl_nd4cat_2026-10-20.py (md5 recorded in
data/restart_2026-10-27/wp-a/t4_originals_md5.txt).  With default flags the
counts/hists/dcell_eta/my_tables blocks equal the original's (test
fork_default_identity).  Added flags (child plan wp-a_plan.md, Outline):
  --plug-threshold T   Plug veto ePlug<T (default 7)
  --no-plug-veto       drop the Plug term (remedy 3)
  --split all|control|test   A2 deep-gap control region (even parity) / its
                       complement; applied as a mask on A_pre before every
                       other mask, so closures I1-I4 stay exact on the kept set
  --period-map CSV     run range -> period (make_period_map_wpa.py)
  --noise-table JSON --chain NAME   remedy 1 (A4/A5): each MC event is
                       assigned a represented data period k (composite-scale
                       projection, probabilities L_k/L(P_c)); with
                       probability q_k an event with ePlug<T is noise-hit and
                       its ePlug is replaced by a draw from the period-k
                       control-region excess spectrum above T, so it leaves
                       the veto-passing class in every mask consistently
  --seed S             base seed (per-sample stream = S + crc32(label|chain))
  --write-keys         write sorted (RunNumber<<32|EventNumber) keys of the
                       kept A_pre set to keys_pre.npy
A "wpa" block adds per-period/sub-period counts, control-region statistics
and the removal bookkeeping.

ORIGINAL DOCSTRING FOLLOWS.
Build one DATA or MC localization-diagnostic partial (campaign
localization_diag_2026-10-20, revival work package 1b; CHANGELOG "USER SCOPE
DECISION (REVIVAL)" 2026-10-20).

ND4CAT EXTENSION FORK (2026-10-20, critic condition 1 on the Stage-1
localization verdict): identical to the validated
scripts/t4/build_localization_n1_mytbl_2026-10-20.py EXCEPT the noODS set,
which is extended with the three wp-1a four-catalog RAPGAP31 ND members
{7094,7096,7097} so the stack-R convention is EXACTLY the registered one
for every noODS component (cluster etaMaxClus08 auto-forced; their
etaMaxVtxExcl08 branch is the -999 sentinel — verified on
data/t2_detector/mc_merged_nd4cat_2026-10-20/7094.root — so without this
extension stack R would silently degenerate to NO eta cut).  Scale rows
come from the DATED SIDECAR
docs/mc/composite_scale_table_v3_nd4cat_sidecar_2026-10-20.csv (signed v3
table UNMODIFIED, binding).  Every other convention (masks, D-cells,
weight recipes, MY tables, fail-closed conservation) is byte-identical to
the parent worker, so the per-component veto-fail-rate bookkeeping is
directly comparable with the ten pre-existing components.

Stage-1 localization diagnostics from EXISTING v6dif trees only.  Two
products per partial, with NO unfolding, NO leakage fit, NO ND-template
work, NO mask change, NO reweighting (frozen weight recipes mirrored
exactly from scripts/t4/build_datamc_cutremoved_eta_7p1_2026-10-20.py):

(A) Plan §10.1 N-1 forward-detector distributions (ledger item 7): the
    Stage-B selection stack with exactly ONE forward element's own veto
    removed, so each element's shape is tested without its self-veto:

      N-1 FMD  : A_pre && eta-cut && (ePlug<7) && selFts   [selFmd REMOVED]  -> nFmu12
      N-1 Plug : A_pre && eta-cut && selFmd  && selFts    [ePlug<7 REMOVED] -> ePlug
      N-1 FTS  : A_pre && eta-cut && selFmd  && (ePlug<7) [selFts REMOVED]  -> nFts28

    Two eta-cut stack conventions are emitted per partial (the DF-SYS-02
    convention-fragility requirement, ledger item 8):
      stack "R" (retained/registered DF-SYS-02 convention): vertex branch
        etaMaxVtxExcl08 for DATA and ODS components, cluster branch
        etaMaxClus08 auto-forced for noODS components {6671,6677,7071,7091}
        (nd4cat fork: ... ,7094,7096,7097});
      stack "C" (mode-consistent, §7.1 campaign principle): cluster branch
        etaMaxClus08 AUTHORITATIVE for every component.
    The with-veto Stage-B reference and the no-veto Stage-A reference are
    emitted alongside for contrast.  Weighting conventions (raw /
    exposure1 / signed) are all retained so the assembler can test
    convention stability; the BINDING headline convention is the ruled
    signed v3 table.

(B) Truth-M_Y-resolved cluster-eta tables (ledger item 5), MC only, via the
    mirMY route the §7.1 worker unblocked: per D-cell (D0/D1/D2 plus the
    outside-D class) and per truth-M_Y bin (the §7.1 my-correlation
    MY_EDGES grid), cluster-eta distributions on the shared 121-edge
    [-1,5) grid on TWO bases:
      - "cutremoved": A_pre && selFmd && ePlug<7 && selFts (eta-max cut
        REMOVED; the §7.1 assembled_v3 fit basis);
      - "sbC": the stack-C Stage-B (all cuts incl. cluster eta<3.2; the
        DF-SYS-01 cluster-mode ODS validation basis).
    Plus Stage-A-denominator eta-pass / veto-pass / both-pass counts per
    (D-cell, M_Y bin) -- the per-D-cell extension of the §7.1 worker's
    my_cluster_eta_pass block.  DATA records the block as unavailable
    (no truth M_Y).

Certified-run-list caveat (policy B5): the run list is NOT frozen
(operations sign-off outstanding); subtractive-only; ACCEPT-only.  Every
consumer of these partials inherits that caveat.

Fail-closed in-worker identities (see "conservation"): MY denominator
closure per D-cell; cutremoved/sbC per-cell eta-template closure against
per-cell counts; N-1 partition and subset identities per stack.
"""
import argparse, csv, hashlib, json
from pathlib import Path
import numpy as np
import uproot

NOODS={"6671","6677","7071","7091","7094","7096","7097"}; PDISS={"9155","9157","9158"}; CHARM={"9024","9033"}
D=((150.,300.,.0021,.030),(300.,700.,.0042,.069),(700.,20000.,.0099,1.5))
ETA_EDGES=np.linspace(-1,5,121)                       # shared §7.1 cluster-eta grid
MY_EDGES=np.array([0.,.75,1.,1.1,1.25,1.5,1.6,1.75,2.,2.5,3.,4.,5.,7.5,10.,20.,50.,100.])
NMY=len(MY_EDGES)-1
EDGES={"nFmu12":np.arange(-.5,20.5),"ePlug":np.linspace(0,50,101),"nFts28":np.arange(-.5,20.5)}
# Overflow = finite values beyond the registered grid (not captured by any bin):
# integer-count grids end at 20.5 (bin 20 = [19.5,20.5) holds value 20);
# ePlug grid ends at 50.0 and its last bin INCLUDES exactly 50.0.
OVFL={"nFmu12":(">=",20.5),"ePlug":(">",50.0),"nFts28":(">=",20.5)}
SCHEMA="t4-localization-n1-mytbl-v1"
B5=("certified run list FROZEN 2026-10-21 (B5 operations sign-off docs/t2/b5_operations_signoff_2026-10-21.md); "
    "subtractive-only; ACCEPT-only")
N1_DEF={
 "N1_FMD":{"removed":"selFmd","kept":"A_pre && eta-cut && (ePlug<7) && selFts","observable":"nFmu12"},
 "N1_PLUG":{"removed":"ePlug<7","kept":"A_pre && eta-cut && selFmd && selFts","observable":"ePlug"},
 "N1_FTS":{"removed":"selFts","kept":"A_pre && eta-cut && selFmd && (ePlug<7)","observable":"nFts28"},
}

def accepted(path):
 with open(path,newline="") as f:return {int(r["run"]) for r in csv.DictReader(f) if r["verdict"]=="ACCEPT"}
def normrow(path,c):
 with open(path,newline="") as f: rows=[r for r in csv.DictReader(f) if r["component"]==c]
 if len(rows)!=1: raise SystemExit("FATAL: exactly one normalization row required for "+c)
 r=rows[0]; return float(r["scale_to_data"]),float(r["exposure_factor"])
def g21(q,y,x):
 qi=np.searchsorted([150,200,300,500,700,1000,np.inf],q,side="right")-1
 yi=np.searchsorted([.10,.25,.45,.70],y,side="right")-1
 return qi,yi,np.where(x>0,x**-1.226,0.)

def hist_ov(x,e,m,w,obs):
 """Histogram of finite x over grid e for mask m with weights w + overflow scalar."""
 cmp_,thr=OVFL[obs]
 mm=m&np.isfinite(x)
 h=np.histogram(x[mm],e,weights=w[mm])[0]
 ovm=mm&((x>=thr) if cmp_==">=" else (x>thr))
 ov=float(w[ovm].sum())
 return h.tolist(),ov

ETA_STRATA=((-1.,0.),(0.,1.),(1.,1.5))
def load_period_map(path):
 with open(path,newline="") as f: rows=list(csv.DictReader(f))
 return [(r["period"],r["sub_period"],int(r["run_lo"]),int(r["run_hi"]),float(r["lumi_accept_nb"])) for r in rows]
def assign(run,pm):
 per=np.full(len(run),"",dtype=object); sub=np.full(len(run),"",dtype=object)
 for p,s,lo,hi,_ in pm:
  m=(run>=lo)&(run<=hi); per[m]=p; sub[m]=s
 return per,sub
def wsum(m,w): return {"raw":int(m.sum()),"signed":float(w[m].sum()),"sumw2":float((w[m]**2).sum())}

def main():
 ap=argparse.ArgumentParser()
 ap.add_argument("--kind",choices=("data","mc"),required=True); ap.add_argument("--label",required=True)
 ap.add_argument("--input",required=True); ap.add_argument("--outdir",required=True)
 ap.add_argument("--runlist"); ap.add_argument("--scale-table")
 ap.add_argument("--plug-threshold",type=float,default=7.0); ap.add_argument("--no-plug-veto",action="store_true")
 ap.add_argument("--split",choices=("all","control","test"),default="all")
 ap.add_argument("--period-map",default="/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-a/period_map.csv")
 ap.add_argument("--noise-table"); ap.add_argument("--chain"); ap.add_argument("--seed",type=int,default=20261027)
 ap.add_argument("--write-keys",action="store_true")
 a=ap.parse_args()
 THR=float(a.plug_threshold)
 if a.noise_table and (a.kind!="mc" or not a.chain): raise SystemExit("FATAL: --noise-table needs --kind mc and --chain")
 if a.kind=="data" and not a.runlist: raise SystemExit("FATAL: DATA requires --runlist")
 if a.kind=="mc" and not a.scale_table: raise SystemExit("FATAL: MC requires --scale-table")
 b=["RunNumber","EventNumber","xIp","mx","passRecoEnv","Q2s","Ys","hatVtxZ","etaMaxVtxExcl08","etaMaxClus08",
    "selFmd","ePlug","selFts","nFmu12","nFts28","nPrt"]
 if a.kind=="mc": b+=["weight1","weight2","hatXpomGki","mirMY"]+(["hatMYGen","hatTGki","hatMXGen"] if a.label in PDISS else [])
 f=uproot.open(a.input); d=f["DiffTot"].arrays(b,library="np")
 n=len(d["Q2s"])
 # ---- WP-A: periods, parity, control region (A1/A2) ----
 PMAP=load_period_map(a.period_map); per,sub=assign(np.asarray(d["RunNumber"],np.int64),PMAP)
 EVKEY=(np.asarray(d["RunNumber"],np.int64).astype(np.uint64)<<np.uint64(32))|np.asarray(d["EventNumber"],np.int64).astype(np.uint64)
 uniq=int(np.unique(EVKEY).size)==n
 par_src=np.asarray(d["EventNumber"],np.int64) if uniq else np.arange(n)
 even=(par_src%2)==0
 acc=accepted(a.runlist) if a.kind=="data" else None
 runok=np.isin(d["RunNumber"],np.fromiter(acc,dtype=d["RunNumber"].dtype)) if acc is not None else np.ones(n,bool)
 pre0=runok & d["passRecoEnv"] & (d["Q2s"]>150) & (d["Ys"]>.1) & (d["Ys"]<.9) & (np.abs(d["hatVtxZ"])<35)
 ev=np.asarray(d["etaMaxVtxExcl08"],float); ec=np.asarray(d["etaMaxClus08"],float)
 xip=np.asarray(d["xIp"],float); mxr=np.asarray(d["mx"],float)
 deep=(ec<1.5)|(ec<-900)
 control=pre0&deep&(xip<0.04)&(mxr>3.)&even
 keep={"all":np.ones(n,bool),"control":control,"test":~control}[a.split]
 pre=pre0&keep
 overlap=int(np.intersect1d(EVKEY[pre0&control],EVKEY[pre0&~control]).size)
 if overlap: raise SystemExit("FATAL: control_disjoint violated (%d shared keys)"%overlap)
 if a.kind=="data":
  pp=set(per[pre0].tolist())
  if len(pp)!=1: raise SystemExit("FATAL: DATA file spans periods %s"%sorted(pp))
 etv=(ev<3.2)|(ev<-900); etc=(ec<3.2)|(ec<-900)
 fmd=d["selFmd"].astype(bool); fts=d["selFts"].astype(bool)
 eplug0=np.asarray(d["ePlug"],float)
 # weight recipes mirrored EXACTLY from the §7.1 worker (frozen; NO reweighting)
 w_signed=np.ones(n); w_expo1=np.ones(n); weight_recipe="unit DATA count"
 if a.kind=="mc":
  scale,expo=normrow(a.scale_table,a.label); event=np.asarray(d["weight1"]*d["weight2"],float)
  if a.label=="9012": event[:]=1.
  if a.label in CHARM:
   qi,yi,raw=g21(d["Q2s"],d["Ys"],d["hatXpomGki"]); event[:]=0.
   for q in range(6):
    for y in range(3):
     m=(qi==q)&(yi==y)&np.isfinite(raw)&(raw>0)
     if m.any(): event[m]=raw[m]/raw[m].mean()
   weight_recipe="g21 18-slice"
  elif a.label in PDISS:
   truth=(d["hatMYGen"]<1.6)&(np.abs(d["hatTGki"])<1)&(d["hatXpomGki"]<.03)&(d["hatMXGen"]>2)
   event[truth]*=5.3611; weight_recipe="native w1*w2; x5.3611 truth-in-cut only"
  else: weight_recipe="native w1*w2" if a.label!="9012" else "w_event=1"
  w_expo1=event*scale; w_signed=w_expo1*expo
 ws={"raw":np.ones(n),"exposure1":w_expo1,"signed":w_signed}
 # ---- WP-A remedy 1: noise-hit replacement of ePlug (MC only) ----
 removal={"applied":False}
 eplug=eplug0.copy()
 if a.noise_table:
  import zlib
  T=json.load(open(a.noise_table)); ch=T["chains"][a.chain]; cc=ch["components"][a.label]
  rp=list(cc["represented_periods"]); pr=np.asarray(cc["period_probs"],float)
  q=np.asarray([ch["q"][k] for k in rp],float)
  grid=np.asarray(T["eplug_edges"],float)
  sd=int(a.seed)+zlib.crc32(("%s|%s"%(a.label,a.chain)).encode())
  rng=np.random.default_rng(sd)
  ia=rng.choice(len(rp),size=n,p=pr/pr.sum()); u=rng.random(n)
  cand=np.isfinite(eplug0)&(eplug0<THR)
  hit=cand&(u<q[ia])
  newv=np.full(n,np.nan)
  for j,k in enumerate(rp):
   pdf=np.asarray(ch["excess_pdf"][k],float)          # len = nbins+1 (last = overflow)
   mj=hit&(ia==j)
   if not mj.any(): continue
   if pdf.sum()<=0: raise SystemExit("FATAL: empty excess pdf for period "+k)
   ib=rng.choice(len(pdf),size=int(mj.sum()),p=pdf/pdf.sum())
   lo=np.where(ib<len(grid)-1,grid[np.minimum(ib,len(grid)-2)],75.0)
   hi=np.where(ib<len(grid)-1,grid[np.minimum(ib,len(grid)-2)+1],75.0)
   v=lo+(hi-lo)*rng.random(ib.size)
   if np.any(v<THR): raise SystemExit("FATAL: excess draw below threshold")
   newv[mj]=v
  eplug[hit]=newv[hit]
  rb={}
  for j,k in enumerate(rp):
   den=pre&cand&(ia==j); rb[k]={"q":float(q[j]),"n_candidates_pre":int(den.sum()),"n_hit_pre":int((den&hit).sum()),
     "signed_candidates_pre":float(w_signed[den].sum()),"signed_hit_pre":float(w_signed[den&hit].sum())}
  removal={"applied":True,"chain":a.chain,"noise_table":str(Path(a.noise_table).resolve()),
   "noise_table_sha256":hashlib.sha256(Path(a.noise_table).read_bytes()).hexdigest(),"seed":sd,
   "represented_periods":rp,"period_probs":pr.tolist(),"per_period":rb,"n_hit_total":int(hit.sum()),"n_hit_pre":int((hit&pre).sum())}
 d["ePlug"]=eplug
 plg=np.ones(n,bool) if a.no_plug_veto else (eplug<THR)
 veto=fmd&plg&fts
 # D-cell classification (worker convention)
 x=d["Q2s"]/(4*27.6*920*d["Ys"]); cell=np.full(n,-1,int)
 for i,(ql,qh,xl,xh) in enumerate(D): cell[(d["Q2s"]>=ql)&(d["Q2s"]<qh)&(x>=xl)&(x<xh)]=i
 # ---- stack definitions ----
 etaR = etc if a.label in NOODS else etv     # retained/registered DF-SYS-02 convention
 stacks={"R":pre&etaR,"C":pre&etc}           # C: cluster authoritative for every component
 masks={"A_pre":pre,"cutremoved":pre&veto}   # cutremoved = §7.1 basis (veto stack, NO eta cut)
 for s,base in stacks.items():
  masks["%s_N1_FMD"%s]=base&plg&fts
  masks["%s_N1_PLUG"%s]=base&fmd&fts
  masks["%s_N1_FTS"%s]=base&fmd&plg
  masks["%s_SB"%s]=base&fmd&plg&fts
 counts={}; hists={}
 for mn,m in masks.items():
  counts[mn]={k:float(w[m].sum()) for k,w in ws.items()}; counts[mn]["raw_int"]=int(m.sum())
  counts[mn]["Dcells_raw"]=[int(np.sum(m&(cell==i))) for i in range(3)]
  counts[mn]["Dcells_signed"]=[float(ws["signed"][m&(cell==i)].sum()) for i in range(3)]
  hh={}
  for obs in ("nFmu12","nFts28","ePlug"):
   v=np.asarray(d[obs],float)
   hh[obs]={"edges":EDGES[obs].tolist()}
   for k,w in ws.items():
    h,ov=hist_ov(v,EDGES[obs],m,w,obs); hh[obs][k]=h; hh[obs]["overflow_"+k]=ov
  hists[mn]=hh
 # ---- per-D-cell cluster-eta templates on the two MC-analysis bases (and DATA totals) ----
 bases={"cutremoved":pre&veto,"sbC":stacks["C"]&fmd&plg&fts}
 inr=np.isfinite(ec)&(ec>=ETA_EDGES[0])&(ec<=ETA_EDGES[-1])
 dcell_eta={}
 for bn,bm in bases.items():
  o={"eta_variable":"etaMaxClus08","edges":ETA_EDGES.tolist(),"basis":bn,
     "definition":"A_pre && selFmd && ePlug<7 && selFts (eta-max cut REMOVED; §7.1 assembled_v3 basis)" if bn=="cutremoved"
      else "stack-C Stage-B: A_pre && (etaMaxClus08<3.2|sentinel) && selFmd && ePlug<7 && selFts (DF-SYS-01 cluster-validation basis)"}
  oor={}
  for dc in (0,1,2):
   m=bm&(cell==dc); mo=m&~inr
   o["D%d"%dc]={k:np.histogram(ec[m&inr],ETA_EDGES,weights=ws[k][m&inr])[0].tolist() for k in ws}
   oor["D%d"%dc]={k:float(ws[k][mo].sum()) for k in ws}; oor["D%d"%dc]["raw_int"]=int(mo.sum())
  m=bm&(cell==-1); mo=m&~inr
  o["outside_D_cells"]={k:np.histogram(ec[m&inr],ETA_EDGES,weights=ws[k][m&inr])[0].tolist() for k in ws}
  oor["outside_D_cells"]={k:float(ws[k][mo].sum()) for k in ws}; oor["outside_D_cells"]["raw_int"]=int(mo.sum())
  o["Dcells_out_of_range_eta"]=oor
  dcell_eta[bn]=o
 # ---- truth-M_Y-resolved tables (MC only) ----
 my_tables={"available":False}
 if a.kind=="mc":
  my=np.asarray(d["mirMY"],float)
  good_my=np.isfinite(my)&(my>=0)&(my<=MY_EDGES[-1])
  ib_full=np.digitize(np.where(good_my,my,0.0),MY_EDGES)-1
  ib_full[~good_my]=-1
  my_tables={"available":True,"truth_variable":"mirMY","my_edges_GeV":MY_EDGES.tolist(),
   "bases":{bn:("A_pre && selFmd && ePlug<7 && selFts (eta-max REMOVED)" if bn=="cutremoved" else "stack-C Stage-B (all cuts, cluster eta<3.2)") for bn in bases},
   "eta_pass_definition":"(etaMaxClus08<3.2)|(etaMaxClus08<-900)","veto_definition":"selFmd && ePlug<7 && selFts",
   "denominator":"stage A (parent-DIS proxy), finite mirMY in [0,100]","per_cell":{}}
  for dc in (-1,0,1,2):
   pm=pre&(cell==dc)
   def bcnt(m,k):
    if k=="raw": return np.bincount(ib_full[m],minlength=NMY).astype(float)
    return np.bincount(ib_full[m],weights=ws[k][m],minlength=NMY)
   pc={"den":{}, "eta_pass":{}, "veto_pass":{}, "eta_and_veto_pass":{}}
   for k in ("raw","exposure1","signed"):
    pc["den"][k]=bcnt(pm&good_my,k)
    pc["eta_pass"][k]=bcnt(pm&good_my&etc,k)
    pc["veto_pass"][k]=bcnt(pm&good_my&veto,k)
    pc["eta_and_veto_pass"][k]=bcnt(pm&good_my&etc&veto,k)
   for key in ("den","eta_pass","veto_pass","eta_and_veto_pass"):
    for k in ("raw","exposure1","signed"): pc[key][k]=[float(v) for v in pc[key][k]]
   # per-MY-bin cluster-eta histograms on both bases (in-grid eta only; the
   # out-of-grid/non-finite residual is bookkept explicitly per basis)
   eta_by_my={}
   for bn,bm in bases.items():
    mm=bm&(cell==dc)&good_my&inr
    hist3={}
    for k in ws:
     H,_,_=np.histogram2d(my[mm],ec[mm],bins=(MY_EDGES,ETA_EDGES),weights=ws[k][mm])
     hist3[k]=H
    eta_by_my[bn]={"edges_eta":ETA_EDGES.tolist(),
      "hists":{k:[[float(v) for v in row] for row in hist3[k]] for k in ws}}
   pc["eta_hist_by_my"]=eta_by_my
   oor_my=int((pm&np.isfinite(my)&(my>MY_EDGES[-1])).sum())
   nonfin=int((pm&((~np.isfinite(my))|(my<0))).sum())
   pc["my_gt_100_raw"]=oor_my; pc["my_nonfinite_or_negative_raw"]=nonfin
   # residual bookkeeping + closure inputs per basis
   for bn,bm in bases.items():
    pc["%s_total_raw"%bn]=int((bm&(cell==dc)).sum())
    pc["%s_invalid_my_raw"%bn]=int((bm&(cell==dc)&~good_my).sum())
    pc["%s_oor_eta_raw"%bn]=int((bm&(cell==dc)&good_my&~inr).sum())
    pc["%s_ingrid_raw"%bn]=int((bm&(cell==dc)&good_my&inr).sum())
   my_tables["per_cell"]["D%d"%dc if dc>=0 else "outside_D_cells"]=pc
 # ---------------- fail-closed conservation ----------------
 res={}
 def ck(name,lhs,rhs,tol=1e-6):
  res[name]=abs(float(lhs)-float(rhs))
  if abs(float(lhs)-float(rhs))>tol*max(1.0,abs(float(rhs))): raise SystemExit("FATAL: conservation violated: %s (%s vs %s)"%(name,lhs,rhs))
 # I1: N-1 partition per stack: N1 = SB + N1 & ~own-veto
 for s in stacks:
  for nm,vetoarr in (("N1_FMD",fmd),("N1_PLUG",plg),("N1_FTS",fts)):
   m=masks["%s_%s"%(s,nm)]
   ck("I1_%s_%s"%(s,nm),counts["%s_%s"%(s,nm)]["raw_int"],counts["%s_SB"%s]["raw_int"]+int((m&~vetoarr).sum()))
   # subset: SB restricted to the N-1 mask reproduces SB histograms bin by bin
   for obs in ("nFmu12","nFts28","ePlug"):
    v=np.asarray(d[obs],float)
    h1,_=hist_ov(v,EDGES[obs],masks["%s_SB"%s],np.ones(n),obs)
    h2,_=hist_ov(v,EDGES[obs],m&vetoarr,np.ones(n),obs)
    if not np.allclose(h1,h2,rtol=0,atol=0): raise SystemExit("FATAL: subset identity violated (%s %s %s)"%(s,nm,obs))
 # I2: cutremoved/sbC per-cell closure: template sum + oor == per-cell basis count
 for bn,bm in bases.items():
  for dc in (0,1,2):
   lhs=int(np.asarray(dcell_eta[bn]["D%d"%dc]["raw"],float).sum())+dcell_eta[bn]["Dcells_out_of_range_eta"]["D%d"%dc]["raw_int"]
   ck("I2_%s_D%d"%(bn,dc),lhs,int(np.sum(bm&(cell==dc))))
  lhs=(sum(int(np.asarray(dcell_eta[bn]["D%d"%c]["raw"],float).sum()) for c in (0,1,2))
       +sum(dcell_eta[bn]["Dcells_out_of_range_eta"]["D%d"%c]["raw_int"] for c in (0,1,2))
       +int(np.asarray(dcell_eta[bn]["outside_D_cells"]["raw"],float).sum())
       +dcell_eta[bn]["Dcells_out_of_range_eta"]["outside_D_cells"]["raw_int"])
  ck("I2_%s_total"%bn,lhs,int(bm.sum()))
 # I3: MY closure per cell: den bins + my>100 + nonfinite == pre&cell
 if a.kind=="mc":
  for dc in (-1,0,1,2):
   key="D%d"%dc if dc>=0 else "outside_D_cells"; pc=my_tables["per_cell"][key]
   lhs=int(sum(pc["den"]["raw"]))+pc["my_gt_100_raw"]+pc["my_nonfinite_or_negative_raw"]
   ck("I3_D%s"%(dc if dc>=0 else "out"),lhs,int(np.sum(pre&(cell==dc))))
   for bn in bases:
    lhs=int(sum(np.asarray(pc["eta_hist_by_my"][bn]["hists"]["raw"]).sum(axis=1)))
    ck("I4a_%s_D%s"%(bn,dc if dc>=0 else "out"),lhs,pc["%s_ingrid_raw"%bn])
    ck("I4b_%s_D%s"%(bn,dc if dc>=0 else "out"),
       pc["%s_ingrid_raw"%bn]+pc["%s_oor_eta_raw"%bn]+pc["%s_invalid_my_raw"%bn],pc["%s_total_raw"%bn])
 out={"schema":SCHEMA,"campaign":"localization_diag_2026-10-20","label":a.label,"kind":a.kind,
  "b5_caveat":B5,"run_policy":"ACCEPT only; REVIEW excluded" if a.kind=="data" else "MC",
  "weight_recipe":weight_recipe,"n1_definition":N1_DEF,
  "stack_definitions":{"R":"retained/registered DF-SYS-02 convention: vertex eta for DATA and ODS, cluster eta auto-forced for noODS {6671,6677,7071,7091,7094,7096,7097}",
   "C":"mode-consistent: cluster etaMaxClus08 authoritative for every component"},
  "counts":counts,"hists":hists,"dcell_eta":dcell_eta,"my_tables":my_tables,
  "nPrt_gt0_raw":{"A_pre":int((pre&(np.asarray(d["nPrt"],int)>0)).sum())},
  "conservation":{"checked_in_worker":True,"max_abs_residual":res,
   "note":"I1 N-1 partition+subset; I2 per-cell eta-template closure; I3 MY denominator closure; I4 MY-resolved basis closure"}}
 # ---- WP-A per-period bookkeeping and control-region statistics ----
 gw=w_signed
 groups={"ALL":np.ones(n,bool)}
 for p_,s_,_,_,_ in PMAP:
  groups[p_]=groups.get(p_,np.zeros(n,bool))|(per==p_)
  if s_!=p_: groups[p_+"|"+s_]=(sub==s_)&(per==p_)
 pstats={}
 for g,gm in groups.items():
  o={}
  for mk in ("C_N1_PLUG","C_SB","C_N1_FMD","C_N1_FTS","R_N1_PLUG","R_SB"):
   o[mk]={("D%d"%c if c>=0 else "out"):wsum(masks[mk]&gm&(cell==c),gw) for c in (-1,0,1,2)}
  o["A_pre"]=wsum(pre&gm,gw)
  pstats[g]=o
 cm=pre0&control
 cstats={}
 for g,gm in groups.items():
  m=cm&gm
  o={"ctrl":wsum(m,gw),"ctrl_stack_pass":wsum(m&veto,gw),"ctrl_fmd_fail":wsum(m&~fmd,gw),
     "ctrl_plug_fail":wsum(m&~plg,gw),"ctrl_fts_fail":wsum(m&~fts,gw),"strata":{}}
  for lo_,hi_ in ETA_STRATA:
   sm=m&(ec>=lo_)&(ec<hi_); o["strata"]["[%g,%g)"%(lo_,hi_)]={"ctrl":wsum(sm,gw),"ctrl_stack_pass":wsum(sm&veto,gw)}
  sm=m&(ec<-900); o["strata"]["sentinel"]={"ctrl":wsum(sm,gw),"ctrl_stack_pass":wsum(sm&veto,gw)}
  hh={}
  for obs,mm in (("ePlug",m&fmd&fts),("nFmu12",m&plg&fts),("nFts28",m&fmd&plg)):
   v=np.asarray(d[obs],float); hh[obs]={"edges":EDGES[obs].tolist()}
   for k,w in (("raw",np.ones(n)),("signed",gw),("sumw2",gw**2)):
    h,ov=hist_ov(v,EDGES[obs],mm,w,obs); hh[obs][k]=h; hh[obs]["overflow_"+k]=ov
  o["n1_hists"]=hh
  cstats[g]=o
 # eta_max > 3.5 forward-energy-scale diagnostic (S1 diagnostic; no gate role):
 # ePlug as read from the tree (eplug0, before any remedy-1 replacement)
 e35={}
 for g,gm in groups.items():
  m=pre&gm&np.isfinite(ec)&(ec>3.5); o={}
  for k,w in (("raw",np.ones(n)),("signed",gw),("sumw2",gw**2)):
   h,ov=hist_ov(eplug0,EDGES["ePlug"],m,w,"ePlug"); o[k]=h; o["overflow_"+k]=ov
  fm=m&np.isfinite(eplug0)
  o["sum_w"]=float(gw[fm].sum()); o["sum_w_eplug"]=float((gw[fm]*eplug0[fm]).sum()); o["n_raw"]=int(fm.sum())
  o["sum_eplug_raw"]=float(eplug0[fm].sum()); o["n_ge_T_raw"]=int((fm&(eplug0>=THR)).sum()); o["w_ge_T"]=float(gw[fm&(eplug0>=THR)].sum())
  e35[g]=o
 if a.write_keys:
  Path(a.outdir).mkdir(parents=True,exist_ok=True); np.save(Path(a.outdir)/"keys_pre.npy",np.sort(EVKEY[pre]))
 out["wpa"]={"split":a.split,"plug_threshold":THR,"no_plug_veto":bool(a.no_plug_veto),
  "control_definition":"A_pre && (etaMaxClus08<1.5 || <-900) && xIp<0.04 && mx>3 && parity even",
  "parity_basis":"EventNumber" if uniq else "entry index","run_event_unique":uniq,
  "n_events":n,"n_pre0":int(pre0.sum()),"n_control":int((pre0&control).sum()),"n_test":int((pre0&~control).sum()),
  "n_kept_pre":int(pre.sum()),"control_test_key_overlap":overlap,
  "period_map":{"path":str(Path(a.period_map).resolve()),"sha256":hashlib.sha256(Path(a.period_map).read_bytes()).hexdigest()},
  "unmapped_pre0":int((pre0&(per=="")).sum()),
  "period_stats":pstats,"control_stats":cstats,"eta35_eplug":e35,"removal":removal}
 if a.kind=="mc":
  out["scale_table"]={"path":str(Path(a.scale_table).resolve()),
   "sha256":hashlib.sha256(Path(a.scale_table).read_bytes()).hexdigest(),
   "component":a.label,"scale_to_data":scale,"exposure_factor":expo}
 else:
  out["runlist"]={"path":str(Path(a.runlist).resolve()),"n_accept_runs":len(acc)}
 o=Path(a.outdir);o.mkdir(parents=True,exist_ok=True)
 json.dump(out,open(o/"partial.json","w"),indent=2)
 print(json.dumps({"label":a.label,"kind":a.kind,"counts":{k:v["raw_int"] for k,v in counts.items()},
  "nPrt_gt0":out["nPrt_gt0_raw"]},indent=2))
if __name__=="__main__": main()
