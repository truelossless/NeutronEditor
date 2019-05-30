# Neutron Editor

An editor that aims productivity.
Inspired by vim, emacs and more modern editors !

It is made using C++ and SFML, so no HTML bullshit here (jk we luv u vs code)
Neutron is intended to be FAST (well in the future because right now not really ... :D)
BUT ... It's really FAST to start, so it's a really convenient editor for quicky editing files !

Right now, Neutron is under heavy development so don't expect to much from it.
We're not even close to a pre-alpha version, so calm down if evrything is broken, and if the code is ugly D:

On a side note; I'd love to integrate your pull requests !
I'll keep a warm place somewhere in the editor to display your name :)

## Current state of development

### Supported
	
	- text editing
	- scrolling
	- File I/O
	- syntax highlighting (mostly c++)
	- commands
	- theming
	- pasting text

### Not supported

	- text selection
	- copying text
	- some basic text shortcuts
	- reading tabs (currently converted to spaces)

### TODO
	
	- ... implement the unsupported features
	- fix bugs (maybe)
	- git integration
	- shell integration via the `!` command

### Known bugs	

*critical*
	- resizing the window with odd coordinates cause the text to blur: this is due to SFML text placement.
	If the text is drawn in floating coordinates, it becomes blurry. I believe that somewhere the width / height get diveded by two.
	This is a really annoying issue and I was not able yet to resolve it.
	see: https://en.sfml-dev.org/forums/index.php?topic=22762.0

*important*
	- cusor stutter when moving to a side / up or down, it is caused by the views but I don't know yet how to fix it.
	The cursor is moving, and then the view is moving, which break the smoothness when scrolling through lines.

*moderate*
	- quotes inside quotes is not working well (try putting double quotes inside single quotes)
	It comes from my loops to check quotes in SyntaxHighlighter.cpp, and order of the m_quotes array.
	I would need to check the pos for all the different types of quotes, and start highlighting for the first one.

## Keybinds

After using the master key `SHIFT+SPACE`, you have access to a variety of commands.

### using a command

To help remembering all the commands, they actually make sense.
For example: let's say you want to open a file.
You would do `SHIFT+SPACE`, then `f` (file) , then `o`(open) , then `ENTER` and TADAAM ! a nice file requester has popped up.
If you stop at `f`, you'll get autocomplete for all file related commands !

### searching a command

If you don't remember the specific keybind for a command, use `:`
For instance, using `:theme` will match all the entries containing theme, and their associated keybindings.
To run the command, you can just hit `ENTER` as soon as only one entry is remaining in the autocomplete container.
This is useful to always keep learning new shortcuts !

## Theming

Neutron is highly customisable, and it's not in any way a trade-off for speed !
Making your own custom theme is really easy: just open the theme.json file: use `sto`
There are extensive comments to help you understand which variable does what.
You can even hot-reload most of the theme (some colors won't be updated before a restart though): use `str`

## Settings

Similarly, use `sco` to open the conf.json file. It contains settings like the editor's font size.
Settings can also be changed without editing yourself the json file, with commands.
For example, to set the font size you can do `sef`

## Syntax highlighting

Featuring ... my homemade syntax highlighter !
Currently only supporting c++ keywords, but it will color quotes, comments, numbers and so on correctly for other languages.

## Downloads

You'd have to build the editor from source, I haven't released any binary yet.

## Contributing

SORRY IF THIS LOOKS LIKE SPAGHETTI CODE, I'M NOT GOOD !
Wanna help ? There is a list of bugs to fix and features to implement above !
Not a developer ? You can also open a github issue if you find a bug, if you want a feature to be added, or just to discuss.
Pull requests are highly appreciated.

## Licence

This project is available under the MIT Licence. Feel free to modify it, though a little mention somewhere would made my day !
