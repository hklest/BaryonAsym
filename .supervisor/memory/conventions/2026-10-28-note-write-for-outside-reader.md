---
domain: writing
summary: Analysis notes are written for an outside reader; every check or result states what is tested and why, the procedure, and what it means, with every number interpretable
affects: [lambda/analysis_note/, sidis_lowQ2/analysis_note/, sidis_highQ2/analysis_note/]
verified: 2026-10-28
source: user 2026-10-28
supersedes:
---
User on the high-Q2 note, verbatim: "Now the analysis note has too little information. For example, you have a whole paragraph that just says: "In a 1000-toy validation both methods are unbiased. The median signed bias is -0.04% (h+) and -0.03% (h-) for TUnfold, and below 0.001% for the Bayesian unfolding." You have to think about how that would be perceived by an outside reader. They will have no idea what you're talking about, you need to explain the validation procedure and what you're validating. There are many similar examples."
Rule: every check or result states what is tested and why, the procedure, and what the result means (the conclusion drawn). Every number carries its definition, unit, reference and scale so it reads alone.
Relation to the compactness convention (2026-10-27-note-no-old-releases-no-caveats.md): "compact, no repetition" means explaining each procedure once, at its home section, and referring to it elsewhere; it never means dropping the explanation. The two entries stand separately and apply together.
Why: a terse results line is unreadable to anyone who did not run the study.
Rejected: bare result statements with the procedure left implicit; repeating the full procedure in every results section.
