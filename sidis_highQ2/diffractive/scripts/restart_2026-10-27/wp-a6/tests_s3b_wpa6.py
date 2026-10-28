"""S3b tests. Usage: tests_s3b_wpa6.py --split even   (plots_complete)."""
import argparse, sys
from pathlib import Path
D = Path("/data/dust/group/h1/klesthen/BaryonAsym/sidis_highQ2/diffractive")


def plots_complete(split):
    od = D / "plots/restart_2026-10-27/wp-a6" / split
    names = ["stacks/%s_%s_%s.png" % (ch, c, k) for ch in ("7071", "7091", "nd4cat") for c in ("D0", "D1", "D2") for k in ("MX4_10", "MX10_20", "MX20_40", "MXout")]
    names += ["fnd_vs_mx.png", "sel_mx.png", "sel_xip.png"]
    cap = (od / "captions.md").read_text() if (od / "captions.md").exists() else ""
    miss = [n for n in names if not (od / n).exists() or (od / n).stat().st_size < 1000]
    nocap = [n for n in names if "## %s\n" % n not in cap]
    ok = not miss and not nocap
    print("plots_complete %s: %d expected, missing %s, no caption %s -> %s" % (split, len(names), miss, nocap, "PASS" if ok else "FAIL"))
    return ok


if __name__ == "__main__":
    ap = argparse.ArgumentParser(); ap.add_argument("--split", required=True); sys.exit(0 if plots_complete(ap.parse_args().split) else 1)
