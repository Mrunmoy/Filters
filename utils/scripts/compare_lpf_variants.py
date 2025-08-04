#!/usr/bin/env python3
import argparse
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(description="Compare multiple LPF outputs on same plot.")
    parser.add_argument("files", nargs="+", help="CSV files with LPF output (must have t,x,lpf columns)")
    parser.add_argument("--title", default="LPF Comparison", help="Plot title")
    parser.add_argument("-o", "--output", help="Optional output PNG")
    args = parser.parse_args()

    plt.figure(figsize=(10, 6))

    for file in args.files:
        csv_path = Path(file)
        df = pd.read_csv(csv_path)

        if not {'t', 'lpf'}.issubset(df.columns):
            print(f"Skipping {file}: required columns 't' and 'lpf' not found.")
            continue

        label = csv_path.stem.replace("lpf_sim_", "α=")
        plt.plot(df['t'], df['lpf'], label=label, linewidth=1)

    plt.xlabel("Time (t)")
    plt.ylabel("Filtered Value")
    plt.title(args.title)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    if args.output:
        plt.savefig(args.output)
        print(f"Saved plot to {args.output}")
    else:
        plt.show()

if __name__ == "__main__":
    main()
