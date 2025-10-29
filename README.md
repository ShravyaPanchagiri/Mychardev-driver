# MyCharDev – Linux Character Device Driver

Simple Linux character device driver demonstrating basic file operations.

## Features
- Open, read, write, and close device operations
- Kernel–user space communication using `copy_to_user` / `copy_from_user`
- Creates `/dev/mychardev` device node
- Unit test for validation
- Can be compiled and loaded on Linux kernel

## How to Build & Run
```bash
make                    # Build kernel module
sudo insmod src/mychardev.ko   # Load module
ls -l /dev/mychardev          # Verify device node

# Test user-space program
cd tests
gcc test_mychardev.c -o test_mychardev
sudo ./test_mychardev

# Manual write/read
echo "Hello Kernel" | sudo tee /dev/mychardev
sudo cat /dev/mychardev

# Unload module
sudo rmmod mychardev
dmesg | tail
