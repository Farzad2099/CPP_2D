#!/data/data/com.termux/files/usr/bin/bash

FILE="test.cpp"
OUTPUT="test"

echo "[INFO] Setting DISPLAY..."
export DISPLAY=:0

# Check whether Termux:X11 is running
if pidof termux-x11 > /dev/null; then
    echo "[INFO] Termux-X11 is already running."
else
    echo "[INFO] Starting Termux-X11..."
    termux-x11 :0 &
    sleep 2
fi

# Check source file
if [ ! -f "$FILE" ]; then
    echo "[ERROR] File $FILE not found!"
    exit 1
fi

# Compile
echo "[INFO] Compiling $FILE..."

g++ "$FILE" -o "$OUTPUT" \
    -lSDL2 -lSDL2_gfx -lSDL2_image

if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi

# Run and keep the program alive
echo "[INFO] Running program..."
./"$OUTPUT"

echo "[INFO] Program closed. Returned to Termux."#!/data/data/com.termux/files/usr/bin/bash

FILE="test.cpp"
OUTPUT="test"

echo "[INFO] Setting DISPLAY..."
export DISPLAY=:0

# چک کردن اینکه X11 در حال اجرا هست یا نه
if pidof termux-x11 > /dev/null; then
    echo "[INFO] Termux-X11 is already running."
else
    echo "[INFO] Starting Termux-X11 ..."
    termux-x11 :0 &
    sleep 1
fi

# چک کردن وجود test.cpp
if [ ! -f "$FILE" ]; then
    echo "[ERROR] File $FILE not found!"
    exit 1
fi

echo "[INFO] Compiling $FILE ..."
g++ "$FILE" -o "$OUTPUT" -lSDL2 -lSDL2_gfx -lSDL2_image -lSDL2_ttf

if [ $? -ne 0 ]; then
    echo "[ERROR] Compilation failed!"
    exit 1
fi

echo "[INFO] Running program in background..."
./"$OUTPUT" &

PID=$!
echo "[INFO] Program PID = $PID"

echo "[INFO] Waiting 10 seconds..."
sleep 10

echo "[INFO] Killing program..."
kill $PID 2>/dev/null

echo "[INFO] Done. Returned to Termux."

