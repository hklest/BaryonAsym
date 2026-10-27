import sys, numpy as np, uproot
sys.path.insert(0,"/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/scripts/restart_2026-10-27/wp-c/study")
import xframe_obs as F
D="/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive/data/"
f=uproot.open(D+"t2_detector/mc9012_merged_2026-10-18/9012.root")
d=f["DiffTot"].arrays(["phPx","phPy","phPz","phE","phM2","mx","passRecoEnv","mirXip","mirMX"],library="np")
m=d["passRecoEnv"]&(d["phM2"]>0)&(d["mx"]>0)
pXA=np.stack([d[k][m] for k in ("phPx","phPy","phPz","phE")],-1).astype(float)
res=F.boost_to_rest(pXA,pXA); MA=np.sqrt(F.msq(pXA))
rA=np.abs(res[:,:3]).max(); rAm=np.abs(res[:,3]-MA).max()
print("boost_closure A (stored HFS, n=%d): max|p*|=%.2e  max|E*-M|=%.2e"%(m.sum(),rA,rAm))
rng=np.random.default_rng(1); n=20000
q=np.stack([rng.normal(0,8,n),rng.normal(0,8,n),-rng.uniform(5,25,n),rng.uniform(2,12,n)],-1)
q[:,3]=np.sqrt(q[:,0]**2+q[:,1]**2+q[:,2]**2+rng.uniform(150,800,n)*-1*-1)*0+ (27.6-rng.uniform(5,25,n)) # arbitrary
Ep=np.full(n,920.)
Q2=-F.msq(q); ok=Q2>0; 
M=rng.uniform(4,40,n)
pXB,xi=F.pX_collinear(q,Ep,M); 
okB=np.isfinite(xi)&ok
resB=F.boost_to_rest(pXB,pXB,M)
print("boost_closure B (collinear): max|p*|=%.2e max|E*-M|=%.2e  max|pX^2-M^2|=%.2e"%(np.abs(resB[okB,:3]).max(),np.abs(resB[okB,3]-M[okB]).max(),np.abs(F.msq(pXB)-M**2)[okB].max()))
pX1=F.pX_xip(q,Ep,rng.uniform(0.001,0.03,n)); m1=F.msq(pX1)>0
r1=F.boost_to_rest(pX1,pX1); print("boost_closure G1: max|p*|=%.2e max|E*-M|=%.2e"%(np.abs(r1[m1,:3]).max(),np.abs(r1[m1,3]-np.sqrt(F.msq(pX1))[m1]).max()))
# axis check: in frame B, the Pomeron direction = P direction => xF of a hadron along +P is positive
print("(all closures must be < 1e-6)")
