import json, numpy as np, sys
O="/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-d/mc_candidates"
L9012=52.273329749  # pb^-1 (CLOSED, sum_w/SIGMT)
res={}
for lab in ["9012","8529","8530","8531"]:
    recs=[json.loads(l) for l in open(f"{O}/hatscan_{lab}.jsonl")]
    s={"files":len(recs),"n":sum(r["n"] for r in recs),"sum_w":sum(r["sum_w"] for r in recs),
       "n_pt2phys":sum(r["n_pt2phys"] for r in recs),"n_hard":sum(r["n_hard"] for r in recs),
       "pt2min":min([r["pt2hat_min_phys"] for r in recs if r["pt2hat_min_phys"] is not None] or [None]),
       "genrad":{}}
    for r in recs:
        for k,v in r["genrad_hist"].items(): s["genrad"][k]=s["genrad"].get(k,0)+v
    for reg in ("R","Rfid"):
        d={k:sum(r[reg][k] for r in recs) for k in ("n","sum_w","n_hard","sum_w_hard","n_soft","sum_w_soft","n_pt2phys")}
        d["q2bins_hard_n"]=[sum(r[reg]["q2bins_hard"][i]["n"] for r in recs) for i in range(4)]
        d["q2bins_hard_w"]=[sum(r[reg]["q2bins_hard"][i]["sum_w"] for r in recs) for i in range(4)]
        d["q2bins_all_n"]=[sum(r[reg]["q2bins_all"][i]["n"] for r in recs) for i in range(4)]
        d["q2bins_all_w"]=[sum(r[reg]["q2bins_all"][i]["sum_w"] for r in recs) for i in range(4)]
        d["mx_hist_hard"]=list(np.sum([r[reg]["mx_hist_hard"] for r in recs],axis=0).astype(int))
        d["mx_hist_all"]=list(np.sum([r[reg]["mx_hist_all"] for r in recs],axis=0).astype(int))
        d["t_hist_all"]=list(np.sum([r[reg]["t_hist_all"] for r in recs],axis=0).astype(int))
        d["xpom_hist_all"]=list(np.sum([r[reg]["xpom_hist_all"] for r in recs],axis=0).astype(int))
        s[reg]=d
    res[lab]=s
    print(f"== {lab}: files={s['files']} n={s['n']} sum_w={s['sum_w']:.1f} pt2phys={s['n_pt2phys']} hard(pt2>9)={s['n_hard']} pt2min={s['pt2min']} genrad={s['genrad']}")
    for reg in ("R","Rfid"):
        d=s[reg]; print(f"   {reg}: n={d['n']} sum_w={d['sum_w']:.1f} n_hard={d['n_hard']} sum_w_hard={d['sum_w_hard']:.1f} n_soft={d['n_soft']} | q2bins all n={d['q2bins_all_n']} hard n={d['q2bins_hard_n']} hard w={[round(x,1) for x in d['q2bins_hard_w']]}")
        print(f"        mx_all={d['mx_hist_all']} mx_hard={d['mx_hist_hard']} t_all={d['t_hist_all']} xpom_all={d['xpom_hist_all']}")
# L_eq(8529) from the hard class in R, Rfid (if 9012 has physical pt2hat)
for reg in ("R","Rfid"):
    nh=res["9012"][reg]["n_hard"]; 
    if nh>0:
        sig=nh/L9012; sw=res["8529"][reg]["sum_w_hard"]
        print(f"L_eq(8529) from {reg}: sigma_hard(9012)={sig:.3f} pb (n={nh}), sum_w_hard(8529)={sw:.1f} -> L_eq={sw/sig:.1f} pb^-1 (+-{sw/sig*np.sqrt(1/nh+ (res['8529'][reg]['n_hard'] and 1/res['8529'][reg]['n_hard']) ):.1f} stat)")
    # all-class comparison for reference
    sig_all=res["9012"][reg]["n"]/L9012; print(f"   reference sigma_all(9012,{reg})={sig_all:.2f} pb; 8529 sum_w/sigma_all={res['8529'][reg]['sum_w']/sig_all:.1f} pb^-1 (not a valid L; class mismatch)")
json.dump(res,open(f"{O}/hatscan_aggregate.json","w"),indent=1,default=lambda o:int(o) if hasattr(o,"__int__") else str(o))
