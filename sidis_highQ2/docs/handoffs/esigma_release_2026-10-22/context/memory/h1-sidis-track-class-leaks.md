---
name: h1-sidis-track-class-leaks
description: "2026-10-20 study — non-primary tracks leak into the H1 SIDIS response body via GTR links (absorbed secondaries, split dups, Tier-2, keeper); proposed truth signal predicate"
metadata:
  node_type: memory
  type: project
  originSessionId: 46e6be01-4fc8-4760-812c-f7bac28c1acd
  modified: 2026-10-20T18:21:38.215Z
---

The user wants every reco track that is not a primary-lineage truth π/K/p removed, including products of detector interactions. Study 2026-10-20 (local `docs/track_truth_classification_study_2026-10-20.md`):
- The det.int. (`IsStr && !IsGtr`) label misses GTR-linked non-primaries in the response body: absorbed secondaries ~1.0–1.1% (p_reco/p_gen≈0.2, 38% charge flip), split-track dups ~0.3%, Tier-2 0.1%, and the K± keeper 0.05% (contains K interactions).
- Total ~1.5% of body, 8.7% at pT 0.15–0.2 GeV.
- Proposed predicate: Tier-0 & GTR & πKp & not e/V0 & charge agree & 0.7<P/GenP<1.5 & angle<10° & dedupe.

**Why:** these tracks are neither signal nor caught by any existing class. The numbers come from two files only (ep0607), so they are not final.

**How to apply:** user decisions pending (window, keeper, Tier-2). Next comes a Condor full census, then a builder variant with miss-row fill for GTR-linked reroutes. Ties into [[h1-sidis-detint-uncertainty-gap]]: the fake-class rate uncertainty must cover ~2.2%.
