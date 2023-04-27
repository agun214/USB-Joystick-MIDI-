#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <alsa/asoundlib.h>

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

    // Open a MIDI device
    snd_seq_t* midi = NULL;
    snd_seq_open(&midi, "default", SND_SEQ_OPEN_OUTPUT, 0);
    snd_seq_set_client_name(midi, "Gamepad MIDI");

    // Create a MIDI port
    int port = snd_seq_create_simple_port(midi, "Gamepad", SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ, SND_SEQ_PORT_TYPE_MIDI_GENERIC);

    // Process events
    while (1) {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0) {
            // Translate input event into MIDI message
            snd_seq_event_t midi_event;
            snd_seq_ev_clear(&midi_event);
            snd_seq_ev_set_source(&midi_event, port);
            snd_seq_ev_set_subs(&midi_event);
            snd_seq_ev_set_direct(&midi_event);
            
            // Check if the event is a joystick event
            if (ev.type == EV_ABS && ev.code == ABS_X) {
                // Calculate the pitch bend value based on the joystick position
                int joystick_min = libevdev_get_abs_minimum(dev, ev.code);
                int joystick_max = libevdev_get_abs_maximum(dev, ev.code);
                int joystick_mid = (joystick_max + joystick_min);
                int pitchbend_val = (int)(((float)(ev.value - joystick_mid) / (float)(joystick_max - joystick_min)) * 16384.0f);
                
                // Set the MIDI event to a pitchbend event
                midi_event.type = SND_SEQ_EVENT_PITCHBEND;
                midi_event.data.control.channel = 0;
                midi_event.data.control.value = pitchbend_val;
            } else if (ev.type == EV_ABS && ev.code == ABS_Y) {
                // Calculate the pitch bend value based on the joystick position
                int joystick_min = libevdev_get_abs_minimum(dev, ev.code);
                int joystick_max = libevdev_get_abs_maximum(dev, ev.code);
                int joystick_mid = (joystick_max + joystick_min) ;
                int pitchbend_val = (int)(((float)(ev.value - joystick_mid) / (float)(joystick_max - joystick_min)) * 16384.0f);
                
                // Set the MIDI event to a pitchbend event
                midi_event.type = SND_SEQ_EVENT_PITCHBEND;
                midi_event.data.control.channel = 1;
                midi_event.data.control.value = pitchbend_val;
            } else if (ev.type == EV_ABS && ev.code == ABS_RX) {
                // Calculate the pitch bend value based on the joystick position
                int joystick_min = libevdev_get_abs_minimum(dev, ev.code);
                int joystick_max = libevdev_get_abs_maximum(dev, ev.code);
                int joystick_mid = (joystick_max + joystick_min);
                int pitchbend_val = (int)(((float)(ev.value - joystick_mid) / (float)(joystick_max - joystick_min)) * 16384.0f);
                
                // Set the MIDI event to a pitchbend event
                midi_event.type = SND_SEQ_EVENT_PITCHBEND;
                midi_event.data.control.channel = 2;
                midi_event.data.control.value = pitchbend_val;
            } else if (ev.type == EV_ABS && ev.code == ABS_RY) {
                // Calculate the pitch bend value based on the joystick position
                int joystick_min = libevdev_get_abs_minimum(dev, ev.code);
                int joystick_max = libevdev_get_abs_maximum(dev, ev.code);
                int joystick_mid = (joystick_max + joystick_min);
                int pitchbend_val = (int)(((float)(ev.value - joystick_mid) / (float)(joystick_max - joystick_min)) * 16384.0f);
                
                // Set the MIDI event to a pitchbend event
                midi_event.type = SND_SEQ_EVENT_PITCHBEND;
                midi_event.data.control.channel = 3;
                midi_event.data.control.value = pitchbend_val;
			} else {
                // Set the MIDI event to a default note on event
				switch (ev.code) {
					case 304:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 60; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 305:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 61; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 307:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 62; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 308:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 63; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 310:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 64; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 311:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 65; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 314:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 66; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 315:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 67; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 316:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 68; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 317:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 69; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 318:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 70; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 2:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 71; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
					case 5:
						midi_event.type = SND_SEQ_EVENT_NOTEON; //indicates event
						midi_event.data.note.channel = 0; //set MIDI channel
						midi_event.data.note.note = 72; //set MIDI note number (middle C)
						midi_event.data.note.velocity = ev.value ? 127 : 0; //set velocity
						break;
				}
            }            
            snd_seq_event_output(midi, &midi_event);
            snd_seq_drain_output(midi);
        } else if (rc == -EAGAIN) {
            // No events available, try again
            continue;
        } else {
            // Error reading event, exit loop
            break;
        }
    }



    // Clean up resources
    libevdev_free(dev);
    close(fd);
    snd_seq_close(midi);
    return 0;
}

