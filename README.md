# roboform-wordlists
Project to generate wordlists for the old roboform RNG.

RoboForm is a password management tool, and about 10 years ago it had a bug in it that would cause the RNG to be seeded with system time. This project is an effort to generate wordlists to help people recover encrypted media from that era that might be otherwise lost, and to accurately measure the impact of this issue.

# Based on work done by Joe Grand et al.
    https://www.youtube.com/watch?v=o5IySpAkThg
    https://grandideastudio.com/portfolio/security/roboform-password-regeneration/
    https://grandideastudio.com/media/roboform_notes.pdf
    https://grandideastudio.com/media/roboform_wrapper_source.zip
    https://www.wired.com/story/roboform-password-3-million-dollar-crypto-wallet/
    https://web.archive.org/web/20130807185324/http://www.roboform.com/dist/RoboForm-Setup.exe

# Summary of Previous Research
The main problem is that in very old versions of roboform, entropy was set with system time. Using this information, wordlists can be generated.
The existing tooling will set windows time, then loads the DLL for word generation, and generates the password, and the process repeats.
This generates about ~30 words per second in the initial testing environment. The bottleneck appears to be disk io on system time getting set.

# Ideas for Future Research
* ~Is there a better way to set up a VM to generate faster?~ (no longer needed)
* ~Can we run a whole bunch of VMs at once?~ (no longer needed)
* ~Can we use detours.h to hook getsystemtime() to just return sequential numbers?~ (no longer needed)
* ~Can we patch the DLL itself to get sequential numbers directly instead of system time?~ (hell yeah we can)
* Can we rip out the entire algorithm itself to generate things without the DLL?
* Can we drop this on CUDA or something? Do we need to?

# Live DLL Patching Solution
The current solution that seems to be working great is a couple slight tweaks to the original code from Joe and Bruno. This new code makes two live patches to the DLL in memory, so the official DLL supplied by RoboForm can be used with no edits. The full code for this can be seen in tools/RoboFormWrapper-hax.cpp. To compile it, download Joe's original build zip linked above, and replace the wrapper with the new code. The new code will also look for the DLL in the current directory, so be mindful of that as well.

There are two main optimizations being patched in. The first replaces the call to `time64` with code that simply returns a number sitting on the stack, so you can pass in the timestamp that you want to generate for (seen on lines 256-257). The second (on line 230) nops out the code that re-seeds the RNG on each run, so the DLL doesn't need to be unloaded and reloaded after each generation.

With these tweaks it appears we can generate wordlists several orders of magnitude faster than the original code, but more extensive benchmarking might be useful.

# Wordlists to Generate
The highest priority generation would be wordlists from the era where this bug was active, using default settings, and then using likely tweaks to the settings.
