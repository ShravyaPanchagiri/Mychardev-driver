# MyCharDev: A Simple Linux Character Driver

This is a basic character device driver template. I wrote this to practice using the modern cdev and device_class APIs rather than the older register_chrdev approach. It handles the fundamentals of moving data between user-space and kernel memory.
Key Details

    Automatic Nodes: Uses device_create so the /dev/mychardev node pops up automatically on load.

    Boundary Safety: Uses copy_to_user and copy_from_user to prevent kernel panics when moving data.

    Dynamic Allocation: Requests a major number dynamically to avoid system conflicts.

How to use it

# Build the driver and test app
make

# Load the module
sudo insmod src/mychardev.ko

# Run the test
cd tests
gcc test_mychardev.c -o test_mychardev
./test_mychardev

# Check the logs
dmesg | tail