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

# Ideas for future research
* Is there a better way to set up a VM to generate faster?
* Can we run a whole bunch of VMs at once?
* Can we use detours.h to hook getsystemtime() to just return sequential numbers?
* Can we patch the DLL itself to get sequential numbers directly instead of system time?
* Can we rip out the entire algorithm itself to generate things without the DLL?
* Can we drop this on CUDA or something? Do we need to?

# Wordlists to generate
The highest priority generation would be wordlists from the era where this bug was active, using default settings, and then using likely tweaks to the settings.
