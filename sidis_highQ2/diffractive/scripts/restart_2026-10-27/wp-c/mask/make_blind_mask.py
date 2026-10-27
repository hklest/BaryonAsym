"""WP-C blinded mask (plan D7): masked if n_sig_pred < 50 or purity < 0.35 or stability < 0.35.
Inputs are MC-only per-bin tables (no data histogram read). Writes mask JSON + sha256 sidecar."""
import csv, hashlib, json, sys, datetime
src, out = sys.argv[1], sys.argv[2]
rule = sys.argv[3] if len(sys.argv) > 3 else "D7"  # "D7" or "D7plus" (adds p_mig>=0.40, f_fake<=0.35, raw MC>=100 per reco sub-bin)
rows = list(csv.DictReader(open(src)))
bins = []
for r in rows:
    n, p, s = float(r["n_sig_pred"]), float(r["purity"]), float(r["stability"])
    why = [k for k, bad in (("N<50", n < 50), ("purity<0.35", p < 0.35), ("stability<0.35", s < 0.35)) if bad]
    if rule == "D7plus":
        why += [k for k, bad in (("p_mig<0.40", float(r["p_mig"]) < 0.40), ("f_fake>0.35", float(r["f_fake"]) > 0.35),
                                 ("raw_reco_subbin<100", float(r["n_mc_raw_minreco"]) < 100)) if bad]
    bins.append(dict(charge=r["charge"], mx_bin=r["mx_bin"], axis=r["axis"], bin=int(r["bin"]), lo=float(r["lo"]),
                     hi=float(r["hi"]), n_sig_pred=n, purity=p, stability=s, masked=bool(why), reasons=why,
                     marginal=(not why) and (p < 0.40 or s < 0.40)))
src_sha = hashlib.sha256(open(src, "rb").read()).hexdigest()
doc = dict(rule=("D7: n_sig_pred<50 or purity<0.35 or stability<0.35 (purity denominator includes fakes)" if rule == "D7" else
                 "D7plus: D7 or p_mig<0.40 or f_fake>0.35 or min raw MC per reco sub-bin<100"),
           source_table=src, source_sha256=src_sha, data_read=False,
           created_utc=datetime.datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%SZ"),
           n_bins=len(bins), n_masked=sum(b["masked"] for b in bins), n_marginal=sum(b["marginal"] for b in bins), bins=bins)
blob = json.dumps(doc, indent=1, sort_keys=True).encode()
open(out, "wb").write(blob)
h = hashlib.sha256(blob).hexdigest()
open(out + ".sha256", "w").write(f"{h}  {out}\n")
print(out, h, "masked", doc["n_masked"], "marginal", doc["n_marginal"], "of", len(bins))
