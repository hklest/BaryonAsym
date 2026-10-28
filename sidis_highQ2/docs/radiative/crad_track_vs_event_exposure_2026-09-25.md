# Gen-only C_rad track-versus-event exposure diagnostic — 2026-09-25

## Scope and status

Read-only diagnostic. `C_rad` was not applied and central products were not modified.

The frozen run3 primitive table was recomputed first.  The as-shipped integrated values reproduce exactly to numerical precision: h+ `0.6770210393632436`, h- `0.8509564022391624`.

## Bounded three-file multiplicity measurement

Each group uses three lexicographically first files from its frozen run3 source pool.  Event weight is `weight1*weight2`. `m_all=sumw(ChargedGenRC all entries)/sumw(unique RunNumber,EventNumber)` and `m_selected=sumw(entries passing the strict run3 truth predicate and 245-bin membership)/sumw(unique events)`.  Thus the identity `L_track=L_event*m` closes algebraically for every bounded group.

| group | entries | unique events | m_all (file range) | m_selected (file range) |
|---|---:|---:|---:|---:|
| RAD e+ | 9,089,294 | 290,000 | 25.032108 (24.938825–25.130012) | 1.261312 (1.247061–1.271898) |
| RAD e- | 9,000,159 | 290,000 | 24.805745 (24.673815–24.907710) | 1.287847 (1.278148–1.293770) |
| OLD NONRAD e+ | 10,352,182 | 441,131 | 19.053918 (18.181629–20.339766) | 3.144209 (3.002787–3.343043) |
| OLD NONRAD e- | 10,346,341 | 367,570 | 22.936069 (21.415793–25.298187) | 3.814000 (3.535819–4.222173) |
| NEW NONRAD e+ | 669,567 | 29,994 | 18.112277 (17.955611–18.232469) | 2.957026 (2.929663–2.979644) |
| NEW NONRAD e- | 666,306 | 29,994 | 18.038165 (17.931284–18.180478) | 2.988093 (2.958407–3.016386) |

The selected converter is `m_selected`, because the frozen per-bin numerators are selected `ChargedGenRC` track sums.  `m_all` and `m_selected` differ materially in both samples.

## Exposure variants

The producer currently uses track-level NONRAD exposures and event-level RAD exposures.  The table supplies the corresponding bounded-estimator conversion variants.  `L_track` is the current producer exposure for NONRAD and `L_event*m` for RAD; `L_event` is `L_track/m` for NONRAD and the exact oomclumi value for RAD. Units are nb^-1.

| group | current producer | L_event (m_all) | L_track (m_all) | m_all | L_event (m_selected) | L_track (m_selected) | m_selected |
|---|---|---:|---:|---:|---:|---:|---:|
| OLD NONRAD e+ | track | 678,081.833 | 12,920,115.855 | 19.053918 | 4,109,178.363 | 12,920,115.855 | 3.144209 |
| OLD NONRAD e- | track | 559,885.497 | 12,841,572.442 | 22.936069 | 3,366,956.906 | 12,841,572.442 | 3.814000 |
| NEW NONRAD e+ | track | 94,818.384 | 1,717,376.821 | 18.112277 | 580,778.389 | 1,717,376.821 | 2.957026 |
| NEW NONRAD e- | track | 94,228.143 | 1,699,702.771 | 18.038165 | 568,825.310 | 1,699,702.771 | 2.988093 |
| RAD e+ | event | 16,206,393.000 | 405,680,185.393 | 25.032108 | 16,206,393.000 | 20,441,312.555 | 1.261312 |
| RAD e- | event | 13,534,017.000 | 335,721,368.111 | 24.805745 | 13,534,017.000 | 17,429,748.155 | 1.287847 |

## Integrated recomputation

| convention | h+ | h- | h+/h- | both 1.05–1.12 |
|---|---:|---:|---:|---|
| (a) current mixed | 0.677021039 | 0.850956402 | 0.795600148 | no |
| (b) both track, m_all | 16.877086184 | 21.212825400 | 0.795607651 | no |
| (c) both event, m_all | 13.843602439 | 17.399406057 | 0.795636494 | no |
| (d) both track, m_selected | 0.862028014 | 1.083517422 | 0.795582975 | no |
| (e) both event, m_selected | 2.291056212 | 2.879519354 | 0.795638414 | no |

The closest single convention is (d), but only h- is in range. No tested convention places both charges in range. The residual correction charge asymmetry is `h+/h-=0.795583`, equivalently `h-/h+=1.256940` (25.694%); it tracks the supplied RAD/NONRAD charge-proxy ratio.

## v1 anchor

The v1 values are `L_NONRAD=3,560,233` and `L_RAD=4,108,957` nb^-1, with C_rad h+/h- `1.055329789/1.055413655`. The stated old e+ relation gives `12,920,115.85/3.7166=3,476,000` nb^-1, 2.37% below v1 NONRAD. This supports v1 as event-level on both sides and identifies the current mixed construction as the regression lead.

Current RAD e+ `16,206,393/4,108,957=3.944`; it is not equal to the bounded RAD `m_all=25.032` or `m_selected=1.261`. Therefore that factor is not a pure track-versus-event conversion under either measured definition; it is a coverage difference (or additional luminosity-construction difference).

## Gates

This diagnostic remains provisional: three-file multiplicities are bounded estimators; publication-grade full-pool selected-track exposure provenance, OLD/NEW bin-local compatibility, sparse policy, and radiative-method approval remain open.
