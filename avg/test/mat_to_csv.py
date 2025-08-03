#!/usr/bin/env python3
import argparse
import os
import sys
import numpy as np

def load_mat(path):
    try:
        from scipy.io import loadmat
    except Exception as e:
        print("Error: scipy is required. Install with: pip install scipy", file=sys.stderr)
        raise
    return loadmat(path)

def pick_vector(mat, prefer):
    """
    Pick a 1-D numeric vector from the MAT dict.
    If 'prefer' is given and exists, return that.
    Otherwise, choose the longest 1-D numeric vector.
    """
    if prefer and prefer in mat:
        arr = np.asarray(mat[prefer]).squeeze()
        if arr.ndim == 1 and np.issubdtype(arr.dtype, np.number):
            return prefer, arr
        else:
            raise ValueError(f"Requested variable '{prefer}' is not a 1-D numeric vector.")
    best_name, best_arr = None, None
    for k, v in mat.items():
        if k.startswith("__"):
            continue
        arr = np.asarray(v).squeeze()
        if arr.ndim == 1 and np.issubdtype(arr.dtype, np.number):
            if best_arr is None or arr.size > best_arr.size:
                best_name, best_arr = k, arr
    if best_arr is None:
        raise ValueError("No 1-D numeric vector found in MAT file.")
    return best_name, best_arr

def list_vars(mat):
    print("Variables in MAT file:")
    for k, v in mat.items():
        if k.startswith("__"): continue
        arr = np.asarray(v)
        print(f" - {k}: shape={arr.shape}, dtype={arr.dtype}")

def main():
    ap = argparse.ArgumentParser(description="Convert MATLAB .mat to CSV (t,z) for moving-average sim.")
    ap.add_argument("mat_path", help="Path to .mat file (e.g., SonarAlt.mat)")
    ap.add_argument("-o", "--out", default=None, help="Output CSV path (default: next to MAT as <name>.csv)")
    ap.add_argument("--tvar", default=None, help="Name of time vector variable (optional)")
    ap.add_argument("--yvar", default=None, help="Name of signal variable (optional)")
    ap.add_argument("--list", action="store_true", help="List variables and exit")
    args = ap.parse_args()

    mat = load_mat(args.mat_path)

    if args.list:
        list_vars(mat)
        return

    # pick y (signal)
    yname, y = pick_vector(mat, args.yvar)

    # pick t (time) if provided or known, else synthesize
    t = None
    if args.tvar:
        tname, t = pick_vector(mat, args.tvar)
        if t.size != y.size:
            raise ValueError(f"Time vector '{args.tvar}' length {t.size} != signal '{yname}' length {y.size}")
    else:
        # common time names
        for cand in ("t", "time", "Time"):
            if cand in mat:
                tname, t = pick_vector(mat, cand)
                if t.size == y.size:
                    break
                else:
                    t = None
        if t is None:
            t = np.arange(y.size, dtype=float)

    out = args.out
    if out is None:
        base, _ = os.path.splitext(args.mat_path)
        out = base + ".csv"
    out = os.path.abspath(out)

    os.makedirs(os.path.dirname(out), exist_ok=True)
    import csv
    with open(out, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["t", "z"])
        for ti, yi in zip(t, y):
            w.writerow([float(ti), float(yi)])

    print(f"Wrote CSV: {out}")
    print(f"Columns: t,z   (from MAT vars: t={'synth' if args.tvar is None else args.tvar}, z={yname})")

if __name__ == "__main__":
    main()
