#!/usr/bin/env python3
"""WP-C engine_shape test: parent IBU engine on arbitrary (NB_R+1, NB_G+1) toys. Engine imported read-only."""
import sys, json, hashlib
import numpy as np
ENG_DIR = "/data/dust/group/h1/klesthen/BaryonAsym/H1ChargedSidis_HighQ2/scripts/unfolding/tunfold_official_v4_percharge"
OUT = "/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/restart_2026-10-27/wp-c/engine_shape/engine_shape_result.json"
sys.dont_write_bytecode = True
sys.path.insert(0, ENG_DIR)
import crosscheck_unfold_methods_v4 as eng
md5 = hashlib.md5(open(eng.__file__, "rb").read()).hexdigest()

def toy(nr, ng, rng, flat_mc):
    """M_ext (nr+1, ng+1); rows=reco, cols=gen; last row=miss, last col=fake."""
    truth = np.full(ng, 2500.) if flat_mc else rng.uniform(500., 5000., ng)
    eff = rng.uniform(0.55, 0.65, ng)
    B = np.zeros((nr, ng))
    for j in range(ng):
        c = (j + 0.5) / ng * nr
        i0 = int(np.clip(round(c - 0.5), 1, nr - 3))
        w = rng.dirichlet([5., 5., 2., 2.])   # two children + neighbours both sides
        B[i0, j] += w[0]; B[i0 + 1, j] += w[1]; B[i0 - 1, j] += w[2]; B[i0 + 2, j] += w[3]
        B[:, j] *= eff[j] * truth[j]
    miss = truth * (1 - eff)
    fake = 0.1 / 0.9 * B.sum(axis=1)
    M = np.zeros((nr + 1, ng + 1)); M[:nr, :ng] = B; M[nr, :ng] = miss; M[:nr, ng] = fake
    # purity: fraction of reco-bin content (incl. fakes) from gen parent of that reco bin
    parent = np.minimum((np.arange(nr) * ng) // nr, ng - 1)
    tot = B.sum(axis=1) + fake
    pur = float(np.mean(B[np.arange(nr), parent] / np.maximum(tot, 1e-9)))
    return M, truth, pur

def run(nr, ng, seed, prior_kind):
    rng = np.random.default_rng(seed)
    flat = prior_kind == "flat"
    M, truth, pur = toy(nr, ng, rng, flat)
    # data truth: MC truth (mc case) or MC(flat) reweighted by linear slope +-30%
    t_data = truth * (1 + 0.3 * np.linspace(-1, 1, ng)) if flat else truth
    sig = (M[:nr, :ng] * (t_data / truth)[None, :]).sum(axis=1)
    Dd = sig * (1 + M[:nr, ng] / M[:nr, :ng].sum(axis=1))   # Asimov; data fake fraction per row kept = MC
    bl = eng.derive_response_blocks(M, Dd)
    # engine convention: P = MC gen truth incl. miss (A_ij=B_ij/P_j); k=0 prior = P
    r = eng.ibu_unfold(Dd * (1 - bl["f_i"]), np.zeros(nr), bl["B"], truth, [4, 50])
    res = {f"k{it}": float(np.max(np.abs(N / t_data - 1))) for it, N in zip(r["iters"], r["N_corr"])}
    return dict(shape_ext=[nr + 1, ng + 1], nb_r=nr, nb_g=ng, prior=("flat MC, data slope +-30%" if flat else "MC truth = data truth"),
                purity_reco_row=pur, fake_frac_mean=float(bl["f_i"].mean()), eff_mean=float(bl["A_j"].mean()),
                max_abs_rel_closure=res, all_valid=bool(r["valid"].all()))

cases = {
 "a_mc_prior_61x31": run(60, 30, 1, "mc"),
 "b_flat_prior_61x31": run(60, 30, 2, "flat"),
 "c_mc_prior_45x17": run(44, 16, 3, "mc"),
 "c_flat_prior_45x17": run(44, 16, 4, "flat"),
}
cases["a_mc_prior_61x31"]["pass_1e-3"] = cases["a_mc_prior_61x31"]["max_abs_rel_closure"]["k4"] < 1e-3
cases["c_mc_prior_45x17"]["pass_1e-3"] = cases["c_mc_prior_45x17"]["max_abs_rel_closure"]["k4"] < 1e-3
res = dict(engine_path=eng.__file__, engine_md5=md5,
           orientation="M_ext (NB_R+1, NB_G+1): rows reco, cols gen; last row = miss per gen, last col = fake per reco; corner ignored; prior P = MC gen truth incl. miss (eps=sum_i B_ij/P_j)",
           cases=cases,
           engine_shape_pass=bool(cases["a_mc_prior_61x31"]["pass_1e-3"] and cases["c_mc_prior_45x17"]["pass_1e-3"]))
json.dump(res, open(OUT, "w"), indent=1)
print(json.dumps(res, indent=1))
