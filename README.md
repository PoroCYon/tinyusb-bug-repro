# tinyusb-bug-repro

## CH32V307

Build instructions:
1. Initialize the `tinyusb` submodule
1. Have a `riscv32-unknown-elf` GCC cross-toolchain (I use upstream 13.2.0)
1. Install `wlink` (using eg `cargo install --git https://github.com/ch32-rs/wlink`) or OpenOCD-OpenWCH
1. Run `make` in the `ch32v307` folder
1. To flash it on a target, run `make flash`

**NOTE**: In the code, currently, pin A9 is used as UART output.

## RP2040

Build instructions:
1. Install the Pico SDK
1. `mkdir cmake-build && cd cmake-build && cmake .. && make -j$(nproc)`
1. Use `picotool` os the UF2 USB bootloader to load the code

**NOTE**: The default board UART TX pin is used as UART output.

## Testing

1. Load the code onto the hardware
1. Monitor the UART output using a USB-UART adapter
1. `dfu-util -D payload.py`
  * On the RP2040, this always works properly
  * On the CH32V307, the following effects may be observed:
    * The data buffer received by the DFU functions is 64 bytes off (can be in either direction)
    * Random bytes in the data buffer are corrupted (watch the CRC)
    * Sometimes it hangs (due to a crash due to the memory corruption, I think)
      * With payloads larger than 512 bytes, crashes seem to almost always happen.
