---
name: feedback-smoke-before-production
description: "Before proposing a large Condor production, run a broad smoke matrix (every period, sample, leg) through the real production wrapper and project storage"
metadata:
  node_type: memory
  type: feedback
  originSessionId: 17159089-dac2-4382-8a78-a99c3532d15b
  modified: 2026-10-21T20:40:07.906Z
---

Before asking to launch a big NAF Condor production, smoke-test every input class: each data period, each MC request block, gen-only, backgrounds, and every systematic leg. Run them through the actual production wrapper, and give measured storage/CPU projections.

**Why:** the user challenged a Wave A request backed by only a 3-job smoke ("It's a lot of compute (and storage space)"). The broader Wave 0 then found 2003-04 GRL gaps, an ODS-dead background request, an intermittent teardown abort, and a 4x storage growth.

**How to apply:** present the smoke matrix plus projected TB/CPU-h before asking for production approval. Prefer slimming outputs (e.g. no gen tree on shifted legs) before spending storage. Related: [[h1-lowq2-takeover]].
