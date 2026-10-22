
#!/data/data/com.termux/files/usr/bin/bash

FILE="test.cpp"
OUTPUT="test"

# Set the display for Termux:X11
echo "[INFO] Setting DISPLAY..."
export DISPLAY=:0

# Check whether Termux:X11 is already running
if pidof termux-x11 > /dev/null; then
    echo "[INFO] Termux:X11 is already running."
else
    echo "[INFO] Starting Termux:X11..."
    termux-x11 :0 &
    sleep 2
fi

# Check whether the source file exists
if [ ! -f "$FILE" ]; then
    echo "[ERROR] File $FILE not found!"
    exit 1
fi

# Compile the C++ source file
echo "[INFO] Compiling $FILE..."

g++ "$FILE" -o "$OUTPUT" \
    -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf

# Stop if compilation fails
if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi

# Run the program in the foreground
echo "[INFO] Running program..."
./"$OUTPUT"

# Report when the program exits
EXIT_CODE=$?
echo "[INFO] Program closed with exit code $EXIT_CODE."

exit "$EXIT_CODE"

