import numpy as np
import pandas as pd
from pathlib import Path

def main():
    N = 200  # number of samples
    dt = 0.1  # time interval (seconds)

    t = np.arange(0, N*dt, dt)
    noise = np.random.normal(loc=0, scale=4, size=N)
    z = 14.4 + noise  # true voltage with noise

    df = pd.DataFrame({"t": t, "z": z})

    out_path = Path("utils/data/Voltage.csv")
    df.to_csv(out_path, index=False)
    print("Voltage.csv generated at utils/data/Voltage.csv")

if __name__ == "__main__":
    main()
