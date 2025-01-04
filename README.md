# PlaydateAMY
Experiments with the AMY synth: https://github.com/shorepine/amy

The current project plays an FM modulated sine wave when you hit B and.. something different.. when you hit A.

Since AMY uses the same render block size as Playdate it was very simple to wire in: we only have to use pd->sound->addSource() to create a callback-based source, then the callback simply calls amy_simple_fill_buffer() and de-interleaves the data into the left/right buffers passed to the callback.
