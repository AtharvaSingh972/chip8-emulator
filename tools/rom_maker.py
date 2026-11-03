import os

roms = {
    "err_stack_overflow.ch8": "2200",
    "err_stack_underflow.ch8": "00EE",
    "err_invalid_5XY1.ch8": "5001",
    "err_invalid_9XY1.ch8": "9001",
    "err_invalid_8XY9.ch8": "8009",
    "err_unsupported_0NNN.ch8": "0123"
}

for name, hex_data in roms.items():
    with open(name, "wb") as f:
        f.write(bytes.fromhex(hex_data))
        
print("Generated 7 test ROMs successfully.")