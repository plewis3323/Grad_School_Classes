#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Fit AME2012 experimental masses with the 5-parameter Liquid Drop Model (B/A).

File assumed: AME2012massesExptOnly_formatted.txt
• Expected columns (names can vary): A, Z, N, and either BoverA or BE (total binding energy in MeV).
• If BE is present (but not BoverA), the script computes BoverA = BE / A.

Outputs:
  ldm_residuals_vs_N.png
  ldm_residuals_vs_Z.png
  (optional) ldm_fit_parameters.txt  with --write-params
"""

from __future__ import annotations
import argparse
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# ------------------------------
# Liquid Drop Model (B/A)
# ------------------------------

def liquid_drop_model(vars, a_v, a_s, a_c, a_a, a_p):
    """
    B/A = a_v
          - a_s * A^(-1/3)
          - a_c * Z(Z-1) * A^(-4/3)
          - a_a * (A - 2Z)^2 * A^(-2)
          + a_p * delta(N,Z) * A^(-3/2)
    where delta = +1 (even-even), -1 (odd-odd), 0 (even-odd).
    """
    A, Z, N = vars
    A = np.asarray(A, dtype=float)
    Z = np.asarray(Z, dtype=float)
    N = np.asarray(N, dtype=float)

    Zi = Z.astype(int)
    Ni = N.astype(int)
    delta = np.zeros_like(A)
    even_even = (Zi % 2 == 0) & (Ni % 2 == 0)
    odd_odd   = (Zi % 2 == 1) & (Ni % 2 == 1)
    delta[even_even] = +1.0
    delta[odd_odd]   = -1.0

    term_volume    = a_v
    term_surface   = -a_s * np.power(A, -1.0/3.0)
    term_coulomb   = -a_c * (Z * (Z - 1.0)) * np.power(A, -4.0/3.0)
    term_asymmetry = -a_a * np.power(A - 2.0*Z, 2.0) * np.power(A, -2.0)
    term_pairing   =  a_p * delta * np.power(A, -1.5)

    return term_volume + term_surface + term_coulomb + term_asymmetry + term_pairing

# ------------------------------
# Data loading (robust to AME2012 variants)
# ------------------------------

CANDIDATE_NAMES = {
    "A": ["A", "a", "MassNumber"],
    "Z": ["Z", "z", "Protons", "Znum"],
    "N": ["N", "n", "Neutrons"],
    # binding energy per nucleon:
    "BoverA": ["BoverA", "BEA", "B/A", "BE_per_A", "BEA_MeV"],
    # total binding energy (MeV):
    "BE": ["BE", "BindingEnergy", "Btot", "BE_MeV", "Ebind"]
}

def _find_col(df_cols, candidates):
    lc = {c.lower(): c for c in df_cols}
    for name in candidates:
        if name.lower() in lc:
            return lc[name.lower()]
    return None

def load_ame2012(path: str) -> pd.DataFrame:
    """
    Try to read AME2012-formatted text (whitespace or CSV).
    Detect columns for A, Z, N and BoverA or BE. If only BE is present, compute BoverA.
    """
    # Try whitespace first; then CSV as fallback
    for try_whitespace in (True, False):
        try:
            if try_whitespace:
                df = pd.read_csv(path, delim_whitespace=True, comment="#")
            else:
                df = pd.read_csv(path, comment="#")
            break
        except Exception:
            df = None
    if df is None:
        raise IOError(f"Could not read file: {path}")

    # If 4 naked columns, assume order A Z N BoverA
    if df.shape[1] == 4 and set(df.columns) == set(range(4)):
        df.columns = ["A", "Z", "N", "BoverA"]

    # Find required columns by name (robust to variants)
    colA = _find_col(df.columns, CANDIDATE_NAMES["A"])
    colZ = _find_col(df.columns, CANDIDATE_NAMES["Z"])
    colN = _find_col(df.columns, CANDIDATE_NAMES["N"])
    colBA = _find_col(df.columns, CANDIDATE_NAMES["BoverA"])
    colBE = _find_col(df.columns, CANDIDATE_NAMES["BE"])

    missing_basic = [x for x, c in (("A", colA), ("Z", colZ), ("N", colN)) if c is None]
    if missing_basic:
        raise ValueError(f"Missing required columns {missing_basic}. "
                         f"Found columns: {list(df.columns)}")

    # Coerce numeric and drop bad rows
    cols_to_keep = [colA, colZ, colN] + ([colBA] if colBA else []) + ([colBE] if colBE else [])
    d = df[cols_to_keep].copy()
    for c in cols_to_keep:
        d[c] = pd.to_numeric(d[c], errors="coerce")
    d = d.replace([np.inf, -np.inf], np.nan).dropna(how="any")

    # Normalize names
    d = d.rename(columns={colA: "A", colZ: "Z", colN: "N"})
    if colBA: d = d.rename(columns={colBA: "BoverA"})
    if colBE: d = d.rename(columns={colBE: "BE"})

    # If BoverA missing but BE present, compute
    if "BoverA" not in d.columns:
        if "BE" in d.columns:
            d["BoverA"] = d["BE"] / d["A"]
        else:
            raise ValueError("Could not find BoverA or BE columns to compute BoverA.")

    # Enforce A = Z + N (AME tables should satisfy this)
    d = d[(d["A"] > 0) & (d["Z"] >= 0) & (d["N"] >= 0)]
    d = d[np.isclose(d["A"], d["Z"] + d["N"])]
    d = d[["A", "Z", "N", "BoverA"]].sort_values(["Z", "N"]).reset_index(drop=True)
    if len(d) == 0:
        raise ValueError("No valid rows after filtering. Check column meanings/units.")
    return d

# ------------------------------
# Pretty print
# ------------------------------

def print_params(popt, perr):
    names = ["a_v", "a_s", "a_c", "a_a", "a_p"]
    print("\nBest-fit parameters (MeV):")
    for n, v, e in zip(names, popt, perr):
        print(f"  {n:>3s} = {v:9.4f} ± {e:6.4f}")

# ------------------------------
# Main
# ------------------------------

def main():
    ap = argparse.ArgumentParser(description="Fit liquid drop model to AME2012 data.")
    ap.add_argument("--data", default="AME2012massesExptOnly_formatted.txt",
                    help="Path to AME2012 formatted text file (default: %(default)s)")
    ap.add_argument("--save-prefix", default="ldm",
                    help="Prefix for output files (default: %(default)s)")
    ap.add_argument("--write-params", action="store_true",
                    help="Write <prefix>_fit_parameters.txt")
    args = ap.parse_args()

    # 1) Load data
    df = load_ame2012(args.data)
    print(f"Loaded {len(df)} nuclei from {args.data}")

    A = df["A"].to_numpy()
    Z = df["Z"].to_numpy()
    N = df["N"].to_numpy()
    BoverA = df["BoverA"].to_numpy()

    # 2) Initial guesses (MeV)
    p0 = [15.0, 18.0, 0.70, 20.0, 10.0]
    # Bounds keep parameters positive and reasonable
    bounds_lower = [  0.0,  0.0, 0.0,  0.0,  0.0]
    bounds_upper = [100.0, 80.0, 5.0, 80.0, 40.0]

    # 3) Fit
    popt, pcov = curve_fit(
        liquid_drop_model, (A, Z, N), BoverA,
        p0=p0, bounds=(bounds_lower, bounds_upper), maxfev=200000
    )
    perr = np.sqrt(np.diag(pcov))
    print_params(popt, perr)

    # 4) Residuals
    Bfit = liquid_drop_model((A, Z, N), *popt)
    resid = BoverA - Bfit
    df["Residual"] = resid
    print(f"\nResidual stats (MeV): mean={resid.mean():.4f}, "
          f"std={resid.std(ddof=1):.4f}, min={resid.min():.4f}, max={resid.max():.4f}")

    # 5) Plots
    # Residuals vs N
    plt.figure(figsize=(7.2, 4.5))
    plt.scatter(df["N"], df["Residual"], s=12)
    plt.axhline(0.0, linestyle="--", linewidth=1)
    plt.xlabel("Neutron number N")
    plt.ylabel("Residual  (B/A data − model)  [MeV]")
    plt.title("Residuals vs N")
    plt.tight_layout()
    outN = f"{args.save_prefix}_residuals_vs_N.png"
    plt.savefig(outN, dpi=150)
    print(f"Saved {outN}")

    # Residuals vs Z
    plt.figure(figsize=(7.2, 4.5))
    plt.scatter(df["Z"], df["Residual"], s=12)
    plt.axhline(0.0, linestyle="--", linewidth=1)
    plt.xlabel("Proton number Z")
    plt.ylabel("Residual  (B/A data − model)  [MeV]")
    plt.title("Residuals vs Z")
    plt.tight_layout()
    outZ = f"{args.save_prefix}_residuals_vs_Z.png"
    plt.savefig(outZ, dpi=150)
    print(f"Saved {outZ}")

    # 6) Optional parameters file
    if args.write_params:
        outP = f"{args.save_prefix}_fit_parameters.txt"
        names = ["a_v", "a_s", "a_c", "a_a", "a_p"]
        with open(outP, "w") as f:
            for n, v, e in zip(names, popt, perr):
                f.write(f"{n} = {v:.6f} ± {e:.6f}  MeV\n")
        print(f"Wrote {outP}")

if __name__ == "__main__":
    main()






















































































































































