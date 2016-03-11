# chip_voicekeyboard_online
Just some code for the 'online' version of a USB voice keyboard
for NTC CHIP, as shown in video:
https://youtu.be/VgyPFYderjc

It is mostly bash shell script with some C to do the audio recording.

Note that the stock kernel does'nt have the necessary modules to support
USB HID, so I use the renzo kernel:
https://bbs.nextthing.co/t/compile-the-linux-kernel-for-chip-my-personal-howto/2669/40

    cd /tmp
    wget http://www.raspibo.org/renzo/chiplinux4.3.0rd235+.tgz
    cd /
    sudo tar zxf /tmp/chiplinux4.3.0rd235+.tgz

    ... and if you want to set this as your standard boot

    sudo cp /boot/vmlinuz-4.3.0rd235+ /boot/zImage

Also, need to disable the USB console as it conflicts otherwise

    cd /etc/modprobe.d
    echo "blacklist g_serial" > g_serial_blacklist.conf 

The entry point is init_online, which can be run as an alternative init to systemd at boot-time, or run manually in a normally running CHIP.

The recording is done by alsa/go.c, which takes chunks of audio and dumps them in /tmp/a.raw. Then a shell script 'alsa/doit' picks up those chunks, converts the format to flac, uploads to google for transcription and dumps the transcription result to /tmp/a.txt

From there, the typer/go.c program reads /tmp/a.txt and generates USB HID keystroke data into the /dev/hidg0 device file.
