"""
Use the saleae module to interface with the Saleae Logic analyzer to capture i2c traffic

Notes:
    - Make sure the Logic GUI has scripting enabled:
      options -> preferences -> developer -> Enable scripting socket server

"""
import time
import logging
from saleae import Saleae
import pandas as pd
import defs

log = logging.getLogger(__name__)

CAPTURE_OUTPUT_CSV = ".\\i2c_capture.csv"


def init_saleae():
    sal = Saleae()
    analyzers = sal.get_analyzers()
    log.info(f"Saleae analyzers: {analyzers}")
    return sal


def run(sal, capture_seconds):
    """

    Args:
        sal (Saleae): Saleae object
        capture_seconds:

    Returns:

    """

    sal.set_capture_seconds(capture_seconds)
    sal.capture_start_and_wait_until_finished()
    sal.export_analyzer(analyzer_index=1, wait_for_processing=True, save_path=CAPTURE_OUTPUT_CSV)
    time.sleep(0.1)  # Make sure export is finished so that CSV file contains latest capture


def process_capture(path=CAPTURE_OUTPUT_CSV):
    """

    Args:
        path: path to exported i2c analyzer (CSV)

    Returns:

    """

    df = pd.read_csv(path)
    log.info(f"Imported df:\n {df.head(5)}")

    # First byte (always write) is register value
    df['Reg'] = df['Data']

    # Second byte (shifted up one) is R/W data
    df['R/W'] = df['Read/Write'].shift(-1)
    df['Data'] = df['Data'].shift(-1)

    print(df.head(5))

    # Combine SMB write/read transactions into single rows
    i2c_df: pd.DataFrame = df[df['ACK/NAK'] != "NAK"].copy()
    i2c_df.drop(columns=["Read/Write"], inplace=True)
    print(i2c_df.head(5))

    log.info(f"Address dtypes: {i2c_df['Address'].dtypes}")
    i2c_df['IC'] = i2c_df['Address'].apply(int, base=16).map(defs.I2C_ADDR_MAP)

    i2c_df.to_csv("i2c_analyzed.csv")


if __name__ == '__main__':
    import sys
    logging.basicConfig(level=logging.INFO, stream=sys.stdout)
    s = init_saleae()
    # run(sal=s, capture_seconds=1)
    process_capture()
