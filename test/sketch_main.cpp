// Linker wrapper for host-compile checking Arduino-style sketches.
void setup();
void loop();

int main() {
    setup();
    while (true) loop();
    return 0;
}
