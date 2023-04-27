#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <libevdev/libevdev.h>

int main(int argc, char** argv) {
    // Open the input device
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open device: %s\n", argv[1]);
        return 1;
    }

    // Create the evdev device object
    struct libevdev* dev = NULL;
    int rc = libevdev_new_from_fd(fd, &dev);
    if (rc < 0) {
        fprintf(stderr, "Failed to init device: %s\n", argv[1]);
        return 1;
    }

    // Print device information
    printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
    printf("Device ID: %d\n", libevdev_get_id_product(dev));

    // Process events
    while (1) {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0) {
            printf("Event: time %ld.%06ld, type %d, code %d, value %d\n",
                   ev.time.tv_sec, ev.time.tv_usec, ev.type, ev.code, ev.value);
        }
    }

    // Cleanup
    libevdev_free(dev);
    close(fd);
    return 0;
}

