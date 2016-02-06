Copyright (C) 2015-2016, Chesley Kraniak

License is GPLv3. I may decide to update this with something less restrictive later, but not yet.

OPEN ROGUE SURVIVOR

Overview:

This game is meant to be a reimplementation of Rogue Survivor. The current
focus is not necessarily a perfectly faithful representation, but most of
the Standard Game type should at least be covered (as of alpha 9 of RS).

The philosophy is to implement the fun parts first. This is because it should
be way easier to work on something that feels like it's working, as opposed
to blowing a ton of time in design (of either the mechanics or the code) and
burning out before something playable is ever released.

The way I see it, the fun comes from:
 a. Killing Zombies
 b. Surviving
I think a decent state will have:
 - Players, enemies, food, and medkits
 - Some kind of map, with walls and ground for terrain, bare minimum;
   specifics aren't important, and mapmaking can be manual at first, so
   long as the resulting map(s) will be fun
That should be at least playable enough to be called a "game". Once I have
something about that level of "done", I'll call it an alpha build.

I'm also eschewing libraries. This is mostly because one of my regular
computers doesn't have good tools, and I can't install the .dlls where they
would need to go (non-admin acct.) Because of this, OpenGL will come ...
well, not first. I mean, the game is a square grid, so ASCII *should* work
for a good while.
