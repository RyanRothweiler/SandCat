Using the BIPED game prototyping tool
-------------------------------------

BIPED has two parts, one to support traditional human playtesting and another
to support automated machine playtesting. In this software project, the two
pieces are called "human" and "machine". You can find the tools for each in
obviously named subdirectories of the one where you found this README.

Human Playtesting
-----------------

Use Netbeans 6.8 to open the "biped" project in the "human" directory.
From here, use the run configuration setting to specify a single command
line argument for the program. This argument should be the relative path
to a game in the BIPED language (something.pl). Using "../games/db6k-aiide.pl"
is a safe bet.

It is usually easier to to the initial debugging of your games using the
human playtesting tool because of the integrated visual feedback it provides.
(Though it does require the designer to mentally juggle the abstract mechanics
and the visual representation, which often recycle vocabulary, at the same
time.)

Machine Playtesting
-------------------
Use the cygwin bash shell to navigate to the "machine" directory. Make sure
that the file "script.pl" in that directory is either a copy of or a symbolic
link to the game in the BIPED language that you are interested in testing.
To extract traces, simply run "make". The output of lparse will be stored in
a file called "solution" which can either be read directly or filtered
through the perl program "fancy.pl" for a more readable view.

The contents of "context.lp" are appended to a translated version of the
game's source before a solvers are run on it. You may edit "smodels.flags"
and "lparse.flags" to adjust the flags passed to each program.

The most common pitfall in developing games that make use of machine playtesting
is ensuring that all of your predicates are full domain-restricted (consult
the lparse manual for a formal definition). Domain restriction predicates
simply ensure that a program has an unambiguous grounding and are optional
during the human playtesting phase that uses a traditional resolution-based
interpretation of the game's rules.

Examples
------------------
* games/db6k-aiide.pl
	This rich example supports both human and machine playtesting, though it
	is somewhat complex as a starting point for learning the language. This
	is a version of the game frozen at the time of writing the aiide2009 paper.
	
* games/db6k_noenergy_onerock.pl
	This demake of db6k (titled DrillBoy 5999) eliminates the timed actions,
	energy limits, and adds restriction of holding no more than one rock
	in the inventory at a time. This is probably a better to base other games on.
	(However, no special declarations are included to support machine playtesting.)
	
* games/monopoly.pl
	Originally a regression test, this minimal interactive demo clearly demonstrates
	how to link abstract state to visual elements.

* games/gridslasher-mockup.pl
	This completely non-interactive demo shows off some of the recent graphical
	flair added to BIPED including visual display property overrides and
	procedural layout of a large number of spaces into a coherent arrangement.
	Without manual layout, spaces are scattered randomly on startup. An interactive
	version of the GridSlasher game with complex rules is in the works.

Previously Unasked Questions
----------------------------
Q: How do I get better at defining the core logic of a game?
A: Study db6k-aiide.pl

Q: How do I get better at defining UI-level mappings and logic?
A: Study monopoly.pl and gridslasher-mockup.pl

Q: How do I get better at exploratory machine playtesting?
A: Read all of the lparse manual. Try it out yourself, get experience, write a new paper.
