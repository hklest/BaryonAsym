---
domain: process
summary: The login-node TeX Live 2020 lacks tcolorbox and revtex4-2; build the note with /opt/texlive/2022 and the paper and letter with TL2025 from CVMFS
affects: [sidis_highQ2/analysis_note/]
verified: 2026-10-27
source: .supervisor/archive/working/plans/2026-10-27-sidis-highq2-finalization.md (P3 and P4 build notes 2026-10-27)
supersedes:
---
Default login-node TL2020 cannot build the note (tcolorbox) or the paper and letter (revtex4-2). Use /opt/texlive/2022 for the note and TL2025 from CVMFS (/cvmfs/sft.cern.ch) for the paper and letter.
